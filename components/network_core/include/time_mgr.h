#pragma once
#include <stdbool.h>

void time_mgr_init(void);
void time_mgr_start_sntp(void);
bool time_mgr_is_time_valid(void);

/* Ajuste manual del reloj (componentes de hora LOCAL, segundos opcionales).
 * Fija la hora del sistema (settimeofday) y el RTC de hardware para que sobreviva
 * reinicios. Devuelve false si la fecha/hora es inválida. Usado por la op de
 * control BLE `set_clock` de la app Sensvax. */
bool time_mgr_set_datetime(int year, int month, int day,
                           int hour, int minute, int second);