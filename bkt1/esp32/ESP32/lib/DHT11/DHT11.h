#ifndef DHT11_H
#define DHT11_H

#include <Arduino.h>
#include <DHT.h>

class DHT11 {
public:
    
    struct Data {
        float temperature;
        float humidity;
        bool valid; 
        bool updated;
    };

private:
    uint8_t _pin;
    DHT _dht; 
    Data _result;
    
    unsigned long _lastReadTime;
    const unsigned long _readInterval = 2000;

public:
    DHT11(uint8_t pin);
    void begin();
    Data read();
};

#endif