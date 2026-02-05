#include "W25QXX.h"

W25QXX::W25QXX(uint8_t csPin, SPIClass &spi) {
    _csPin = csPin;
    _spi   = &spi;
}

bool W25QXX::begin() {
    pinMode(_csPin, OUTPUT);
    digitalWrite(_csPin, HIGH);

    
    _spi->begin(18, 19, 23, _csPin);
    return true;
}

uint8_t W25QXX::transfer(uint8_t data) {
    return _spi->transfer(data);
}

W25QXX::ID W25QXX::readID() {
    ID id;
    id.valid = false;

    _spi->beginTransaction(SPISettings(
        1 * 1000 * 1000,   
        MSBFIRST,
        SPI_MODE0
    ));

    digitalWrite(_csPin, LOW);

    transfer(0x9F);                
    id.manufacturer = transfer(0xFF);
    id.memoryType   = transfer(0xFF);
    id.capacity     = transfer(0xFF);

    digitalWrite(_csPin, HIGH);
    _spi->endTransaction();

    if (id.manufacturer == 0xEF) {
        id.valid = true;
    }

    return id;
}
