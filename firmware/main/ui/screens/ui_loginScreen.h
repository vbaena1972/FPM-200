#ifndef UI_LOGINSCREEN_H
#define UI_LOGINSCREEN_H
#include "lvgl.h"
#ifdef __cplusplus
extern "C" {
#endif
extern lv_obj_t *ui_loginScreen;
void ui_loginScreen_screen_init(void);
void ui_loginScreen_screen_destroy(void);
/* Fija el destino tras un login exitoso (NULL = General por defecto). Llamar
 * DESPUÉS de crear la pantalla de login; se consume en el primer acierto. */
void ui_login_set_destination(void (*cb)(void));
#ifdef __cplusplus
}
#endif
#endif