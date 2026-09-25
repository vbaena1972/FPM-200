#ifndef UI_SPLASHSCREEN_H
#define UI_SPLASHSCREEN_H

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Duración del splash (ms): cubre la verificación de calibración EEPROM (~2.3 s)
 * y el arranque de sensores, que ahora corren detrás del splash. */
#define UI_SPLASH_MS 3200

extern lv_obj_t *ui_splashScreen;

void ui_splashScreen_screen_init(void);
void ui_splashScreen_screen_destroy(void);

#ifdef __cplusplus
}
#endif

#endif /* UI_SPLASHSCREEN_H */
