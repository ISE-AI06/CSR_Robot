#include <esp_now.h>
#include <WiFi.h>

#define CHANNEL 1

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("Plub_1", "RX_1_Password", CHANNEL, 0);
  esp_now_init();
  esp_now_register_recv_cb(OnDataRecv);
  pinMode(2,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void OnDataRecv(const esp_now_recv_info *info, const uint8_t *data, int data_len){
  Serial.print("I just recieved:");
  Serial.println(*data);
  digitalWrite(2,*data);
}