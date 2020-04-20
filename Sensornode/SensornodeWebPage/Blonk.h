/* * * * * * * * * * *     Blonk class     * * * * * * * * * * */

#ifndef Blonk_h
#define Blonk_h

//#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>

//connect to wifi and start server
void startWebPage(const char* ssid, const char* password, uint32_t baudRate = 115200);

//associate file to send on a get request
void sendFileOnGet(String getRequest, String file);

//associate text to send on a get request
void sendTextOnGet(const char* getRequest, String reply);

#endif
