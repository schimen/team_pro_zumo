#include "Blonk.h"
#include "Definitions.h"

#define BLONK_PRINT Serial

AsyncWebServer server(80);  //this is only temporary, i want to make a better solution

void Blonk::startWebPage(char* ssid, char* password, uint32_t baudRate)  {
  /*
  connect to wifi and begin server
  */
  BLONK_PRINT.begin(baudRate);
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
  server.begin();
}

void Blonk::sendFileOnGet(String getRequest, String file) {
  /*
  associate a file to send upon a get request from client
  */
  auto sendReply = [](AsyncWebServerRequest *request)  {
                          request->send(SPIFFS, file);
                      });
  server.on(getRequest, HTTP_GET, sendReply);
}

void Blonk::sendTextOnGet(String getRequest, String text) {
  /*
  associate text to send upon a get request from client
  */
  auto sendReply = [](AsyncWebServerRequest *request)  {
                          request->send(200, "text/plain" text);
  server.on(getRequest, HTTP_GET, sendReply);
}
