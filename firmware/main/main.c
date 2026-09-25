#include "flow_meter.h"
#include "fpm_i2c_guard.h"
#include "esp_task_wdt.h"
#include "cJSON.h"
#include "esp_core_dump.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

#include "esp_log.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_vfs_fat.h"
#include "esp_heap_trace.h"
#include "esp_heap_caps.h"
#include "esp_app_trace.h"
//#include "segger_sysview.h"
#include "esp_trace.h"

#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "nvs_flash.h"
#include "pinmap.h"
#include "lvgl.h"
#include "ui.h"
#include "ui_sd.h"
#include "driver/i2c_master.h"
#include "driver/sdmmc_host.h"
#include "driver/gpio.h"
#include "ft5x06.h"
#include "st7796.h"

#include "sdmmc_cmd.h"
#include "mem_diag.h"
#include "task_tracer.h"
#include "ui_wifi_main_icon.h"
#include "ui_statusbar_controller.h"
#include "wifi_mgr.h"
#include "eth_mgr.h"
#include "storage.h"
#include "metrics_store.h"
#include "net_core.h"
#include "cert_store.h"
#include "sensors_runtime.h"
#include "transport_ble.h"
#include "fpm_ble_config.h"
#include "state_pub.h"
#include "http_api.h"
#include "mdns_svc.h"
#include "lwip/ip_addr.h"
#include "rtc_rv3028.h"
#include "ms5803.h"
#include "bmp280.h"
#include "ads1115.h"
#include "at24c256.h"
#include "sfm3300.h"
#include "alarm_mgr.h"
#include "transport_mqtt.h"
#include "time_mgr.h"

// Adaptador: al cambiar el estado clinico de alarma, forzamos publish inmediato
// a la nube. La firma incluye el estado (no lo usamos: la telemetria lee el
// estado real al construir el payload), pero cumple el prototipo del callback.
static void on_alarm_state_change(alarm_clinical_state_t new_state)
{
    (void)new_state;
    transport_mqtt_publish_now();
}

// #define SD_CD_PIN 42
//  Tareas de Procesamiento de Datos (Workers)
#define PRIO_BLE_WORKER 4 // Procesar JSON rÃƒÂ¡pido, pero dejar renderizar a LVGL

// Tareas de LÃƒÂ³gica de Interfaz (Refresco)
#define PRIO_UI_LOGIC 2 // BAJA. Debe ser MENOR que la tarea de LVGL para evitar deadlock.

// Tareas de InicializaciÃƒÂ³n (Corren una vez y mueren/suspenden)
#define PRIO_INIT_TASK 3 // Media.

//#define MY_TASK_STACK_WORDS 16384
#define MY_TASK_STACK_WORDS (16384 / sizeof(StackType_t))

#define BLE_TASK_STACK_SIZE 4096

typedef struct
{
    TaskHandle_t handle;
    StackType_t *stack_buf;
    StaticTask_t *tcb_buf;
} init_task_ctx_t;

static init_task_ctx_t screen_task_ctx;
static init_task_ctx_t eth_task_ctx;
static init_task_ctx_t wifi_task_ctx;
static init_task_ctx_t ble_task_ctx;
static EventGroupHandle_t init_events;
#define EVT_SCREEN_DONE (1 << 0)
#define EVT_WIFI_INIT_DONE (1 << 1)
#define EVT_ETH_INIT_DONE (1 << 2)
#define EVT_BT_INIT_DONE (1 << 3)
// Variable global para guardar la referencia de la tarjeta (necesaria para desmontarla luego)
sdmmc_card_t *s_sd_card = NULL;
/* true mientras el overlay de microSD está en pantalla o el flujo se está
 * procesando. Bloquea que la interrupción CD (que rebota mecánicamente varias
 * veces por inserción) vuelva a disparar el montaje -> evita el storm de
 * "0x105 no available sd host controller". Lo limpia el cierre del overlay. */
static volatile bool s_sd_flow_active = false;

i2c_master_bus_handle_t bus_handle;
i2c_master_bus_handle_t sfm_bus_handle; // 2do bus I2C (SFM3300 en GPIO43/44)
static const char *TAG = "app";
extern void ui_wifi_main_icon_init(lv_obj_t *img_obj);

static bool wifi_cfg_valid = false;

/* I2C */
#define BSP_I2C_SCL (GPIO_NUM_15)
#define BSP_I2C_SDA (GPIO_NUM_16)
#define BSP_I2C_PORT 1

// DefiniciÃƒÂ³n de la macro de seguridad: Si el objeto es NULL, salta y evita un potencial crash.
#define LV_OBJ_GUARD(obj)                                        \
    if ((obj) == NULL)                                           \
    {                                                            \
        ESP_LOGW("ui_refresh", "LVGL Object is NULL: %s", #obj); \
        vTaskDelay(pdMS_TO_TICKS(10));                           \
        continue;                                                \
    }

static SemaphoreHandle_t s_sd_sem = NULL;
/* La UI del flujo SD vive ahora en main/ui/ui_sd.c (overlay modal del design system). */

static volatile TickType_t s_ui_progress;
static void ui_refresh_task(void *arg);

// ISR (Manejador de la interrupciÃƒÂ³n por hardware)
static void IRAM_ATTR sd_cd_isr_handler(void *arg)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    // Si el pin pasa a bajo (0) significa que la tarjeta entrÃƒÂ³ (comportamiento comÃƒÂºn de SD_CD)
    /* Si ya hay un flujo activo (overlay abierto o procesando) ignoramos los
     * rebotes de la interrupción CD: no re-disparamos el montaje. */
    if (!s_sd_flow_active && gpio_get_level(SD_CD_PIN) == 0)
    {
        xSemaphoreGiveFromISR(s_sd_sem, &xHigherPriorityTaskWoken);
    }
    if (xHigherPriorityTaskWoken)
    {
        portYIELD_FROM_ISR();
    }
}

// Evento del botÃƒÂ³n de reinicio en LVGL
static void restart_btn_event_cb(lv_event_t *e)
{
    ESP_LOGW("MAIN", "Reiniciando equipo por confirmaciÃƒÂ³n del usuario...");
    esp_restart();
}

