cmake_minimum_required(VERSION 3.16)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
# 1.4.0: sensores reales estables (SFM3300 leyendo, touch tolerante a I2C,
# recuperacion de bus, 1er paso calibracion FS7, decimales de dashboard).
set(PROJECT_VER "1.4.0")
project(esp32s3_hmi_skeleton)
