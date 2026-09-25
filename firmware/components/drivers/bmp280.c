#include "fpm_i2c_guard.h"
#include "driver_delay.h"
#include "bmp280.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "bmp280";

// Registros
#define BMP280_REG_CHIPID   0xD0
#define BMP280_REG_RESET    0xE0
#define BMP280_REG_CALIB    0x88 // 24 bytes de calibracion (T1..T3, P1..P9)
#define BMP280_REG_CTRL     0xF4
#define BMP280_REG_CONFIG   0xF5
#define BMP280_REG_DATA     0xF7 // press msb/lsb/xlsb, temp msb/lsb/xlsb

#define BMP280_CHIPID_280   0x58
#define BMP280_CHIPID_BME   0x60 // BME280 (mismo mapa de P/T; ignoramos humedad)
#define BMP280_RESET_WORD   0xB6

// ctrl_meas: osrs_t=x2 (010), osrs_p=x16 (101), mode=normal (11) -> 0x57
#define BMP280_CTRL_NORMAL  0x57
// config: t_sb=0.5ms (000), filter=x16 (100), spi3w=0 -> 0x10
#define BMP280_CONFIG_IIR   0x10

static i2c_master_dev_handle_t s_dev = NULL;
static bool s_ready = false;

// Parametros de calibracion de fabrica
static uint16_t dig_T1, dig_P1;
static int16_t  dig_T2, dig_T3;
static int16_t  dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
static int32_t  t_fine; // se comparte entre la comp. de temp y la de presion

static esp_err_t bmp280_read_regs(uint8_t reg, uint8_t *buf, size_t len)
{
    return i2c_master_transmit_receive(s_dev, &reg, 1, buf, len, 100);
}

static esp_err_t bmp280_write_reg(uint8_t reg, uint8_t val)
{
    uint8_t tx[2] = {reg, val};
    return i2c_master_transmit(s_dev, tx, 2, 100);
}

esp_err_t bmp280_init(i2c_master_bus_handle_t bus_handle, uint8_t i2c_addr)
{
    if (!bus_handle)
        return ESP_ERR_INVALID_ARG;

    s_ready = false;
    s_dev = NULL;

    uint8_t addr = i2c_addr ? i2c_addr : 0x77;
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = addr,
        .scl_speed_hz = 100000,
    };
    esp_err_t err = i2c_master_bus_add_device(bus_handle, &dev_cfg, &s_dev);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error agregando BMP280 al bus I2C: %s", esp_err_to_name(err));
        return err;
    }

    uint8_t id = 0;
    err = bmp280_read_regs(BMP280_REG_CHIPID, &id, 1);
    if (err != ESP_OK || (id != BMP280_CHIPID_280 && id != BMP280_CHIPID_BME))
    {
        ESP_LOGE(TAG, "BMP280 no responde en 0x%02X (chip_id=0x%02X, err=%s)",
                 addr, id, esp_err_to_name(err));
        i2c_master_bus_rm_device(s_dev);
        s_dev = NULL;
        return (err != ESP_OK) ? err : ESP_ERR_NOT_FOUND;
    }
    const char *name = (id == BMP280_CHIPID_BME) ? "BME280" : "BMP280";

    // Soft reset y espera al arranque (NVM copy ~2 ms)
    (void)bmp280_write_reg(BMP280_REG_RESET, BMP280_RESET_WORD);
    driver_delay_ms(10);

    // Calibracion de fabrica (24 bytes, little-endian)
    uint8_t c[24] = {0};
    err = bmp280_read_regs(BMP280_REG_CALIB, c, sizeof(c));
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "%s: error leyendo calibracion: %s", name, esp_err_to_name(err));
        i2c_master_bus_rm_device(s_dev);
        s_dev = NULL;
        return err;
    }
    dig_T1 = (uint16_t)(c[0]  | (c[1]  << 8));
    dig_T2 = (int16_t) (c[2]  | (c[3]  << 8));
    dig_T3 = (int16_t) (c[4]  | (c[5]  << 8));
    dig_P1 = (uint16_t)(c[6]  | (c[7]  << 8));
    dig_P2 = (int16_t) (c[8]  | (c[9]  << 8));
    dig_P3 = (int16_t) (c[10] | (c[11] << 8));
    dig_P4 = (int16_t) (c[12] | (c[13] << 8));
    dig_P5 = (int16_t) (c[14] | (c[15] << 8));
    dig_P6 = (int16_t) (c[16] | (c[17] << 8));
    dig_P7 = (int16_t) (c[18] | (c[19] << 8));
    dig_P8 = (int16_t) (c[20] | (c[21] << 8));
    dig_P9 = (int16_t) (c[22] | (c[23] << 8));

    // Configuracion: filtro IIR primero, luego ctrl_meas (arranca el muestreo)
    err = bmp280_write_reg(BMP280_REG_CONFIG, BMP280_CONFIG_IIR);
    if (err == ESP_OK)
        err = bmp280_write_reg(BMP280_REG_CTRL, BMP280_CTRL_NORMAL);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "%s: error configurando: %s", name, esp_err_to_name(err));
        i2c_master_bus_rm_device(s_dev);
        s_dev = NULL;
        return err;
    }
    driver_delay_ms(50); // primera conversion normal-mode

    s_ready = true;
    ESP_LOGI(TAG, "%s OK en 0x%02X (referencia barometrica, osr_p=x16, IIR=x16)", name, addr);
    return ESP_OK;
}