// FunciÃƒÂ³n obligatoria en ESP-IDF v6 para enlazar SystemView y AppTrace
esp_trace_open_params_t esp_trace_get_user_params(void)
{
    esp_trace_open_params_t trace_params = {
        .core_cfg = NULL,
        .encoder_name = "sysview", // <--- Esto corrige el error 'ext'
        .encoder_cfg = NULL,
        .transport_name = "apptrace",
        .transport_cfg = NULL,
    };
    return trace_params;
}


static esp_err_t bsp_i2c_init(void)
{
    i2c_master_bus_config_t cfg = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = BSP_I2C_PORT,
        .scl_io_num = BSP_I2C_SCL,
        .sda_io_num = BSP_I2C_SDA,
        .glitch_ignore_cnt = 1,
        .flags.enable_internal_pullup = true,
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&cfg, &bus_handle));
    ESP_LOGI(TAG, "I2C master listo");
    return ESP_OK;
}

void on_network_ready(void)
{
    ESP_ERROR_CHECK(http_api_init());
    /* mDNS: publica <serial>.local para que la app resuelva la IP viva en cada
     * apertura (inmune a cambios de DHCP). Réplica de MedGuard. */
    const AppConfig *cfg = appcfg_cache_peek();
    esp_err_t merr = mdns_svc_start(cfg ? cfg->general.serial : NULL);
    if (merr != ESP_OK) {
        ESP_LOGW(TAG, "mDNS no iniciado: %s", esp_err_to_name(merr));
    }
}

esp_err_t mount_sdcard_hotplug(void)
{
    ESP_LOGI("SD_CD", "Inicializando hardware SD...");

    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    host.flags = SDMMC_HOST_FLAG_1BIT;

    sdmmc_slot_config_t slot = SDMMC_SLOT_CONFIG_DEFAULT();
    slot.width = 1;
#if SOC_SDMMC_USE_GPIO_MATRIX
    slot.clk = 45;
    slot.cmd = 0;
    slot.d0 = 21;
#endif

    const esp_vfs_fat_sdmmc_mount_config_t mnt = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 0,
        .disk_status_check_enable = true, // Ideal para sistemas Hot-Plug
    };

    return esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slot, &mnt, &s_sd_card);
}

void unmount_sdcard_hotplug(void)
{
    if (s_sd_card)
    {
        esp_vfs_fat_sdcard_unmount("/sdcard", s_sd_card);
        s_sd_card = NULL;
        ESP_LOGI("SD_CD", "Tarjeta desmontada de forma segura.");
    }
}

/* Limpia el flujo de la microSD: desmonta (idempotente), drena los rebotes de la
 * CD que se acumularon y baja la bandera para permitir una futura inserción. */
static void sd_flow_clear(void)
{
    unmount_sdcard_hotplug();
    if (s_sd_sem)
    {
        while (xSemaphoreTake(s_sd_sem, 0) == pdTRUE) { /* descarta rebotes */ }
    }
    s_sd_flow_active = false;
}

/* Callback del botón "Cerrar" del overlay (registrado con ui_sd_set_close_cb).
 * Corre en el contexto de LVGL: cierra el overlay y libera el flujo. */
static void sd_ui_close_cb(lv_event_t *e)
{
    (void)e;
    ui_sd_close();
    sd_flow_clear();
    ESP_LOGI("SD_CD", "Overlay microSD cerrado por el usuario.");
}

