# ksz8851snl 1.2.0 — copia local PARCHEADA (MedGuard / FPM-200)

Origen: `espressif/ksz8851snl` 1.2.0 (ESP Component Registry, esp-eth-drivers).
Se vendoriza aquí (y se quitó de `main/idf_component.yml`) para aplicar un fix que
upstream aún no tiene.

**Bug:** `emac_ksz8851_receive()` lee `8 + byte_count` bytes del FIFO del chip en
`rx_buffer` (2016 B, heap interno DMA). `byte_count` sale del registro RXFHBCR
(12 bits, hasta 4095) por SPI; si llega corrupto o > 2008, la lectura desborda el
heap interno. En HW (soaks 2026-09-28 y 2026-09-30) aparecían ráfagas de
`received frame was truncated` y después panics al recorrer el heap
(`LoadProhibited` / `Interrupt wdt timeout` en `heap_caps_get_largest_free_block`).

**Fix:** validar `byte_count` antes de reservar/leer; si es inválido se descarta la
cola RX (aviso `RX byte count invalido`, 1 de cada 50). Buscar `PARCHE` en
`src/esp_eth_mac_ksz8851snl.c`.

Al actualizar el driver: comparar con upstream y conservar el parche si no está
corregido allá.
