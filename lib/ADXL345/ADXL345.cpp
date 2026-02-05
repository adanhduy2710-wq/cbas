#include "ADXL345.h"

#define ADXL345_DEVID       0x00
#define ADXL345_POWER_CTL  0x2D
#define ADXL345_DATA_FORMAT 0x31
#define ADXL345_DATAX0     0x32

ADXL345::ADXL345(uint8_t address) {
    _address = address;
    result = {0, 0, 0, false};
}

bool ADXL345::begin(uint8_t sda, uint8_t scl) {
    Wire.begin(sda, scl);
    delay(100);

    // Kiểm tra DEVID
    uint8_t devid;
    if (!_readRegisters(ADXL345_DEVID, &devid, 1)) return false;
    if (devid != 0xE5) return false;

    // ±16g
    _writeRegister(ADXL345_DATA_FORMAT, 0x0B);

    // Enable measurement
    _writeRegister(ADXL345_POWER_CTL, 0x08);

    delay(10);
    return true;
}

bool ADXL345::_readRegisters(uint8_t reg, uint8_t *buf, uint8_t len) {
    Wire.beginTransmission(_address);
    Wire.write(reg);
    if (Wire.endTransmission(false) != 0) return false;

    Wire.requestFrom(_address, len);
    for (uint8_t i = 0; i < len; i++) {
        if (!Wire.available()) return false;
        buf[i] = Wire.read();
    }
    return true;
}

bool ADXL345::_writeRegister(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(_address);
    Wire.write(reg);
    Wire.write(value);
    return (Wire.endTransmission() == 0);
}

ADXL345::Data ADXL345::read() {
    uint8_t buf[6];

    if (!_readRegisters(ADXL345_DATAX0, buf, 6)) {
        result.valid = false;
        return result;
    }

    result.x = (int16_t)((buf[1] << 8) | buf[0]);
    result.y = (int16_t)((buf[3] << 8) | buf[2]);
    result.z = (int16_t)((buf[5] << 8) | buf[4]);
    result.valid = true;

    return result;
}
