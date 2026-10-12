idf_component_register(
    SRCS "ota_update.c" "ota_manifest.c"
    INCLUDE_DIRS "include"
    REQUIRES esp_app_format
    PRIV_REQUIRES app_update bootloader_support esp_partition esp_http_client esp_https_ota mbedtls cjson)
