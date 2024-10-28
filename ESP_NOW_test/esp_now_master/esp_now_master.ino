#include <esp_now.h>
#include <WiFi.h>

#define CHANNEL 1

esp_now_peer_info_t slave;

uint8_t data = 0;

const int button1Pin = 12;
const int button4Pin = 25;
const int button3Pin = 26;
const int button2Pin = 27;

uint8_t buttonState1 = 0;
uint8_t buttonState2 = 0;
uint8_t buttonState3 = 0;
uint8_t buttonState4 = 0;

int cmd_vel_x = 0;
int cmd_angular_z = 0;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_register_send_cb(OnDataSent);
  ScanForSlave();
  esp_now_add_peer(&slave);

  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);
  pinMode(button3Pin, INPUT);
  pinMode(button4Pin, INPUT);
}

void loop() {
  buttonState1 = digitalRead(button1Pin);
  buttonState2 = digitalRead(button2Pin);
  buttonState3 = digitalRead(button3Pin);
  buttonState4 = digitalRead(button4Pin);

  cmd_vel_x = 0;
  cmd_angular_z = 0;
  
  if (buttonState1 == HIGH) {
    cmd_vel_x += 1;
  }
  if (buttonState2 == HIGH) {
    cmd_vel_x += -1;
  }
  if ((buttonState1 == LOW) && (buttonState2 == LOW)) {
    cmd_vel_x = 0;
  }


  if (buttonState3 == HIGH) {
    cmd_angular_z += -1;
  }
  if (buttonState4 == HIGH) {
    cmd_angular_z += 1;
  }
  if ((buttonState3 == LOW) && (buttonState4 == LOW)) {
    cmd_angular_z = 0;
  }
  uint8_t dataToSend[2] = { cmd_vel_x, cmd_angular_z };
  esp_now_send(slave.peer_addr, dataToSend, sizeof(dataToSend));
  delay(200);
}

/** Scan for slaves in AP mode and add as peer if found **/
void ScanForSlave() {
  int8_t scanResults = WiFi.scanNetworks();

  for (int i = 0; i < scanResults; ++i) {
    String SSID = WiFi.SSID(i);
    String BSSIDstr = WiFi.BSSIDstr(i);

    if (SSID.indexOf("Plub") == 0) {

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
  Serial.print("cmd_vel_x: ");
  Serial.print(cmd_vel_x);
  Serial.print(" | cmd_angular_z: ");
  Serial.println(cmd_angular_z);
}
