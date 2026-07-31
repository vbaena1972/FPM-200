#ifndef UI_USER_EDIT_SCREEN_H
#define UI_USER_EDIT_SCREEN_H

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Formulario de alta/edición de una cuenta. Fijar el índice ANTES de abrir:
 *   >= 0  editar users[index]
 *   -1    crear una cuenta nueva
 *   -2    cambiar la passphrase del fabricante (solo sesión FABRICANTE) */
extern lv_obj_t *ui_userEditScreen;

#define UI_USER_EDIT_NEW      (-1)
#define UI_USER_EDIT_FACTORY  (-2)

void ui_userEditScreen_set_index(int index);
void ui_userEditScreen_screen_init(void);
void ui_userEditScreen_screen_destroy(void);

#ifdef __cplusplus
}
#endif

#endif /* UI_USER_EDIT_SCREEN_H */
