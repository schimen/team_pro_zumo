#include "ZumoKontroll.h"

ZumoKontroll::ZumoKontroll()  {
  //constructor

  //set start values:
  timeOverSeventyPercent = 0;
  distanceTotal = 0;
  measuredMaxSpeed = 0;
  newCharge = false;
  turnSense = false;
  seventyCalc = false;
}

void ZumoKontroll::updateGyro() {
  /*
  Sjekker rotasjonsposisjon til Zumoen
  */
  static unsigned long timeInterval = 0;
  static int yAxis = 0;
  gyro.read(); //Leser gyroens verdi
  if ((millis() - timeInterval) >= 100) { //Oppdaterer endringa av Y hvert 100ms
    timeInterval = millis();
    yAxis = gyro.g.y; //Lagrer Y-posisjon
  }
  if (abs(yAxis) > 3000) { //Hvis bilen roterer rundt tverrgående-akse
    unsigned long startTime = millis(); //Starter en teller
    bool OneSwitch = true; //Sikrer at turnSense kun endrer seg en gang per rotasjon
    while ( abs(yAxis) > 3000) { //Så læng Zumoen rotere
      if ((millis() >= startTime + 500) && (OneSwitch == true)) { //Hvis bilen har rotert i mer enn 0.5 sek
        turnSense = !turnSense; //Toggle turnSense slik at bilen er opp ned eller rett vei annen hver gang
        Serial.print("turnSense: ");
        Serial.println(turnSense);
        OneSwitch = false; // turnSense kan ikke toggles mer så lenge bilen roterer før neste runde igjen
      }
      gyro.read();// Sjekke om bilen har sluttet å rotere
      if ((millis() - timeInterval) >= 100) { //Oppdaterer endringen av Y hvert 100ms
        timeInterval = millis();
        yAxis = gyro.g.y;
      }
    }
  }
}

void ZumoKontroll::updateCharge()  {
  batteryLeft += batteryCharged;
  batteryChargedTotal += batteryCharged; //Teller hvor mye batteriet totalt har blitt oppladet. "Overlading" tærer også på batteriet, som i virkeligheten (Altså om man forsøker å lade over 100%)
  batteryChargeCycles = batteryChargedTotal / (BATTERY_MAX - (BATTERY_MAX * (ceil(batteryChargeCycles)) / 10)); //Sjekker hvor mye batteriet har blitt oppladet totalt siden programmet startet
  batteryLeft = constrain(batteryLeft, 0, BATTERY_MAX - (BATTERY_MAX * (ceil(batteryChargeCycles)) / 10));

  Serial.print("batteryChargeCycles: ");
  Serial.println(batteryChargeCycles);
  newCharge = false;
}

void ZumoKontroll::updateMaxSpeed() {
  float speedo = getSpeed();
  if (speedo >  measuredMaxSpeed) {
    measuredMaxSpeed = speedo;
  }
}

void ZumoKontroll::updateTotalDistance()  {
  float distance = getDistanceDriven();
  distanceTotal += distance; //Nullstilles aldri. Historisk mål for total distanse kjørt
}

void ZumoKontroll::updateBatteryPercent() {
  static float batteryCapasity = BATTERY_MAX;
  float distance = getDistanceDriven();
  batteryLeft -= distance; //Minker med likt med antall meter kjørt
  batteryCapasity = constrain(batteryCapasity, 0, BATTERY_MAX - (BATTERY_MAX * (ceil(batteryChargeCycles)) / 10)); //Begrenser maks batterikapasitet med 10% for hver hele ladesyklus.
  batteryPercent = batteryLeft / batteryCapasity * 100;
}

void ZumoKontroll::updateTimeOverSeventyPercent() {

  static unsigned long seventyPercentStartTime = millis();

  if (getSpeed() >= SEVENTY_LIMIT && seventyCalc == false) {
    seventyCalc = true;
    seventyPercentStartTime = millis();
  }
  if (getSpeed() < SEVENTY_LIMIT && seventyCalc == true) {
    seventyCalc = false;
    timeOverSeventyPercent += millis() - seventyPercentStartTime;
  }
}

void ZumoKontroll::setupGyro()  {
  gyro.init();
  gyro.enableDefault();
}

