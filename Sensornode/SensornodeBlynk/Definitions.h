/*
  Definitions for use in Sensornode.ino
*/

//wifi credentials, IP address, port and blynk authorization
#define SSID "YourWifiHere"
#define PASSWORD "YourSSIDHere"
#define IP 91,192,221,40
#define PORT 8080
#define AUTH "xg7VAxJBTHMktDhmX8ZwidPa6ZSMjm4w"

//ESP32 gpio pins
#define LED_PIN 25
#define SERVO_PIN 26
#define BUZZER_PIN 27
#define SENSOR1_PIN 32  //TMP36
#define SENSOR2_PIN 33  //LDR
#define SENSOR3_PIN 34  //POT

//blynk virtual pins
#define DB_SENSOR1 V1   //DB (dashborad), pins
#define DB_SENSOR2 V2   //for graphs on dashboard
#define DB_SENSOR3 V3
#define DB_AVERAGE1 V4
#define DB_AVERAGE2 V5
#define DB_AVERAGE3 V6
#define DB_MAX V21
#define DB_MIN V22
#define CURRENT_VALUE_PIN V7  //pins for detailed sensor readings
#define AVERAGE_VALUE_PIN V8
#define MENU_PIN V10          //pins for blynk widgets
#define SHOW_ALL_PIN V11
#define AVERAGE_COUNT_SLIDER V12
#define TERMINAL_PIN V13
#define SERVOTEST_BUTTON V14
#define SERVO_SLIDER V15

//timer intervals
#define SERVO_MOVE_INTERVAL 200L
#define ALARM_INTERVAL 500L
#define READINGS_INTERVAL 1000L
#define MAX_MIN_INTERVAL 30000L

//max allowed sensor values
#define SENSOR1_MAX 3000
#define SENSOR2_MAX 3500
#define SENSOR3_MAX 4000

//maximum and minimum possible sensor values:
#define MAX_READ 4096
#define MIN_READ 0

//PWM channels
#define SERVO_CHANNEL 1
#define BUZZER_CHANNEL 2

//buzzer frequencies
#define BUZZER_HIGH 2000
#define BUZZER_LOW 1000

//servo values (found by testing):
#define SERVO_LEFT 8100
#define SERVO_RIGHT 1700
#define MIN_DEGREE 0
#define MAX_DEGREE 180
#define SERVO_STEP 200