static void sd_monitor_task(void *pvParameters)
{
    s_sd_sem = xSemaphoreCreateBinary();

    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << SD_CD_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = 1,
        .pull_down_en = 0,
        .intr_type = GPIO_INTR_NEGEDGE // Flanco de bajada
    };
    gpio_config(&io_conf);

    // =========================================================
    // INSTALACIÃƒâ€œN SEGURA DEL SERVICIO ISR
    // =========================================================
    esp_err_t isr_err = gpio_install_isr_service(0);
    // Si da ESP_ERR_INVALID_STATE significa que ya estaba instalado, lo ignoramos.
    if (isr_err != ESP_OK && isr_err != ESP_ERR_INVALID_STATE)
    {
        ESP_LOGE("SD_CD", "Fallo al instalar servicio ISR: %s", esp_err_to_name(isr_err));
    }

    gpio_isr_handler_add(SD_CD_PIN, sd_cd_isr_handler, NULL);
    // =========================================================

    /* El botón "Cerrar" del overlay desmonta la SD y libera el flujo. */
    ui_sd_set_close_cb(sd_ui_close_cb);

    while (1)
    {
        if (xSemaphoreTake(s_sd_sem, portMAX_DELAY) == pdTRUE)
        {
            // 1. DEBOUNCE: Esperamos 500ms a que pase el ruido elÃƒÂ©ctrico de la inserciÃƒÂ³n
            vTaskDelay(pdMS_TO_TICKS(500));
            if (gpio_get_level(SD_CD_PIN) != 0)
                continue; // Falso contacto, abortar

            /* A partir de aquí bloqueamos nuevos disparos de la CD hasta que el
             * flujo termine y se cierre el overlay (sd_flow_clear). Drenamos los
             * rebotes que la ISR encoló durante el debounce para que la rama de
             * "reinicio pendiente" (que no vuelve a bloquear en el semáforo) no
             * consuma un disparo viejo y re-monte. */
            s_sd_flow_active = true;
            while (xSemaphoreTake(s_sd_sem, 0) == pdTRUE) { /* descarta rebotes */ }

            ESP_LOGW("SD_CD", "Ã‚Â¡Tarjeta MicroSD insertada (Hot-Plug)!");

            // 2. MONTAJE: Ã‚Â¡AFUERA DEL MUTEX!
            esp_err_t r = mount_sdcard_hotplug();
            if (r != ESP_OK)
            {
                ESP_LOGE("SD_CD", "Fallo al inicializar SD. Error: %d (%s)", r, esp_err_to_name(r));
                /* Muestra el fallo en pantalla con botón "Cerrar" (antes el
                 * montaje fallido no mostraba NADA -> el usuario no sabía qué
                 * pasaba). El flujo queda activo hasta que el usuario cierre. */
                if (bsp_display_lock(portMAX_DELAY))
                {
                    ui_sd_show();
                    ui_sd_error("No se pudo montar la microSD. Revísela y reinsértela.");
                    bsp_display_unlock();
                }
                continue; // el overlay de error queda; "Cerrar" libera el flujo
            }
            else
            {
                ESP_LOGI("SD_CD", "Sistema de archivos de la SD inicializado con ÃƒÂ©xito.");
            }

            // 3. UI: Ã‚Â¡ADENTRO DEL MUTEX!
            if (bsp_display_lock(portMAX_DELAY))
            {
                ui_sd_show();
                ui_sd_progress(10, "Tarjeta detectadaÃ¢â‚¬Â¦");
                bsp_display_unlock();
            }

            bool debede_actualizar = false;

            // 4. LECTURA DE ARCHIVOS: Ã‚Â¡AFUERA DEL MUTEX!
            DIR *dir = opendir("/sdcard");
            if (dir)
            {
                struct dirent *ent;
                while ((ent = readdir(dir)) != NULL)
                {
                    ESP_LOGI("SD_CD", "Archivo SD: %s", ent->d_name);
                }
                closedir(dir);
            }

            FILE *f_cfg = fopen("/sdcard/AppConfig.json", "rb");
            if (f_cfg)
            {
                if (bsp_display_lock(portMAX_DELAY))
                {
                    ui_sd_progress(50, "Aplicando AppConfig.jsonÃ¢â‚¬Â¦");
                    bsp_display_unlock();
                }

                fseek(f_cfg, 0, SEEK_END);
                long size = ftell(f_cfg);
                fseek(f_cfg, 0, SEEK_SET);

                // ftell() may fail (-1); cap size so a bad file cannot exhaust RAM.
                char *buf = (size > 0 && size <= 64 * 1024) ? malloc(size + 1) : NULL;
                if (!buf)
                    ESP_LOGE("SD_CD", "AppConfig.json invalido o demasiado grande (%ld bytes)", size);
                if (buf)
                {
                    size_t bytes_read = fread(buf, 1, size, f_cfg);
                    buf[bytes_read] = '\0';

                    char *json_ptr = buf;
                    // Sanitizador BOM UTF-8
                    if (bytes_read >= 3 && (unsigned char)json_ptr[0] == 0xEF && (unsigned char)json_ptr[1] == 0xBB && (unsigned char)json_ptr[2] == 0xBF)
                    {
                        json_ptr += 3;
                    }

                    // Sanitizador de Espacios Duros
                    char *r_ptr = json_ptr, *w_ptr = json_ptr;
                    while (*r_ptr)
                    {
                        if ((unsigned char)*r_ptr == 0xC2 && (unsigned char)*(r_ptr + 1) == 0xA0)
                        {
                            *w_ptr++ = ' ';
                            r_ptr += 2;
                        }
                        else if ((unsigned char)*r_ptr == 0xA0)
                        {
                            *w_ptr++ = ' ';
                            r_ptr++;
                        }
                        else
                        {
                            *w_ptr++ = *r_ptr++;
                        }
                    }
                    *w_ptr = '\0';

                    AppConfig *temp_cfg = malloc(sizeof(AppConfig));
                    if (temp_cfg)
                    {
                        appcfg_load(temp_cfg);
                        cJSON *root = cJSON_Parse(json_ptr);
                        if (root)
                        {
                            cfg_from_json(temp_cfg, root);
                            appcfg_save(temp_cfg);
                            cJSON_Delete(root);
                            debede_actualizar = true;
                            ESP_LOGI("SD_CD", "NVS AppConfig actualizada con ÃƒÂ©xito.");
                        }
                        else
                        {
                            ESP_LOGE("SD_CD", "Fallo cJSON_Parse. Error: [%s]", cJSON_GetErrorPtr());
                        }
                        free(temp_cfg);
                    }
                    free(buf);
                }
                fclose(f_cfg);
                rename("/sdcard/AppConfig.json", "/sdcard/AppConfig.bak");
            }

            if (bsp_display_lock(portMAX_DELAY))
            {
                ui_sd_progress(70, "Importando certificados AWSÃ¢â‚¬Â¦");
                bsp_display_unlock();
            }

            // 5. IMPORTAR CERTIFICADOS: Ã‚Â¡AFUERA DEL MUTEX!
            if (cert_store_import_from_sd("/sdcard/aws") == ESP_OK)
            {
                debede_actualizar = true;
                ESP_LOGI("SD_CD", "Certificados procesados.");
            }

            if (bsp_display_lock(portMAX_DELAY))
            {
                ui_sd_progress(100, "Completado");
                bsp_display_unlock();
            }
            vTaskDelay(pdMS_TO_TICKS(500));

            // 6. DESMONTAJE INMEDIATO: todo lo leído (AppConfig -> NVS, certs ->
            //    NVS) ya está persistido, así que liberamos el host SDMMC AHORA.
            //    Antes se dejaba la tarjeta montada esperando el reinicio -> el
            //    host quedaba ocupado y los rebotes de la CD provocaban el storm
            //    "0x105 no available sd host controller".
            unmount_sdcard_hotplug();

            // 7. CONCLUSIÃƒâ€œN (UI ADENTRO DEL MUTEX)
            if (debede_actualizar)
            {
                if (bsp_display_lock(portMAX_DELAY))
                {
                    ui_sd_finish_restart(restart_btn_event_cb);
                    bsp_display_unlock();
                }
                /* El overlay queda con "Aplicar y reiniciar" + "Cerrar"; el flujo
                 * sigue activo (s_sd_flow_active=true) hasta que el usuario elija.
                 * La CD queda bloqueada mientras tanto. */
                ESP_LOGW("SD_CD", "Cambios aplicados. Esperando reinicio o cierre del usuario.");
            }
            else
            {
                if (bsp_display_lock(portMAX_DELAY))
                {
                    ui_sd_close();
                    bsp_display_unlock();
                }
                ESP_LOGI("SD_CD", "No se actualizÃƒÂ³ nada. Cerrando overlay.");
                sd_flow_clear(); // drena rebotes + libera la bandera
            }
        }
    }
}

