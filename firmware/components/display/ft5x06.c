/* MIT License */
#include "ft5x06.h"
#include "esp_lcd_touch_ft5x06.h"
#include "esp_lcd_panel_io.h"    // esp_lcd_panel_io_tx_param (config G_MODE)
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"         // gpio_get_level (INT-gating del touch)
#include "sensors_runtime.h"     // sensors_runtime_bus1_hung()

// FT5x06: registro de modo de interrupción (ID_G_MODE). 0 = POLLING (INT
// permanece BAJO mientras hay dedo), 1 = TRIGGER (solo un pulso por evento).
#define FT5X06_REG_G_MODE  0xA4
#define FT5X06_G_MODE_POLL 0x00

esp_err_t fpm_new_panel_io_i2c(i2c_master_bus_handle_t bus, const esp_lcd_panel_io_i2c_config_t *cfg, esp_lcd_panel_io_handle_t *out);

static const char *TAG = "ft5x06:";

static lv_indev_t *disp_indev = NULL;
static esp_lcd_touch_handle_t tp; // LCD touch handle

static SemaphoreHandle_t refresh_finish __attribute__((unused)) = NULL;

// Lectura de touch TOLERANTE a errores de I2C. Reemplaza al read interno de
// esp_lvgl_port (lvgl_port_touchpad_read), que hacia ESP_ERROR_CHECK sobre
// esp_lcd_touch_read_data: un solo glitch del bus 1 compartido -> abort() y
// REBOOT del equipo. Aqui un error I2C se trata como "sin toque".
// 1.5.34: pero con el dedo apoyado un glitch aislado devolvia RELEASED, LVGL
// veia soltar/apoyar y generaba DOS clics (el PIN "1" salia "11", HW
// 2026-10-11). Ahora un glitch repite el ultimo estado; si dura mas de
// TOUCH_GLITCH_HOLD lecturas seguidas (~90 ms) se declara soltado.
#define TOUCH_GLITCH_HOLD 3
static lv_indev_state_t s_last_state = LV_INDEV_STATE_RELEASED;
static lv_point_t s_last_point;
static uint8_t s_glitch_count;

// 1.6.10: el INT (GPIO39, flanco de bajada) va por INTERRUPCION. Antes solo se
// miraba el NIVEL del pin cada 20 ms: si el FT5x06 entrega el INT como pulso corto
// (por cada reporte) la mayoria de los toques caian entre lecturas -> habia que
// tocar varias veces y el toque se sentia lento (sobre todo en Informacion/OTA).
// Ahora la ISR marca "toque pendiente" y DESPIERTA a taskLVGL al instante; desde
// ahi se lee por I2C hasta que el propio chip reporta 0 puntos (el soltado lo
// decide el dato, no el nivel del pin). En reposo sigue sin trafico I2C.
static volatile bool s_touch_pending;   // lo pone la ISR
static bool s_touch_active;             // hay dedo segun el ultimo dato del chip

static void touch_isr(esp_lcd_touch_handle_t h)
{
    (void)h;
    s_touch_pending = true;
    lvgl_port_task_wake(LVGL_PORT_EVENT_TOUCH, NULL);
}

static void touch_glitch(lv_indev_data_t *data)
{
    if (s_last_state == LV_INDEV_STATE_PRESSED && s_glitch_count < TOUCH_GLITCH_HOLD)
    {
        s_glitch_count++;
        data->point = s_last_point;
        data->state = LV_INDEV_STATE_PRESSED;
        return;
    }
    s_last_state = LV_INDEV_STATE_RELEASED;
    s_touch_active = false;
    data->state = LV_INDEV_STATE_RELEASED;
}

