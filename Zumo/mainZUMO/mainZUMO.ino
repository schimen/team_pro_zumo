#include <Wire.h>
//#include <EEPROM.h>
#include "SimpleTimer.h"
#include "ZumoKOntroll.h"
#include "Driving.h"
#include "Definitions.h"


/*

veldig bra jobba, ryddet masse og ting ser bedre ut :D

todo:
  - rydd de funksjonene her
  - test koden
  - kommenter
*/




SimpleTimer timer;
ZumoKontroll zumo;

char inChar;

//maxSpeed = 200; //Startfart, justeres i Blynk 100-400
bool manualMode = true; //kontrollerer auto/manuell kjøring

void writeToESP(uint8_t index, String message, float value) {
  Serial1.write(index);
  delay(2);
  Serial1.print(value);
  //debugging:
  Serial.print(message);
  Serial.println(value);
}

void eachSecond() {
  writeToESP(1, "BLYNK speedo: ", zumo.getSpeed()); //skriver fart til esp
  writeToESP(2, "BLYNK distanceTotal: ", zumo.getTotalDistance());
  writeToESP(5, "BLYNK maxspeed: ", zumo.getMaxSpeed());
  writeToESP(6, "BLYNK battery percent: ", zumo.getBatteryPercent());
  writeToESP(7,"Time: ", zumo.getTimeDriven());
  zumo.checkBatteryHealth();

  static bool lowBattery = false;
  if (zumo.isLowBattery() and lowBattery)  {
    //toggle led
  }
  else if (not zumo.isLowBattery() and not lowBattery) {
    //toggle led
  }

  //reset values used:
  zumo.resetEachSecond();
}

void eachMinute() {
  writeToESP(3, "BLYNK average speed: ", zumo.getAverageSpeed());
  writeToESP(4, "BLYNK new distance: ", zumo.getNewDistance());
}

void setMode() {  //Velger hvilken modus
  switch (inChar) {
    case 'L': //linjefølging
      manualMode = false;
      zumo.lineSensors.initFiveSensors();  //Starter linjelesings-sensorer
      zumo.calibrateSensors();
      break;

    case 'M': //manuell kjøring
      manualMode = true;
      zumo.motors.setSpeeds(0, 0);
      break;
  }
}

void setMaxSpeed() { //Setter maxhastighet for begge kjøremoduser
  switch (inChar) {
    case '1':
      zumo.maxSpeed = 100;
      break;

    case '2':
      zumo.maxSpeed = 200;
      break;

    case '3':
      zumo.maxSpeed = 300;
      break;

    case '4':
      zumo.maxSpeed = 400;
      break;
  }
}

void setup() {
  zumo.encoders.getCountsAndResetLeft();
  zumo.encoders.getCountsAndResetRight();
  Wire.begin();
  zumo.setupGyro();
  Serial1.begin(9600); //Den Serial som ESP er koblet til
  Serial.begin(9600); //Feilsøking
  Serial1.flush();
  timer.setInterval(1000L, eachSecond);
  timer.setInterval(60000L, eachMinute);
}

void loop() {
  timer.run();
  zumo.checkIfTurned();
  patternDriving(&zumo);
  
  if (Serial1.available() > 0) {
    inChar = Serial1.read();
    Serial.print(inChar);

    setMode();
    setMaxSpeed();

    if (manualMode == true) {
      manualDriving(&zumo, inChar);
    }

    if (manualMode == false) {
      //kjører linje så lenge den ikke får beskjed om å skifte modus
      while (inChar != 'M') {
        followLine(&zumo);
        setMaxSpeed();
        inChar = Serial1.read();
      }
      manualMode = true; //skifter tilbake til manuell
      zumo.motors.setSpeeds(0, 0);
    }
  }
}
