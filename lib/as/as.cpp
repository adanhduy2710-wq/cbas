#include "as.h"

#define ADC_MAX 4095        // ESP32 ADC 12-bit
#define SAMPLE_COUNT 20    

// --- Khởi tạo ---
AS::AS(uint8_t pin) {
    _pin = pin;
    pinMode(_pin, INPUT);
    result = {0.0f, false};
}

// --- Đọc ADC ---
int AS::_readRaw() {
    uint32_t sum = 0;

    for (int i = 0; i < SAMPLE_COUNT; i++) {
        sum += analogRead(_pin);
        delay(2);
    }

    return sum / SAMPLE_COUNT;
}

// --- Hàm read(): trả về % ánh sáng ---
AS::Data AS::read() {
    int raw = _readRaw();

    if (raw < 0 || raw > ADC_MAX) {
        result.valid = false;
        return result;
    }

    
    result.intensity = (1.0f - (raw / (float)ADC_MAX)) * 100.0f;


    // Giới hạn an toàn
    if (result.intensity > 100.0f) result.intensity = 100.0f;
    if (result.intensity < 0.0f)   result.intensity = 0.0f;

    result.valid = true;
    return result;
}
