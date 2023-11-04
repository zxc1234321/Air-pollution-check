#include <Arduino.h>
#include <HardwareSerial.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

#define TRIG1 19 // 첫 번째 초음파 센서의 TRIG 핀 설정
#define ECHO1 18 // 첫 번째 초음파 센서의 ECHO 핀 설정
#define TRIG2 13 // 두 번째 초음파 센서의 TRIG 핀 설정
#define ECHO2 12 // 두 번째 초음파 센서의 ECHO 핀 설정

const char* ssid = "KT_GIGA_2G_Wave2_7652";
const char* password = "bk2fef5307";

String openWeatherMapApiKey = "f47d7b34c834ccb126b14130f1dbc5e4";
String lat = "37.5666791";
String lon = "126.9782914";

HTTPClient http;
String jsonBuffer;


unsigned long lastTime = 0;
const unsigned long timerDelay = 10000;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);
}

void loop() {
  long duration1, distance1;
  long duration2, distance2;

  // 첫 번째 초음파 센서로 거리 측정
  digitalWrite(TRIG1, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG1, LOW);
  duration1 = pulseIn(ECHO1, HIGH);
  distance1 = duration1 * 0.034 / 2;

  // 두 번째 초음파 센서로 거리 측정
  digitalWrite(TRIG2, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG2, LOW);
  duration2 = pulseIn(ECHO2, HIGH);
  distance2 = duration2 * 0.034 / 2;

  Serial.print("Distance 1: ");
  Serial.print(distance1);
  Serial.print(" cm, Distance 2: ");
  Serial.print(distance2);
  Serial.println(" cm");

  if ((millis() - lastTime) > timerDelay) {
    if (WiFi.status() == WL_CONNECTED) {
      String serverPath = "http://api.openweathermap.org/data/2.5/air_pollution?lat=" + lat + "&lon=" + lon + "&APPID=" + openWeatherMapApiKey;

      if (http.begin(serverPath)) {
        int httpCode = http.GET();
        if (httpCode > 0) {
          jsonBuffer = http.getString();
          Serial.println(jsonBuffer);
          JSONVar myObject = JSON.parse(jsonBuffer);

          // JSON.typeof(jsonVar) can be used to get the type of the var
          if (JSON.typeof(myObject) == "undefined") {
            Serial.println("Parsing input failed!");
            return;
          }

          double pm2_5 = myObject["list"][0]["components"]["pm2_5"];

          // 센서 데이터를 문자열로 변환하여 보냅니다.
          String data = String(distance1) + "," + String(distance2) + "," + String(pm2_5);
          Serial.println(data);
        }
        http.end();
      }
    }
    lastTime = millis();
  }
}