__attribute__((unused)) static void ui_main_deinit_cb(lv_event_t *e)
{
    ui_wifi_main_icon_deinit();
    ui_statusbar_controller_deinit();
}

// I2C bus-1 devices + calibration EEPROM (+ bus-2 SFM3300). Runs before touch
// is started so the calibration read has no touch traffic on the shared bus.
static void init_bus1_sensors(void)
{
    // Sensores reales y EEPROM de calibraciÃ³n (comparten el mismo bus I2C).
    // Los handles quedan estÃ¡ticos dentro de cada driver; sensors_runtime
    // los usa mÃ¡s adelante en su tarea de adquisiciÃ³n.
    // Escaneo del bus I2C para diagnostico (que direcciones responden ACK).
    {
        char found[128];
        int n = 0;
        found[0] = '\0';
        for (uint16_t a = 0x08; a <= 0x77; a++)
        {
            if (i2c_master_probe(bus_handle, a, 50) == ESP_OK)
            {
                n += snprintf(found + n, sizeof(found) - n, "0x%02X ", a);
                if (n >= (int)sizeof(found) - 6)
                    break;
            }
        }
        ESP_LOGI(TAG, "I2C scan: dispositivos detectados => %s", n ? found : "(ninguno)");
    }

    at24c256_init(bus_handle, 0); // EEPROM AT24C256C @ 0x50 (parametros de calibraciÃ³n)
    sensors_runtime_preload_calibration(); // Read/verify before starting touch or acquisition.
    ms5803_init(bus_handle);      // Sensor de presiÃ³n/temperatura MS5803-14BA @ 0x76
    ads1115_init(bus_handle);     // ADC del sensor de flujo FS7 @ 0x48 (AIN0)
    bmp280_init(bus_handle, 0x77); // BMP280/BME280 @ 0x77: referencia atmosferica (cero de presion)

    // Segundo bus I2C (GPIO43=SDA, GPIO44=SCL) para el caudalimetro de
    // referencia Sensirion SFM3300-D (para calibrar/observar el FS7).
    i2c_master_bus_config_t sfm_cfg = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = SFM_I2C_PORT,
        .scl_io_num = SFM_I2C_SCL,
        .sda_io_num = SFM_I2C_SDA,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    esp_err_t sfm_bus_err = i2c_new_master_bus(&sfm_cfg, &sfm_bus_handle);
    if (sfm_bus_err == ESP_OK)
    {
        sfm3300_init(sfm_bus_handle); // caudalimetro de referencia @ 0x40
    }
    else
    {
        ESP_LOGE(TAG, "No se pudo crear el 2do bus I2C (SFM3300): %s",
                 esp_err_to_name(sfm_bus_err));
        sfm_bus_handle = NULL;
    }
}

static void screen_init_task(void *arg)
{
    (void)arg;
    UBaseType_t hwm_start = uxTaskGetStackHighWaterMark(NULL);
    ESP_LOGI("screen_init", "Stack HWM at start (words): %u", (unsigned)hwm_start);

    ESP_LOGI(TAG, "Starting screen initialization");

    if (bsp_i2c_init() != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize I2C");
        if (init_events)
        {
            xEventGroupSetBits(init_events, EVT_SCREEN_DONE);
        }
        vTaskDelete(NULL);
        return;
    }
    else
    {
        ESP_LOGI(TAG, "I2C initialized successfully");
    }
    // Dentro de screen_init_task, justo despuÃƒÂ©s de bsp_i2c_init():
    rtc_rv3028_init(bus_handle); // Le pasamos el bus I2C que ya tienes creado
    time_mgr_init();             // Intenta cargar la hora local

    lv_display_t *disp = bsp_display_start();
    if (!disp)
    {
        ESP_LOGE(TAG, "Failed to initialize display");
        init_bus1_sensors(); // alarms/telemetry still need sensors without a display
        if (init_events)
        {
            xEventGroupSetBits(init_events, EVT_SCREEN_DONE);
        }
        vTaskSuspend(NULL);
        return;
    }
    else
    {
        ESP_LOGI(TAG, "Display initialized successfully");
    }

    // bsp_display_rotate(disp, LV_DISP_ROTATION_270);

    // if (bsp_display_lock(portMAX_DELAY) == ESP_OK)
    // {
    //     ESP_LOGI("Display Init Task", "Locking display for initialization");
    // }else
    // {
    //     ESP_LOGE("Display Init Task", "FALLO CRITICO: No se pudo adquirir el mutex para init.");
    // }

    // ROTACIÃƒâ€œN: Hacerla ANTES de cualquier otra cosa grÃƒÂ¡fica
    // Nota: bsp_display_rotate usa lv_disp_set_rotation.
    // INTENTA BLOQUEAR ANTES DE ROTAR
    if (bsp_display_lock(portMAX_DELAY))
    {
        bsp_display_rotate(disp, LV_DISP_ROTATION_270);
        bsp_display_unlock();
    }
    else
    {
        ESP_LOGE("screen_init", "No se pudo bloquear para rotaciÃƒÂ³n");
    }

    // UI INIT
    // El problema principal: ui_init() crea widgets. ESTO REQUIERE LOCK.
    // Si el lock falla aquÃƒÂ­, la UI no se crearÃƒÂ¡ correctamente o corromperÃƒÂ¡ la memoria de LVGL.

    ESP_LOGI("screen_init", "Intentando bloquear para ui_init...");
    if (bsp_display_lock(portMAX_DELAY))
    {
        ESP_LOGI("screen_init", "Mutex adquirido. Iniciando UI.");

        bsp_display_off(); // Apagar display para evitar parpadeo visual
        bsp_display_backlight_off();

        ui_init(); // <--- ESTO ES LO QUE CREA LOS LABELS
        ESP_LOGI(TAG, "ui_init - OK");

        extern lv_obj_t *ui_wifiStatusMain;
        ui_wifi_main_icon_init(ui_wifiStatusMain);
        ui_statusbar_controller_init();

        bsp_display_unlock();
        ESP_LOGI("screen_init", "Mutex liberado.");
    }
    else
    {
        ESP_LOGE("screen_init", "FALLO TOTAL: No se pudo bloquear para ui_init. LA UI NO SE MOSTRARÃƒÂ CORRECTAMENTE.");
        // Intentar ui_init sin lock como ÃƒÂºltimo recurso desesperado
        abort();
    }

    bsp_display_brightness_set(10);
    bsp_display_backlight_on();
    vTaskDelay(pdMS_TO_TICKS(10));
    {
        /* Brillo configurado por el usuario (general.brightness, saneado en appcfg_migrate) */
        const AppConfig *bcfg = appcfg_cache_peek();
        bsp_display_brightness_set(bcfg ? bcfg->general.brightness : 100);
    }
    bsp_display_on();
    ESP_LOGI(TAG, "UI iniciada");

    // Display + splash first (SPI only, no I2C) so the screen lights at ~1 s and
    // the splash covers the ~2.3 s EEPROM calibration check. Calibration is still
    // read and verified BEFORE touch starts on the shared I2C bus (1.5.10 rule).
    init_bus1_sensors();

    // Touch last: only after calibration was read without touch traffic on bus 1.
    (void)bsp_display_indev_init(disp, bus_handle);

    UBaseType_t hwm_end = uxTaskGetStackHighWaterMark(NULL);
    ESP_LOGI("screen_init", "Stack HWM before delete (words): %u", (unsigned)hwm_end);

    xEventGroupSetBits(init_events, EVT_SCREEN_DONE);

    vTaskSuspend(NULL);
}

