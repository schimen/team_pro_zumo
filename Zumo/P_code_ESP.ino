/*Authors: Torje & Line
  Denne koden inneholder ingen funksjoner.
  Den inneholder alt som skal være i ESP'en,
  for å binde sammen Zumoen og Blynk.
*/
//

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
char auth[] = "zoEcxQd4vlVVnVFabiLI1Czffukl3K-F"; //Lines ntnu.io atuh
char ssid[] = "Lines nettverk";
char pass[] = "asdf1234";
const int W = 12;
const int A = 13;
const int S = 14; //Sett inn pinneverdier. Samsvar med Blynk-app.
const int D = 15;
const int R = 16;
bool moved; //Bool som passer på at ESP bare printer hvert tegn en gang
int inByte;

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, IPAddress(91, 192, 221, 40), 8080);
  pinMode(W, INPUT);
  pinMode(A, INPUT);
  pinMode(S, INPUT);
  pinMode(D, INPUT);
  pinMode(R, INPUT);
  Serial.flush();
}

BLYNK_WRITE(V1)//Stepper som sender 1-4 til Zumo for å bestemme farten
{
  int newStepValue = param.asInt();
  Serial.print(newStepValue);
}


void loop() {
  Blynk.run();

  if (digitalRead(W) == 1 && moved == false) { //Printer "W" i Serial
    Serial.write("W");
    moved = true;
  }
  if (digitalRead(A) == 1 &&  moved == false) { //Printer "A" i Serial
    Serial.write("A");
    moved = true;
  }
  if (digitalRead(S) == 1 && moved == false) { //Printer "S" i Serial
    Serial.write("S");
    moved = true;
  }
  if (digitalRead(D) == 1 && moved == false) { //Printer "D" i Serial
    Serial.write("D");
    moved = true;
  }
  if (digitalRead(R) == 1 && moved == false) { //Printer "D" i Serial
    Serial.write("R");
    moved = true;
  }
  if (digitalRead(W) == 0 && digitalRead(A) == 0 && digitalRead(S) == 0 && digitalRead(D) == 0) { //Denne burde kunne være en "Else", men da printa den gjentatte ganger. Feilsøkte ikke mer. Det finner du ut av hvis du vil
    if (moved == true) {
      Serial.write("0");
      moved = false;
    }
  }

  if (Serial.available() > 0) {
    inByte = Serial.read();
    Serial1.print(inByte);
    switch (inByte) {
      case '1': //newSpeedo, new speed
        while (inByte == 1) inByte = Serial.read(); //Venter på ny verdi i Serial
        Blynk.virtualWrite(V5, inByte);
        break;

      case '2': //newDistanceTotal
        while (inByte == 2) inByte = Serial.read();
        Blynk.virtualWrite(V6, inByte);
        break;

      case '3': //speedSixtyFinal
        while (inByte == 3) inByte = Serial.read();
        Blynk.virtualWrite(V7, inByte);
        break;

      case '4': //distanceSixtyFinal
        while (inByte == 4) inByte = Serial.read();
        Blynk.virtualWrite(V8, inByte);
        break;

      case '5': //maxSpeed
        while (inByte == 5) inByte = Serial.read();
        Blynk.virtualWrite(V9, inByte);
        break;

      case '6': //batteryPercent
        while (inByte == 6) inByte = Serial.read();
        Blynk.virtualWrite(V10, inByte);
        break;

      case '7': //lowBattery
        while (inByte == 7) inByte = Serial.read();
        if (inByte = 1) Blynk.virtualWrite(V11, HIGH);
        else Blynk.virtualWrite(V11, LOW);
        break;
    }
  }
}
