#include "sfm3300.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "sfm3300";

#define SFM3300_ADDR         0x40
#define SFM3300_CMD_START    0x1000 // start continuous measurement
#define SFM3300_CMD_SOFTRST  0x2000 // soft reset

// Conversion (datasheet SFM3300-D, aire/N2/O2):
//   flow[slm] = (valor - offset) / scale
#define SFM3300_OFFSET       32768.0f
#define SFM3300_SCALE        120.0f

static i2c_master_dev_handle_t s_dev = NULL;
static bool s_ready = false;

// CRC-8, polinomio 0x31 (x^8+x^5+x^4+1), init 0xFF, sin XOR final.
static uint8_t sfm3300_crc8(const uint8_t *data, int len)
{
    uint8_t crc = 0xFF;
    for (int i = 0; i < len; i++)
    {
        crc ^= data[i];
        for (int b = 0; b < 8; b++)
            crc = (crc & 0x80) ? (uint8_t)((crc << 1) ^ 0x31) : (uint8_t)(crc << 1);
    }
    return crc;
}

static esp_err_t sfm3300_send_cmd(uint16_t cmd)
{
    uint8_t tx[2] = {(uint8_t)(cmd >> 8), (uint8_t)(cmd & 0xFF)};
    return i2c_master_transmit(s_dev, tx, 2, 100);
}

esp_err_t sfm3300_init(i2c_master_bus_handle_t bus_handle)
{
    if (!bus_handle)
        return ESP_ERR_INVALID_ARG;

    s_ready = false;
    s_dev = NULL;

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = SFM3300_ADDR,
        .scl_speed_hz = 100000,
    };
    esp_err_t err = i2c_master_bus_add_device(bus_handle, &dev_cfg, &s_dev);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error agregando SFM3300 al bus I2C: %s", esp_err_to_name(err));
        return err;
    }

    // Soft reset (puede no responder ACK; lo toleramos)
    esp_err_t rst = sfm3300_send_cmd(SFM3300_CMD_SOFTRST);
    vTaskDelay(pdMS_TO_TICKS(100)); // tiempo de reset ~80 ms

    // Arrancar la medicion continua
    err = sfm3300_send_cmd(SFM3300_CMD_START);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "SFM3300 no responde en 0x%02X (start): %s. Revisar 5V/pull-ups.",
                 SFM3300_ADDR, esp_err_to_name(err));
        i2c_master_bus_rm_device(s_dev);
        s_dev = NULL;
        return err;
    }
    vTaskDelay(pdMS_TO_TICKS(100)); // start-up ~100 ms

    // La primera lectura tras el reset suele ser invalida (0xFFFF): la descartamos
    uint8_t rx[3] = {0};
    (void)i2c_master_receive(s_dev, rx, 3, 100);

    s_ready = true;
    ESP_LOGI(TAG, "SFM3300-D OK en 0x%02X (rango +-250 slm, softrst=%s)",
             SFM3300_ADDR, esp_err_to_name(rst));
    return ESP_OK;
}

bool sfm3300_is_ready(void)
{
    return s_ready;
}

esp_err_t sfm3300_read(float *slm)
{
    if (!s_dev || !s_ready)
        return ESP_ERR_INVALID_STATE;
    if (!slm)
        return ESP_ERR_INVALID_ARG;

    // En modo continuo basta una lectura I2C de 3 bytes (2 datos + CRC).
    uint8_t rx[3] = {0};
    esp_err_t err = i2c_master_receive(s_dev, rx, 3, 100);
    if (err != ESP_OK)
        return err; // NACK -> aun no hay dato nuevo valido

    if (sfm3300_crc8(rx, 2) != rx[2])
        return ESP_ERR_INVALID_CRC;

    uint16_t raw = ((uint16_t)rx[0] << 8) | rx[1];
    if (raw == 0xFFFF)
        return ESP_ERR_INVALID_RESPONSE; // dato marcado invalido

    *slm = ((float)raw - SFM3300_OFFSET) / SFM3300_SCALE;
    return ESP_OK;
}
