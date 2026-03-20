#ifndef ADXL345_H
#define ADXL345_H

#include <Arduino.h>
#include <Wire.h>

class ADXL345 {
public:
    struct Data {
        int16_t x;
        int16_t y;
        int16_t z;
        bool valid;
    };

private:
    uint8_t _address;
    Data result;

    bool _readRegisters(uint8_t reg, uint8_t *buf, uint8_t len);
    bool _writeRegister(uint8_t reg, uint8_t value);

public:
    ADXL345(uint8_t address = 0x53);
    bool begin(uint8_t sda = 23, uint8_t scl = 18);
    Data read();
};

#endif
