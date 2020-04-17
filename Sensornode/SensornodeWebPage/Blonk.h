/* * * * * * * * * * *     Blonk class     * * * * * * * * * * */

#ifndef Blonk_h
#define Blonk_h

#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>

class Blonk: public AsyncWebServer {
  public:

    //connect to wifi and start server
    void startWebPage(char* ssid, char* password, uint32_t baudRate = 115200);

    //associate file to send on a get request
    void sendFileOnGet(String getRequest, String file);

    //associate text to send on a get request
    void sendTextOnGet(String getRequest, String reply);
    
};

#endif
