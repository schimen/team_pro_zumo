#include "ZumoKontroll.h"

ZumoKontroll::ZumoKontroll()  {
  //constructor

  //set start values:
  timeOverSeventyPercent = 0;
  distanceTotal = 0;
  measuredMaxSpeed = 0;
  batteryPercent = 100;
  batteryLeft = 100;
  newCharge = false;
  turnSense = false;
  seventyCalc = false;
}

void ZumoKontroll::updateGyro() {
  /*
  Checks the rotation of the Zumo. Is used for the batteryservice and -change.
  */
  static unsigned long timeInterval = 0;
  static int yAxis = 0;
  gyro.read(); //Reads the value of the Gyro
  if ((millis() - timeInterval) >= 100) { //Updates the change of the Y-value every 100ms
    timeInterval = millis();
    yAxis = gyro.g.y; //Saves the value of the Y-position
  }
  if (abs(yAxis) > 3000) { //If the car is flipped
    unsigned long startTime = millis(); //Starts a counter
    bool OneSwitch = true; //Makes sure the "turnSense" is changed only once per flip.
    while ( abs(yAxis) > 3000) { //While the car is upside-down
      if ((millis() >= startTime + 500) && (OneSwitch == true)) {
        turnSense = !turnSense; //Toggles "turnSense" whenever the car is flipped
        Serial.print("turnSense: ");
        Serial.println(turnSense);
        OneSwitch = false; // turnSense can't be toggled while flipping.
      }
      gyro.read();// Checks if the car stopped flipping
      if ((millis() - timeInterval) >= 100) { //Updates the change of the Y-value every 100ms
        timeInterval = millis();
        yAxis = gyro.g.y;
      }
    }
  }
}

void ZumoKontroll::updateCharge()  {
/*
This function updates the values regarding the battery after a charge
it updates whats left of the battery, how much it has been charged in total, and how many chargin cycles there has been.
*/
  batteryLeft += batteryCharged;
  batteryChargedTotal += batteryCharged; //Counts the total charge.
  batteryChargeCycles = batteryChargedTotal / (BATTERY_MAX - (BATTERY_MAX * (ceil(batteryChargeCycles)) / 10)); //Checks how many full charging cycles the battery has had
  batteryLeft = constrain(batteryLeft, 0, BATTERY_MAX - (BATTERY_MAX * (ceil(batteryChargeCycles)) / 10)); //Constrains the maximum battery capacity based on the number of full cycles

  Serial.print("batteryChargeCycles: ");
  Serial.println(batteryChargeCycles);
  newCharge = false;
}

void ZumoKontroll::updateMaxSpeed() {
  /*
This function updates the Max Speed.
If the current speed is higher than the previous max speed, its saved as the new max
  */
  float speedo = getSpeed();
  if (speedo >  measuredMaxSpeed) {
    measuredMaxSpeed = speedo;
  }
}

void ZumoKontroll::updateTotalDistance()  {
/*
This function updates the total distance travelled.
This value is never reset. It's an historic measure of distance travelled
*/
  float distance = getDistanceDriven();
  distanceTotal += distance;
}

void ZumoKontroll::updateBatteryPercent() {
/*
This function updates the battery percentage based on the distance traveller
*/
  static float batteryCapasity = BATTERY_MAX;
  float distance = getDistanceDriven();
  batteryLeft -= distance;
  batteryCapasity = constrain(batteryCapasity, 0, BATTERY_MAX - (BATTERY_MAX * (ceil(batteryChargeCycles)) / 10)); //Begrenser maks batterikapasitet med 10% for hver hele ladesyklus.
  batteryPercent = batteryLeft / batteryCapasity * 100;
}

void ZumoKontroll::updateTimeOverSeventyPercent() {
/*
This function checks if the car is going faster than 70%
of max speed, and measures the duration for it
*/
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
/*
This function simply initializes the gyro sensor
*/
  gyro.init();
  gyro.enableDefault();
}

