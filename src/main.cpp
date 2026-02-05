#include <Arduino.h>
#include "W25QXX.h"

#define PIN_CS   4
#define PIN_SCK  18
#define PIN_MISO 19
#define PIN_MOSI 23

W25QXX flash(PIN_CS);

void setup() {
    Serial.begin(9600);
    delay(1000);

    SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_CS);

        flash.begin();
}

void loop() {
    W25QXX::ID id = flash.readID();

    Serial.print("Manufacturer ID: 0x");
    Serial.println(id.manufacturer, HEX);

    Serial.print("Memory Type    : 0x");
    Serial.println(id.memoryType, HEX);

    Serial.print("Capacity       : 0x");
    Serial.println(id.capacity, HEX);

    Serial.println("---------------------------");
    delay(1000);
}