void ZumoKontroll::checkIfTurned() {
  static int workDone = 0;
  bool serviceSoundPlayed = false;
  bool changedSoundPlayed = false;
  updateGyro(); // Oppdaterer Y-posisjon
  if (isTurned()) { //Hvis bilen har blitt snudd
    unsigned long startTime = millis(); //Starter en teller

    while (isTurned()) { //Så lengde bilen er snudd
      if (startTime + 3000 <= millis() < startTime + 5000) { //Hvis bilen har vært snudd 2s eller over, men mindre enn 5s
        if (not serviceSoundPlayed) {
          Serial.println("SERVICE");
          workDone = SERVICE;
          buzzer.play("!T87 L8 V7 ce"); //Gir lydvarsel for service utført
          serviceSoundPlayed = true;
          while (buzzer.isPlaying());
        }
      }
      if (millis() >= startTime + 5000) { //Hvis bilen har vært snudd 5s eller over
        if (not changedSoundPlayed) {
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

    }
    if (workDone == CHANGED) {
      batteryChargeCycles = 0; //Resetter ladesyklusene til 0
      Serial.println("workDone is changed");
      batteryLeft = 100; //Resetter batteriprosent til 100
      batteryChargedTotal = 0; //resetter batteryChargeCycles-utregningen
      workDone = 0;
    }
  }
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

void ZumoKontroll::chargeBattery(uint32_t chargeTime) {
  /*
  Run when "charge"-button in Blynk is held in,
  and saves amount of seconds the button is held
  in for calculation of charge percentage
  */

  //round up to closest number of seconds driven. 1 second = 10% battery charged:
  batteryCharged = (ceil((millis() - chargeTime) / 1000)) * 10;
  batteryCharged = constrain(batteryCharged, 0, BATTERY_MAX - (BATTERY_MAX * (ceil(batteryChargeCycles)) / 10));
  Serial.print("charged: ");
  Serial.println(batteryCharged);
  newCharge = true;
}

void ZumoKontroll::checkBatteryHealth() {

  //sjekk med torje og line om dette stemmer:
  if (isNewCharge())  {
    updateCharge();
  }

  if (not batteryChangeNeeded)  {
    if (batteryChargeCycles >= 3) {
      batteryChangeNeeded = true;
      Serial.println("CHANGE NEEDED");
    }
    else if (batteryChargeCycles >= 2)  {
      batteryServiceNeeded = true;
      Serial.println("SERVICE NEEDED");
    }
  }
}

void ZumoKontroll::resetAverageSpeed() {
  cumulativeSpeed = 0;
  countsSpeed = 0;
  timeOverSeventyPercent = 0;
}

void ZumoKontroll::resetEachSecond() {
  //Lager nåværende tid, for regne ut nøyaktig tid fra forrige måling:
  previousResetTime = millis();
  //Resetter encoders, gjør klar for ny måling:
  encoders.getCountsAndResetLeft();
  encoders.getCountsAndResetRight();
}

bool ZumoKontroll::isNewCharge()  {
  return newCharge;
}

bool ZumoKontroll::isTurned() {
  return turnSense;
}

bool ZumoKontroll::isLowBattery()  {
  if (getBatteryPercent() <= 95) {
    return true;
  }
  else  {
    return false;
  }
}

float ZumoKontroll::getTimeDriven() {
  /*
  Regner ut tid for målingen av bevegelsen, og konverterer til sekund
  */
  return (millis() - previousResetTime) / 1000;
}

float ZumoKontroll::getAverageSpeed() {
  /*
  putt inn en god beskrivelse
  */
  //gjennomsnittshatighet over 60 sekunder:
  float averageSpeed = cumulativeSpeed / countsSpeed; //endre navn
  //nullstiller tellere for å gjøre klart for en ny måling:
  resetAverageSpeed();

  return averageSpeed;
}

float ZumoKontroll::getNewDistance() {
  static float lastDistance = 0;
  float newDistance = distanceTotal - lastDistance;
  lastDistance = distanceTotal;
  return newDistance;
}

float ZumoKontroll::getDistanceDriven()  {
  float speedLeft = encoders.getCountsLeft(); //Henter encoderverdiene.
  float speedRight = encoders.getCountsRight(); //Henter encoderverdiene.
  float combinedSpeed = (speedLeft + speedRight) / 2;
  return combinedSpeed / SPEED_CONVERSION; //Gjør om encoderveriden til meter
}

float ZumoKontroll::getTotalDistance()  {
  updateTotalDistance();
  return distanceTotal;
}

float ZumoKontroll::getSpeed() {
  float distance = getDistanceDriven();
  uint32_t timeDriven = getTimeDriven();
  float speedo = distance / timeDriven; //Hastighet for bevegelsen i m/s

  if (speedo < 0) speedo = 0;

  else if (speedo > 0)  {
    cumulativeSpeed += speedo;
    countsSpeed++;
  }
  return speedo;
}

float ZumoKontroll::getMaxSpeed() {
  updateMaxSpeed();
  return measuredMaxSpeed;
}

float ZumoKontroll::getBatteryPercent() {
  updateBatteryPercent();
  return batteryPercent;
}

uint32_t ZumoKontroll::getTimeOverSeventyPercent() {
  updateTimeOverSeventyPercent();
  return timeOverSeventyPercent;
}
