#ifndef AS_H
#define AS_H

#include <Arduino.h>

class AS {
public:
    struct Data {
        float intensity;   // Cường độ ánh sáng (%)
        bool valid;        // Cờ báo dữ liệu hợp lệ
    };

private:
    uint8_t _pin;
    Data result;

    int _readRaw();        // Đọc ADC thô

public:
    AS(uint8_t pin);
    Data read();           // Đọc cảm biến và trả kết quả
};

#endif
