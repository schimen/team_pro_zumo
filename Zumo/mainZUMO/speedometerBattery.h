/* * * * * * * * * * *     Speedometer and battery    * * * * * * * * * * *
   Author: Torje
   Revidert: Line
   Denne koden inneholder funksjoner for Zumo.
   Disse funksjoner skal kjøres sammen som beskrevet.
   De funksjoner "spedoometer()" består av bør ikke kjøres individuelt.
   Funksjonen skal loope i main og kjøres i hver iterasjon.
   Hvis det mer mye delay blir målingene mindre presise.
   Denne funksjonen tar vare på:
    - Hastigheten, meter/sek. Oppdatert hvert sekund.
    - Høyeste hastighet.
    - Total distanse kjørt siden programmet startet Oppdateres hvert sekund.
    - gjennomsnittshastighet, oppdatert hvert 60. sekund
    - Distanse kjørt hvert 60. sekund
    - Antall sekunder bilen har kjørt over 70% av maks hastighet,
      utregnes hvert 60. sekund.
    - Batteri forbrukt
    - Batteri gjenstående
    - Antall ladesykluser på batteriet
  Programmet begrenser også batterikapasiteten utfra antall
  ladinger og overlading, slik som med et ekte batteri.

  Todo:
  Slå sammen med alle andre funksjoner for Zumo, og lag bibliotek

*/
/*
comment fra simen:
  holy shit dette var mye på en gang
  veldig bra med mye kommentarer og gode navn men dette er 300 linjer
  med kraftig kost.
  Tror kanskje dette burde vært en class istedet,
  send meg melding så kan vi diskutere det
*/

#ifndef speedometerBattery_h
#define speedometerBattery_h

#include <Arduino.h>
#include "zumoObjects.h"

//flyttet denne fra main sketch:
static int maxSpeed = 200;

//flyttet denne fra manual driving
unsigned long chargeTime;

//jeg flyttet disse fra gyrodelen, d\regner med at de padder bedre inn her...
float batteryChargeCycles = 0;  //Antall totale ladesykluser for batteriet. Brukes også til å begrense toppverdien for batteriet. Flere sykluser gir lavere batterikapasietet.
const int BATTERY_MAX = 100; //Max batterikapasitet, i meter
float batteryLeft = BATTERY_MAX; //Gjenstående batteri, i meter
float batteryChargedTotal; //Totalt opplading av batteriet siden programmets start
bool newCharge = false;

//--------Global-----
const float SEVENTY_LIMIT = 0.462; //Grensen for 70% av bilens maxhastighet. (maks hastighet er 0.66 m/s)

float speedSixtyFinal;      //Verdien tas vare på for å printe til BLYNK, selv om en ny måling blir gjennomført
float distanceSixtyFinal;   //Verdien tas vare på for å printe til BLYNK, selv om en ny måling blir gjennomført
float distanceTotal = 0;    //Distansen kjørt, oppgitt i meter
float speedSixty;           //Mellomregningsmål for å beregne gjennomsnittshastighet
int numSixty = 0;           //Teller for hvor mange summer speedSixty består av. Brukes for å regne ut gjennomsnittet.
float avgSpeedSixty;        //Gjennomsnittshastighet hvert 60. sekund

float speedo;             //Utregnet meter/sekund. Faktiske speedometerverdien.
float measuredMaxSpeed = 0;       //Historisk mål for høyeste hastighet
float speedLeft;            //Encoderverdi
float speedRight;           //Encoderverdi
unsigned long movementTime;         //Tiden for en bevegelse. Brukes for å regne ut hastigheten.

bool newSpeedo = false;             //Brukes for å si om det finnes en ny verdi å skrive til ESP
bool newDistanceTotal = false;      //Brukes for å si om det finnes en ny verdi å skrive til ESP
bool newSpeedSixtyFinal = false;    //Brukes for å si om det finnes en ny verdi å skrive til ESP
bool newDistanceSixtyFinal = false; //Brukes for å si om det finnes en ny verdi å skrive til ESP
bool newMaxSpeed = false;           //Brukes for å si om det finnes en ny verdi å skrive til ESP
bool newBatteryPercent = false;     //Brukes for å si om det finnes en ny verdi å skrive til ESP
bool lowBatteryToESP = false;       //Brukes for å si om det finnes en ny verdi å skrive til ESP
bool toggleLED = false;             //Toggel for lavt batterivarsel-led

//float batteryChargeCycles = 0;  //Antall totale ladesykluser for batteriet. Brukes også til å begrense toppverdien for batteriet. Flere sykluser gir lavere batterikapasietet.
int batteryPercent;             //Batterinivå i prosent.

unsigned long currentMillis;
bool oneCalc = false; //For å sjekke om man er i en beregning av 1 sekunder
unsigned long oneCalcTime = 0; //Timestamp for start av en måling av 1 sekund

unsigned long seventyTime; //Total tid over 70% av max
float distanceSixty; //Total distanse innen 60 sekunder
bool sixtyCalc = false; // For å sjekke om man er i en beregning av 60 sekunder
unsigned long sixtyCalcTime; //Timestamp for start av en måling av 1 sekund

bool seventyCalc = false; //For å sjekke når bilen kjører over 70% av maxfart
unsigned long seventyCalcTime; //Timestamp for start av periode med kjøring over 70% av max

//--------------batteryCheck---------
float charged; //Teller hvor mye som lades
int batteryCapasity = BATTERY_MAX;
bool lowBattery = false;
bool batteryServiceNeeded = false;
bool prevServiceState = false;
bool batteryChangeNeeded = false;
bool prevChangeState = false;

void speedometer();

void oneCheck();

void speedCheck();

void sixtyCheck();

// Funksjonen beregner tiden man er over 70% av max hastighet, etter en terskel man har lagt inn.
void seventyCheck();

void batteryCheck();

void chargeBattery();

void batteryHealth();

//Skriver alle nye verdier til ESP
void writeToESP();

#endif
