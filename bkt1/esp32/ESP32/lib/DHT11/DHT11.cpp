#include "DHT11.h"
DHT11::DHT11(uint8_t pin) : _pin(pin), _dht(pin, DHT11) {
    _result.temperature = 0.0;
    _result.humidity = 0.0;
    _result.valid = false;
    _result.updated = false;
    _lastReadTime = 0;
}

void DHT11::begin() {
    _dht.begin();
}

DHT11::Data DHT11::read() {
    _result.updated = false; 
    
    unsigned long currentMillis = millis();
    if (currentMillis - _lastReadTime >= _readInterval) {
        _lastReadTime = currentMillis;

        float h = _dht.readHumidity();
        float t = _dht.readTemperature();
        if (isnan(h) || isnan(t)) {
            _result.valid = false;
        } else {
            _result.temperature = t;
            _result.humidity = h;
            _result.valid = true;
        }

        _result.updated = true; 
    }
    return _result;
}