#include "transport_ble.h"
#include <string.h>
#include "esp_log.h"
#include "esp_err.h"
#include "esp_bt.h"

#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"

#include "host/ble_hs.h"
#include "host/ble_gap.h"
#include "host/ble_gatt.h"

#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"

#include "cJSON.h"
#include "fpm_ble_config.h"

static const char *TAG = "BLE";

// Manufacturer data (Axira)
static uint8_t mfg_data[4] = {0xA9, 0x11, 0x00, 0x01};

// Estado global
static bool s_enabled = false;
static bool s_stack_ready = false;
static bool s_adv_on = false;
static bool s_notify_enabled = false;
static bool s_mesh_enabled = false;

static uint16_t s_conn = BLE_HS_CONN_HANDLE_NONE;

static char s_dev_name[32] = "Axira-BLE";

static ble_cmd_handler_t s_handler = NULL;

// Declarada en tu proyecto
extern void ui_statusbar_request_refresh(void);

// Forward declarations
static void ble_start_adv(void);
static int gap_event(struct ble_gap_event *ev, void *arg);
static void host_task(void *param);
static void ble_on_sync(void);
static void ble_on_reset(int reason);

/******************** GATT (contrato de la app Sensvax) ********************/
/* Servicio 9f3c1000-6d4f-4d5b-8c9a-2f3e7a110001 + 7 características (1001-1007).
 * BLE_UUID128_INIT recibe los 16 bytes en orden little-endian (igual que el
 * equipo hermano ClaudeHMI/main/ble_service.c). Solo cambia el 13º byte (índice). */
#define AXIRA_UUID128(idx) BLE_UUID128_INIT(0x01,0x00,0x11,0x7a,0x3e,0x2f,0x9a,0x8c, \
                                            0x5b,0x4d,0x4f,0x6d,(idx),0x10,0x3c,0x9f)
static const ble_uuid128_t s_service_uuid = AXIRA_UUID128(0x00);
static const ble_uuid128_t s_info_uuid    = AXIRA_UUID128(0x01);
static const ble_uuid128_t s_wifi_uuid    = AXIRA_UUID128(0x02);
static const ble_uuid128_t s_cloud_uuid   = AXIRA_UUID128(0x03);
static const ble_uuid128_t s_status_uuid  = AXIRA_UUID128(0x04);
static const ble_uuid128_t s_control_uuid = AXIRA_UUID128(0x05);
static const ble_uuid128_t s_channel_uuid = AXIRA_UUID128(0x06);
static const ble_uuid128_t s_config_uuid  = AXIRA_UUID128(0x07);

typedef enum {
    BLE_VALUE_INFO = 1, BLE_VALUE_WIFI, BLE_VALUE_CLOUD, BLE_VALUE_STATUS,
    BLE_VALUE_CONTROL, BLE_VALUE_CHANNEL, BLE_VALUE_CONFIG,
} ble_value_id_t;

#define BLE_JSON_MAX 640

/* Estado de la sesión de config. Todas las llamadas GATT corren en la tarea host
 * de NimBLE (serializadas) -> no hace falta mutex adicional. */
static char  *s_config_json;      /* snapshot para lectura troceada (op config_read) */
static size_t s_config_len, s_config_offset;
static uint8_t s_selected_channel;/* 0 = Presión, 1 = Flujo */
static bool   s_finish_after_disc;/* la app pidió "finish" -> cortar al responder */
static void (*s_finish_cb)(void); /* hook opcional (main lo liga a config_mode_exit) */

static void free_config_buffer(void)
{
    if (s_config_json) { free(s_config_json); s_config_json = NULL; }
    s_config_len = 0; s_config_offset = 0;
}

/* Envía un JSON completo en una sola lectura y libera el string (malloc del mapeo). */
static int append_json(struct os_mbuf *om, char *text)
{
    if (!text) return BLE_ATT_ERR_INSUFFICIENT_RES;
    int rc = os_mbuf_append(om, text, strlen(text));
    free(text);
    return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
}

