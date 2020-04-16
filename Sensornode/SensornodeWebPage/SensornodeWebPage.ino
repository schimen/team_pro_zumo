#include <WiFi.h>
#include <SPIFFS.h>
//#include <WebServer.h>

#include <ESPAsyncWebServer.h>


/*//SensorLibaryFrom Schimen
#include "Sensor.h"  //sensor class for control of sensor reading
#include "SensornodePWM.h"  //pwm class to control buzzer and servo
#include "Definitions.h" //defintions class for cleaner code

uint8_t  Sensor::averageCount = 10; //Number of reads per average value.
uint8_t  Sensor::sensorsOverMax = 0;  //Number of sensors over max limit.
uint16_t Sensor::maxValue = MIN_READ;   // Maximum and minimum
uint16_t Sensor::minValue = MAX_READ;   // measured value.

Sensor sensor1(33, 1000);

*/


const char* ssid = "Marco";
const char* pass = "7Mgb67HK";


/*
WebServer server (80); //Create WebServer object on port 80

void setup(){
    Serial.begin(115200);

    if(!SPIFFS.begin(true)){
        Serial.println("ERROR: Failed to mount SPIFFS");
        return;
    }

    WiFi.begin(ssid, pass);
    Serial.println("Connnecting...");
    if(WiFi.status() != WL_CONNECTED){
        delay(1000);
        Serial.println(".");
    }
    Serial.print("Connected: ");
    Serial.println(ssid);
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());

    server.on("/", []{server.send(SPIFFS, "text/html", "/index.html");});
}

void loop(){

}
*/
//AsyncVersion 

int randNum;

AsyncWebServer server(80);
void setup() {
  Serial.begin(115200);
  
  if(!SPIFFS.begin(true)){ //Upload the data files
    Serial.println("ERROR: Failed to mount SPIFFS");
    return;

  }

  randomSeed(analogRead(34));

  WiFi.begin(ssid, pass);
  Serial.println("Connecting...");
  while (WiFi.status() != WL_CONNECTED){ //Wait for connection
    delay(1000);
    Serial.println(".");
  }
  Serial.println("CONNECTED!");
  Serial.println(WiFi.localIP());

  //Route for webpage
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
    });
  //Route for images
  server.on("/blynk_bootlegLONG.svg", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/blynk_bootlegLONG.svg");
  });
  server.on("/midjoo.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/midjoo.png");
  });
  server.on("/blynk_bootleg.svg", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/blynk_bootleg.svg");
  });


  //Route for live sensor data
  server.on("/sens1Val", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(random(0,100)));
  });

  server.on("/sens2Val", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(random(0,100)));
  });
  
  server.on("/sens3Val", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(random(0,100)));
  });

  //Route for average data
  server.on("/sens1Avg", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(random(0,100)));
  });

  server.on("/sens2Avg", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(random(0,100)));
  });

  server.on("/sens3Avg", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(random(0,100)));
  });


  //Route for min/max
  server.on("/max", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(random(0,100)));
  });
  
  server.on("/min", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(random(0,100)));
  });



  server.begin();
}

 void loop(){

 }
