#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "at24c256.h"
#include "fpm_i2c_guard.h"
#include "driver_delay.h"
static unsigned clock_ms, ready_ms, writes, corrupt, fail_forever, delays, reject_blocks, write_protect;
static uint8_t memory[32768];
void vTaskDelay(TickType_t ticks) {
    assert(ticks > 0);
    // Worst phase: next tick is immediate, so only n-1 full ticks elapse.
    clock_ms += (ticks - 1) * 10;
    ++delays;
}
esp_err_t fpm_i2c_bus_add_device(i2c_master_bus_handle_t b, const i2c_device_config_t *c, i2c_master_dev_handle_t *d) { assert(c->scl_speed_hz == 100000); *d=(void*)1; return ESP_OK; }
esp_err_t fpm_i2c_bus_rm_device(i2c_master_dev_handle_t d) { return ESP_OK; }
esp_err_t fpm_i2c_transmit(i2c_master_dev_handle_t d, const uint8_t *b, size_t n, int timeout) {
    ++writes;
    if (fail_forever || (reject_blocks && n > 3) || clock_ms < ready_ms) return ESP_ERR_INVALID_RESPONSE;
    unsigned a = b[0]*256 + b[1];
    assert(n == 3);
    assert(a/64 == (a+n-3)/64);
    if (!write_protect) memcpy(memory+a,b+2,n-2);
    if (corrupt) memory[a] ^= 1;
    ready_ms=clock_ms+5;
    return ESP_OK;
}
esp_err_t fpm_i2c_stop_read(i2c_master_dev_handle_t d, const uint8_t *tx, size_t nt, uint8_t *rx, int timeout) {
    assert(nt == 2);
    if(clock_ms < ready_ms) return ESP_ERR_INVALID_RESPONSE;
    unsigned a=tx[0]*256+tx[1]; *rx=memory[a]; return ESP_OK;
}
int main(void) {
    uint8_t payload[96]; for(unsigned i=0;i<sizeof payload;i++) payload[i]=(uint8_t)i;
    assert(at24c256_init((void*)1,0)==ESP_OK);
    assert(at24c256_write(0,payload,54)==ESP_OK);
    assert(writes==54 && memcmp(memory,payload,54)==0);
    // Crossing a physical page must split safely; start near its boundary.
    assert(at24c256_write(60,payload,96)==ESP_OK);
    assert(memcmp(memory+60,payload,96)==0);
    corrupt=1; assert(at24c256_write(200,payload,20)==ESP_ERR_INVALID_RESPONSE); corrupt=0;
    memory[300]=0xff; fail_forever=1; unsigned before=writes;
    assert(at24c256_write(300,payload,20)==ESP_ERR_INVALID_RESPONSE);
    assert(writes-before==3);
    fail_forever=0;
    reject_blocks=1;
    assert(at24c256_write(400,payload,54)==ESP_OK);
    assert(memcmp(memory+400,payload,54)==0);
    reject_blocks=0; write_protect=1;
    assert(at24c256_write(500,payload,20)==ESP_ERR_INVALID_RESPONSE);
    write_protect=0;
    assert(at24c256_write(32760,payload,20)==ESP_ERR_INVALID_SIZE);
    assert(at24c256_write(0,NULL,20)==ESP_ERR_INVALID_ARG);
    uint8_t original[64];
    memcpy(original, memory + 0x130, sizeof original);
    assert(at24c256_self_test() == ESP_OK);
    assert(memcmp(original, memory + 0x130, sizeof original) == 0);
    write_protect = 1;
    assert(at24c256_self_test() != ESP_OK);
    assert(memcmp(original, memory + 0x130, sizeof original) == 0);
    write_protect = 0;
    printf("PASS: EEPROM timing at 100 Hz, 0x0020, page boundaries, readback corruption, bounded retries, range checks\n");
    return 0;
}
