#include <esp_now.h>
#include <WiFi.h>

#define CHANNEL 1

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("Plub_1", "RX_1_Password", CHANNEL, 0);
  esp_now_init();
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // put your main code here, to run repeatedly:
}

void OnDataRecv(const esp_now_recv_info *info, const uint8_t *data, int data_len) {
  if (data_len >= 2) { // Check if we received at least two bytes
    int cmd_vel_x = (int8_t)data[0];   // First byte for cmd_vel_x
    int cmd_angular_z = (int8_t)data[1]; // Second byte for cmd_angular_z

    Serial.print("Received cmd_vel_x: ");
    Serial.print(cmd_vel_x);
    Serial.print(" | cmd_angular_z: ");
    Serial.println(cmd_angular_z);
  } else {
    Serial.println("Received data length is insufficient.");
  }
}
