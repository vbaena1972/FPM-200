#ifndef UI_CONNECTIVITYSCREEN_H
#define UI_CONNECTIVITYSCREEN_H
#include "lvgl.h"
#ifdef __cplusplus
extern "C" {
#endif
extern lv_obj_t *ui_connectivityScreen;
void ui_connectivityScreen_screen_init(void);
void ui_connectivityScreen_screen_destroy(void);
/* Reconstruye la pantalla conservando su posición en la pila de navegación.
 * Usar mientras otra pantalla está activa (p. ej. al guardar WiFi). */
void ui_connectivityScreen_refresh(void);
#ifdef __cplusplus
}
#endif
#endif
