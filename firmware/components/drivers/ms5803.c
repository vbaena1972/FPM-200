#include "fpm_i2c_guard.h"
#include "driver_delay.h"
#include "ms5803.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "ms5803";

// Direcciones posibles segun el pin CSB
#define MS5803_ADDR_CSB_HIGH 0x76 // CSB a VDD
#define MS5803_ADDR_CSB_LOW  0x77 // CSB a GND

// Comandos MS5803
#define MS5803_CMD_RESET     0x1E
#define MS5803_CMD_ADC_READ  0x00
#define MS5803_CMD_PROM_READ 0xA0 // 0xA0..0xAE (8 words)
#define MS5803_CMD_CONV_D1   0x48 // presion,     OSR=4096
#define MS5803_CMD_CONV_D2   0x58 // temperatura, OSR=4096

// Tiempo maximo de conversion a OSR=4096 = 9.04 ms -> usamos 10 ms de margen
#define MS5803_CONV_DELAY_MS 10

static i2c_master_dev_handle_t s_dev = NULL;
static uint16_t s_prom[8] = {0}; // C0..C7 (C1..C6 son los coeficientes utiles)
static bool s_ready = false;

// -------------------------------------------------------------------
// Helpers I2C
// -------------------------------------------------------------------

static esp_err_t ms5803_send_cmd(uint8_t cmd)
{
    // Timeout corto: si el bus se cuelga, no queremos bloquear a los otros
    // usuarios del bus 1 (touch/LVGL) por mucho tiempo. La recuperacion de
    // bus y el antirrebote toleran un fallo puntual.
    return i2c_master_transmit(s_dev, &cmd, 1, 50);
}

// Reintenta el reset varias veces (el datasheet advierte que la 1a transaccion
// puede fallar si el modulo dejo SDA bloqueado tras un power-on incompleto).
static esp_err_t ms5803_reset_retry(void)
{
    esp_err_t err = ESP_FAIL;
    for (int i = 0; i < 5; i++)
    {
        err = ms5803_send_cmd(MS5803_CMD_RESET);
        if (err == ESP_OK)
            return ESP_OK;
        driver_delay_ms(10);
    }
    return err;
}

static esp_err_t ms5803_read_prom_word(uint8_t index, uint16_t *out)
{
    uint8_t cmd = MS5803_CMD_PROM_READ + (uint8_t)(index * 2);
    uint8_t rx[2] = {0};
    esp_err_t err = ESP_FAIL;
    for (int retry = 0; retry < 3; retry++)
    {
        err = i2c_master_transmit_receive(s_dev, &cmd, 1, rx, 2, 100);
        if (err == ESP_OK)
        {
            *out = ((uint16_t)rx[0] << 8) | rx[1];
            return ESP_OK;
        }
        driver_delay_ms(5);
    }
    return err;
}

// Lee el ADC (24 bits) tras una conversion
static esp_err_t ms5803_read_adc(uint32_t *out)
{
    uint8_t cmd = MS5803_CMD_ADC_READ;
    uint8_t rx[3] = {0};
    esp_err_t err = i2c_master_transmit_receive(s_dev, &cmd, 1, rx, 3, 50);
    if (err != ESP_OK)
        return err;
    *out = ((uint32_t)rx[0] << 16) | ((uint32_t)rx[1] << 8) | rx[2];
    return ESP_OK;
}

// CRC4 de la PROM segun AN520 de TE (algoritmo estandar MS58xx)
static uint8_t ms5803_crc4(const uint16_t *prom)
{
    uint16_t n_rem = 0;
    uint16_t crc_read = prom[7];
    uint16_t buf[8];
    for (int i = 0; i < 8; i++)
        buf[i] = prom[i];
    buf[7] = (0xFF00 & buf[7]); // el byte bajo (CRC) se pone a 0

    for (int cnt = 0; cnt < 16; cnt++)
    {
        if (cnt % 2 == 1)
            n_rem ^= (uint16_t)((buf[cnt >> 1]) & 0x00FF);
        else
            n_rem ^= (uint16_t)(buf[cnt >> 1] >> 8);

        for (int n_bit = 8; n_bit > 0; n_bit--)
        {
            if (n_rem & 0x8000)
                n_rem = (uint16_t)((n_rem << 1) ^ 0x3000);
            else
                n_rem = (uint16_t)(n_rem << 1);
        }
    }
    n_rem = (uint16_t)((n_rem >> 12) & 0x000F);
    (void)crc_read;
    return (uint8_t)n_rem;
}

// -------------------------------------------------------------------
// Init
// -------------------------------------------------------------------

static esp_err_t ms5803_try_addr(i2c_master_bus_handle_t bus, uint8_t addr)
{
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = addr,
        .scl_speed_hz = 100000,
    };
    esp_err_t err = i2c_master_bus_add_device(bus, &dev_cfg, &s_dev);
    if (err != ESP_OK)
    {
        ESP_LOGW(TAG, "add_device 0x%02X fallo: %s", addr, esp_err_to_name(err));
        return err;
    }

    // Reset (con reintentos). Aunque falle el ACK, intentamos leer la PROM: en
    // algunos casos la 1a escritura NACKea pero la lectura ya funciona.
    esp_err_t rst = ms5803_reset_retry();
    ESP_LOGI(TAG, "0x%02X reset -> %s", addr, esp_err_to_name(rst));
    driver_delay_ms(10); // el reset recarga la PROM (~2.8 ms)

    bool any_ok = false;
    for (int i = 0; i < 8; i++)
    {
        err = ms5803_read_prom_word((uint8_t)i, &s_prom[i]);
        if (err != ESP_OK)
        {
            ESP_LOGW(TAG, "0x%02X PROM[%d] read fallo: %s", addr, i, esp_err_to_name(err));
            s_prom[i] = 0;
        }
        else
        {
            any_ok = true;
        }
    }
    ESP_LOGI(TAG, "0x%02X PROM: %04X %04X %04X %04X %04X %04X %04X %04X",
             addr, s_prom[0], s_prom[1], s_prom[2], s_prom[3],
             s_prom[4], s_prom[5], s_prom[6], s_prom[7]);

    if (!any_ok)
    {
        // Ninguna lectura de PROM respondio: el chip no esta comunicando.
        i2c_master_bus_rm_device(s_dev);
        s_dev = NULL;
        return (rst != ESP_OK) ? rst : ESP_ERR_INVALID_RESPONSE;
    }
    return ESP_OK;
}

