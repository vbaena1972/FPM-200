# Simulador FPM-200 — instrucciones rápidas

Todo se corre desde la **raíz del repo** (`D:\SoftwareDevelop\Embedded\FPM-200`), en PowerShell.

## 1. Compilar

**Terminal normal** (sin el entorno de ESP-IDF):

```powershell
cmake -S Simulator -B Simulator/build
cmake --build Simulator/build --config Debug
```

**Terminal con ESP-IDF** (`(venv)` en el prompt): su CMake 4.0.3 no conoce Visual Studio 2026, usar el
del sistema con la ruta completa:

```powershell
& "C:\Program Files\CMake\bin\cmake.exe" -S Simulator -B Simulator/build
& "C:\Program Files\CMake\bin\cmake.exe" --build Simulator/build --config Debug
```

- El primer comando (`-S … -B …`) solo hace falta la primera vez, o si se agregó un `.c` nuevo en
  `firmware/main/ui` (se toman solos al volver a correrlo).
- **Cerrar la ventana del simulador antes de compilar**: si está abierta, falla con
  `LNK1168: cannot open … fpm200_sim.exe for writing`.
- `unresolved external symbol …` al enlazar = la HMI empezó a usar una función nueva del firmware: agregar
  su versión vacía en `Simulator/sim_backend.c`.

## 2. Correr

| Comando | Qué muestra |
|---|---|
| `Simulator\build\Debug\fpm200_sim.exe` | Pantalla principal (480×320 ×2); cicla NORMAL → ADVERTENCIA → ALARMA → SILENCIADA cada 2,5 s |
| `Simulator\build\Debug\fpm200_sim.exe login` | Abre el **login** a los 4 s (tras el arranque) |

- Toques = clic del ratón. La hora es la del PC.
- Usuarios de demostración (en `sim_backend.c`): Técnico `4321`, Administrador `1234`; Fabricante
  (candado) `axira-2026`.
