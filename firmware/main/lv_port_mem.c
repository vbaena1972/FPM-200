/**
 * Allocator de memoria de LVGL redirigido a PSRAM.
 *
 * Motivo: con CONFIG_SPIRAM_USE_CAPS_ALLOC, malloc() solo entrega RAM INTERNA.
 * Si LVGL usa el allocator clib (malloc), TODO el árbol de objetos de la UI
 * consume RAM interna, que es escasa (~230 KB, muy disputada por Wi-Fi/AWS/BLE).
 * Al construir pantallas de configuración con la interna casi agotada, lv_malloc
 * fallaba y, con CONFIG_LV_USE_ASSERT_MALLOC, LVGL entraba en un bucle -> watchdog
 * de taskLVGL (crashes recurrentes en config/sensorDiag).
 *
 * Solución: LV_USE_STDLIB_MALLOC = CUSTOM y aquí enrutamos toda la memoria de
 * LVGL a PSRAM (2 MB). El buffer de dibujo (DMA) lo maneja aparte esp_lvgl_port
 * con sus propias caps, así que no se ve afectado.
 */
#include "sdkconfig.h"

/* Guard por la macro de Kconfig (siempre presente en sdkconfig.h para todo TU),
 * no por LV_USE_STDLIB_MALLOC: así siempre proveemos los símbolos que LVGL
 * (compilado en modo CUSTOM) espera resolver externamente. */
#ifdef CONFIG_LV_USE_CUSTOM_MALLOC

#include "lvgl.h"
#include "esp_heap_caps.h"

#define LV_PSRAM_CAPS (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT)

void lv_mem_init(void) { /* nada que inicializar: usamos el heap de PSRAM del sistema */ }
void lv_mem_deinit(void) { }

lv_mem_pool_t lv_mem_add_pool(void *mem, size_t bytes)
{
    LV_UNUSED(mem);
    LV_UNUSED(bytes);
    return NULL;   /* no soportado (usamos el heap del sistema, no un pool fijo) */
}
void lv_mem_remove_pool(lv_mem_pool_t pool) { LV_UNUSED(pool); }

void *lv_malloc_core(size_t size)
{
    return heap_caps_malloc(size, LV_PSRAM_CAPS);
}

void *lv_realloc_core(void *p, size_t new_size)
{
    return heap_caps_realloc(p, new_size, LV_PSRAM_CAPS);
}

void lv_free_core(void *p)
{
    heap_caps_free(p);
}

void lv_mem_monitor_core(lv_mem_monitor_t *mon_p)
{
    LV_UNUSED(mon_p);   /* no soportado (el heap del sistema no expone estas métricas aquí) */
}

lv_result_t lv_mem_test_core(void)
{
    return LV_RESULT_OK;
}

#endif /* CONFIG_LV_USE_CUSTOM_MALLOC */
