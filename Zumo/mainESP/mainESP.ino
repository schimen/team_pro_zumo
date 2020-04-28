#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "zoEcxQd4vlVVnVFabiLI1Czffukl3K-F"; //Lines ntnu.io atuh
char ssid[] = "Lines nettverk";
char pass[] = "asdf1234";
int inByte;
const int W = 12;
const int A = 13;
const int S = 14;
const int D = 15;
const int C = 16; //Button for charging
bool moved; //Boolean to make sure the values are printed only once
bool manualMode = false; // Controls the switching between manual- and auto mode
bool prevManualMode; // Saved the current status of the same value

BLYNK_WRITE(V0) {
  /*
  Switches between auto and manual mode
  */
  switch (param.asInt()) {
    case 1: //Manual
      manualMode = true;
      if (manualMode != prevManualMode) { //Prints only once
        Serial.print("M");
        prevManualMode = manualMode;
      }
      break;

    case 2: // Auto / Line follow mode
      manualMode = false;
      if (manualMode != prevManualMode) {
        Serial.print("L");
        prevManualMode = manualMode;
      }
      break;
  }
}

BLYNK_WRITE(V1)
/*
The stepper that sends a value from 1 - 4, used to change the max speed
*/
{
  int newStepValue = param.asInt();
  Serial.print(newStepValue);
}

void manualDriving() {
  /*
  Reads the direction, WASD, and charge, C, buttons, change
  */
  if (digitalRead(W) == 1 && moved == false) { //Prints "W" in the Serial
    Serial.write("W");
    moved = true;
  }
  if (digitalRead(A) == 1 &&  moved == false) { //Prints "A"
    Serial.write("A");
    moved = true;
  }
  if (digitalRead(S) == 1 && moved == false) { //Prints "S"
    Serial.write("S");
    moved = true;
  }
  if (digitalRead(D) == 1 && moved == false) { //Prints "D"
    Serial.write("D");
    moved = true;
  }
  if (digitalRead(C) == 1 && moved == false) { //Prints "C"
    Serial.write("C");
    moved = true;
  }

  if (digitalRead(W) == 0 && digitalRead(A) == 0 && digitalRead(S) == 0 && digitalRead(D) == 0 && digitalRead(C) == 0) {
    if (moved == true) {
      Serial.write("0");
      moved = false;
    }
  }
}

void updateBlynkDisplays() {
  /*
 Handles information from the Zumo to be displayed in Blynk
  */
  if (Serial.available() > 0) {
    inByte = Serial.read();

    switch (inByte) {
      case '1': //newSpeedo, new speed
        while (inByte == 1) inByte = Serial.read(); //Awaits a new value in the serial
        Blynk.virtualWrite(V5, inByte);
        break;

      case '2': //newDistanceTotal
        while (inByte == 2) inByte = Serial.read();
        Blynk.virtualWrite(V6, inByte);
        break;

      case '3': //avgSpeed
        while (inByte == 3) inByte = Serial.read();
        Blynk.virtualWrite(V7, inByte);
        break;

      case '4': //distanceSixtyFinal
        while (inByte == 4) inByte = Serial.read();
        Blynk.virtualWrite(V8, inByte);
        break;

      case '5': //measuaredMaxSpeed
        while (inByte == 5) inByte = Serial.read();
        Blynk.virtualWrite(V9, inByte);
        break;

      case '6': //batteryPercent
        while (inByte == 6) inByte = Serial.read();
        Blynk.virtualWrite(V10, inByte);
        break;

      case '7': //lowBattery
        while (inByte == 7) inByte = Serial.read();
        if (inByte == 1) Blynk.virtualWrite(V11, HIGH);
        if (inByte == 0) Blynk.virtualWrite(V11, LOW);
        break;

      case '8': //chargeCycles
        while (inByte == 8) inByte = Serial.read();
        Blynk.virtualWrite(V4, inByte);
      break;

      case'9': //Time over 70% maxSpeed
        while (inByte == 9) inByte = Serial.read();
        Blynk.virtualWrite(V12, inByte);
      break;
    }
  }
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, IPAddress(91, 192, 221, 40), 8080); //ntnu.io server
  while (Blynk.connect() == false) //wait
    Serial.flush();
}

void loop() {
  Blynk.run();
  if (manualMode == true) {
    manualDriving();
    //updateBlynkDisplays();
  }
  else {
    //linjefølging
    //updateBynkDisplays();
  }
}
