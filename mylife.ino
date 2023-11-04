#define TRIG1 19 //TRIG 핀 설정 (초음파 보내는 핀)

#define ECHO1 18 //ECHO 핀 설정 (초음파 받는 핀)
#define TRIG2 13 //TRIG 핀 설정 (초음파 보내는 핀)

#define ECHO2 12 //ECHO 핀 설정 (초음파 받는 핀)

#include <ESP32Servo.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

const char* ssid = "KT_GIGA_2G_Wave2_7652";
const char* password = "bk2fef5307";

String openWeatherMapApiKey = "f47d7b34c834ccb126b14130f1dbc5e4";
String lat = "37.5666791";
String lon = "126.9782914";


int pos1 = 0;
const int servo2 = 4;
const int servo1 = 2;
const int servo3 = 18;
unsigned long lastTime = 0;
unsigned long timerDelay = 10000;
String jsonBuffer;
const int resolvePin = 3;    // 문제 해결을 제어하는 핀 번호 
// 청소기 주기적으로 동작 추가, 청소기 동작시 공기청정기 정지



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

  Serial.println("Timer set to 10 seconds (timerDelay variable), it will take 10 seconds before publishing the first reading.");

  pinMode(14,OUTPUT);   
  pinMode(27,OUTPUT);
  pinMode(servo3,OUTPUT);
  pinMode(resolvePin, OUTPUT);
  digitalWrite(resolvePin, LOW);

  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);

  pinMode(5, OUTPUT);
   ESP32PWM::allocateTimer(0);
   ESP32PWM::allocateTimer(1);
   ESP32PWM::allocateTimer(2);
   ESP32PWM::allocateTimer(3);
   pinMode(servo1, OUTPUT);
  pinMode(servo2, OUTPUT);
  pinMode(servo3, OUTPUT);
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    if (WiFi.status() == WL_CONNECTED) {
      String serverPath = "http://api.openweathermap.org/data/2.5/air_pollution?lat=" + lat + "&lon=" + lon + "&APPID=" + openWeatherMapApiKey;

      HTTPClient http;
      http.begin(serverPath);
      int httpCode = http.GET();
      if (httpCode > 0){
        jsonBuffer = http.getString();
      }
      else{
        Serial.println("Connection Fail.");
      }

      //jsonBuffer = httpGETRequest(serverPath.c_str());
      Serial.println(jsonBuffer);
      JSONVar myObject = JSON.parse(jsonBuffer);

      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
    
      //String pm2_5 = myObject["components"][5];
      double pm2_5 = myObject["list"][0]["components"]["pm2_5"];
      Serial.print("pm2_5 : ");
      Serial.println(pm2_5);
      //double num = pm2_5.toDouble(); // 형변환
      //Serial.print(num);*/
      if (pm2_5<30){
    //팬 작동 30
    analogWrite(servo3,30);
  }
  else if (pm2_5<60){
    //팬 작동 100
    analogWrite(servo3,100);
  }
  else if (pm2_5 >= 60){
    //팬 작동 190
    analogWrite(servo3,190);
    delay(300000);
    analogWrite(servo3,0);
  }  
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
  //
}

void detectPollution() {
  Serial.println("대기오염을 발견했습니다.");

    
    }


void resolveIssue() {
  Serial.println("문제를 해결했습니다.");
  digitalWrite(resolvePin, HIGH); // LED 전등으로 초록불 신호 보내기
  delay(1000);
  digitalWrite(resolvePin, LOW);
}

void moveAround() {
  Serial.println("주변을 이동합니다.");
  long duration1, distance1;
  digitalWrite(TRIG1, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG1, LOW);
  duration1 = pulseIn (ECHO1, HIGH);
  distance1 = duration1 * 17 / 1000; 
  Serial.println(duration1 );
  Serial.print("\nDIstance1 : ");
  Serial.print(distance1);
  Serial.println(" Cm");
  delay(1000);
  long duration2, distance2;
  digitalWrite(TRIG2, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG2, LOW);
  duration2 = pulseIn (ECHO2, HIGH);
  distance2 = duration2 * 17 / 1000; 
  Serial.println(duration2 );
  Serial.print("\nDIstance2 : ");
  Serial.print(distance2);
  Serial.println(" Cm");
  delay(1000);
  



  digitalWrite(servo1, HIGH);
  digitalWrite(servo2, HIGH); 
  delay(1000);
  digitalWrite(servo1, LOW);
  digitalWrite(servo2, LOW);
  delay(1000);  
}