/* La app envía la op "finish" por BLE (corre en la tarea host de NimBLE). Diferimos
 * a la tarea LVGL: volver al dashboard descarga la pantalla "Configurar por app",
 * lo que dispara config_mode_exit() (restaura Wi-Fi/AWS). */
/* La app puede mandar la op "finish" (corre en la tarea host de NimBLE). NO tocamos
 * LVGL desde aquí (sería inseguro sin el lock): el cierre de la pantalla y el retorno
 * al dashboard los maneja el tick de ui_bleAppScreen (tarea LVGL) al ver la desconexión. */
static void on_ble_finish(void) { ESP_LOGW("app", "op finish recibida (la pantalla BLE cierra al desconectar)"); }

/* La app ajusta el reloj por BLE (op "set_clock"). Delegamos en time_mgr, que fija
 * la hora del sistema + el RTC de hardware. Evita acoplar transport_ble a network_core. */
static bool on_ble_set_clock(int y, int mo, int d, int h, int mi, int s)
{
    return time_mgr_set_datetime(y, mo, d, h, mi, s);
}

static void ble_init_task(void *arg)
{
    const AppConfig *cfg = (const AppConfig *)arg;

    UBaseType_t hwm_start = uxTaskGetStackHighWaterMark(NULL);
    ESP_LOGI("ble_init", "Stack HWM at start (words): %u", (unsigned)hwm_start);

    esp_err_t e = transport_ble_init(true);
    if (e != ESP_OK)
    {
        ESP_LOGE("ble_init", "init BLE: %s", esp_err_to_name(e));
        ESP_LOGE("ble_init", "init BLE failed");
        xEventGroupSetBits(init_events, EVT_BT_INIT_DONE);
        vTaskSuspend(NULL);
        return; // <-- NECESARIO
    }

    transport_ble_set_finish_cb(on_ble_finish);   /* "finish" de la app -> volver al dashboard */
    fpm_ble_config_set_clock_cb(on_ble_set_clock); /* "set_clock" de la app -> time_mgr (sistema + RTC) */

    ESP_ERROR_CHECK(transport_ble_set_enabled(cfg->bt.enabled));

    if (cfg->bt.legacy.name[0])
    {
        ESP_ERROR_CHECK(transport_ble_set_name(cfg->bt.legacy.name));
    }

    bool use_passkey = (cfg->bt.legacy.sec_mode == 1 /* APP_BT_SEC_PASSKEY */) && cfg->bt.legacy.pin[0];
    uint32_t pk = 0;
    if (use_passkey)
    {
        pk = (uint32_t)strtoul(cfg->bt.legacy.pin, NULL, 10);
    }
    ESP_ERROR_CHECK(transport_ble_set_passkey_mode(use_passkey, pk));

    ESP_ERROR_CHECK(transport_ble_set_tx_power(cfg->bt.tx_power));
    if (cfg->bt.advertise)
        ESP_ERROR_CHECK(transport_ble_start_adv());
    else
        ESP_ERROR_CHECK(transport_ble_stop_adv());

    UBaseType_t hwm_end = uxTaskGetStackHighWaterMark(NULL);
    ESP_LOGI("ble_init", "Stack HWM before delete (words): %u", (unsigned)hwm_end);

    if (init_events)
    {
        xEventGroupSetBits(init_events, EVT_BT_INIT_DONE);
        ESP_LOGW("ble_init", "Entro aqui 2");
    }

    vTaskSuspend(NULL);
}

static void eth_init_task(void *arg)
{
    // const AppConfig *cfg = (const AppConfig *)arg;

    UBaseType_t hwm_start = uxTaskGetStackHighWaterMark(NULL);
    ESP_LOGI("eth_init", "Stack HWM at start (words): %u", (unsigned)hwm_start);

    esp_err_t r = eth_mgr_init();
    if (r != ESP_OK)
    {
        ESP_LOGE("eth_init", "eth_mgr_init failed: %s", esp_err_to_name(r));
        if (init_events)
        {
            xEventGroupSetBits(init_events, EVT_ETH_INIT_DONE);
            ESP_LOGW("eth_init", "Entro aqui");
        }
        vTaskSuspend(NULL);
        return;
    }

    esp_err_t r2 = eth_mgr_start();
    if (r2 != ESP_OK)
    {
        ESP_LOGE("eth_init", "eth_mgr_start failed: %s", esp_err_to_name(r2));
    }

    UBaseType_t hwm_end = uxTaskGetStackHighWaterMark(NULL);
    ESP_LOGI("eth_init", "Stack HWM before suspend (words): %u", (unsigned)hwm_end);

    if (init_events)
    {
        xEventGroupSetBits(init_events, EVT_ETH_INIT_DONE);
        ESP_LOGW("eth_init", "Entro aqui 2");
    }

    vTaskSuspend(NULL);
}