static int chr_access(uint16_t conn, uint16_t attr_handle,
                      struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    (void)attr_handle;
    const ble_value_id_t id = (ble_value_id_t)(intptr_t)arg;

    if (ctxt->op == BLE_GATT_ACCESS_OP_READ_CHR) {
        switch (id) {
        case BLE_VALUE_INFO:    return append_json(ctxt->om, fpm_ble_info_json());
        case BLE_VALUE_WIFI:    return append_json(ctxt->om, fpm_ble_wifi_read_json());
        case BLE_VALUE_CLOUD:   return append_json(ctxt->om, fpm_ble_cloud_read_json());
        case BLE_VALUE_STATUS:  return append_json(ctxt->om, fpm_ble_status_read_json(s_selected_channel));
        case BLE_VALUE_CHANNEL: return append_json(ctxt->om, fpm_ble_channel_read_json(s_selected_channel));
        case BLE_VALUE_CONFIG: {
            /* trozo siguiente del JSON; una lectura vacía (0 bytes) = EOF */
            if (!s_config_json || s_config_offset >= s_config_len) return 0;
            uint16_t mtu = ble_att_mtu(conn);
            size_t chunk = mtu > 3 ? (size_t)(mtu - 3) : 20;
            size_t remaining = s_config_len - s_config_offset;
            if (chunk > remaining) chunk = remaining;
            if (os_mbuf_append(ctxt->om, s_config_json + s_config_offset, chunk) != 0)
                return BLE_ATT_ERR_INSUFFICIENT_RES;
            s_config_offset += chunk;
            return 0;
        }
        default: return BLE_ATT_ERR_READ_NOT_PERMITTED;
        }
    }

    if (ctxt->op != BLE_GATT_ACCESS_OP_WRITE_CHR)
        return BLE_ATT_ERR_UNLIKELY;

    uint16_t length = OS_MBUF_PKTLEN(ctxt->om);
    if (length == 0 || length >= BLE_JSON_MAX)
        return BLE_ATT_ERR_INVALID_ATTR_VALUE_LEN;
    char text[BLE_JSON_MAX];
    uint16_t copied = 0;
    if (ble_hs_mbuf_to_flat(ctxt->om, text, sizeof(text) - 1, &copied) != 0)
        return BLE_ATT_ERR_UNLIKELY;
    text[copied] = '\0';

    switch (id) {
    case BLE_VALUE_WIFI:    fpm_ble_wifi_apply_json(text); return 0;
    case BLE_VALUE_CLOUD:   fpm_ble_cloud_apply_json(text); return 0;
    case BLE_VALUE_CHANNEL: fpm_ble_channel_apply_json(text); return 0;
    case BLE_VALUE_CONTROL: {
        cJSON *root = cJSON_Parse(text);
        const cJSON *op = root ? cJSON_GetObjectItemCaseSensitive(root, "op") : NULL;
        if (cJSON_IsString(op)) {
            if (strcmp(op->valuestring, "config_read") == 0) {
                free_config_buffer();
                s_config_json = fpm_ble_config_read_json(true);   /* redacta secretos */
                s_config_len = s_config_json ? strlen(s_config_json) : 0;
                s_config_offset = 0;
            } else if (strcmp(op->valuestring, "set_config") == 0) {
                fpm_ble_config_apply_json(text);
            } else if (strcmp(op->valuestring, "select_channel") == 0) {
                const cJSON *cid = cJSON_GetObjectItemCaseSensitive(root, "id");
                if (cJSON_IsNumber(cid) && cid->valueint >= 1 && cid->valueint <= 2)
                    s_selected_channel = (uint8_t)(cid->valueint - 1);
            } else if (strcmp(op->valuestring, "finish") == 0) {
                s_finish_after_disc = true;
                if (s_finish_cb) s_finish_cb();   /* main -> config_mode_exit + salir de la pantalla */
            } else if (strcmp(op->valuestring, "user_upsert") == 0 ||
                       strcmp(op->valuestring, "user_remove") == 0) {
                fpm_ble_user_op_json(text);
            } else if (strcmp(op->valuestring, "set_clock") == 0) {
                fpm_ble_set_clock_json(text);   /* ajusta hora del sistema + RTC */
            } else {
                ESP_LOGW(TAG, "op de control no implementada: %s", op->valuestring);
            }
        }
        if (root) cJSON_Delete(root);
        if (s_finish_after_disc && s_conn != BLE_HS_CONN_HANDLE_NONE)
            ble_gap_terminate(s_conn, BLE_ERR_REM_USER_CONN_TERM);
        return 0;
    }
    default: return BLE_ATT_ERR_WRITE_NOT_PERMITTED;
    }
}

