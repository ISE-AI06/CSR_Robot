#include <esp_now.h>
#include <WiFi.h>

#define CHANNEL 1

esp_now_peer_info_t slave;

const int button1 = 22;
const int button2 = 19;
const int button3 = 32;
const int button4 = 14;

int V = 0;
int A = 0;
int prevV = 0;
int prevA = 0;

void setup() {
  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_register_send_cb(OnDataSent);
  ScanForSlave();

  esp_now_add_peer(&slave);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  pinMode(button4, INPUT_PULLDOWN);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  V = digitalRead(button1) - digitalRead(button2);
  A = digitalRead(button4) - digitalRead(button3);
  // Serial.print(V);
  // Serial.println(A);
  if ((V != prevV)|(A != prevA)){
    uint8_t dataToSend[2] = { V, A};
    esp_now_send(slave.peer_addr, dataToSend, sizeof(dataToSend));
  }
  prevV = V;
  prevA = A;
}

/** Scan for slaves in AP mode and add as peer if found **/
void ScanForSlave() {
  int8_t scanResults = WiFi.scanNetworks();

  for (int i = 0; i < scanResults; ++i) {
    String SSID = WiFi.SSID(i);
    String BSSIDstr = WiFi.BSSIDstr(i);

    if (SSID.indexOf("123") == 0) {

      int mac[6];
      if (6 == sscanf(BSSIDstr.c_str(), "%x:%x:%x:%x:%x:%x", &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5])) {
        for (int ii = 0; ii < 6; ++ii) {
          slave.peer_addr[ii] = (uint8_t)mac[ii];
        }
      }

      slave.channel = CHANNEL;  // pick a channel
      slave.encrypt = 0;        // no encryption
      break;
    }
  }
}

/** callback when data is sent from Master to Slave **/
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print(V);
  Serial.println(A);
  Serial.println("Message sent!");
}