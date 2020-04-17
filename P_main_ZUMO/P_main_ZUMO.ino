#include <Zumo32U4.h>
#include <EEPROM.h>

Zumo32U4Buzzer buzzer;
Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4ButtonC buttonC;
Zumo32U4LineSensors lineSensors;
Zumo32U4Motors motors;
Zumo32U4Encoders encoders;

int maxSpeed = 100; //Justeres i Blynk
char inChar; //leser innkommende verider fra ESP
bool manualMode = true; //kontrollerer auto/manuell kjøring

void setMode() {  //Velger hvilken modus
  switch (inChar) {
    case 'L': //linjefølging
      manualMode = false;
      lineSensors.initFiveSensors();  //Starter sensorer
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
  Serial1.begin(9600); //Den Serial som ESP er koblet til
  Serial.begin(9600); //Feilsøking
  Serial1.flush();
}

void loop() {
  if (Serial1.available() > 0) {
    inChar = Serial1.read();
    Serial.print(inChar);

    setMode();
    setMaxSpeed();

    if (manualMode == true) {
      manualDriving();
      //patternDriving();
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
      motors.setSpeeds(0,0);
    }
  }
}