static void wifi_init_task(void *arg)
{
    const AppConfig *cfg = (const AppConfig *)arg;

    UBaseType_t hwm_start = uxTaskGetStackHighWaterMark(NULL);
    ESP_LOGI("wifi_init", "Stack HWM at start (words): %u", (unsigned)hwm_start);

    esp_err_t e = wifi_mgr_init();
    if (e != ESP_OK)
    {
        ESP_LOGE("wifi_init", "wifi_mgr_init failed: %s", esp_err_to_name(e));
        if (init_events)
        {
            xEventGroupSetBits(init_events, EVT_WIFI_INIT_DONE);
        }
        vTaskSuspend(NULL); // o simplemente "return;" si prefieres
        return;
    }

    if (cfg != NULL && wifi_cfg_valid)
    {
        ESP_ERROR_CHECK(wifi_mgr_apply_from_cache());
    }

    ESP_ERROR_CHECK(wifi_mgr_start());

    UBaseType_t hwm_end = uxTaskGetStackHighWaterMark(NULL);
    ESP_LOGI("wifi_init", "Stack HWM before delete (words): %u", (unsigned)hwm_end);
    if (init_events)
    {
        xEventGroupSetBits(init_events, EVT_WIFI_INIT_DONE);
    }
    vTaskSuspend(NULL);
}

