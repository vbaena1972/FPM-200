/* MIT License */
#include "ft5x06.h"
#include "esp_lcd_touch_ft5x06.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"         // gpio_get_level (INT-gating del touch)
#include "sensors_runtime.h"     // sensors_runtime_bus1_hung()

static const char *TAG = "ft5x06:";

static lv_indev_t *disp_indev = NULL;
static esp_lcd_touch_handle_t tp; // LCD touch handle

static SemaphoreHandle_t refresh_finish __attribute__((unused)) = NULL;

// Lectura de touch TOLERANTE a errores de I2C. Reemplaza al read interno de
// esp_lvgl_port (lvgl_port_touchpad_read), que hacia ESP_ERROR_CHECK sobre
// esp_lcd_touch_read_data: un solo glitch del bus 1 compartido -> abort() y
// REBOOT del equipo. Aqui un error I2C se trata como "sin toque".
static void ft5x06_lvgl_read_cb(lv_indev_t *indev, lv_indev_data_t *data)
{
    (void)indev;
    uint16_t x = 0, y = 0, strength = 0;
    uint8_t cnt = 0;

    // --- INT-gating: solo leemos el touch por I2C cuando el pin INT (GPIO39,
    // activo en BAJO) esta asertado, es decir, cuando hay un dedo en la pantalla.
    // En reposo (INT en alto) NO tocamos el bus I2C -> eliminamos el sondeo a
    // ~30 Hz que satura el bus 1 compartido con los sensores (causa probable del
    // cuelgue) y garantizamos que en reposo nunca bloqueamos el mutex de display.
    if (gpio_get_level(BSP_LCD_TP_INT) != 0)
    {
        data->state = LV_INDEV_STATE_RELEASED;
        return;
    }

    // --- Guarda anti-deadlock del bus I2C 1 (compartido con los sensores) ---
    // Con un dedo puesto SI leemos por I2C. esp_lcd_touch_read_data() ->
    // esp_lcd_panel_io_i2c usa i2c_master_transmit_receive con timeout INFINITO
    // (-1); si el bus se cuelga esa lectura NO retorna nunca y esta funcion corre
    // con el mutex de LVGL/display tomado -> UI congelada y buzzer atascado. Si el
    // subsistema de sensores ya detecto el bus colgado (ambos MS5803+ADS caidos),
    // NO tocamos el bus. (No sondeamos con i2c_master_probe: hacerlo desde esta
    // tarea concurrente con las transacciones de la tarea de sensores corrompia el
    // driver I2C y provocaba un panic StoreProhibited en el ISR de recepcion.)
    if (sensors_runtime_bus1_hung())
    {
        data->state = LV_INDEV_STATE_RELEASED;
        return;
    }

    if (esp_lcd_touch_read_data(tp) != ESP_OK)
    {
        // Glitch I2C: mantenemos el ultimo estado como "soltado" y seguimos.
        data->state = LV_INDEV_STATE_RELEASED;
        return;
    }

    bool pressed = esp_lcd_touch_get_coordinates(tp, &x, &y, &strength, &cnt, 1);
    if (pressed && cnt > 0)
    {
        data->point.x = x;
        data->point.y = y;
        data->state = LV_INDEV_STATE_PRESSED;
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

lv_indev_t *bsp_display_indev_init(lv_display_t *disp, i2c_master_bus_handle_t bus_handle)
{
    // Reintentamos el init del touch: un glitch puntual del bus 1 al arranque
    // (visto en logs) hacia que quedara SIN touch toda la sesion.
    esp_err_t err = ESP_FAIL;
    for (int i = 0; i < 3 && (err != ESP_OK || tp == NULL); i++)
    {
        if (i) vTaskDelay(pdMS_TO_TICKS(30));
        err = bsp_touch_new(NULL, &tp, bus_handle);
    }
    if (err != ESP_OK || tp == NULL)
    {
        // Sin panel tactil conectado: no registramos indev y seguimos.
        ESP_LOGW(TAG, "Sin touch: se omite el registro de entrada LVGL");
        return NULL;
    }
    ESP_LOGI(TAG, "Touch FT5x06 inicializado");

    // Registramos NUESTRO indev (en vez de lvgl_port_add_touch) para usar el
    // read callback tolerante de arriba. El mutex de LVGL es recursivo, asi
    // que tomarlo aqui es seguro aunque el caller ya lo tenga.
    lvgl_port_lock(0);
    disp_indev = lv_indev_create();
    lv_indev_set_type(disp_indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(disp_indev, ft5x06_lvgl_read_cb);
    lv_indev_set_display(disp_indev, disp);
    lvgl_port_unlock();

    return disp_indev;
}

lv_indev_t *bsp_display_get_input_dev(void)
{
    return disp_indev;
}

esp_err_t bsp_touch_new(const bsp_touch_config_t *config, esp_lcd_touch_handle_t *ret_touch, i2c_master_bus_handle_t touch_handle)
{
   // ESP_ERROR_CHECK(i2c_master_get_bus_handle(BSP_I2C_NUM, &touch_handle));
    // Initialize touch
    const esp_lcd_touch_config_t tp_cfg = {
        .x_max = BSP_LCD_V_RES,
        .y_max = BSP_LCD_H_RES,
        .rst_gpio_num = BSP_LCD_TP_RST, // Shared with LCD reset
        .int_gpio_num = BSP_LCD_TP_INT,
        .levels = {
            .reset = 0,
            .interrupt = 0,
        },
        .flags = {
            .swap_xy = 0,
            .mirror_x = 0,
            .mirror_y = 0,
        },
    };
    esp_lcd_panel_io_handle_t tp_io_handle = NULL;

    const esp_lcd_panel_io_i2c_config_t tp_io_config = ESP_LCD_TOUCH_IO_I2C_FT5x06_CONFIG();
    esp_err_t err = esp_lcd_new_panel_io_i2c(touch_handle, &tp_io_config, &tp_io_handle);
    if (err != ESP_OK)
        return err;

    // No abortamos si el touch no responde (p.ej. panel no conectado): el resto
    // del firmware (sensores, red, etc.) debe seguir funcionando.
    err = esp_lcd_touch_new_i2c_ft5x06(tp_io_handle, &tp_cfg, ret_touch);
    if (err != ESP_OK)
    {
        ESP_LOGW(TAG, "Touch FT5x06 no disponible (%s). Continuo sin panel tactil.",
                 esp_err_to_name(err));
        if (ret_touch)
            *ret_touch = NULL;
        return err;
    }

    return ESP_OK;
}
