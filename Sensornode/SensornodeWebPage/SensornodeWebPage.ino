/* * * * * * * * * * *     Sensornode Web Page     * * * * * * * * * * */

#include <WiFi.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include "Sensor.h"  //sensor class for control of sensor reading
#include "Definitions.h" //defintions class for cleaner code

//start values of static members of Sensor class:
uint8_t  Sensor::averageCount = 10; //Number of reads per average value.
uint8_t  Sensor::sensorsOverMax = 0;  //Number of sensors over max limit.
uint16_t Sensor::maxValue = MIN_READ;   // Maximum and minimum
uint16_t Sensor::minValue = MAX_READ;   // measured value.

//objects:
Sensor         sensor1(SENSOR1_PIN, SENSOR1_MIN, SENSOR1_MAX);  //TMP36 sensor
Sensor         sensor2(SENSOR2_PIN, SENSOR2_MIN, SENSOR2_MAX);  //LDR sensor
Sensor         sensor3(SENSOR3_PIN, SENSOR3_MIN, SENSOR3_MAX);  //POT sensor
AsyncWebServer server(PORT);

void setup() {
  Serial.begin(115200);

  if(!SPIFFS.begin(true)){ //Upload the data files
    Serial.println("Error, could not mount SPIFFS");
    return;

  }

  WiFi.begin(SSID, PASSWORD);
  Serial.println("Connecting ");
  while (WiFi.status() != WL_CONNECTED){ //Wait for connection
    delay(1000);
    Serial.print(" .");
  }
  Serial.print("\nConnected to: ");
  Serial.println(WiFi.localIP());

  //send index:
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
    });

  //send images
  server.on(LOGO_LONG, HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, LOGO_LONG);
  });
  server.on(MIDJOSKY, HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, MIDJOSKY);
  });
  server.on(LOGO, HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, LOGO);
  });

  //send js chart library:
  server.on(CHART_BUNDLE, HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, CHART_BUNDLE);
  });

  //Route for live sensor data
  server.on("/currentValues", HTTP_GET, [](AsyncWebServerRequest *request) {
    String json = "{";
    json += "\"sens1Val\": \"" + String(sensor1.getValue(true)) + "\", ";
    json += "\"sens2Val\": \"" + String(sensor2.getValue(true)) + "\", ";
    json += "\"sens3Val\": \"" + String(sensor3.getValue(true)) + "\"";
    json += "}";
    request->send(200, "application/json", json);
  });
  server.on("/averageValues", HTTP_GET, [](AsyncWebServerRequest *request) {
    String json = "{";
    json += "\"sens1Avg\": \"" + String(sensor1.getAverage()) + "\", ";
    json += "\"sens2Avg\": \"" + String(sensor2.getAverage()) + "\", ";
    json += "\"sens3Avg\": \"" + String(sensor3.getAverage()) + "\"";
    json += "}";
    request->send(200, "application/json", json);
  });

    server.on("/minMax", HTTP_GET, [](AsyncWebServerRequest *request) {
    String json = "{";
    json += "\"max\": \"" + String(Sensor::getMax()) + "\", ";
    json += "\"min\": \"" + String(Sensor::getMin()) + "\"";
    json += "}";
    request->send(200, "application/json", json);
  });

  server.begin();
}

void loop() {
  //empty :)
}