esp_err_t ms5803_init(i2c_master_bus_handle_t bus_handle)
{
    if (!bus_handle)
        return ESP_ERR_INVALID_ARG;

    s_ready = false;
    s_dev = NULL;

    // Intentamos primero 0x76 (CSB=High), luego 0x77 (CSB=Low)
    uint8_t addr = MS5803_ADDR_CSB_HIGH;
    esp_err_t err = ms5803_try_addr(bus_handle, addr);
    if (err != ESP_OK)
    {
        ESP_LOGW(TAG, "No responde en 0x76, probando 0x77...");
        addr = MS5803_ADDR_CSB_LOW;
        err = ms5803_try_addr(bus_handle, addr);
    }
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "MS5803 no detectado en el bus I2C: %s", esp_err_to_name(err));
        return err;
    }

    // Validamos CRC4 de la PROM
    uint8_t crc_calc = ms5803_crc4(s_prom);
    uint8_t crc_read = (uint8_t)(s_prom[7] & 0x000F);
    if (crc_calc != crc_read)
    {
        ESP_LOGE(TAG, "CRC4 de PROM invalido (calc=0x%X read=0x%X). Coeficientes no confiables.",
                 crc_calc, crc_read);
        // El sensor responde pero la calibracion no es valida
        return ESP_ERR_INVALID_CRC;
    }

    s_ready = true;
    ESP_LOGI(TAG, "MS5803-14BA OK en 0x%02X. C1=%u C2=%u C3=%u C4=%u C5=%u C6=%u",
             addr, s_prom[1], s_prom[2], s_prom[3], s_prom[4], s_prom[5], s_prom[6]);
    return ESP_OK;
}

bool ms5803_is_ready(void)
{
    return s_ready;
}

// -------------------------------------------------------------------
// Lectura y compensacion
// -------------------------------------------------------------------

esp_err_t ms5803_read(float *pressure_kpa, float *temp_c)
{
    if (!s_dev || !s_ready)
        return ESP_ERR_INVALID_STATE;

    uint32_t d1 = 0, d2 = 0;
    esp_err_t err;

    // D1: presion
    err = ms5803_send_cmd(MS5803_CMD_CONV_D1);
    if (err != ESP_OK)
        return err;
    driver_delay_ms(MS5803_CONV_DELAY_MS);
    err = ms5803_read_adc(&d1);
    if (err != ESP_OK)
        return err;

    // D2: temperatura
    err = ms5803_send_cmd(MS5803_CMD_CONV_D2);
    if (err != ESP_OK)
        return err;
    driver_delay_ms(MS5803_CONV_DELAY_MS);
    err = ms5803_read_adc(&d2);
    if (err != ESP_OK)
        return err;

    if (d1 == 0 || d2 == 0)
        return ESP_ERR_INVALID_RESPONSE; // conversion no lista / lectura invalida

    // Coeficientes
    int64_t C1 = s_prom[1];
    int64_t C2 = s_prom[2];
    int64_t C3 = s_prom[3];
    int64_t C4 = s_prom[4];
    int64_t C5 = s_prom[5];
    int64_t C6 = s_prom[6];

    // --- Primer orden (datasheet MS5803-14BA) ---
    int64_t dT = (int64_t)d2 - (C5 << 8);
    int64_t TEMP = 2000 + (dT * C6) / (1LL << 23); // en centesimas de grado

    int64_t OFF = (C2 << 16) + (C4 * dT) / (1LL << 7);
    int64_t SENS = (C1 << 15) + (C3 * dT) / (1LL << 8);

    // --- Segundo orden (compensacion a baja temperatura) MS5803-14BA ---
    int64_t T2 = 0, OFF2 = 0, SENS2 = 0;
    if (TEMP < 2000)
    {
        T2 = (3 * (dT * dT)) / (1LL << 33);
        int64_t d = TEMP - 2000;
        OFF2 = (3 * d * d) / 2;
        SENS2 = (5 * d * d) / 8;
        if (TEMP < -1500)
        {
            int64_t dl = TEMP + 1500;
            OFF2 += 7 * dl * dl;
            SENS2 += 4 * dl * dl;
        }
    }
    TEMP -= T2;
    OFF -= OFF2;
    SENS -= SENS2;

    // Presion: P en unidades de 0.1 mbar
    int64_t P = (((int64_t)d1 * SENS) / (1LL << 21) - OFF) / (1LL << 15);

    // Conversion a SI:
    //   P [0.1 mbar]  ->  mbar = P/10  ->  kPa = mbar/10 = P/100
    //   TEMP [0.01 C] ->  C = TEMP/100
    if (pressure_kpa)
        *pressure_kpa = (float)P / 100.0f;
    if (temp_c)
        *temp_c = (float)TEMP / 100.0f;

    return ESP_OK;
}