/* v1: características SIN cifrado (Just Works, sin passkey) para que la app pueda
 * leer/escribir sin emparejamiento con PIN. TODO seguridad: pasar a *_ENC +
 * passkey mostrado en la HMI (como el hermano). */
static const struct ble_gatt_svc_def gatt_svcs[] = {
    {.type = BLE_GATT_SVC_TYPE_PRIMARY,
     .uuid = &s_service_uuid.u,
     .characteristics = (struct ble_gatt_chr_def[]){
         { .uuid = &s_info_uuid.u,    .access_cb = chr_access, .arg = (void *)(intptr_t)BLE_VALUE_INFO,    .flags = BLE_GATT_CHR_F_READ },
         { .uuid = &s_wifi_uuid.u,    .access_cb = chr_access, .arg = (void *)(intptr_t)BLE_VALUE_WIFI,    .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_WRITE },
         { .uuid = &s_cloud_uuid.u,   .access_cb = chr_access, .arg = (void *)(intptr_t)BLE_VALUE_CLOUD,   .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_WRITE },
         { .uuid = &s_status_uuid.u,  .access_cb = chr_access, .arg = (void *)(intptr_t)BLE_VALUE_STATUS,  .flags = BLE_GATT_CHR_F_READ },
         { .uuid = &s_control_uuid.u, .access_cb = chr_access, .arg = (void *)(intptr_t)BLE_VALUE_CONTROL, .flags = BLE_GATT_CHR_F_WRITE },
         { .uuid = &s_channel_uuid.u, .access_cb = chr_access, .arg = (void *)(intptr_t)BLE_VALUE_CHANNEL, .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_WRITE },
         { .uuid = &s_config_uuid.u,  .access_cb = chr_access, .arg = (void *)(intptr_t)BLE_VALUE_CONFIG,  .flags = BLE_GATT_CHR_F_READ },
         {0}}},
    {0}};

/******************** GAP EVENTS ********************/

static int gap_event(struct ble_gap_event *ev, void *arg)
{
    (void)arg;

    switch (ev->type)
    {
    case BLE_GAP_EVENT_CONNECT:
        if (ev->connect.status == 0)
        {
            s_conn = ev->connect.conn_handle;
            ESP_LOGI(TAG, "BLE connected");
            ui_statusbar_request_refresh();
        }
        else
        {
            ESP_LOGW(TAG, "BLE connect failed; status=%d", ev->connect.status);
            s_conn = BLE_HS_CONN_HANDLE_NONE;
            if (s_adv_on && !ble_gap_adv_active())
                ble_start_adv();
        }
        return 0;

    case BLE_GAP_EVENT_DISCONNECT:
        ESP_LOGI(TAG, "BLE disconnected; reason=%d", ev->disconnect.reason);
        s_conn = BLE_HS_CONN_HANDLE_NONE;
        s_notify_enabled = false;
        free_config_buffer();      /* libera el snapshot de config_read */
        s_finish_after_disc = false;
        ui_statusbar_request_refresh();
        if (s_adv_on && !ble_gap_adv_active())
            ble_start_adv();
        return 0;

    default:
        return 0;
    }
}

/******************** Advertising ********************/

