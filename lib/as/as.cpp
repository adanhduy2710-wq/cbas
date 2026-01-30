#include "as.h"

// Constructor
LightSensor::LightSensor(int pin) {
  sensorPin = pin;
  minValue = 0;
  maxValue = 4095;
  lastValue = 2048;  // Khởi tạo giá trị trung bình
  inverted = false;  // Mặc định không đảo ngược
}

// Khởi tạo cảm biến
void LightSensor::init() {
  pinMode(sensorPin, INPUT);
}

// Đọc giá trị analog thô (không lọc)
int LightSensor::readRawValue() {
  return analogRead(sensorPin);
}

// Đọc giá trị analog với averaging filter (lấy trung bình)
int LightSensor::readValue() {
  long sum = 0;
  // Đọc nhiều mẫu và lấy trung bình
  for(int i = 0; i < SAMPLE_SIZE; i++) {
    sum += analogRead(sensorPin);
    delayMicroseconds(100);  // Delay rất nhỏ giữa các lần đọc
  }
  int avgValue = sum / SAMPLE_SIZE;  // Trung bình
  
  // Áp dụng EMA filter để làm mượt dữ liệu theo thời gian
  int smoothValue = (int)(EMA_ALPHA * avgValue + (1.0 - EMA_ALPHA) * lastValue);
  lastValue = smoothValue;  // Lưu giá trị cho lần đọc tiếp theo
  
  return smoothValue;
}

// Calibrate min/max value dựa trên cảm biến thực tế
void LightSensor::calibrate(int minVal, int maxVal) {
  minValue = minVal;
  maxValue = maxVal;
}

// Bật/tắt chế độ đảo ngược
void LightSensor::setInverted(bool inv) {
  inverted = inv;
}

// Reset EMA filter
void LightSensor::resetFilter() {
  lastValue = (minValue + maxValue) / 2;  // Đặt lastValue về giữa min/max
}

// Đặt minValue = 0 (mức tối thiểu là 0%)
void LightSensor::setMinValueToZero() {
  minValue = 0;
}

// Đọc giá trị được mapping thành phần trăm (0-100%)
int LightSensor::readPercentage() {
  int rawValue = readValue();  // Dùng readValue() để có giá trị lọc
  
  // Nếu inverted = true, đảo ngược giá trị
  if(inverted) {
    rawValue = maxValue - (rawValue - minValue);
  }
  
  // Map giá trị từ minValue-maxValue thành 0-100
  int percentage = map(rawValue, minValue, maxValue, 0, 100);
  // Đảm bảo giá trị nằm trong khoảng 0-100
  if (percentage < 0) percentage = 0;
  if (percentage > 100) percentage = 100;
  return percentage;
}
