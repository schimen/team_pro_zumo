#include "gyroZumo.h"

void setupGyro()  {
  gyro.init(); //Initaliserer gyro
  gyro.enableDefault(); //Setter alt i gyroen til default
}


/*bool isTurned() {
  if (turned) {
    return true;
  }
  else  {
    return false;
  }
}*/

void updateGyro() { //Sjekker rotasjonsposisjon til Zumoen
  static unsigned long timeInterval = 0;
  static int yAxis = 0;
  gyro.read(); //Leser gyroens verdi
  if ((millis() - TimeInterval) >= 100) { //Oppdaterer endringa av Y hvert 100ms
    TimeInterval = millis();
    yAxis = gyro.g.y; //Lagrer Y-posisjon
  }
  if (abs(yAxis) > 3000) { //Hvis bilen roterer rundt tverrgående-akse
    startTime = millis(); //Starter en teller
    bool OneSwitch = true; //Sikrer at turnSense kun endrer seg en gang per rotasjon
    while ( abs(yAxis) > 3000) { //Så læng Zumoen rotere
      if ((millis() >= startTime + 500) && (OneSwitch == true)) { //Hvis bilen har rotert i mer enn 0.5 sek
        turnSense = !turnSense; //Toggle turnSense slik at bilen er opp ned eller rett vei annen hver gang
        Serial.print("turnSense: ");
        Serial.println(turnSense);
        OneSwitch = false; // turnSense kan ikke toggles mer så lenge bilen roterer før neste runde igjen
      }
      gyro.read();// Sjekke om bilen har sluttet å rotere
      if ((millis() - TimeInterval) >= 100) { //Oppdaterer endringen av Y hvert 100ms
        TimeInterval = millis();
        yAxis = gyro.g.y;
      }
    }
  }
}


void checkIfTurned() {  //turnt
  bool serviceSoundPlayed = false;
  bool changedSoundPlayed = false;
  updateGyro(); // Oppdaterer Y-posisjon
  if (turnSense == true) { //Hvis bilen har blitt snudd
    startTime = millis(); //Starter en teller

    while (turnSense == true) { //Så lengde bilen er snudd
      if (startTime + 3000 <= millis() < startTime + 5000) { //Hvis bilen har vært snudd 2s eller over, men mindre enn 5s
        if (serviceSoundPlayed == false) {
          Serial.println("SERVICE");
          workDone = SERVICE;
          buzzer.play("!T87 L8 V7 ce"); //Gir lydvarsel for service utført
          serviceSoundPlayed = true;
          while (buzzer.isPlaying());
        }
      }
      if (millis() >= startTime + 5000) { //Hvis bilen har vært snudd 5s eller over
        if (changedSoundPlayed == false) {
          Serial.println("CHANGED");
          workDone = CHANGED;
          buzzer.play("!T60 L8 V7 ged"); //Gir lydvarsel for bytte av batteri utført
          while (buzzer.isPlaying());
          changedSoundPlayed = true;
        }
      }
      updateGyro(); //Oppdatere Y-posisjon fer å sjekke om bilen er snudd igjen
    }

    if (workDone == SERVICE) {
      Serial.println("workDone is service");
      batteryChargeCycles = 1; // Fjerner 1 ladesyklus
      batteryChargedTotal = 100;
      batteryLeft = 100; // Resetter batteriprosent til 100
      newCharge == true; // Oppdaterer batteriutregninger
      workDone = 0; //  Resetter workDone variablen
      speedometer();

    }
    if (workDone == CHANGED) {
      batteryChargeCycles = 0; //Resetter ladesyklusene til 0
      Serial.println("workDone is changed");
      batteryLeft = 100; //Resetter batteriprosent til 100
      batteryChargedTotal = 0; //resetter batteryChargeCycles-utregningen
      workDone = 0;
      speedometer();
    }
  }
}
