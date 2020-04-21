#include <Wire.h>
#include <EEPROM.h>
#include "zumoObjects.h"
#include "lineFollower.h"
#include "gyroZumo.h"
#include "manualDriving.h"
#include "patternDriving.h"

//maxSpeed = 200; //Startfart, justeres i Blynk 100-400
char inChar; //leser innkommende verider fra ESP
bool manualMode = true; //kontrollerer auto/manuell kjøring

void setMode() {  //Velger hvilken modus
  switch (inChar) {
    case 'L': //linjefølging
      manualMode = false;
      lineSensors.initFiveSensors();  //Starter linjelesings-sensorer
      calibrateSensors();
      break;

    case 'M': //manuell kjøring
      manualMode = true;
      motors.setSpeeds(0, 0);
      break;
  }
}

void setMaxSpeed() { //Setter maxhastighet for begge kjøremoduser
  switch (inChar) {
    case '1':
      maxSpeed = 100;
      break;

    case '2':
      maxSpeed = 200;
      break;

    case '3':
      maxSpeed = 300;
      break;

    case '4':
      maxSpeed = 400;
      break;
  }
}
void setup() {
  encoders.getCountsAndResetLeft();
  encoders.getCountsAndResetRight();
  Wire.begin();
  setupGyro();
  Serial1.begin(9600); //Den Serial som ESP er koblet til
  Serial.begin(9600); //Feilsøking
  Serial1.flush();
}

void loop() {
  checkIfTurned();
  patternDriving();
  if (Serial1.available() > 0) {
    inChar = Serial1.read();
    Serial.print(inChar);

    setMode();
    setMaxSpeed();

    if (manualMode == true) {
      manualDriving(inChar);
    }

    if (manualMode == false) {
      //kjører linje så lenge den ikke får beskjed om å skifte modus
      while (inChar != 'M') {
        followLine();
        speedometer();
        setMaxSpeed();
        inChar = Serial1.read();
      }
      manualMode = true; //skifter tilbake til manuell
      motors.setSpeeds(0, 0);
    }
  }
}
