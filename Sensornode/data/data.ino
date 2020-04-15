/* * * * * * * * * * *     Blonk website     * * * * * * * * * * * */
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

//include web files

WebServer server(80);

//Enter your SSID and PASSWORD
const char* ssid = ".........";
const char* password = ".........";

void sendHTML() {
  /*
  called when browser open IP
  Sends html content and the message 200 (means everything is ok)
  */
  server.send(200, "text/html", htmlFile);
}

void sendValues() {
  /*
  sends all values
  */

  //values
  server.send(200, "text/plane", String(random(0, 100))); //sensor 1
  server.send(200, "text/plane", String(random(0, 100))); //sensor 2
  server.send(200, "text/plane", String(random(0, 100))); //sensor 3
  //average
  server.send(200, "text/plane", String(random(0, 100))); //sensor 1
  server.send(200, "text/plane", String(random(0, 100))); //sensor 2
  server.send(200, "text/plane", String(random(0, 100))); //sensor 3
  //max min
  server.send(200, "text/plane", String(random(0, 100))); //max
  server.send(200, "text/plane", String(random(0, 100))); //min
}

void setup(void){
  /*
  setup function
  */


  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting Sketch...");

  //connect to wifi:
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println("Connecting to ");
  Serial.print(ssid);

  //Wait for WiFi to connect
  while(WiFi.waitForConnectResult() != WL_CONNECTED){
      Serial.print(".");
    }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

  server.on("/", sendHTML);      //displays page in client browser

  server.begin();                  //Start server
  Serial.println("HTTP server started");

  timer.setInterval(1000L, sendValues); //send values every second
  
  randomSeed(analogRead(A0));
}

void loop() {
  /*
  main loop
  */
  server.handleClient();
  timer.run();
  delay(1);
}
