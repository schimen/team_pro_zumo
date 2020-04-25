#include <Wire.h>
//#include <EEPROM.h>
#include "SimpleTimer.h"
#include "ZumoKOntroll.h"
#include "Driving.h"
#include "Definitions.h"


/*

veldig bra jobba, ryddet masse og ting ser bedre ut :D

todo:
  - sjekk at alt funker
  - fiks EEPROM
  - rydd de funksjonene her
  - test koden
  - kommenter
  - få bort magiske tall
*/

//objects
SimpleTimer  timer;   //- used library from https://github.com/jfturcot/SimpleTimer
ZumoKontroll zumo;    //- own library, see "ZumoKontroll.h"

//globals:
char inChar;  //character input from esp32
bool manualMode = true; //kontrollerer auto/manuell kjøring

void writeToESP(uint8_t index, String message, float value) {
  Serial1.write(index); //send index to esp32
  delay(2);
  Serial1.print(value); //send value to esp32

  //debugging:
  Serial.print(message);
  Serial.println(value);
}

void eachSecond() {
  //send values to esp32:
  writeToESP(currentSpeedToESP,      "BLYNK speedo: ",          zumo.getSpeed());
  writeToESP(distanceTotalToEsP,     "BLYNK distanceTotal: ",   zumo.getTotalDistance());
  writeToESP(measuredMaxSpeedToESP,  "BLYNK maxspeed: ",        zumo.getMaxSpeed());
  writeToESP(newBatteryPercentToESP, "BLYNK battery percent: ", zumo.getBatteryPercent());
  //writeToESP(chargeCyclesToESP,      "BLYNK chargeCycles: ",    TRENGER FUNKJSON HER);
  //writeToESP(seventyTimeToESP,       "BLYNK seventyTime: ",     TRENGER FUNKSJON HER);
  zumo.checkBatteryHealth();

  static bool lowBattery = false;
  if (zumo.isLowBattery() and lowBattery)  { //Fra defenitions: Index til blynk led: #define lowBatteryToESP 7
    //toggle led
  }
  else if (not zumo.isLowBattery() and not lowBattery) {
    //toggle led
  }

  //reset values used:
  zumo.resetEachSecond();
}

void eachMinute() {
  writeToESP(speedSixtyFinalToESP,    "BLYNK average speed: ", zumo.getAverageSpeed());
  writeToESP(distanceSixtyFinalToESP, "BLYNK new distance: ",  zumo.getNewDistance());
}

void setMode(char input) {  //Velger hvilken modus
  switch (input) {
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

void setMaxSpeed(char input) {
  /*
  Setter maxhastighet for begge kjøremoduser
  alternativer 1 til 4 som korresponderer med 100 til 400
  */
  switch (input) {
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
  timer.setInterval(1000L, eachSecond);   //- timer called each second
  timer.setInterval(60000L, eachMinute);  //- timer called each minute
}

void loop() {
  timer.run();  //start timers
  zumo.checkIfTurned();
  patternDriving(&zumo);  //from "Driving.h"

  if (Serial1.available() > 0) {
    inChar = Serial1.read();
    Serial.print(inChar);

    setMode(inChar);
    setMaxSpeed(inChar);

    if (manualMode == true) {
      manualDriving(&zumo, inChar); //from "Driving.h"
    }

    if (manualMode == false) {
      //kjører linje så lenge den ikke får beskjed om å skifte modus
      while (inChar != 'M') {
        followLine(&zumo);  //from "Driving.h"
        setMaxSpeed(inChar);
        inChar = Serial1.read();
      }
      manualMode = true; //skifter tilbake til manuell
      zumo.motors.setSpeeds(0, 0);
    }
  }
}
