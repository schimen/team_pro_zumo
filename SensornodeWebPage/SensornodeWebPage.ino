#include <WiFi.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>

#include "Sensor.h"  //sensor class for control of sensor reading
#include "SensornodePWM.h"  //pwm class to control buzzer and servo
#include "Definitions.h" //defintions class for cleaner code


const char* ssid = "Marco";
const char* pass = "7Mgb67HK";

AsyncWebServer server (80); //Create WebServer object on port 80

 
void setup() {
  Serial.begin(115200);
  
  if(!SPIFFS.begin(true)){ //Upload the data files
    Serial.println("An Error has occurred while mounting SPIFFS");
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

  server.on("/sensor1Val", [](AsyncWebServerRequest *request){
    request->send(200, String(sensor1.getValue()))
  })

  server.begin();
}
 
void loop() {

}
