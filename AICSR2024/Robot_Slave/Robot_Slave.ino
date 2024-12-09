#include <esp_now.h>
#include <WiFi.h>

#define CHANNEL 1

const int pwm1 = 18;
const int Motor1A = 33;
const int Motor1B = 32;
const int pwm2 = 19;
const int Motor2A = 12;
const int Motor2B = 14;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ชื่อทีม", "RX_1_Password", CHANNEL, 0);
  esp_now_init();
  esp_now_register_recv_cb(OnDataRecv);

  pinMode(pwm1, OUTPUT);
  pinMode(Motor1A, OUTPUT);
  pinMode(Motor1B, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(Motor2A, OUTPUT);
  pinMode(Motor2B, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
}

void OnDataRecv(const esp_now_recv_info *info, const uint8_t *data, int data_len) {
  if (data_len >= 2) {                    // Check if we received at least two bytes
    int cmd_vel_x = (int8_t)data[0];      // First byte for cmd_vel_x
    int cmd_angular_z = (int8_t)data[1];  // Second byte for cmd_angular_z

    Serial.print("Received cmd_vel_x: ");
    Serial.print(cmd_vel_x);
    Serial.print(" | cmd_angular_z: ");
    Serial.println(cmd_angular_z);

    Motor1(min(200 * cmd_vel_x + 100 * cmd_angular_z,255));
    Motor2(min(200 * cmd_vel_x - 100 * cmd_angular_z,255));
  } else {
    Serial.println("Received data length is insufficient.");
  }
}

void Motor1(int speed) {
  analogWrite(pwm1, abs(speed));
  if (speed > 0) {
    digitalWrite(Motor1A, LOW);
    digitalWrite(Motor1B, HIGH);
  } else {
    digitalWrite(Motor1A, HIGH);
    digitalWrite(Motor1B, LOW);
  }
}

void Motor2(int speed) {
  analogWrite(pwm2, abs(speed));
  if (speed > 0) {
    digitalWrite(Motor2A, LOW);
    digitalWrite(Motor2B, HIGH);
  } else {
    digitalWrite(Motor2A, HIGH);
    digitalWrite(Motor2B, LOW);
  }
}
