/*
  Definitions for use in web page sensornode
*/

//wifi credentials, IP address, port and blynk authorization
#define SSID "yourWifiHere"
#define PASSWORD "yourPasswordHere"
#define PORT 80

//ESP32 gpio pins
#define LED_PIN 25
#define SERVO_PIN 26
#define BUZZER_PIN 27
#define SENSOR1_PIN 32  //TMP36
#define SENSOR2_PIN 33  //LDR
#define SENSOR3_PIN 34  //POT

//max and min allowed sensor values
#define MIN_OUT 0
#define MAX_OUT 100
#define SENSOR1_MIN 700
#define SENSOR1_MAX 900
#define SENSOR2_MIN 2000
#define SENSOR2_MAX 4000
#define SENSOR3_MIN 0
#define SENSOR3_MAX 4095

//maximum and minimum possible sensor values:
#define MAX_READ 4096
#define MIN_READ 0

//file names
#define LOGO_LONG "/blynk_bootlegLONG.svg"
#define LOGO "/blynk_bootleg.svg"
#define MIDJOSKY "/midjoo.png"
#define CHART_BUNDLE
