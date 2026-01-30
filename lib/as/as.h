#ifndef AS_H
#define AS_H

#include <Arduino.h>

#define SAMPLE_SIZE 50  // Số lần đọc để lấy trung bình (tăng để lọc mạnh hơn)
#define EMA_ALPHA 0.3   // Hệ số EMA filter (0.1-0.5, nhỏ hơn = mượt hơn)

class LightSensor {
  private:
    int sensorPin;
    int minValue;
    int maxValue;
    int lastValue;  // Lưu giá trị trước đó cho EMA filter
    bool inverted;  // Flag để đảo ngược tín hiệu (true = tối = 0%, sáng = 100%)
    
  public:
    // Constructor - nhận chân cảm biến
    LightSensor(int pin);
    
    // Khởi tạo cảm biến
    void init();
    
    // Đọc giá trị analog từ cảm biến (với averaging)
    int readValue();
    
    // Đọc giá trị analog thô (không lọc)
    int readRawValue();
    
    // Đọc giá trị được mapping (0-100)
    int readPercentage();
    
    // Calibrate min/max value
    void calibrate(int minVal, int maxVal);
    
    // Đặt minValue = 0 (mức tối thiểu là 0%)
    void setMinValueToZero();
    
    // Bật/tắt chế độ ngược (khi photodiode hoạt động ngược)
    void setInverted(bool inv);
    
    // Reset EMA filter
    void resetFilter();
};

#endif // AS_H