bool bmp280_is_ready(void)
{
    return s_ready;
}

// Compensacion de temperatura (Bosch, double). Devuelve grados C y fija t_fine.
// Se usa double por los productos intermedios grandes (recomendacion Bosch).
static double bmp280_compensate_temp(int32_t adc_T)
{
    double var1 = (((double)adc_T) / 16384.0 - ((double)dig_T1) / 1024.0) * ((double)dig_T2);
    double var2 = ((((double)adc_T) / 131072.0 - ((double)dig_T1) / 8192.0) *
                   (((double)adc_T) / 131072.0 - ((double)dig_T1) / 8192.0)) * ((double)dig_T3);
    t_fine = (int32_t)(var1 + var2);
    return (var1 + var2) / 5120.0;
}

// Compensacion de presion (Bosch, double). Devuelve Pa. Usa t_fine (llamar temp antes).
static double bmp280_compensate_press(int32_t adc_P)
{
    double var1 = ((double)t_fine / 2.0) - 64000.0;
    double var2 = var1 * var1 * ((double)dig_P6) / 32768.0;
    var2 = var2 + var1 * ((double)dig_P5) * 2.0;
    var2 = (var2 / 4.0) + (((double)dig_P4) * 65536.0);
    var1 = (((double)dig_P3) * var1 * var1 / 524288.0 + ((double)dig_P2) * var1) / 524288.0;
    var1 = (1.0 + var1 / 32768.0) * ((double)dig_P1);
    if (var1 == 0.0)
        return 0.0; // evita division por cero
    double p = 1048576.0 - (double)adc_P;
    p = (p - (var2 / 4096.0)) * 6250.0 / var1;
    var1 = ((double)dig_P9) * p * p / 2147483648.0;
    var2 = p * ((double)dig_P8) / 32768.0;
    p = p + (var1 + var2 + ((double)dig_P7)) / 16.0;
    return p; // Pa
}

esp_err_t bmp280_read(float *pressure_kpa, float *temp_c)
{
    if (!s_dev || !s_ready)
        return ESP_ERR_INVALID_STATE;

    uint8_t d[6] = {0};
    esp_err_t err = bmp280_read_regs(BMP280_REG_DATA, d, sizeof(d));
    if (err != ESP_OK)
        return err;

    int32_t adc_P = ((int32_t)d[0] << 12) | ((int32_t)d[1] << 4) | (d[2] >> 4);
    int32_t adc_T = ((int32_t)d[3] << 12) | ((int32_t)d[4] << 4) | (d[5] >> 4);

    double t = bmp280_compensate_temp(adc_T);   // fija t_fine
    double pa = bmp280_compensate_press(adc_P);  // Pa

    if (temp_c)
        *temp_c = (float)t;
    if (pressure_kpa)
        *pressure_kpa = (float)(pa / 1000.0); // Pa -> kPa
    return ESP_OK;
}
