# Publicar firmware FPM-200

La guía completa (compilar, publicar por OTA en Beta/Estable, instalar, reglas de versiones, microSD/USB) está en
`MedGuard-12IoT/docs/entrega/PUBLICAR_FIRMWARE.md`: el script `scripts/publish_firmware.sh` de ese repo publica
los dos productos.

Resumen FPM:

```powershell
cd D:\SoftwareDevelop\Embedded\FPM-200\firmware
idf.py build
cd D:\SoftwareDevelop\Embedded\MedGuard-12IoT
bash scripts/publish_firmware.sh fpm200 beta ../FPM-200/firmware/build/fpm200_<version>.bin
```

En el equipo: Configuración → login Fabricante → Ajustes generales → **Información** → canal Beta → Buscar →
Instalar (reinicia solo). Release: el mismo `.bin` con `stable` y tag `fpm200-v<version>`. Sin sufijo `-beta.N`.
