# Genera la clave PRIVADA de firma OTA (RSA-3072, esquema Secure Boot V2) en
# firmware\keys\ota_signing_key.pem. Se hace UNA sola vez por producto.
# Equivalente para Windows de gen_ota_signing_key.sh (no requiere WSL ni bash).
#
#  * NO toca el chip ni quema eFuses: solo crea un archivo en el PC.
#  * El archivo es SECRETO: .gitignore lo excluye. Guardar 2 respaldos fuera de
#    linea (p. ej. USB + gestor de contrasenas de la empresa).
#  * Si se pierde: los equipos ya no aceptan OTA; se actualizan por USB con un
#    firmware firmado con una clave nueva (reversible, sin danar nada).
#  * Usar una clave DISTINTA por producto (MedGuard vs FPM-200).
#
# Uso (desde la terminal "ESP-IDF PowerShell", en la raiz del repo):
#   powershell -ExecutionPolicy Bypass -File tools\gen_ota_signing_key.ps1

$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$keyDir = Join-Path $root 'firmware\keys'
$key = Join-Path $keyDir 'ota_signing_key.pem'

if (Test-Path $key) {
    Write-Host "Ya existe $key - no se sobrescribe (borrarla invalida las OTA futuras)." -ForegroundColor Yellow
    exit 1
}

New-Item -ItemType Directory -Force $keyDir | Out-Null
python -m espsecure generate_signing_key --version 2 --scheme rsa3072 $key
if ($LASTEXITCODE -ne 0 -or -not (Test-Path $key)) {
    Write-Host "No se pudo crear la clave. Abra la terminal 'ESP-IDF PowerShell' (para que exista espsecure) y repita." -ForegroundColor Red
    exit 1
}

Write-Host "Clave creada: $key" -ForegroundColor Green
Write-Host "HAGA AHORA 2 RESPALDOS FUERA DE LINEA de este archivo." -ForegroundColor Yellow
