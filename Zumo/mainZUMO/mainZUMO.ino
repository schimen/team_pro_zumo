#include <Wire.h>
//#include <EEPROM.h>
#include "SimpleTimer.h"
#include "ZumoKOntroll.h"
#include "Driving.h"
#include "Definitions.h"

/*
This is the main code file to be uploaded to the Zumo car
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
  /*
  send values to esp32:
  */
  writeToESP(currentSpeedToESP,      "BLYNK speedo: ",          zumo.getSpeed());
  writeToESP(distanceTotalToEsP,     "BLYNK distanceTotal: ",   zumo.getTotalDistance());
  writeToESP(measuredMaxSpeedToESP,  "BLYNK maxspeed: ",        zumo.getMaxSpeed());
  writeToESP(newBatteryPercentToESP, "BLYNK battery percent: ", zumo.getBatteryPercent());
  writeToESP(chargeCyclesToESP,      "BLYNK chargeCycles: ",    zumo.batteryChargeCycles;
  writeToESP(seventyTimeToESP,       "BLYNK seventyTime: ",     zumo.getTimeOverSeventyPercent());
  zumo.checkBatteryHealth();

  static bool lowBattery = false;
  if (zumo.isLowBattery() and lowBattery)  { //From defenitions: Index til blynk led: #define lowBatteryToESP 7
    //toggle led
  }
  else if (not zumo.isLowBattery() and not lowBattery) {
    //toggle led
  }

  //reset values used:
  zumo.resetEachSecond();
}

void eachMinute() {
  /*
  function to be used in timer,interval 1 minute
  */
  writeToESP(speedSixtyFinalToESP,    "BLYNK average speed: ", zumo.getAverageSpeed());
  writeToESP(distanceSixtyFinalToESP, "BLYNK new distance: ",  zumo.getNewDistance());
}

void setMode(char input) {
  /*
  Chooses the mode
  */
  switch (input) {
    case 'L': //Auto mode / Line following
      manualMode = false;
      zumo.lineSensors.initFiveSensors();  //Starting the line follow sensors
      zumo.calibrateSensors();
      break;

    case 'M': //manual driving mode
      manualMode = true;
      zumo.motors.setSpeeds(0, 0);
      break;
  }
}

void setMaxSpeed(char input) {
  /*
  Sets the max speed for both driving mode
  in steps from 1 to 4, representing 100, 200, 300 and 400.
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
  /*
  setup code
  */
  zumo.encoders.getCountsAndResetLeft();
  zumo.encoders.getCountsAndResetRight();
  Wire.begin();
  zumo.setupGyro();
  Serial1.begin(9600); //The serial which the ESP is connected
  Serial.begin(9600); //Debugging
  Serial1.flush();
  timer.setInterval(1000L, eachSecond);   //- timer called each second
  timer.setInterval(60000L, eachMinute);  //- timer called each minute
}

void loop() {
  /*
  main loop
  */
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
      //Drives in line mode until it gets another instruction
      while (inChar != 'M') {
        followLine(&zumo);  //from "Driving.h"
        setMaxSpeed(inChar);
        inChar = Serial1.read();
      }
      manualMode = true; //Switches back to manual mode
      zumo.motors.setSpeeds(0, 0);
    }
  }
}
