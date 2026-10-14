# Simulador de PC — HMI FPM-200

Corre la **misma** HMI del firmware (`firmware/main/ui/**.c`: pantallas, widgets y fuentes) en una
ventana de Windows de 480×320 (la del ST7796) con zoom 200 %. Cicla 5 escenarios cada 2,5 s:
NORMAL → ADVERTENCIA (flujo) → ADVERTENCIA (presión baja) → ALARMA → ALARMA SILENCIADA. Usa LVGL 9.5.0 (la de
`firmware/dependencies.lock`) y el driver de Windows de LVGL: no necesita ESP-IDF ni SDL2.

Desde 2026-10-14 es parte de este repo (antes era el repo aparte `FPM-200-Sim`).

> **Instrucciones rápidas (compilar, correr y opciones): [INSTRUCCIONES.md](INSTRUCCIONES.md).**

## Compilar y correr

Requisitos: Visual Studio con C/C++ y CMake 3.20 o más. Desde la raíz del repo:

```bash
cmake -S Simulator -B Simulator/build
cmake --build Simulator/build --config Debug
Simulator\build\Debug\fpm200_sim.exe
```

**Ojo:** correrlo en una terminal **sin** el entorno de ESP-IDF. Ese entorno pone primero su propio CMake
(4.0.3), que no conoce Visual Studio 2026 (`could not create CMAKE_GENERATOR "Visual Studio 18 2026"`). Si
ya estás en la terminal de IDF, usar la ruta completa: `& "C:\Program Files\CMake\bin\cmake.exe" --build
Simulator/build --config Debug`.

La primera vez descarga LVGL en `Simulator/build/_deps/`. También se puede abrir la carpeta
`Simulator/` en Visual Studio (*Archivo → Abrir → Carpeta*).

## Cómo está armado

| Archivo | Para qué |
|---|---|
| `CMakeLists.txt` | Descarga LVGL y compila `main.c`, `sim_backend.c`, `ui_pin_hash.c` y todo `firmware/main/ui/**.c` (glob: una pantalla nueva entra sola; si no aparece, volver a correr `cmake -S Simulator -B Simulator/build`) |
| `main.c` | Ventana 480×320 ×2, escenarios de demostración, hora del PC |
| `sim_backend.c` | `AppConfig` de demostración + versiones vacías de las funciones del firmware que llama la HMI (storage, alarmas, sensores, OTA, MQTT, NVS, tareas) |
| `stubs/` | Encabezados mínimos de ESP-IDF (`esp_err.h`, `esp_log.h`, `nvs.h`, `freertos/…`) |
| `lv_conf.h` | Configuración de LVGL del simulador (32 bpp, `LV_USE_WINDOWS`, QR activado) |

Los encabezados de los componentes (`firmware/components/*/include`) son los **reales**: si cambia
`AppConfig` o una API, el simulador lo ve. Si al enlazar aparece un `unresolved external symbol` nuevo, es
una función del firmware que la HMI empezó a usar: agregar su versión vacía en `sim_backend.c`.

## Notas

- Las tareas de FreeRTOS que cree la HMI no se ejecutan en el simulador (se informa por consola).
- Si tras tocar `lv_conf.h` la pantalla sale en blanco: `cmake --build Simulator/build --config Debug --clean-first`.
- El simulador trabaja a 32 bpp; el equipo, a 16 bpp: cambia la precisión del color, no el diseño.
