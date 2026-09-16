#ifndef UI_SENSOREDITSCREEN_H
#define UI_SENSOREDITSCREEN_H
#include "lvgl.h"
#ifdef __cplusplus
extern "C" {
#endif
extern lv_obj_t *ui_sensorEditScreen;
void ui_sensorEditScreen_screen_init(void);
void ui_sensorEditScreen_screen_destroy(void);
/* Reconstruye la pantalla Sensores EN LA PILA (aunque no sea la activa) para que
 * refleje limites recien editados por teclado. Seguro de llamar desde el dialogo
 * de confirmacion antes de hacer pop_to. */
void ui_sensorEditScreen_refresh(void);
#ifdef __cplusplus
}
#endif
#endif
