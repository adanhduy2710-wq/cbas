#include <Arduino.h>
#include "as.h"

// Tạo đối tượng cảm biến ánh sáng trên pin 4
LightSensor lightSensor(4);

void setup() {
  // Khởi tạo Serial để debug
  Serial.begin(9600);
  delay(1000);
  
  // Khởi tạo cảm biến
  lightSensor.init();
  
  Serial.println("\n\n============================================");
  Serial.println("=== LIGHT SENSOR SIMPLE CALIBRATION ===");
  Serial.println("============================================");
  
  Serial.println("\n** CALIBRATION STEP **");
  Serial.println("Để cảm biến ở môi trường SÁNG BÌNH THƯỜNG");
  Serial.println("Đợi 5 giây...");
  delay(3000);
  
  Serial.println("Đang đo mức sáng bình thường...");
  int normalBrightness = 0;
  for(int i = 0; i < 50; i++) {
    int val = lightSensor.readValue();
    normalBrightness = val;
    Serial.print(".");
    delay(100);
  }
  
  Serial.print("\n  Brightness level: ");
  Serial.println(normalBrightness);
  
  // Calibrate: minValue = sáng bình thường (0%), maxValue = cao hơn (để sáng nhất = 100%)
  // Chúng tôi đặt maxValue = 4095 (giá trị ADC tối đa của ESP32)
  lightSensor.calibrate(normalBrightness, 4095);
  
  Serial.print("\n✓ Calibration done!\n");
  Serial.print("  Reference brightness: ");
  Serial.print(normalBrightness);
  Serial.println(" (0%)");
  Serial.println("  Max brightness: 4095 (100%)");
  Serial.println("============================================");
  Serial.println("Ready to measure!\n");
  
  // Reset EMA filter sau calibration
  lightSensor.resetFilter();
  
  // BẬT CHẾ ĐỘ ĐẢO NGƯỢC (vì photodiode hoạt động ngược)
  lightSensor.setInverted(true);
  Serial.println("✓ Mode: INVERTED");
  Serial.println("  Sáng bình thường = 0%");
  Serial.println("  Sáng nhất = 100%\n");
}

void loop() {
  // Đọc giá trị analog từ cảm biến (đã lọc)
  int avgValue = lightSensor.readValue();
  
  // Đọc giá trị thô để debug
  int rawValue = lightSensor.readRawValue();
  
  // Đọc giá trị phần trăm
  int percentage = lightSensor.readPercentage();
  
  // Xuất dữ liệu ra Serial Monitor
  Serial.print("Raw: ");
  Serial.print(rawValue);
  Serial.print("\t| Avg: ");
  Serial.print(avgValue);
  Serial.print("\t| Brightness: ");
  Serial.print(percentage);
  Serial.println("%");
  
  delay(500); // Đọc mỗi 0.5 giây
}
