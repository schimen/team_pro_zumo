/* Author: Torje & Line
 Denne koden har funksjoner for Zumo.
 Den beskriver switch-casen som håndterer
 instrukser fra Blynk via ESP.
 Den inneholder funksjonen "jesusTakeTheWheel()" som inneholder
 hele switch.casen, og funksjonen "charge()" som er ladefunksjonen.
 Ladefunksjonen av en egen knapp i Blynk.

 Todo:
 Slå sammen med alle andre funksjoner for Zumo, og lag bibliotek

*/
//

#include <Zumo32U4.h>

Zumo32U4Buzzer buzzer;
Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4ButtonC buttonC;
Zumo32U4LineSensors lineSensors;
Zumo32U4Motors motors;
Zumo32U4Encoders encoders;

int maxSpeed = 100; //Starter alltid på 1 i Blynk
char inChar;
bool chargeCheck = false;

void jesusTakeTheWheel() {
  if (Serial1.available() > 0) {
    inChar = Serial1.read();

    switch (inChar) {
      case 'W':
        while (inChar == 'W') {
          motors.setSpeeds(maxSpeed, maxSpeed);
          inChar = Serial1.read();
        }
        delay(20);
        break;
      case 'A':
        while (inChar == 'A') {
          motors.setSpeeds(-200, 200);
          inChar = Serial1.read();
        }
        delay(20);
        break;
      case 'S':
        while (inChar == 'S') {
          motors.setSpeeds(-maxSpeed/1.5, -maxSpeed/1.5);
          inChar = Serial1.read();
        }
        delay(20);
        break;
      case 'D':
        while (inChar == 'D') {
          motors.setSpeeds(200, -200);
          inChar = Serial1.read();
        }
        delay(50);
        break;
      case 'R':
        while (inChar == 'D') {
          Charge();
          inChar = Serial1.read();
        }
        delay(50);
        break;

      case '0':
        motors.setSpeeds(0, 0);
        delay(20);
        break;

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
    } //end switch
  } //end if
} //end func

//----------Charge-----
//Denne funksjonen----- skal kalles på samme måte som WASD så lenge "Charge" i Blynk holdes inne.
//Den lader én prosent per sekund, mens den snurrer og lager lyd.
void charge() {
  motors.setSpeeds(100, -100);
  if (chargeCheck = false) {
    chargeTime = millis();
    chargeCheck = true;
  }
  if (millis(); >= chargeCheck + 1000) {
    charged += 1;
    for (int i = 6000, i > 10000, i += 1000) {
      buzzer.playFrequency(i, 150, 12);
    }
  }
  chargeCheck = false;
  buzzer.stopPlaying();
}


void setup()
{
  Serial1.begin(9600); //Den Serial som ESP er koblet til
  Serial.begin(9600); //Feilsøking
  Serial1.flush();
}

void loop() {
  jesusTakeTheWheel();
}
