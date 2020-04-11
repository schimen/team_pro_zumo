/* * * * * * * * * * *     Sensornode     * * * * * * * * * * *

  Dette er sensornode-modul for datateknikk prosjekt.
  Denne versjonen bruker blynk.

  Description:
    This is a program for ESP32 to speak to blynk app: (insert url)

Todo:
  - description of program (this comment)
  - make code more readable
  - clean up unneccessary functions
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "Sensor.h"  //sensor class for control of sensor reading
#include "SensornodePWM.h"  //pwm class to control buzzer and servo
#include "Definitions.h" //Definitions here for a cleaner code

//start values of static members of Sensor class:
uint8_t  Sensor::averageCount = 10; //Number of reads per average value.
uint8_t  Sensor::sensorsOverMax = 0;  //Number of sensors over max limit.
uint16_t Sensor::maxValue = MIN_READ;   // Maximum and minimum
uint16_t Sensor::minValue = MAX_READ;   // measured value.

//objects:
PWM            servo(SERVO_CHANNEL, SERVO_PIN, 16);      //channel, pin,
PWM            buzzer(BUZZER_CHANNEL, BUZZER_PIN);       //freq=50, res=8
Sensor         sensor1(SENSOR1_PIN, SENSOR1_MAX);  //pin, max allowed value
Sensor         sensor2(SENSOR2_PIN, SENSOR2_MAX);  //   (before alarm starts)
Sensor         sensor3(SENSOR3_PIN, SENSOR3_MAX);
WidgetTerminal terminal(TERMINAL_PIN);
BlynkTimer     timer;

bool     servoTestOn = false;
bool     blynkShowAll = false;
uint8_t  blynkMenuSelection = 1;
uint16_t servoTimer;

void alarm()  {
  /*
  Start alarm with buzzer and led blinking.
  Turn led on and off, change buzzer frequency between 2000Hz and 1000Hz.
  This function should be called in a timer.
  */
  static bool state = false;  //only initialized first function call
  state = not state;

  if (state)  {
    buzzer.setTone(BUZZER_HIGH);
    digitalWrite(LED_PIN, HIGH);
  }
  else  {
    buzzer.setTone(BUZZER_LOW);
    digitalWrite(LED_PIN, LOW);
  }
}

void resetAlarm() {
  /*
  turn off buzzer and led
  */
  buzzer.setOff();
  digitalWrite(LED_PIN, LOW);
}

void checkAlarm() {
  /*
  check if two or more sensors are over max allowed sensor value.
  */

  //make timer for alarm on first function call:
  static uint8_t alarmTimer = timer.setInterval(ALARM_INTERVAL, alarm);
  static bool alarmTimerEnabled = timer.isEnabled(alarmTimer);

  if (not alarmTimerEnabled) {
    if (Sensor::isAlarm())  {
      timer.enable(alarmTimer);
      timer.enable(servoTimer);
      alarmTimerEnabled = timer.isEnabled(alarmTimer);
    }
    else if (buzzer.isOn())  {
      resetAlarm();
    }
  }
  else if (not Sensor::isAlarm()) {
    timer.disable(alarmTimer);
    alarmTimerEnabled = timer.isEnabled(alarmTimer);
  }
}

void sendMaxMin() {
  /*
  send max and min values from sensor class to blynk
  */
  if (blynkShowAll)  {
    Blynk.virtualWrite(DB_MAX, Sensor::getMax());
    Blynk.virtualWrite(DB_MIN, Sensor::getMin());
  }
}

void sendDetailed(Sensor *sensor, bool reset) {
  /*
  Sends data of one sensor to blynk.
  Takes pointer of sensor object and a bool to determine if
  readCount should be reset on getAverage() method.
  */
  uint16_t value = sensor->getValue();
  Blynk.virtualWrite(CURRENT_VALUE_PIN, value); //send read value
  terminal.print("  value: ");  //print read value
  terminal.println(value);      //on terminal
  if (sensor->isNewAverage()) {
    uint16_t average = sensor->getAverage(reset);
    Blynk.virtualWrite(AVERAGE_VALUE_PIN, average); //send average value
    terminal.print("average: ");  //print average value
    terminal.println(average);    //on terminal
  }
}

