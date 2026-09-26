#!/usr/bin/env bash
# Genera la clave PRIVADA de firma OTA (RSA-3072, esquema Secure Boot V2) en
# firmware/keys/ota_signing_key.pem. Se hace UNA sola vez por producto.
#
#  * NO toca el chip ni quema eFuses: solo crea un archivo en el PC.
#  * El archivo es SECRETO: .gitignore lo excluye. Guardar 2 respaldos fuera de
#    linea (p. ej. USB cifrada + gestor de contraseñas de la empresa).
#  * Si se pierde: los equipos ya no aceptan OTA; se actualizan por USB con un
#    firmware firmado con una clave nueva (reversible, sin dañar nada).
#  * Usar una clave DISTINTA por producto (FPM-200 vs MedGuard).
#
# Requiere el entorno de ESP-IDF activo (espsecure.py).
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
KEY="$ROOT/firmware/keys/ota_signing_key.pem"
if [ -f "$KEY" ]; then
  echo "Ya existe $KEY — no se sobrescribe (borrarla invalida las OTA futuras)."; exit 1
fi
mkdir -p "$(dirname "$KEY")"
espsecure.py generate_signing_key --version 2 --scheme rsa3072 "$KEY"
chmod 600 "$KEY"
echo "Clave creada: $KEY"
echo "HAGA AHORA 2 RESPALDOS FUERA DE LINEA de este archivo."
