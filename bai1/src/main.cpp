#include <Arduino.h>
#include "ADXL345.h"

ADXL345 accel;

void setup() {
    Serial.begin(9600);
    delay(100);

    if (!accel.begin()) {
        Serial.println("ADXL345 not found!");
        while (1);
    }

    Serial.println("ADXL345 ready");
}

void loop() {
    ADXL345::Data d = accel.read();

    if (d.valid) {
        Serial.print("X: "); Serial.print(d.x);
        Serial.print(" | Y: "); Serial.print(d.y);
        Serial.print(" | Z: "); Serial.println(d.z);
    } else {
        Serial.println("Read error");
    }

    delay(500);
}