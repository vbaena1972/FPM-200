#include "ads1115.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "ads1115";

#define ADS1115_ADDR 0x48 // ADDR a GND

// Punteros de registro
#define ADS1115_REG_CONVERSION 0x00
#define ADS1115_REG_CONFIG     0x01

// Campos del registro CONFIG (16 bits)
#define ADS1115_OS_SINGLE    (1u << 15) // iniciar conversion single-shot

// MUX single-ended: 100=AIN0, 101=AIN1, 110=AIN2, 111=AIN3
#define ADS1115_MUX_SE_BASE  0x04       // 100b
// PGA: 001 = +-4.096 V
#define ADS1115_PGA_4_096V   (0x01u << 9)
// MODE: 1 = single-shot
#define ADS1115_MODE_SINGLE  (1u << 8)
// Data rate: 100 = 128 SPS
#define ADS1115_DR_128SPS    (0x04u << 5)
// Comparador deshabilitado
#define ADS1115_COMP_DISABLE 0x03

// LSB con PGA +-4.096 V = 4.096 / 32768 = 125 uV
#define ADS1115_LSB_VOLTS (4.096f / 32768.0f)

static i2c_master_dev_handle_t s_dev = NULL;
static i2c_master_bus_handle_t s_bus = NULL; // guardado para poder recuperar el device
static bool s_ready = false;

static esp_err_t ads1115_write_reg(uint8_t reg, uint16_t val)
{
    uint8_t tx[3] = {reg, (uint8_t)(val >> 8), (uint8_t)(val & 0xFF)};
    // Timeout corto para no bloquear el bus 1 compartido si se cuelga.
    return i2c_master_transmit(s_dev, tx, 3, 50);
}

static esp_err_t ads1115_read_reg(uint8_t reg, uint16_t *val)
{
    uint8_t rx[2] = {0};
    esp_err_t err = i2c_master_transmit_receive(s_dev, &reg, 1, rx, 2, 50);
    if (err != ESP_OK)
        return err;
    *val = ((uint16_t)rx[0] << 8) | rx[1];
    return ESP_OK;
}

esp_err_t ads1115_init(i2c_master_bus_handle_t bus_handle)
{
    if (!bus_handle)
        return ESP_ERR_INVALID_ARG;

    s_ready = false;
    s_dev = NULL;
    s_bus = bus_handle;

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = ADS1115_ADDR,
        .scl_speed_hz = 100000,
    };
    esp_err_t err = i2c_master_bus_add_device(bus_handle, &dev_cfg, &s_dev);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error agregando ADS1115 al bus I2C: %s", esp_err_to_name(err));
        return err;
    }

    // Verificamos comunicacion leyendo el registro de config
    uint16_t cfg = 0;
    err = ads1115_read_reg(ADS1115_REG_CONFIG, &cfg);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "ADS1115 no responde en 0x%02X: %s", ADS1115_ADDR, esp_err_to_name(err));
        i2c_master_bus_rm_device(s_dev);
        s_dev = NULL;
        return err;
    }

    s_ready = true;
    ESP_LOGI(TAG, "ADS1115 OK en 0x%02X (config=0x%04X, PGA=+-4.096V)", ADS1115_ADDR, cfg);
    return ESP_OK;
}

bool ads1115_is_ready(void)
{
    return s_ready;
}

esp_err_t ads1115_recover(void)
{
    if (!s_bus)
        return ESP_ERR_INVALID_STATE;

    // Soltar el device actual (ignoramos error: puede estar ya invalido).
    if (s_dev)
    {
        i2c_master_bus_rm_device(s_dev);
        s_dev = NULL;
    }
    s_ready = false;

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = ADS1115_ADDR,
        .scl_speed_hz = 100000,
    };
    esp_err_t err = i2c_master_bus_add_device(s_bus, &dev_cfg, &s_dev);
    if (err != ESP_OK)
    {
        s_dev = NULL;
        return err;
    }

    // Reverificamos leyendo el registro de config.
    uint16_t cfg = 0;
    err = ads1115_read_reg(ADS1115_REG_CONFIG, &cfg);
    if (err != ESP_OK)
    {
        i2c_master_bus_rm_device(s_dev);
        s_dev = NULL;
        return err;
    }

    s_ready = true;
    ESP_LOGW(TAG, "ADS1115 recuperado (re-add device, config=0x%04X)", cfg);
    return ESP_OK;
}

// Numero de reintentos ante errores I2C transitorios en el bus compartido.
#define ADS1115_IO_RETRIES 4
// Backoff creciente entre intentos: un NACK del ADS suele durar varios ms.
#define ADS1115_RETRY_BACKOFF_MS 8

static esp_err_t ads1115_read_raw_once(ads1115_channel_t ch, int16_t *raw)
{
    uint16_t mux = (uint16_t)((ADS1115_MUX_SE_BASE + (uint8_t)ch) << 12);
    uint16_t config = ADS1115_OS_SINGLE | mux | ADS1115_PGA_4_096V |
                      ADS1115_MODE_SINGLE | ADS1115_DR_128SPS | ADS1115_COMP_DISABLE;

    esp_err_t err = ads1115_write_reg(ADS1115_REG_CONFIG, config);
    if (err != ESP_OK)
        return err;

    // A 128 SPS la conversion tarda ~7.8 ms. Esperamos con margen y confirmamos OS=1.
    vTaskDelay(pdMS_TO_TICKS(10));

    bool done = false;
    for (int i = 0; i < 8; i++)
    {
        uint16_t st = 0;
        err = ads1115_read_reg(ADS1115_REG_CONFIG, &st);
        if (err != ESP_OK)
            return err;
        if (st & ADS1115_OS_SINGLE) // OS=1 -> conversion terminada
        {
            done = true;
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(2));
    }
    if (!done)
        return ESP_ERR_TIMEOUT; // la conversion no termino a tiempo

    uint16_t conv = 0;
    err = ads1115_read_reg(ADS1115_REG_CONVERSION, &conv);
    if (err != ESP_OK)
        return err;

    *raw = (int16_t)conv;
    return ESP_OK;
}

esp_err_t ads1115_read_raw(ads1115_channel_t ch, int16_t *raw)
{
    if (!s_dev || !s_ready)
        return ESP_ERR_INVALID_STATE;
    if (!raw)
        return ESP_ERR_INVALID_ARG;
    if (ch > ADS1115_MUX_AIN3)
        return ESP_ERR_INVALID_ARG;

    // Reintentar ante glitches transitorios del bus 1 compartido (MS5803 + ADS).
    esp_err_t err = ESP_FAIL;
    for (int attempt = 0; attempt < ADS1115_IO_RETRIES; attempt++)
    {
        err = ads1115_read_raw_once(ch, raw);
        if (err == ESP_OK)
            return ESP_OK;
        vTaskDelay(pdMS_TO_TICKS(ADS1115_RETRY_BACKOFF_MS * (attempt + 1)));
    }
    ESP_LOGW(TAG, "ads1115_read_raw ch=%d fallo tras %d intentos: %s",
             (int)ch, ADS1115_IO_RETRIES, esp_err_to_name(err));
    return err;
}

esp_err_t ads1115_read_voltage(ads1115_channel_t ch, float *volts)
{
    if (!volts)
        return ESP_ERR_INVALID_ARG;
    int16_t raw = 0;
    esp_err_t err = ads1115_read_raw(ch, &raw);
    if (err != ESP_OK)
        return err;
    *volts = (float)raw * ADS1115_LSB_VOLTS;
    return ESP_OK;
}
