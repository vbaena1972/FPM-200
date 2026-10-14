# Simulador de PC de la HMI del FPM-200 (Windows, LVGL 9 + su driver de Windows).
#
# Compila los MISMOS fuentes de la HMI que el firmware (firmware/main/ui/**.c por glob: una pantalla
# nueva entra sola) con LVGL 9.5.0, la version que fija firmware/dependencies.lock. Los encabezados de
# los componentes del firmware son los REALES (asi AppConfig nunca se desfasa); solo ESP-IDF esta
# simulado en stubs/ y las funciones del backend en sim_backend.c.
#
#   cmake -S Simulator -B Simulator/build
#   cmake --build Simulator/build --config Debug
#   Simulator\build\Debug\fpm200_sim.exe
#
# O en Visual Studio: Archivo > Abrir > Carpeta... > Simulator (lo reconoce como proyecto CMake).
cmake_minimum_required(VERSION 3.20)
project(fpm200_sim C)

if(NOT WIN32)
    message(FATAL_ERROR "Este simulador usa el driver de Windows de LVGL (LV_USE_WINDOWS)")
endif()

set(CMAKE_C_STANDARD 11)
set(FIRMWARE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../firmware)

# ---------------------------------------------------------------- LVGL
include(FetchContent)
# LVGL toma Simulator/lv_conf.h (directorio raiz del proyecto CMake).
set(CONFIG_LV_BUILD_DEMOS OFF CACHE BOOL "" FORCE)
set(CONFIG_LV_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CONFIG_LV_USE_THORVG_INTERNAL OFF CACHE BOOL "" FORCE)
FetchContent_Declare(lvgl
    URL https://github.com/lvgl/lvgl/archive/refs/tags/v9.5.0.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_MakeAvailable(lvgl)

# ---------------------------------------------------------------- HMI
file(GLOB_RECURSE HMI_SOURCES CONFIGURE_DEPENDS ${FIRMWARE_DIR}/main/ui/*.c)
file(GLOB COMPONENT_INCLUDE_DIRS LIST_DIRECTORIES true ${FIRMWARE_DIR}/components/*/include)

add_executable(fpm200_sim
    main.c
    sim_backend.c
    ${FIRMWARE_DIR}/components/storage/ui_pin_hash.c
    ${HMI_SOURCES})
target_include_directories(fpm200_sim PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${FIRMWARE_DIR}/main
    ${FIRMWARE_DIR}/main/ui
    ${FIRMWARE_DIR}/main/ui/screens
    ${FIRMWARE_DIR}/main/ui/fonts
    ${FIRMWARE_DIR}/components/storage
    ${COMPONENT_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs)
target_link_libraries(fpm200_sim PRIVATE lvgl)

if(MSVC)
    # /utf-8: los textos de la HMI llevan tildes/ñ en UTF-8.
    target_compile_options(fpm200_sim PRIVATE /utf-8)
    target_compile_options(lvgl PRIVATE /utf-8)
    target_compile_definitions(fpm200_sim PRIVATE _CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_WARNINGS)
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT fpm200_sim)
endif()
