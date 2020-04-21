#include "LineFollower.h"

void calibrateSensors() {
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

void followLine() {
  int position = lineSensors.readLine(lineSensorValues);

  int error = position - 2000; // regner ut feilmargin fra sentrum
  //Bruker feilmarginen til å sette en fartsforskjell på motorene basert på nåværende og forrige feilmargin
  int16_t speedDifference = error / 4 + 6 * (error - lastError);
  lastError = error;
  int leftSpeed = maxSpeed + speedDifference; // Setter motorhastigheter
  int rightSpeed = maxSpeed - speedDifference;

  leftSpeed = constrain(leftSpeed, 0, maxSpeed); //Setter et "tak" for hastighetene i begge ender, om farten er under 0 eller over 400
  rightSpeed = constrain(rightSpeed, 0, maxSpeed);

  motors.setSpeeds(leftSpeed, rightSpeed); // Endrer hastigheten til motorene
}
