#include <WiFi.h>
#include <SPIFFS.h>
#include <WebServer.h>

#include <ESPAsyncWebServer.h>  
//#include "Blonk.h"
#include "Sensor.h"  //sensor class for control of sensor reading
#include "SensornodePWM.h"  //pwm class to control buzzer and servo
#include "Definitions.h" //defintions class for cleaner code

uint8_t  Sensor::averageCount = 10; //Number of reads per average value.
uint8_t  Sensor::sensorsOverMax = 0;  //Number of sensors over max limit.
uint16_t Sensor::maxValue = MIN_READ;   // Maximum and minimum
uint16_t Sensor::minValue = MAX_READ;   // measured value.

Sensor sensor1(32, 4096);
Sensor sensor2(33, 4096);
Sensor sensor3(34, 4096);

const char* ssid = "marco";
const char* pass = "polo";

//AsyncVersion 

int randNum;

AsyncWebServer server(80);

void setup() {
  /*
  Blonk.sendFileOnGet("/", "/index.html");
  Blonk.sendFileOnGet("/blynk_bootlegLONG.svg", "/blynk_bootlegLONG.svg")
  Blonk.sendFileOnGet("/midjoo.png", "/midjoo.png");
  Blonk.sendFileOnGet("/blynk_bootleg.svg", "/blynk_bootleg.svg");
  Blonk.sendTextOnGet("/sens1Val", String(random(0, 100)));
  Blonk.sendTextOnGet("/sens2Val", String(random(0, 100)));
  Blonk.sendTextOnGet("/sens3Val", String(random(0, 100)));
  Blonk.sendTextOnGet("/sens1Avg", String(random(0, 100)));
  Blonk.sendTextOnGet("/sens2Avg", String(random(0, 100)));
  Blonk.sendTextOnGet("/sens2Avg", String(random(0, 100)));
  Blonk.sendTextOnGet("/min", String(random(0, 100)));
  Blonk.sendTextOnGet("/max", String(random(0, 100)));
  Blonk.startWebPage(ssid, pass);
  
  rip :(
  
  */
  
  Serial.begin(115200);
  
  if(!SPIFFS.begin(true)){ //Upload the data files
    Serial.println("ERROR: Failed to mount SPIFFS");
    return;

  }


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

  //Route for Chart.js
  server.on("/ChartBundle.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/ChartBundle.js");
  });

  //Route for live sensor data
  server.on("/sens1Val", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(sensor1.getValue()));
  });

  server.on("/sens2Val", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(sensor2.getValue()));
  });
  
  server.on("/sens3Val", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(sensor3.getValue()));
  });

  //Route for average data
  server.on("/sens1Avg", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(sensor1.getAverage()));
  });

  server.on("/sens2Avg", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(sensor2.getAverage()));
  });

  server.on("/sens3Avg", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(sensor3.getAverage()));
  });


  //Route for min/max
  server.on("/max", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(Sensor::getMax()));
  });
  
  server.on("/min", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(Sensor::getMin()));
  });
  
  server.begin();
}

void loop(){

}
