#include <Arduino.h>

#define RXp2 16
#define TXp2 17
#define DHTPIN 4

unsigned long previousMillis = 0;
const long interval = 2000;

bool readDHT11(uint8_t pin, float &temp, float &hum) {
  uint8_t data[5] = {0, 0, 0, 0, 0};
  uint32_t timeout;

  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delay(18);
  digitalWrite(pin, HIGH);
  delayMicroseconds(30);
  pinMode(pin, INPUT_PULLUP);

  timeout = 10000;
  while (digitalRead(pin) == HIGH) { if (--timeout == 0) return false; }
  timeout = 10000;
  while (digitalRead(pin) == LOW) { if (--timeout == 0) return false; }
  timeout = 10000;
  while (digitalRead(pin) == HIGH) { if (--timeout == 0) return false; }

  for (int i = 0; i < 40; i++) {
    timeout = 10000;
    while (digitalRead(pin) == LOW) { if (--timeout == 0) return false; }
    
    unsigned long startTime = micros();
    timeout = 10000;
    while (digitalRead(pin) == HIGH) { if (--timeout == 0) return false; }
    unsigned long duration = micros() - startTime;

    int byteIndex = i / 8;
    data[byteIndex] <<= 1;
    if (duration > 40) {
      data[byteIndex] |= 1;
    }
  }

  if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
    hum = data[0] + data[1] * 0.1;
    temp = data[2] + data[3] * 0.1;
    return true;
  }
  
  return false;
}

void setup() {
  Serial.begin(9600); 
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2); 
  
  pinMode(DHTPIN, INPUT_PULLUP);
}

void loop() {
  if (Serial2.available()) {
    String sensorData = Serial2.readStringUntil('\n');
    sensorData.trim();
    if(sensorData.length() > 0 && sensorData.indexOf("X=") != -1) {
      Serial.println(sensorData);
    }
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    float temperature = 0;
    float humidity = 0;

    if (readDHT11(DHTPIN, temperature, humidity)) {
      Serial2.print("Pham Anh Duy - B22DCDT064 - Nhiet do: ");
      Serial2.print(temperature, 1);
      Serial2.print("*C, Do am: ");
      Serial2.print(humidity, 1);
      Serial2.println("%");
    }
  }
}