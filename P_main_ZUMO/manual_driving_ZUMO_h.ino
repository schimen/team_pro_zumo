unsigned long chargeTime;

void manualDriving() {
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
        motors.setSpeeds(-150, 150);
        inChar = Serial1.read();
      }
      delay(20);
      break;
    case 'S':
      while (inChar == 'S') {
        motors.setSpeeds(-maxSpeed, -maxSpeed);
        inChar = Serial1.read();
      }
      delay(20);
      break;
    case 'D':
      while (inChar == 'D') {
        motors.setSpeeds(150, -150);
        inChar = Serial1.read();
      }
      delay(50);
      break;

    case 'C':
      chargeTime = millis();
      while (inChar != '0') {
        motors.setSpeeds(150, -150);
        inChar = Serial1.read();
      }
      chargeBattery();
      delay(50);
      break;

    case '0':
      motors.setSpeeds(0, 0);
      delay(20);
      break;
  } //end switch
} //end func
