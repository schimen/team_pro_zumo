/* * * * * * * * * * *     Gyro fuctions     * * * * * * * * * * *

  - Programmet brukes til å simulere batteriservice eller batteriskift
    ved å oppdage når bilen er snudd opp ned, hvor lenge den holdes der.
  - Den markerer ved 3s etter snudd bil at batteriet er gjort service på,
    og ved 5s at batteriet er skiftet. Den utfører bare en av handlingene
    per gang.

*/

#ifndef gyroZumo_h
#define gyroZumo_h

#include <Arduino.h>
#include "zumoObjects.h"
#include "speedometerBattery.h"



//how many globals do you need? :'(
unsigned long TimeInterval = 0; //Oppdaterer Y-rotasjon i et gitt intervall
int Y = 0;                      // Holder y-posisjonen
unsigned long startTime;        //teller
bool turnSense = false;         //Lagrer om Zumoen er opp ned eller ikke. (true = opp ned, false = rett vei)
int workDone;                   //Lagrer hvilket arbeid som er utført på batteriet (SERVICE / CHANGED)
const int SERVICE = 1;          //Utført service på batteriet
const int CHANGED = 2;          //Utført bytte av batteri

//jeg lagde denne for en litt penere kode :)
void setupGyro();

//gi litt info om bruk her
void checkIfTurned();

//og her
void updateGyro();

#endif
