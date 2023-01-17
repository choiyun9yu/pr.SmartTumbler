// 로드셀 센서와 와이파이 통신 코드
// 2022. 08

#include "HX711.h"
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "최윤규의 iPhone";
const char* password = "cyg920901";

const int LOADCELL_DOUT_PIN = 25;
const int LOADCELL_SCK_PIN = 26;
HTTPClient http;

HX711 scale;

void setup() {
  pinMode(15,OUTPUT);
  Serial.begin(115200);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
   delay(1000);
   Serial.println("Connecting to WiFi..");
 }

 Serial.println("Connected to the WiFi network");

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(420.0983);  // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();  // reset the scale to 0

}

void loop() {
 delay(1000);
  int x = scale.get_units(10);
  int weight = x;
  Serial.print("Weight : ");
  Serial.print(weight, 1);
  Serial.println(" g");
  digitalWrite(15,1);
  delay(100);
  digitalWrite(15,0);
  delay(100);
  digitalWrite(15,1);
  delay(100);
  digitalWrite(15,0);
  delay(100);
  digitalWrite(15,1);
  delay(100);
  digitalWrite(15,0);
  delay(100);

 if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status


   if ((weight > 100) && (weight < 2500)) {
     delay(2000);
     String send_value = "http://172.20.10.5:9500/insert/loadcell?loadcell=" + String(weight);
     Serial.println(weight);
     http.begin(send_value);
     digitalWrite(15,1);
     delay(2000);
     int httpCode = http.GET();                                        //Make the request
     

     if (httpCode > 0) { //Check for the returning code

       String payload = http.getString();
       Serial.println(httpCode);
       Serial.println(payload);
     }

     else {
       Serial.println("Error on HTTP request");
     }
   }
 }
 http.end(); //Free the resources
 delay(1000);
}
