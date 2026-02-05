#ifndef W25QXX_H
#define W25QXX_H

#include <Arduino.h>
#include <SPI.h>

class W25QXX {
public:
    struct ID {
        uint8_t manufacturer;
        uint8_t memoryType;
        uint8_t capacity;
        bool    valid;
    };

    W25QXX(uint8_t csPin, SPIClass &spi = SPI);

    bool begin();
    ID   readID();

private:
    uint8_t   _csPin;
    SPIClass *_spi;

    uint8_t transfer(uint8_t data);
};

#endif