static void ft5x06_lvgl_read_cb(lv_indev_t *indev, lv_indev_data_t *data)
{
    (void)indev;
    esp_lcd_touch_point_data_t pt = {0};
    uint8_t cnt = 0;

    // Sin toque pendiente (ISR), sin dedo activo y con el INT en alto: reposo,
    // no se toca el bus I2C (compartido con los sensores).
    const bool int_low = gpio_get_level(BSP_LCD_TP_INT) == 0;
    if (!s_touch_pending && !s_touch_active && !int_low)
    {
        s_last_state = LV_INDEV_STATE_RELEASED;
        s_glitch_count = 0;
        data->state = LV_INDEV_STATE_RELEASED;
        return;
    }
    s_touch_pending = false;

    // Avoid extra traffic while sensors recover the bus. All touch transfers
    // also have finite deadlines: a fault after this check cannot hang LVGL.
    if (sensors_runtime_bus1_hung())
    {
        touch_glitch(data);
        return;
    }

    if (esp_lcd_touch_read_data(tp) != ESP_OK)
    {
        // Glitch I2C con el INT asertado: no inventar un "soltado".
        touch_glitch(data);
        return;
    }

    // esp_lcd_touch_get_data: replacement for the deprecated get_coordinates
    // (same coordinate processing; returns ESP_OK with cnt=0 when not touched).
    s_glitch_count = 0;
    if (esp_lcd_touch_get_data(tp, &pt, &cnt, 1) == ESP_OK && cnt > 0)
    {
        data->point.x = pt.x;
        data->point.y = pt.y;
        data->state = LV_INDEV_STATE_PRESSED;
        s_last_point = data->point;
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }
    s_last_state = data->state;
    s_touch_active = data->state == LV_INDEV_STATE_PRESSED;
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
    // 1.6.10: con el dedo, un toque se desliza unos px; con el limite por defecto
    // (10 px) en pantallas desplazables (Informacion) el toque se volvia "scroll" y
    // el boton no respondia. 20 px sigue permitiendo desplazar normalmente.
    lv_indev_set_scroll_limit(disp_indev, 20);
    lvgl_port_unlock();

    // 1.6.10: INT por interrupcion (flanco de bajada, ya configurado por el driver).
    // Si falla, se sigue con el muestreo del nivel (comportamiento anterior).
    esp_err_t ierr = esp_lcd_touch_register_interrupt_callback(tp, touch_isr);
    if (ierr != ESP_OK)
        ESP_LOGW(TAG, "INT del touch sin interrupcion (%s): se usa muestreo", esp_err_to_name(ierr));
    else
        ESP_LOGI(TAG, "INT del touch por interrupcion (GPIO%d)", (int)BSP_LCD_TP_INT);

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
    esp_err_t err = fpm_new_panel_io_i2c(touch_handle, &tp_io_config, &tp_io_handle);
    if (err != ESP_OK)
        return err;

    // No abortamos si el touch no responde (p.ej. panel no conectado): el resto
    // del firmware (sensores, red, etc.) debe seguir funcionando.
    err = esp_lcd_touch_new_i2c_ft5x06(tp_io_handle, &tp_cfg, ret_touch);
    if (err != ESP_OK)
    {
        ESP_LOGW(TAG, "Touch FT5x06 no disponible (%s). Continuo sin panel tactil.",
                 esp_err_to_name(err));
        esp_lcd_panel_io_del(tp_io_handle);
        if (ret_touch)
            *ret_touch = NULL;
        return err;
    }

    // Forzar modo INT POLLING: el INT-gating por NIVEL del read_cb necesita que el
    // pin permanezca BAJO mientras el dedo está puesto. En el default de fábrica
    // (a menudo TRIGGER/pulso) el gate leía "sin toque" casi siempre -> touch
    // torpe / hay que tocar varias veces. Se escribe 0xA4=0 (best-effort).
    uint8_t gmode = FT5X06_G_MODE_POLL;
    esp_err_t merr = esp_lcd_panel_io_tx_param(tp_io_handle, FT5X06_REG_G_MODE,
                                               &gmode, 1);
    if (merr != ESP_OK)
        ESP_LOGW(TAG, "No se pudo fijar G_MODE=polling (%s)", esp_err_to_name(merr));
    else
        ESP_LOGI(TAG, "FT5x06 en modo INT POLLING (nivel)");

    return ESP_OK;
}
