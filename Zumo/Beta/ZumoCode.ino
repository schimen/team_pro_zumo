/* Koden skal være på Zumoen, og er ment for å ta imot instrukser
    fra ESP'en som er koblet til den.
    Den består nå av switch..case som kaller ulike funksjoner avhengig
    av hva den får inn på seriemonitoren.



*/

#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4Buzzer buzzer;
Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4ButtonC buttonC;
Zumo32U4LineSensors lineSensors;
Zumo32U4ProximitySensors proxSensors;
LSM303 compass;
L3G gyro;
Zumo32U4Motors motors;
Zumo32U4Encoders encoders;


const int leftSpeedPin = 9;
const int rightSpeedPin = 10;
const int leftDir = 15;
const int rightDir = 16;
long int batteryUsedTotal;
long int batteryUsedLeft;
long int batteryUsedRight;
long int batteryTotal = 10000;
int maxSpeed = 300;  //I DENNE KODEN SETTER MAN SPEED SELV HER! Vi får nå maxspeed printet via Serial når man endrer i blynk. Dette er IKKE hensyntatt her!
int i;
char inChar;

void setup()
{
  Serial1.begin(9600); //Den Serial som ESP er koblet til
  pinMode(leftSpeedPin, OUTPUT);
  pinMode(rightSpeedPin, OUTPUT);
  pinMode(leftDir, OUTPUT);
  pinMode(rightDir, OUTPUT);
  Serial1.flush(); //Flusher for å fjerne alt det "tullet" som Blynk printer ved oppkobling. Tror flashen skjer FØR det printes, så den betyr vel i grunn ingenting.
}


//----------- Funksjon for å kjøre bilen bare. Vanlig motors.setspeeds()
void setMotorSpeeds(int leftSpeed, int rightSpeed) {
  if (leftSpeed >= 0) {
    digitalWrite(leftDir, 0);
  }
  if (rightSpeed >= 0) {
    digitalWrite(rightDir, 0);
  }
  if (leftSpeed <= 0) {
    digitalWrite(leftDir, 1);
  }
  if (rightSpeed <= 0) {
    digitalWrite(rightDir, 1);
  }
  analogWrite(leftSpeedPin, abs(leftSpeed));
  analogWrite(rightSpeedPin, abs(rightSpeed));
}


//------ Funksjoner for WASD. A og D snurrer om egen akse med gitt hastighet.
//------ Rygging skjer med redusert hastighet.

void w() {
  delay(50);
  setMotorSpeeds(maxSpeed, maxSpeed);
}

void a() {
  delay(50);
  setMotorSpeeds(50, -50);
}

void s() {
  delay(50);
  setMotorSpeeds(maxSpeed / 1.5, -maxSpeed / 1.5);
}

void d() {
  delay(50);
  setMotorSpeeds(-50, 50);
}
//--------------------------


void loop() {
  if (Serial1.available() > 0) { //Bilen tar imot ulike verdier fra ESP via seriemonitor
    inChar = Serial1.read(); //Switch Case avgjør konsekvensen av de ulike input
    switch (inChar) {
      case 'W': //ESP sender over en streng som inneholder 'W'
        while (inChar == 'W') {
          w();  //Kaller funksjonen som får bilen til å kjøre fremover, så lenge siste inChar er 'W'
          inChar = Serial1.read();
        }
        delay(50);
        break;
      case 'A':
        while (inChar == 'A') {
          a();
          inChar = Serial1.read();
        }
        delay(50);
        break;
      case 'S':
        while (inChar == 'S') {
          s();
          inChar = Serial1.read();
        }
        delay(50);
        break;
      case 'D':
        while (inChar == 'D') {
          d();
          inChar = Serial1.read();
        }
        delay(50);
        break;
      case '0': //ESP sender en '0' som default. Bilen står i ro.
        setMotorSpeeds(0, 0);
        delay(50);
        i = 0;
        break;
    }
    else {
      if (lenght(inChar) > 3) { // ESP sender ny maxspeed fra BLYNK som en verdi mellom 1000 og 1400.
        maxSpeed = int(inChar) - 1000 //Dette omgjøres fra str til int, og det trekkes ifra 1000.
                                      //Det gjørs slik fordi verdier fra 0 - 999 kan representere andre tegn,
                                      //så for å unngå "forvirring" for bilen sender vi maxspeed som verdi fra 1000 - 1400 istedenfor 0 - 400
      }
    }
  }
}
