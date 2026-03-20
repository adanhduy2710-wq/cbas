#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11

uint8_t receiverMAC[] = {0xA4, 0xF0, 0x0F, 0x84, 0x09, 0x24};

typedef struct {
    float temperature;
    float humidity;
    bool valid;
} SensorData;

SensorData dataToSend;
DHT dht(DHTPIN, DHTTYPE);

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("[SENDER] Trạng thái gửi: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Thành công" : "Thất bại");
}

void setup() {
    Serial.begin(115200);
    dht.begin();

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    Serial.print("[SENDER] MAC Address: ");
    Serial.println(WiFi.macAddress());

    if (esp_now_init() != ESP_OK) {
        Serial.println("Khởi tạo ESP-NOW thất bại!");
        return;
    }

    esp_now_register_send_cb(onDataSent);

    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, receiverMAC, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Thêm peer thất bại!");
        return;
    }

    Serial.println("ESP-NOW Sender sẵn sàng!");
}

void loop() {
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
        Serial.println("Lỗi đọc DHT11!");
        dataToSend.valid = false;
    } else {
        dataToSend.temperature = t;
        dataToSend.humidity = h;
        dataToSend.valid = true;

        Serial.printf("[SENDER] Nhiệt độ: %.1f°C | Độ ẩm: %.1f%%\n", t, h);
    }

    esp_err_t result = esp_now_send(receiverMAC, (uint8_t *)&dataToSend, sizeof(dataToSend));
    if (result != ESP_OK) {
        Serial.println("Gửi thất bại!");
    }

    delay(1000);
}