void sendAll()  {
  /*
  send read value and average value of all sensors to blynk.
  */
  Blynk.virtualWrite(DB_SENSOR1, sensor1.getValue());
  Blynk.virtualWrite(DB_SENSOR2, sensor2.getValue());
  Blynk.virtualWrite(DB_SENSOR3, sensor3.getValue());
  if (sensor1.isNewAverage() or
      sensor2.isNewAverage() or
      sensor3.isNewAverage()) {
    Blynk.virtualWrite(DB_AVERAGE1, sensor1.getAverage());
    Blynk.virtualWrite(DB_AVERAGE2, sensor2.getAverage());
    Blynk.virtualWrite(DB_AVERAGE3, sensor3.getAverage());
  }
}

void readSensors()  {
  /*
  Read sensors
  */
  sensor1.read();
  sensor2.read();
  sensor3.read();
  checkAlarm(); //check for alarm criteria every sensor read
}

void sendReadings() {
  /*
  Send the readings.
  Determine sensor to send detailed readings.
  If blynkShowAll, send details and all. If not blynkShowAll, send only details
  */
  readSensors();

  Sensor *chosenSensor; //pointer to chosen sensor object

  //determines chosen sensor based on blynkMenu
  switch (blynkMenuSelection) {
    case 1:
      chosenSensor = &sensor1;
      break;

    case 2:
      chosenSensor = &sensor2;
      break;

    default:  //default statement instead of case 3 for failproofing
      chosenSensor = &sensor3;
      break;
  }

  if (blynkShowAll) {
    sendDetailed(chosenSensor, false); //send detailed but keep readCount
    sendAll();                         //(readCount is used in send all)
  }
  else  {
    sendDetailed(chosenSensor, true); //send detailed and reset readCount
  }
}

void moveServo()  {
  /*
  move servo one step. change direction of step each time servo end
  todo:
    make servo stop on end and alternate between ends.
  */
  static int16_t change = SERVO_STEP; //run first fctn call

  uint16_t dutyCycle = servo.getDuty();
  if (dutyCycle >= SERVO_LEFT)  {
    change = -SERVO_STEP;
  }
  else if (dutyCycle <= SERVO_RIGHT) {
    change = SERVO_STEP;
  }
  servo.changeDuty(change);
  Blynk.virtualWrite(SERVO_SLIDER, servo.getDegree());

  //check for stop requirements:
  if (servo.isEnd() and not Sensor::isAlarm() and not servoTestOn) {
    timer.disable(servoTimer);
    servo.changeEnd();  //change side for servo to stop
  }
}

BLYNK_WRITE(SERVOTEST_BUTTON)  {
  /*
  called on new value for servotest button in blynk app
  Starts and ends servotest
  */
  servoTestOn = param.asInt();

  if (servoTestOn) {
    timer.enable(servoTimer);
  }
}

BLYNK_WRITE(MENU_PIN) {
  /*
  called on new value for menu selection in blynk app.
  Updates blynkMenuSelection
  */
  blynkMenuSelection = param.asInt();
}

BLYNK_WRITE(SHOW_ALL_PIN) {
  /*
  called on new value for show-all button in blynk app.
  Updates blynkShowAll
  */
  blynkShowAll = param.asInt();
}

BLYNK_WRITE(AVERAGE_COUNT_SLIDER) {
  /*
  called on new value for averageCount slider in blynk app.
  Updates averageCount
  */
  Sensor::averageCount = param.asInt();
}

BLYNK_WRITE(SERVO_SLIDER)  {
  /*
  called on new value for servo position slider in blynk app.
  Sets new position on servo
  */
  servo.setDegree(param.asInt());
}

void setup() {
  /*
  setup code
  Set up:
    Blynk, sendReadings timer, sendMaxMin timer,
    servo loop and LED_PIN as OUTPUT
  */
  Blynk.begin(AUTH, SSID, PASSWORD, IPAddress(IP), PORT);
  timer.setInterval(READINGS_INTERVAL, sendReadings);
  timer.setInterval(MAX_MIN_INTERVAL, sendMaxMin);
  servoTimer = timer.setInterval(SERVO_MOVE_INTERVAL, moveServo);
  timer.disable(servoTimer);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  /*
  main loop
  Run:
    blynk, timer
  */
  Blynk.run();  //starts blynk
  timer.run();  //starts all enabled timers on timer object
}
