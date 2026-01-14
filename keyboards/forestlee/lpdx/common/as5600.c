#include "as5600.h"
#include "print.h"

static const uint8_t AS5600_I2C_ADDRESS = (0x36 << 1);

void as5600_init(void) {
    i2c_init();
}

uint16_t as5600_get_rawangle(void) {
    uint8_t data[] = {0, 0};
    i2c_status_t s = i2c_read_register(AS5600_I2C_ADDRESS, REG_RAWANGLE, data, 2, POINTING_DEVICE_AS5600_I2C_TIMEOUT);
    if (s == I2C_STATUS_TIMEOUT) {
        uprintf("Timeout on as5600_get_rawangle()\n");
    } else if (s == I2C_STATUS_ERROR) {
        uprintf("Error on as5600_get_rawangle()\n");
    } else {
        ;
    }
    uint16_t rawangle = data[0] << 8 | data[1];
    return rawangle;
}

bool as5600_is_magnet_too_high(void) {
    uint8_t data[] = {0};
    i2c_read_register(AS5600_I2C_ADDRESS, REG_STATUS, data, 1, 100);
    uint8_t v = (data[0] >> 3) & 0x1;
    if (v == 1) {
        return true;
    } else {
        return false;
    }
}

bool as5600_is_magnet_too_low(void) {
    uint8_t data[] = {0};
    i2c_read_register(AS5600_I2C_ADDRESS, REG_STATUS, data, 1, 100);
    uint8_t v = (data[0] >> 4) & 0x1;
    if (v == 1) {
        return true;
    } else {
        return false;
    }
}

bool as5600_is_magnet_present(void) {
    uint8_t data[] = {0};
    i2c_read_register(AS5600_I2C_ADDRESS, REG_STATUS, data, 1, 100);
    uint8_t v = (data[0] >> 5) & 0x1;
    if (v == 1) {
        return true;
    } else {
        return false;
    }
}
