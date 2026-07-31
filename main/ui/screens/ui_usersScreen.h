#ifndef UI_USERS_SCREEN_H
#define UI_USERS_SCREEN_H

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Gestión de usuarios (no está en los mockups; complementa el login por rol).
 * Lista las cuentas; ADMIN gestiona técnicos, FABRICANTE también administradores.
 * Se llega desde el chip de usuario del header de "Ajustes generales". */
extern lv_obj_t *ui_usersScreen;

void ui_usersScreen_screen_init(void);
void ui_usersScreen_screen_destroy(void);

#ifdef __cplusplus
}
#endif

#endif /* UI_USERS_SCREEN_H */
