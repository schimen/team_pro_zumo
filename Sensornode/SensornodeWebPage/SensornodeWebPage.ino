#include <WiFi.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>


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

  server.begin();
}
 
void loop() {

}
