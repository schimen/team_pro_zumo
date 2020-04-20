#include "Blonk.h"
#include "Definitions.h"

#define BLONK_PRINT Serial

AsyncWebServer server(80);  //this is only temporary, i want to make a better solution

auto sendReply = [](AsyncWebServerRequest *request){
// Lambda body implementation
};

void startWebPage(char* ssid, char* password, uint32_t baudRate)  {
  /*
  connect to wifi and begin server
  */
  /*BLONK_PRINT.begin(baudRate);
  if(!SPIFFS.begin(true)){ //Upload the data files
    BLONK_PRINT.println("ERROR: Failed to mount SPIFFS");
    return;
  }
  WiFi.begin(ssid, password);
    BLONK_PRINT.println("Connecting...");
  while (WiFi.status() != WL_CONNECTED){ //Wait for connection
    delay(1000);
    BLONK_PRINT.println(".");
  }
  BLONK_PRINT.println("CONNECTED!");
  BLONK_PRINT.println(WiFi.localIP());
  server.begin();*/
  Serial.println(" ");
}

/*auto testFctn = [](AsyncWebServerRequest *request)  {
                          request->send(SPIFFS, "test");
                      };*/

//void sendFileOnGet(String getRequest, String file) {
  /*
  associate a file to send upon a get request from client
  */
  /*sendReply = [](AsyncWebServerRequest *request)  {
                          request->send(SPIFFS, "test");
                      };*/
  const char* getRequest = "/";
  server.on(getRequest, HTTP_GET, [](AsyncWebServerRequest *request){
// Lambda body implementation
});
  Serial.print(" ");
}

void sendTextOnGet(const char* getRequest, String text) {
  /*
  associate text to send upon a get request from client
  */
  /*auto sendReply = [](AsyncWebServerRequest *request) {
                        request->send(200, "text/plain", "testin");
                      };
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  // Lambda body implementation
  });*/
  Serial.println(" ");
}