static void ble_start_adv(void)
{
    if (!s_stack_ready)
        return;
    if (ble_gap_adv_active())
        return;

    /* Advertising: flags + UUID de servicio de 128 bits (la app escanea filtrando
     * por este servicio) + mfg data. El nombre va en el SCAN RESPONSE porque el
     * UUID de 128 bits ya ocupa casi todo el paquete de 31 bytes. */
    struct ble_hs_adv_fields f;
    memset(&f, 0, sizeof(f));
    f.flags = BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP;
    f.uuids128 = (ble_uuid128_t *)&s_service_uuid;
    f.num_uuids128 = 1;
    f.uuids128_is_complete = 1;
    f.mfg_data = mfg_data;
    f.mfg_data_len = sizeof(mfg_data);

    int rc = ble_gap_adv_set_fields(&f);
    if (rc != 0)
    {
        ESP_LOGE(TAG, "ble_gap_adv_set_fields rc=%d", rc);
        return;
    }

    struct ble_hs_adv_fields rsp;
    memset(&rsp, 0, sizeof(rsp));
    rsp.name = (uint8_t *)s_dev_name;
    rsp.name_len = strlen(s_dev_name);
    rsp.name_is_complete = 1;
    (void)ble_gap_adv_rsp_set_fields(&rsp);

    struct ble_gap_adv_params p;
    memset(&p, 0, sizeof(p));
    p.conn_mode = BLE_GAP_CONN_MODE_UND;
    p.disc_mode = BLE_GAP_DISC_MODE_GEN;
    p.itvl_min = 0x20;
    p.itvl_max = 0x40;

    rc = ble_gap_adv_start(BLE_OWN_ADDR_PUBLIC, NULL, BLE_HS_FOREVER,
                           &p, gap_event, NULL);
    if (rc != 0)
    {
        ESP_LOGE(TAG, "ble_gap_adv_start rc=%d", rc);
        return;
    }

    ESP_LOGI(TAG, "Advertising started");
}

/******************** Host task & callbacks ********************/

static void host_task(void *param)
{
    (void)param;
    nimble_port_run(); // No retorna hasta nimble_port_stop()
    nimble_port_freertos_deinit();
    vTaskDelete(NULL);
}

static void ble_on_sync(void)
{
    s_stack_ready = true;

    uint8_t addr_val[6] = {0};
    uint8_t addr_type;
    int rc = ble_hs_id_infer_auto(0, &addr_type);
    if (rc == 0)
    {
        rc = ble_hs_id_copy_addr(addr_type, addr_val, NULL);
        if (rc == 0)
        {
            ESP_LOGI(TAG, "BLE addr: %02X:%02X:%02X:%02X:%02X:%02X",
                     addr_val[5], addr_val[4], addr_val[3],
                     addr_val[2], addr_val[1], addr_val[0]);
        }
    }

    ble_svc_gap_device_name_set(s_dev_name);

    if (s_adv_on)
        ble_start_adv();
}

static void ble_on_reset(int reason)
{
    ESP_LOGE(TAG, "BLE reset, reason=%d", reason);
}

/******************** INIT ********************/

esp_err_t transport_ble_init(bool enable)
{
    if (s_enabled)
        return ESP_OK;

    if (!enable)
    {
        s_enabled = false;
        return ESP_OK;
    }

    esp_err_t ret = esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
    if (ret != ESP_OK && ret != ESP_ERR_INVALID_STATE)
    {
        ESP_LOGW(TAG, "esp_bt_controller_mem_release: %s", esp_err_to_name(ret));
    }

    int rc = nimble_port_init();
    if (rc != 0)
    {
        ESP_LOGE(TAG, "nimble_port_init rc=%d", rc);
        return ESP_FAIL;
    }

    ble_hs_cfg.reset_cb = ble_on_reset;
    ble_hs_cfg.sync_cb = ble_on_sync;

    // Seguridad básica: bonding, sin MITM (Just Works)
    ble_hs_cfg.sm_io_cap = BLE_SM_IO_CAP_NO_IO;
    ble_hs_cfg.sm_bonding = 1;
    ble_hs_cfg.sm_mitm = 0;
    ble_hs_cfg.sm_our_key_dist = BLE_SM_PAIR_KEY_DIST_ENC;
    ble_hs_cfg.sm_their_key_dist = BLE_SM_PAIR_KEY_DIST_ENC;

    ble_svc_gap_init();
    ble_svc_gatt_init();

    rc = ble_gatts_count_cfg(gatt_svcs);
    if (rc != 0)
    {
        ESP_LOGE(TAG, "ble_gatts_count_cfg rc=%d", rc);
        return ESP_FAIL;
    }

    rc = ble_gatts_add_svcs(gatt_svcs);
    if (rc != 0)
    {
        ESP_LOGE(TAG, "ble_gatts_add_svcs rc=%d", rc);
        return ESP_FAIL;
    }

    nimble_port_freertos_init(host_task);

    s_enabled = true;
    s_adv_on = true; // empezamos anunciando por defecto
    ESP_LOGI(TAG, "NimBLE initialized OK");
    return ESP_OK;
}