void ZumoKontroll::checkIfTurned() {
  /*
This function checks if the car is turned around, and for how long.
Is used to call the battery service and battery change
  */
  static int workDone = 0;
  bool serviceSoundPlayed = false;
  bool changedSoundPlayed = false;
  updateGyro();
  if (isTurned()) {
    unsigned long startTime = millis();

    while (isTurned()) {
      if (startTime + 3000 <= millis() < startTime + 5000) { //If the car has been flipped for more than two, but less than five seconds
        if (not serviceSoundPlayed) {
          Serial.println("SERVICE");
          workDone = SERVICE;
          buzzer.play("!T87 L8 ce"); //Plays a sound for completed service
          serviceSoundPlayed = true;
          while (buzzer.isPlaying());
        }
      }
      if (millis() >= startTime + 5000) { //If the car has been flipped for more than five seconds
        if (not changedSoundPlayed) {
          Serial.println("CHANGED");
          workDone = CHANGED;
          buzzer.play("!T60 L8 ged"); //Plays a different sound for a completed battery change
          while (buzzer.isPlaying());
          changedSoundPlayed = true;
        }
      }
      updateGyro(); //Updates the gyro to see if the car has been flipped back over
    }

    if (workDone == SERVICE) {
      /*
      Updates the battery variables according to a service
      */
      Serial.println("workDone is service");
      batteryChargeCycles = 1;
      batteryChargedTotal = 100;
      batteryLeft = 100;
      newCharge == true;
      workDone = 0;

    }
    if (workDone == CHANGED) {
      /*
      Updates the battery variables according to a change
      */
      batteryChargeCycles = 0;
      Serial.println("workDone is changed");
      batteryLeft = 100;
      batteryChargedTotal = 0;
      workDone = 0;
    }
  }
}

void ZumoKontroll::calibrateSensors() {
  /*
This function calibrates the sensors used for line following
  */
  for (int i = 0; i < 120; i++) {
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
  and saves amount of time the button is held
  in for calculation of charge percentage
  round up to closest number of seconds driven. 1 second = 10% battery charged
  */
  batteryCharged = (ceil((millis() - chargeTime) / 1000)) * 10;
  batteryCharged = constrain(batteryCharged, 0, BATTERY_MAX - (BATTERY_MAX * (ceil(batteryChargeCycles)) / 10));
  Serial.print("charged: ");
  Serial.println(batteryCharged);
  newCharge = true;
}

void ZumoKontroll::checkBatteryHealth() {
/*
This function checks if a batteryservice or change
is needed, and gives a notification
*/
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
  /*
This function resets the variables used for
calculating and saving the average speed
  */
  cumulativeSpeed = 0;
  countsSpeed = 0;
  timeOverSeventyPercent = 0;
}

void ZumoKontroll::resetEachSecond() {
  /*
This function resets the encoders between each reading
while also doing a timestamp. Used for the calculations
made every second.
  */
  previousResetTime = millis();
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
  Measures the duration of movement, converted to seconds
  */
  return (millis() - previousResetTime) / 1000;
}

float ZumoKontroll::getAverageSpeed() {
  /*
  This function returns the average speed of the past 60
  seconds and resets the counters, preparing new calculations
  */
  float averageSpeed = cumulativeSpeed / countsSpeed;
  resetAverageSpeed();
  return averageSpeed;
}

float ZumoKontroll::getNewDistance() {
  /*
  This function returns the distance travelled in
  the last minute
  */
  static float lastDistance = 0;
  float newDistance = distanceTotal - lastDistance;
  lastDistance = distanceTotal;
  return newDistance;
}

float ZumoKontroll::getDistanceDriven()  {
  /*
  This function returns the distance travelled in
  the current movement.
  */
  float speedLeft = encoders.getCountsLeft(); //Henter encoderverdiene.
  float speedRight = encoders.getCountsRight(); //Henter encoderverdiene.
  float combinedSpeed = (speedLeft + speedRight) / 2;
  return combinedSpeed / SPEED_CONVERSION; //Gjør om encoderveriden til meter
}

float ZumoKontroll::getTotalDistance()  {
  /*
This function returns the total distance travelled
  */
  updateTotalDistance();
  return distanceTotal;
}

float ZumoKontroll::getSpeed() {
  /*
  This function returns the actual speed in the last second
  */
  float distance = getDistanceDriven();
  float timeDriven = getTimeDriven();
  float speedo = distance / timeDriven; //Hastighet for bevegelsen i m/s

  if (speedo < 0) speedo = 0;

  else if (speedo > 0)  {
    cumulativeSpeed += speedo;
    countsSpeed++;
  }
  return speedo;
}

float ZumoKontroll::getMaxSpeed() {
  /*
This function returns the maximum speed driven
  */
  updateMaxSpeed();
  return measuredMaxSpeed;
}

float ZumoKontroll::getBatteryPercent() {
  /*
  This function returns the current battery percentage
  */
  updateBatteryPercent();
  return batteryPercent;
}

uint32_t ZumoKontroll::getTimeOverSeventyPercent() {
  /*
  This function returns the duration driven at more
  than 70% of max speed.
  */
  updateTimeOverSeventyPercent();
  return timeOverSeventyPercent;
}
