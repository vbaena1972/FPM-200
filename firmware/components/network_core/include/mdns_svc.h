#pragma once
/*
 * mdns_svc — publica el equipo FPM-200 en la LAN por mDNS/DNS-SD para que la app
 * Sensvax lo resuelva por NOMBRE (<serial>.local) en cada apertura, sin depender
 * de una IP DHCP provisionada por BLE (que cambia al reasignar el router).
 *
 * Réplica de lo hecho en MedGuard (firmware/main/mdns_service.c). La app resuelve
 * el A record <serial>.local a la IP viva; el servidor HTTP (transport_http) es
 * quien sirve los endpoints /api/v1.
 */
#include "esp_err.h"

/* Inicia mDNS y registra hostname <instance_id> + servicio _fpm._tcp:80.
 * Idempotente. Llamar con una interfaz de red ya inicializada. */
esp_err_t mdns_svc_start(const char *instance_id);

/* Detiene y libera mDNS. */
void mdns_svc_stop(void);
