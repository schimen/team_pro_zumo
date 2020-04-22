#include "ZumoKontroll.h"

ZumoKontroll::ZumoKontroll()  {
  //constructor
}

void ZumoKontroll::writeToESP(uint8_t index, String message, float value) {
  Serial1.write(index);
  delay(2);
  Serial1.print(value);
  //debugging:
  Serial.print(message);
  Serial.println(value);
}

void ZumoKontroll::calibrateSensors() {
  for (int i = 0; i < 120; i++) { //kjører mot høyre, så venstre, så sentrerer seg igjen
    if (i > 30 && i < 90) {
      motors.setSpeeds(200, -200);
    }
    else {
      motors.setSpeeds(-200, 200);
    }

    lineSensors.calibrate(); //Kalibrerer sensor
  }
  motors.setSpeeds(0, 0);
}

void ZumoKontroll::checkIfTurned()  {

}

void ZumoKontroll::updateGyro() {

}

void ZumoKontroll::oneCheck() {

}

void ZumoKontroll::speedCheck() {

}

void ZumoKontroll::sixtyCheck() {

}

void ZumoKontroll::seventyCheck() {

}

void ZumoKontroll::batteryCheck() {

}

void ZumoKontroll::batteryHealth() {

}

void ZumoKontroll::update() {
  oneCheck();     //Oppdaterer hastighet hvert sekund
  seventyCheck(); //Sjekker om man kjører over 70% eller mer av max, og lagrer tiden
  sixtyCheck();   //Sjekker om der har gått 60 sekunder og regner ut gj.snitts hastighet og distanse kjørt på den tiden
  batteryCheck(); //Regner ut batteri brukt, eller oppladet
  batteryHealth(); //Gir beskjed om service eller bytte av batteri
  writeToESP();   //Skriver til ESP som deretter oppdaterer blynk
}



/*
void writeToESP() {
  if (newSpeedo == true) {
    Serial1.write(1);
    delay(2);
    Serial1.print(float(speedo));

    Serial.print("BLYNK speedo: ");
    Serial.println(float(speedo));

    newSpeedo = false;
  }
  if (newDistanceTotal == true) {
    Serial1.write(2);
    delay(2);
    Serial1.print(float(distanceTotal));

    Serial.print("BLYNK distanceTotal: ");
    Serial.println(float(distanceTotal));

    newDistanceTotal = false;
  }
  if (newSpeedSixtyFinal == true) {
    Serial1.write(3);
    delay(2);
    Serial1.print(float(speedSixtyFinal));

    Serial.print("BLYNK speedSixtyFinal: ");
    Serial.println(speedSixtyFinal);

    newSpeedSixtyFinal = false;
  }
  if (newDistanceSixtyFinal == true) {
    Serial1.write(4);
    delay(2);
    Serial1.print(float(distanceSixtyFinal));

    Serial.print("BLYNK distanceSixtyFinal: ");
    Serial.println(float(distanceSixtyFinal));

    newDistanceSixtyFinal = false;
  }
  if (newMaxSpeed == true) {
    Serial1.write(5);
    delay(2);
    Serial1.write(int(measuredMaxSpeed));

    Serial.print("BLYNK MAXSPEED: ");
    Serial.println(measuredMaxSpeed);

    newMaxSpeed = false;
  }

  if (newBatteryPercent == true) {
    Serial1.write(6);
    delay(2);
    Serial1.write(batteryPercent);

    Serial.print("BLYNK batteryPercent: ");
    Serial.println(batteryPercent);

    newBatteryPercent = false;
  }
  if (lowBatteryToESP == true) {
    Serial1.write(7);
    delay(2);
    toggleLED = !toggleLED;
    Serial1.write(toggleLED);

    Serial.print("BLYNK update low battery LED: ");
    Serial.println(toggleLED);

    lowBatteryToESP = false;
  }
}
*/
