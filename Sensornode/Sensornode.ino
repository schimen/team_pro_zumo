/*
Sensornode main program. Not finished.
Reading and sending values to Blynk currently working.

Todo:
  - fix comments, make them more descriptive and comment weird sections
  - max/min alarm
  - servo
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "Sensor.h"
#include "Servo.h"
#include "Passwords.h"

#define BLYNK_PRINT Serial
#define SERVO_PIN 26
#define SENSOR1_PIN 34
#define SENSOR2_PIN 32
#define SENSOR3_PIN 33
#define CURRENT_VALUE_PIN V7
#define AVERAGE_VALUE_PIN V8
#define MENU_PIN V10
#define SHOW_ALL_PIN V11
#define SLIDER_PIN V12
#define TERMINAL_PIN V13
#define SERVOTEST_PIN V14

uint8_t        Sensor::averageCount = 10;
uint16_t       Sensor::maxValue = MAX_READ;
uint16_t       Sensor::minValue = MIN_READ;
Sensor         sensor1(SENSOR1_PIN);
Sensor         sensor2(SENSOR2_PIN);
Sensor         sensor3(SENSOR3_PIN);
WidgetTerminal terminal(V13);
BlynkTimer     timer;

char auth[] = AUTH;
char ssid[] = SSID;
char pass[] = PASSWORD;

uint16_t sendReadingsPause = 500L;
uint32_t maxMinPause = 30000L;
uint8_t  blynkMenuSelection = 1;
bool     blynkShowAll = false;

void sendMaxMin() {
  /*
  pretty basic stuff
  */
  if (blynkShowAll)  {
    Blynk.virtualWrite(V21, Sensor::getMax());
    Blynk.virtualWrite(V22, Sensor::getMin());
  }
}

void sendDetailed(Sensor *pSensor, bool resetCount) {
  /*
  need some comments
  */
  uint16_t value = pSensor->getValue();
  Blynk.virtualWrite(CURRENT_VALUE_PIN, value);
  terminal.print("  value: ");
  terminal.println(value);
  if (pSensor->newAverage) {
    uint16_t average = pSensor->getAverage(resetCount);
    Blynk.virtualWrite(AVERAGE_VALUE_PIN, average);
    terminal.print("average: ");
    terminal.println(average);
  }
}

void sendAll()  {
  /*
  generic description
  */
  Blynk.virtualWrite(V1, sensor1.getValue());
  Blynk.virtualWrite(V2, sensor2.getValue());
  Blynk.virtualWrite(V3, sensor3.getValue());
  if (sensor1.newAverage or sensor2.newAverage or sensor3.newAverage) {
    Blynk.virtualWrite(V4, sensor1.getAverage());
    Blynk.virtualWrite(V5, sensor2.getAverage());
    Blynk.virtualWrite(V6, sensor3.getAverage());
  }
}

void sendReadings() {
  /*
  events executed with a fixed pause

  comment the shit out of this
  */
  sensor1.read();
  sensor2.read();
  sensor3.read();

  Sensor *pChosenSensor;
  switch (blynkMenuSelection) {
    case 1:
    pChosenSensor = &sensor1;
    break;

    case 2:
    pChosenSensor = &sensor2;
    break;

    case 3:
    pChosenSensor = &sensor3;
    break;
  }

  if (blynkShowAll) {
    sendDetailed(pChosenSensor, false);
    sendAll();
  }
  else  {
    sendDetailed(pChosenSensor, true);
  }
}

BLYNK_WRITE(SERVOTEST_PIN)  {
  bool servoTestOn = param.asInt();
  static uint16_t servoTimerNum;
  uint32_t servoTestPause = 10000L;
  if (servoTestOn) {
    servoTimerNum = timer.setInterval(servoTestPause, servoTest);
  }
  else  {
    timer.deleteTimer(servoTimerNum);
  }
}

BLYNK_WRITE(MENU_PIN) {
  /*
  called when a new value is written on menu pin in blynk app
  */
  blynkMenuSelection = param.asInt();
}

BLYNK_WRITE(SHOW_ALL_PIN) {
  /*
  called when a new value is written on show-all button in blynk app
  */
  blynkShowAll = param.asInt();
}

BLYNK_WRITE(SLIDER_PIN) {
  /*
  called when a new value is written on the slider in blynk app
  */
  Sensor::averageCount = param.asInt();
}

void setup() {
  /*
  setup
  */
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, IPAddress(91,192,221,40), 8080);
  timer.setInterval(sendReadingsPause, sendReadings);
  timer.setInterval(maxMinPause, sendMaxMin);
  servoSetup(SERVO_PIN);
}

void loop() {
  /*
  loop
  */
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
}
