# Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)

Documento común a los dos proyectos hermanos (copia idéntica en
`MedGuard-12IoT/docs/entrega/SEGURIDAD_PRODUCCION.md`). Ambos equipos deben comportarse igual.

## 1. Estado actual: lo activado es 100 % reversible (NINGÚN eFuse)

| Medida | MedGuard | FPM-200 | ¿Toca el chip? |
|---|---|---|---|
| Task WDT con reinicio (panic) + lazo de alarmas supervisado | Sí (1.7.0-dev) | Sí (1.5.22) | No |
| Core dump en flash (partición `coredump`) | Sí | Sí | No |
| Vuelta atrás de OTA (`BOOTLOADER_APP_ROLLBACK_ENABLE`) | Sí | Sí (nuevo) | No* |
| OTA firmada sin Secure Boot (`SECURE_SIGNED_APPS_NO_SECURE_BOOT`) | Sí (nuevo) | Sí (nuevo) | No |
| PIN/passphrase como hash PBKDF2-SHA256 con sal | Sí (nuevo) | Sí (nuevo) | No |
| Secure Boot V2 | **No** | **No** | eFuse — solo producción (§4) |
| Cifrado de flash + NVS cifrada | **No** | **No** | eFuse — solo producción (§4) |

\* El rollback exige un bootloader compilado con esa opción: se graba **por USB una
vez** (el bootloader no viaja por OTA). Es software; se puede volver a grabar otro.

Verificado en `sdkconfig` de ambos: `# CONFIG_SECURE_BOOT is not set` y
`# CONFIG_FLASH_ENCRYPTION_ENABLED is not set`. Con estas opciones **por USB siempre
se puede grabar cualquier firmware**: no hay forma de dejar una placa inservible.

## 2. OTA firmada — cómo funciona y cómo operarla

- Al compilar, ESP-IDF firma la app con `firmware/keys/ota_signing_key.pem`
  (RSA-3072, esquema Secure Boot V2). Salen `build/<app>-unsigned.bin` y
  `build/<app>.bin` (**firmado**: ese es el que se publica / copia a microSD).
- El equipo acepta una OTA (red o microSD) solo si está firmada con **la misma
  clave** que el firmware que ya corre. Un `.bin` ajeno se rechaza en `esp_ota_end`.
- **Primera vez**: generar la clave (una por producto) y grabar el primer firmware
  firmado **por USB** (o por una OTA normal desde un firmware viejo, que no verifica).
  - MedGuard: `bash scripts/gen_ota_signing_key.sh`
  - FPM-200: `bash tools/gen_ota_signing_key.sh`
- Sin la clave el build falla con un error claro (es intencional: nadie publica un
  firmware sin firmar).
- **Custodia de la clave**: es el secreto más importante del producto. Nunca en git
  (`.gitignore` lo excluye). Dos respaldos fuera de línea (USB cifrada + gestor de
  contraseñas de la empresa). Si se pierde: los equipos dejan de aceptar OTA, pero se
  actualizan por USB con un firmware firmado con una clave nueva. Nada se daña.
- Verificar una imagen: `espsecure.py verify_signature --version 2 --keyfile
  firmware/keys/ota_signing_key.pem build/<app>.bin`.
- **Nombre del archivo para microSD** (2026-09-28): el build deja una copia firmada
  con la versión en el nombre — MedGuard `build/medguard_<version>.bin`, FPM-200
  `build/fpm200_<version>.bin`. Los equipos aceptan ese nombre (y el heredado
  `firmware.bin`); si hay varios, eligen la **versión más alta** leída del
  descriptor DENTRO de la imagen e ignoran imágenes de otro producto. El nombre es
  solo para las personas: lo que se valida es proyecto + versión + firma internos.
- **Cómo probar el rechazo de una imagen sin firma/ajena** (no usar el
  `-unsigned.bin` del MISMO build que ya está instalado en la otra partición: antes
  del fix 2026-09-28 heredaba la firma que quedaba en la flash — falso positivo, ver
  §2.1): firmar un build con OTRA clave (`espsecure.py generate_signing_key ...
  otra.pem` + `sign_data`) o usar el `-unsigned.bin` de una versión distinta.

### 2.1 Fix 2026-09-28: bloque de firma residual