static volatile TickType_t s_ui_progress;
static void ui_refresh_task(void *arg)
{
    const AppConfig *cfg = (const AppConfig *)arg;
    (void)cfg;   /* ya no se usa: ui_main_update lee el cachÃƒÂ© vivo */

    ESP_LOGI("ui_refresh", "ui_refresh_task START");

    sensor_sample_t last, min_s, max_s;
    char clockbuf[8];
    char datebuf[16];
    time_t last_clock_min = -1;

    while (true)
    {
        bool have_last = sensors_runtime_get_last(&last);
        bool have_mm = sensors_runtime_get_min_max(60000, &min_s, &max_s);

        // Alarm processing runs independently in app_main.
        alarm_clinical_state_t current_state = alarm_mgr_get_current_state();
        bool is_muted = alarm_mgr_is_muted();

        // Refrescar la HMI: todo el manejo de widgets vive en la capa UI (ui_main_update)
        if (bsp_display_lock(100))
        {
            /* NULL -> ui_main_update usa el AppConfig del cachÃƒÂ© VIVO (appcfg_cache_peek),
               asÃƒÂ­ los cambios de unidad/umbral hechos desde la UI se reflejan sin reiniciar. */
            ui_main_update(&last, have_last, &min_s, &max_s, have_mm,
                           NULL, current_state, is_muted);

            // #4: al estar en el dashboard, se restablece el audio no-critico
            // (la pantalla de login lo inhibio al entrar a configuracion).
            if (lv_screen_active() == ui_mainScreen)
                alarm_mgr_set_audio_inhibit_noncritical(false);

            // Reloj de la cabecera (se actualiza al cambiar de minuto)
            time_t now = time(NULL);
            if (now / 60 != last_clock_min)
            {
                struct tm tm_now;
                localtime_r(&now, &tm_now);
                strftime(clockbuf, sizeof(clockbuf), "%H:%M", &tm_now);
                ui_main_set_clock(clockbuf);
                strftime(datebuf, sizeof(datebuf), "%d/%m/%Y", &tm_now);
                ui_main_set_date(datebuf);
                last_clock_min = now / 60;


            }

            // LVGL invalidates changed widgets; avoid repainting the whole screen.
            s_ui_progress = xTaskGetTickCount();
            bsp_display_unlock();
        }
        else
        {
            static TickType_t last_warn;
            TickType_t now_tick = xTaskGetTickCount();
            if (now_tick - last_warn >= pdMS_TO_TICKS(5000)) {
                last_warn = now_tick;
                ESP_LOGW("ui_refresh", "Display mutex timeout (100 ms)");
            }
        }

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

// cJSON makes many small allocations (<8 KB -> internal RAM by
// SPIRAM_MALLOC_ALWAYSINTERNAL) on every telemetry/HTTP/BLE document, which
// fragmented internal heap (largest block 31 KB -> 11 KB). Keep them in PSRAM;
// fall back to the default heap if PSRAM is exhausted. free() handles both.
static void *cjson_psram_malloc(size_t sz)
{
    void *p = heap_caps_malloc(sz, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    return p ? p : malloc(sz);
}

void app_main(void)
{
    cJSON_Hooks cjson_hooks = { .malloc_fn = cjson_psram_malloc, .free_fn = free };
    cJSON_InitHooks(&cjson_hooks);
    fpm_i2c_init();
    transport_mqtt_runtime_init();

    // --- TRAMPA PARA EL LINKER ---
    // Forzamos al compilador a inyectar la telemetrÃƒÂ­a en el binario final
    // volatile void *dummy_trace = (void *)esp_apptrace_init;
    // volatile void *dummy_sysview = (void *)SEGGER_SYSVIEW_Start;
    // (void)dummy_trace;
    // (void)dummy_sysview;
    // -----------------------------

    mem_diag_report("BOOT-START");

    // NVS primero
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }
    mem_diag_report("AFTER-NVS");
    // A previous crash leaves an ELF core dump in the "coredump" partition. Report
    // it at boot; read it with: idf.py -B build-fixes coredump-info
    {
        size_t cd_addr = 0, cd_size = 0;
        if (esp_core_dump_image_check() == ESP_OK &&
            esp_core_dump_image_get(&cd_addr, &cd_size) == ESP_OK) {
            char reason[96] = "";
            esp_core_dump_get_panic_reason(reason, sizeof(reason));
            ESP_LOGW(TAG, "Core dump from previous crash: %u bytes @0x%x, reason: %s",
                     (unsigned)cd_size, (unsigned)cd_addr, reason[0] ? reason : "?");
        }
    }
    ESP_ERROR_CHECK(flow_meter_init());

    // ... tras nvs_flash_init() OK
    extern esp_err_t appcfg_cache_reload(void);
    appcfg_cache_reload(); // carga NVS -> DRAM (stack interno de main), seguro

    /* Zona horaria configurada -> TZ del RTC (localtime en ui_refresh_task) */
    {
        extern void ui_cfg_apply_timezone(const char *iana);
        const AppConfig *tzc = appcfg_cache_peek();
        if (tzc) ui_cfg_apply_timezone(tzc->general.timezone);
    }
    mem_diag_report("AFTER-APPCFG-RELOAD");
    // LVGL init
    lv_init();
    mem_diag_report("AFTER-LV-INIT");

    alarm_mgr_init(BUZZER_PWM_GPIO);

    // Al cambiar el estado de alarma, forzar un publish INMEDIATO a la nube
    // (no esperar el periodo de 30 s de la telemetria periodica).
    alarm_mgr_set_state_change_cb(on_alarm_state_change);

    // Estado pÃƒÂºblico (MQTT, BLE, etc)
    ESP_ERROR_CHECK(esp_netif_init());
    esp_err_t er = esp_event_loop_create_default();
    if (er != ESP_OK && er != ESP_ERR_INVALID_STATE)
    {
        ESP_ERROR_CHECK(er);
    }
    net_core_init();
    on_network_ready(); // adentro hace http_api_init()
    mem_diag_report("AFTER-STATE-PUB");

    // Monta microSD (para import/borrado de certs desde UI_CloudCfg)
    xTaskCreate(sd_monitor_task, "sd_monitor", 4096, NULL, 5, NULL);
    mem_diag_report("AFTER-SDCARD");

    init_events = xEventGroupCreate();
    if (init_events == NULL)
    {
        ESP_LOGE("app", "No se pudo crear init_events");
        abort();
    }

    // I2C (touch) y Display
    //screen_task_ctx.stack_buf = heap_caps_malloc(16384, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    screen_task_ctx.stack_buf = heap_caps_malloc(MY_TASK_STACK_WORDS * sizeof(StackType_t), MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    screen_task_ctx.tcb_buf = heap_caps_malloc(sizeof(StaticTask_t), MALLOC_CAP_INTERNAL);
    assert(screen_task_ctx.stack_buf && screen_task_ctx.tcb_buf);

    screen_task_ctx.handle = xTaskCreateStaticPinnedToCore(
        screen_init_task,
        "screen_init",
        MY_TASK_STACK_WORDS,
        NULL,
        PRIO_INIT_TASK,
        screen_task_ctx.stack_buf,
        screen_task_ctx.tcb_buf,
        1);

    if (screen_task_ctx.handle == NULL)
    {
        ESP_LOGE("screen_init", "No se pudo crear screen_init task");
    }
    else
    {
        ESP_LOGI("screen_init", "screen_init task creada");
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
    xEventGroupWaitBits(init_events, EVT_SCREEN_DONE,
                        pdTRUE,
                        pdFALSE,
                        portMAX_DELAY);
    vTaskDelete(screen_task_ctx.handle);
    heap_caps_free(screen_task_ctx.stack_buf);
    heap_caps_free(screen_task_ctx.tcb_buf);
    screen_task_ctx.handle = NULL;
    screen_task_ctx.stack_buf = NULL;
    screen_task_ctx.tcb_buf = NULL;
    ESP_LOGW("screen_init", "Done!");
    mem_diag_report("AFTER-LVGL");

    static AppConfig cfg;
    if (appcfg_cache_get(&cfg) != ESP_OK)
    {
        memset(&cfg, 0, sizeof(cfg));
    }

    // Registramos el bus I2C 1 para habilitar la recuperacion de bus si queda
    // colgado (MS5803+ADS fallando juntos). Debe ir antes del init.
    sensors_runtime_set_bus(bus_handle);
    sensors_runtime_init(&cfg);
    mem_diag_report("AFTER-SENSORS-RUNTIME");

    // *** BLUETOOTH ***//
    if (appcfg_cache_get(&cfg) == ESP_OK && cfg.bt.enabled)
    {

        // ASIGNAMOS DIRECTAMENTE AL CONTEXTO GLOBAL PARA PODER LIBERARLO DESPUÃƒâ€°S
        ble_task_ctx.stack_buf = (StackType_t *)heap_caps_malloc(BLE_TASK_STACK_SIZE, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
        ble_task_ctx.tcb_buf = (StaticTask_t *)heap_caps_malloc(sizeof(StaticTask_t), MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);

        if (ble_task_ctx.stack_buf != NULL && ble_task_ctx.tcb_buf != NULL)
        {
            // Ã‚Â¡AQUÃƒÂ ESTÃƒÂ LA MAGIA! Guardamos el handle.
            ble_task_ctx.handle = xTaskCreateStaticPinnedToCore(
                ble_init_task,
                "ble_init",
                BLE_TASK_STACK_SIZE / sizeof(StackType_t), // FreeRTOS estÃƒÂ¡tico espera palabras, no bytes
                &cfg,
                5,
                ble_task_ctx.stack_buf,
                ble_task_ctx.tcb_buf,
                0);
        }
        else
        {
            ESP_LOGE("MAIN", "Error: No hay memoria INTERNA para la tarea BLE");
            if (ble_task_ctx.stack_buf)
                heap_caps_free(ble_task_ctx.stack_buf);
            if (ble_task_ctx.tcb_buf)
                heap_caps_free(ble_task_ctx.tcb_buf);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
        xEventGroupWaitBits(init_events, EVT_BT_INIT_DONE, pdTRUE, pdFALSE, portMAX_DELAY);

        if (ble_task_ctx.handle != NULL)
        {
            vTaskDelete(ble_task_ctx.handle);
        }
        heap_caps_free(ble_task_ctx.stack_buf);
        heap_caps_free(ble_task_ctx.tcb_buf);

        ble_task_ctx.handle = NULL;
        ble_task_ctx.stack_buf = NULL;
        ble_task_ctx.tcb_buf = NULL;
        ESP_LOGW("bt_mgr", "Bluetooth enabled!");
    }
    else
    {
        ESP_LOGW("bt_mgr", "Bluetooth disabled; skipping start");
    }
    mem_diag_report("AFTER-BT");

    // *** WIFI ***//
    if (appcfg_cache_get(&cfg) == ESP_OK && cfg.wifi.enabled)
    {
        wifi_cfg_valid = true;
        wifi_task_ctx.stack_buf = heap_caps_malloc(16384, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
        wifi_task_ctx.tcb_buf = heap_caps_malloc(sizeof(StaticTask_t), MALLOC_CAP_INTERNAL);
        assert(wifi_task_ctx.stack_buf && wifi_task_ctx.tcb_buf);
        wifi_task_ctx.handle = xTaskCreateStaticPinnedToCore(
            wifi_init_task,
            "wifi_init",
            MY_TASK_STACK_WORDS,
            &cfg,
            PRIO_INIT_TASK,
            wifi_task_ctx.stack_buf,
            wifi_task_ctx.tcb_buf,
            0);
        if (wifi_task_ctx.handle == NULL)
        {
            ESP_LOGE("wifi_init", "No se pudo crear screen_init task");
        }
        else
        {
            ESP_LOGI("wifi_init", "wifi_init task creada");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
        xEventGroupWaitBits(init_events, EVT_WIFI_INIT_DONE,
                            pdTRUE,
                            pdFALSE,
                            portMAX_DELAY);
        vTaskDelete(wifi_task_ctx.handle);
        heap_caps_free(wifi_task_ctx.stack_buf);
        heap_caps_free(wifi_task_ctx.tcb_buf);
        wifi_task_ctx.handle = NULL;
        wifi_task_ctx.stack_buf = NULL;
        wifi_task_ctx.tcb_buf = NULL;
        ESP_LOGW("wifi_mgr", "Wi-Fi enabled!");
    }
    else
    {
        ESP_LOGW("wifi_mgr", "Wi-Fi disabled; skipping start");
    }
    mem_diag_report("AFTER-WIFI");

    // *** ETHERNET ***//
    if (appcfg_cache_get(&cfg) == ESP_OK && cfg.eth.enabled)
    {
        eth_task_ctx.stack_buf = heap_caps_malloc(16384, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
        eth_task_ctx.tcb_buf = heap_caps_malloc(sizeof(StaticTask_t), MALLOC_CAP_INTERNAL);
        assert(eth_task_ctx.stack_buf && eth_task_ctx.tcb_buf);
        eth_task_ctx.handle = xTaskCreateStaticPinnedToCore(
            eth_init_task,
            "eth_init",
            MY_TASK_STACK_WORDS,
            &cfg,
            PRIO_INIT_TASK,
            eth_task_ctx.stack_buf,
            eth_task_ctx.tcb_buf,
            0);
        if (eth_task_ctx.handle == NULL)
        {
            ESP_LOGE("eth_init", "No se pudo crear eth_init task");
        }
        else
        {
            ESP_LOGI("eth_init", "eth_init task creada");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
        xEventGroupWaitBits(init_events, EVT_ETH_INIT_DONE,
                            pdTRUE,
                            pdFALSE,
                            portMAX_DELAY);
        vTaskDelete(eth_task_ctx.handle);
        heap_caps_free(eth_task_ctx.stack_buf);
        heap_caps_free(eth_task_ctx.tcb_buf);
        eth_task_ctx.handle = NULL;
        eth_task_ctx.stack_buf = NULL;
        eth_task_ctx.tcb_buf = NULL;
        ESP_LOGW("eth_mgr", "ETH Enabled");
    }
    else
    {
        ESP_LOGW("eth_mgr", "ETH disabled; not creating task");
    }
    mem_diag_report("AFTER-ETH");

    //== == == == INICIAR TAREA DE REFRESCO DE UI == == == ==
    TaskHandle_t ui_refresh_handle = NULL;
    BaseType_t r = xTaskCreatePinnedToCore(
        ui_refresh_task,
        "ui_refresh",
        8192, // 2048 palabras (~8 KB), mÃƒÂ¡s que suficiente para 4 labels
        &cfg,
        PRIO_UI_LOGIC,
        &ui_refresh_handle,
        1);

    if (r != pdPASS || ui_refresh_handle == NULL)
    {
        ESP_LOGE("ui_refresh", "No se pudo crear ui_refresh_task (err=%ld)", (long)r);
    }
    else
    {
        ESP_LOGI("ui_refresh", "ui_refresh_task creada OK");
    }
    mem_diag_report("AFTER-UI-REFRESH");
    mem_diag_report_full("END");

    // Diagnostico de heap cada 30 s
    mem_diag_start_periodic(30000);

    // Traza de tareas: muestrea CPU%+estado cada 2 s.
    // Captura con: idf.py -p COMX monitor | tee captura.log
    // Analiza con: python tools/trace_analyze.py captura.log
    task_tracer_start(10000);

    ESP_ERROR_CHECK(esp_task_wdt_add(NULL));
    TickType_t alarm_wake = xTaskGetTickCount();
    TickType_t health_log = alarm_wake;
    while (true) {
        sensor_sample_t sample = {0};
        bool have = sensors_runtime_get_last(&sample);
        uint32_t faults = sensors_runtime_get_faults();
        // Invalid/stale/missing channels are passed as NAN: they raise the technical
        // (silenceable) fault via `faults`, never a clinical ALERT from a fake 0 kPa.
        float p_eval = (have && !(sample.invalid_mask & SENSOR_INVALID_PRESSURE)) ? sample.pressure_kpa : NAN;
        float f_eval = (have && !(sample.invalid_mask & SENSOR_INVALID_FLOW)) ? sample.flow_lpm : NAN;
        bool alarm_updated = alarm_mgr_process(p_eval, f_eval, faults);
        TickType_t tick = xTaskGetTickCount();
        if (tick - health_log >= pdMS_TO_TICKS(5000)) {
            health_log = tick;
            if (tick - s_ui_progress > pdMS_TO_TICKS(5000))
                ESP_LOGE("health", "UI has made no progress for >5 s; alarm control remains active");
            if (faults & SENSOR_FAULT_STALE)
                ESP_LOGW("health", "Sensor data stale; technical alarm active");
        }
        if (alarm_updated) esp_task_wdt_reset();
        if (xTaskDelayUntil(&alarm_wake, pdMS_TO_TICKS(50)) == pdFALSE)
            alarm_wake = xTaskGetTickCount();
    }
}