/******************** API PÚBLICA ********************/

void transport_ble_set_cmd_handler(ble_cmd_handler_t h)
{
    s_handler = h;   /* obsoleto: el pipe RX/TX se reemplazó por el servicio GATT */
    (void)s_handler;
}

void transport_ble_set_finish_cb(void (*cb)(void))
{
    s_finish_cb = cb;
}

esp_err_t transport_ble_set_enabled(bool on)
{
    if (on && !s_enabled)
    {
        return transport_ble_init(true);
    }

    if (!on && s_enabled)
    {
        s_enabled = false;
        if (s_stack_ready)
        {
            if (ble_gap_adv_active())
                ble_gap_adv_stop();
            if (s_conn != BLE_HS_CONN_HANDLE_NONE)
                ble_gap_terminate(s_conn, BLE_ERR_REM_USER_CONN_TERM);
        }
    }
    return ESP_OK;
}

esp_err_t transport_ble_set_name(const char *name)
{
    if (!name || !name[0])
        return ESP_ERR_INVALID_ARG;

    strncpy(s_dev_name, name, sizeof(s_dev_name));
    s_dev_name[sizeof(s_dev_name) - 1] = '\0';

    if (s_stack_ready)
    {
        ble_svc_gap_device_name_set(s_dev_name);
        if (s_adv_on)
        {
            if (ble_gap_adv_active())
                ble_gap_adv_stop();
            ble_start_adv();
        }
    }
    return ESP_OK;
}

esp_err_t transport_ble_set_passkey_mode(bool enable, uint32_t passkey)
{
    // Stub: no se implementa passkey real con NimBLE en esta versión.
    ESP_LOGW(TAG,
             "transport_ble_set_passkey_mode(enable=%d, passkey=%06u) "
             "-> modo passkey aún no implementado (stub).",
             (int)enable, (unsigned)(passkey % 1000000U));
    return ESP_OK;
}

esp_err_t transport_ble_set_tx_power(uint8_t level)
{
    // Sin API directa en NimBLE/IDF aquí -> stub.
    ESP_LOGW(TAG,
             "transport_ble_set_tx_power(%u) -> no cambia TX real en NimBLE (stub).",
             (unsigned)level);
    return ESP_OK;
}

esp_err_t transport_ble_start_adv(void)
{
    s_adv_on = true;
    if (s_stack_ready)
        ble_start_adv();
    return ESP_OK;
}

esp_err_t transport_ble_stop_adv(void)
{
    s_adv_on = false;
    if (ble_gap_adv_active())
        ble_gap_adv_stop();
    return ESP_OK;
}

bool transport_ble_is_enabled(void)
{
    return s_enabled;
}

bool transport_ble_is_advertising(void)
{
    return s_adv_on && ble_gap_adv_active();
}

esp_err_t transport_ble_set_mesh_enabled(bool on)
{
    s_mesh_enabled = on;
    // BLE Mesh real no implementado aquí.
    return ESP_OK;
}

bool transport_ble_is_mesh_enabled(void)
{
    return s_mesh_enabled;
}

bool transport_ble_is_connected(void)
{
    return s_conn != BLE_HS_CONN_HANDLE_NONE;
}
