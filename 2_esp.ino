#include <HardwareSerial.h>

const int motor1 = 2; // 모터 1 핀 1
const int motor2 = 4; // 모터 1 핀 2
const int fan = 5; // fan

int distance1 = 0;
int distance2 = 0;
int pm2_5 = 0;

void setup() {
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(27, OUTPUT);

  Serial.begin(115200);
}

void loop() {
  // 시리얼 통신을 통해 거리 및 PM2.5 값을 받아옴
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    int comma1 = data.indexOf(',');
    int comma2 = data.lastIndexOf(',');
    if (comma1 != -1 && comma2 != -1) {
      distance1 = data.substring(0, comma1).toInt();
      distance2 = data.substring(comma1 + 1, comma2).toInt();
      pm2_5 = data.substring(comma2 + 1).toInt();
    }
  }

  // 데이터를 문자열로 변환하여 시리얼 모니터로 출력
  Serial.print("Distance 1: ");
  Serial.print(distance1);
  Serial.print(" cm, Distance 2: ");
  Serial.print(distance2);
  Serial.print(" cm, PM2.5: ");
  Serial.print(pm2_5);
  Serial.println(" ug/m3");

  // 모터 및 팬 제어
  if (distance1 < 10 && distance2 < 10) {
    // 양쪽 센서에서 장애물이 감지될 때, 정지하고 팬 정지
    controlMotors(0, 0);
    controlFan(0);
  } else {
    // 양쪽 센서에서 장애물이 감지되지 않을 때, 전진 및 팬 제어
    controlMotors(1, 1);
    controlFan(pm2_5);
  }
}

void controlMotors(int motor1Speed, int motor2Speed) {
  digitalWrite(motor1, motor1Speed);
  digitalWrite(motor2, motor2Speed);
}

void controlFan(int fanSpeed) {
  analogWrite(fan, fanSpeed);
  analogWrite(14, fanSpeed);
  analogWrite(27, fanSpeed);
}
