/*Author: Line
Denne koden skal være på Zumo,
og inneholder all Linjekjøring.

Todo:
Gjør til funksjoner
slå sammen med alle andre funksjoner for Zumo, og lag bibliotek

*/

#include <Zumo32U4.h>
#define NUM_SENSORS 5

Zumo32U4Buzzer buzzer;
Zumo32U4LineSensors lineSensors;
Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;
Zumo32U4LCD lcd;

const int MAXSPEED = 400;
unsigned int lineSensorValues[NUM_SENSORS];
int position = 0;
int lastError = 0;

void calibrate_sensors(){
  lcd.clear();
  lcd.print("Calibrating");
  delay(1000);

  for(int i = 0; i < 120;i++){  //kjører mot høyre, så venstre, så sentrerer seg igjen
    if(i>30 && i<90) { motors.setSpeeds(200,-200);}
    else {motors.setSpeeds(-200,200);}

    lineSensors.calibrate(); //Kalibrerer sensor
  }
  motors.setSpeeds(0,0);
}

void setup() {
  lineSensors.initFiveSensors();  //Starter sensorer
  //LEGG TIL NOE SOM GJØR AT BLIEN VENTER FØR DEN KALIBRERER HER, KNAPP I BLYNK/DELAY
  delay(2000); //LAGT TIL 2 SEK DELAY
  calibrate_sensors(); //kaliberer sensorer
  buzzer.play("L16 cdegreg4");
  while(buzzer.isPlaying());
}

void loop() {
  /*position = lineSensors.readLine(lineSensorValues); //Får posisjon fra sensorer
  lcd.clear();
  lcd.print(position); //Printer posisjon på skjermen*/
  int error = position - 2000; // regner ut feilmargin fra sentrum
  //Bruker feilmarginen til å sette en fartsforskjell på motorene basert på nåværende og forrige feilmargin
  int16_t speedDifference = error / 4 + 6 * (error - lastError);
  lastError = error;
  int leftSpeed = MAXSPEED + speedDifference; // Setter motorhastigheter
  int rightSpeed = MAXSPEED - speedDifference;

  leftSpeed = constrain(leftSpeed, 0, MAXSPEED); //Setter et "tak" for hastighetene i begge ender, om farten er under 0 eller over 400
  rightSpeed = constrain(rightSpeed, 0, MAXSPEED);

  motors.setSpeeds(leftSpeed,rightSpeed); // Endrer hastigheten til motorene
}