El bloque de firma va justo después del final de la imagen (alineado a 4 KB). La
escritura OTA secuencial borra solo lo que escribe, así que el resto de la partición
conserva lo que tenía. Si la partición destino guardaba la versión FIRMADA de un
binario y se copiaba el mismo binario SIN firmar, la verificación encontraba la
firma vieja (válida para ese mismo contenido) y lo aceptaba. No permitía instalar
código distinto al ya firmado, pero debilitaba la garantía. Ahora ambos equipos
borran esa zona (64 KB tras el final escrito) antes de `esp_ota_end`
(MedGuard `storage/ota_service.c`, FPM `components/ota_update/ota_update.c`).
- **OTA por microSD en FPM-200** (1.5.26-dev; nombre versionado desde 1.5.27-dev):
  copiar el `.bin` FIRMADO (`build/fpm200_<version>.bin`, o
  `build/esp32s3_hmi_skeleton.bin` renombrado `firmware.bin`) a la raíz de la
  tarjeta e insertarla: el mismo overlay que aplica `AppConfig.json` y certificados valida el
  proyecto y la firma, instala en la otra partición y pide reiniciar. El archivo se
  renombra a `<nombre>.instalado` / `.rechazado`. Un corte de energía durante la
  escritura deja el firmware actual intacto. (OTA por red: pendiente, la URL ya
  existe en la config.)

## 3. PIN con hash — cómo funciona

- Formato guardado: `p1$<sal 8 bytes hex>$<clave derivada 16 bytes hex>`,
  PBKDF2-HMAC-SHA256, 4096 iteraciones. Módulo portable idéntico en ambos:
  MedGuard `firmware/ui/ui_pin_hash.c`, FPM `firmware/components/storage/ui_pin_hash.c`.
- **Migración automática**: al arrancar, los PIN en claro del firmware anterior se
  convierten a hash y se guardan (una vez). Los PIN siguen funcionando igual.
- Un PIN olvidado no se puede "ver": se restablece (fabricante / administrador).
- La app sigue igual: lee PIN redactados y solo envía un PIN cuando es nuevo; el
  equipo lo convierte a hash al recibirlo. (FPM además dejó de **borrar** el PIN
  cuando la app reenviaba la config con el PIN redactado — alineado con MedGuard.)
- Límite honesto: evita **leer** PIN de un volcado/respaldo. Un PIN de 4 dígitos se
  puede adivinar por fuerza bruta si alguien extrae el hash; la protección completa
  del almacenamiento es el cifrado de flash (§4). La passphrase del fabricante
  (≥ 8 alfanuméricos) sí queda bien protegida.

## 4. Producción — Secure Boot V2 + cifrado de flash (IRREVERSIBLE)

> **NO EJECUTAR EN PLACAS DE DESARROLLO.** Quema eFuses de forma permanente. Hacerlo
> solo cuando el firmware esté congelado, y primero en 1–2 placas sacrificables.

Protege lo que el software no puede: la **clave privada de AWS** y la config en NVS
(cifradas en flash), y que el chip solo arranque firmware firmado.

1. **Preparación** (una vez por producto): clave de Secure Boot (puede ser la misma
   de firma OTA), clave de cifrado de flash generada por el chip, procedimiento de
   respaldo aprobado. Revisar que el hardware no necesite JTAG/USB-JTAG en campo.
2. **Configuración** (rama de release, no `main` de desarrollo):
   `CONFIG_SECURE_BOOT=y` (V2 RSA), `CONFIG_SECURE_FLASH_ENC_ENABLED=y` en modo
   **RELEASE**, `CONFIG_NVS_ENCRYPTION=y`, `CONFIG_BOOTLOADER_APP_ANTI_ROLLBACK`
   (versión mínima segura). Partición `nvs_keys` en la tabla.
3. **Prueba en placa sacrificable**: grabar, verificar arranque, OTA firmada, rollback,
   NVS (config + certificados AWS) y reimportación de certificados.
4. **Línea de producción**: script único que graba + verifica + registra el número de
   serie y el resultado. Tras el primer arranque la flash queda cifrada y el USB solo
   acepta firmware firmado.
5. **Riesgo principal**: perder la clave de Secure Boot = no se puede actualizar nunca
   más esos equipos (ni por USB). Custodia doble obligatoria.

## 5. Mientras tanto: mitigación en la nube (sin tocar el chip)

- **Un certificado AWS por equipo** con política IoT mínima (solo sus propios tópicos
  y su shadow; nada de `iot:*`).
- Si un equipo se pierde o lo roban: **revocar su certificado** en AWS IoT → esa clave
  privada deja de servir, aunque alguien la extraiga.
- Rotar el token LAN (se regenera borrando la clave `httpmon/token` en NVS / por
  restablecimiento) si un equipo cambia de sitio.
