
#include "speedometerBattery.h"


//-----------------writeToESP----------
void writeToESP(uint8_t index, String message, float value) {
  Serial1.write(index);
  delay(2);
  Serial1.print(value);
  //debugging:
  Serial.print(message);
  Serial.println(value);
}

//-----------------------oneCheck--------------------
void oneCheck() {
  if (oneCalc == false) { //Sjekke om man er i en "sekundtelling" eller ikke. Starter en måling
    oneCalcTime = millis();  //Timestamp
    encoders.getCountsAndResetLeft(); //Resetter encoders i starten av en måling
    encoders.getCountsAndResetRight(); //Resetter encoders i starten av en måling
    oneCalc = true; //Markerer at man er i en måling
  }
  if (millis() - oneCalcTime >= 1000) { //Når det har gått ett sekund siden sist måling
    movementTime = millis() - oneCalcTime; //Regner ut tid for målingen av bevegelsen.
    speedLeft = encoders.getCountsLeft(); //Henter encoderverdiene.
    speedRight = encoders.getCountsRight(); //Henter encoderverdiene.

    speedCheck(); //Regner ut farten under sekundet, og oppdaterer evt. ny maks.hastighet
    oneCalc = false; //Ferdig med en hastighetsmåling.

    speedSixty += speedo; //Mellomregning for å regne ut gjennomsnittshastighet hvert 60. sekund. Tar med summen av alle fartsberegninger.
    numSixty++; //Teller for antall summer i speedSixty. Brukes for å senere regne ut gjennomsnittet av dem.

    distanceTotal += (speedo * (movementTime / 1000)); //Nullstilles aldri. Historisk mål for total distanse kjørt, målt i meter
    writeToESP(2, "BLYNK distanceTotal: ", float(distanceTotal));
  }
}

//-----------------------SpeedCheck------------------
void speedCheck() { //Brukes til å beregne hastigheten til gjennomsnittsforflyttningen til hjulene.
  const int SPEED_CONVERSION = 7750; // omregning fra dekodertellinger til m.
  float combinedSpeed = (speedLeft + speedRight) / 2;
  speedo = ((combinedSpeed / SPEED_CONVERSION) / movementTime) * 1000; // hastighet for bevegelsen, hensyntatt konvertering fra "dekodertellinger per millisekund" til "meter/sek"
  if (speedo <= 0) { //Hvis man snurrer om sin egen akse har man ikke fart, og det skal ikke påvirke snittfarta.
    speedo = 0;
    numSixty--;
  }
  if (speedo >  measuredMaxSpeed) { //Historisk mål for høyeste hastighet
    measuredMaxSpeed = speedo;
    writeToESP(5, "BLYNK MAXSPEED: ", measuredMaxSpeed);
  }
  writeToESP(1, "BLYNK speedo: ", float(speedo));
}

//-----------------------sixtyCheck-----------------
void sixtyCheck() {
  if (sixtyCalc == false) { //Starter på en ny telling dersom en ikke eksisterer
    sixtyCalcTime = millis(); //Timestamp ved starten av en telling
    distanceSixty = distanceTotal; //Midlertidig verdi for beregning av total distanse over 60 sekunder
    sixtyCalc = true; //Bool for at timestamp ikke skal sjekke igjen
  }
  if (millis() - sixtyCalcTime >= 60000) { //Sjekker om det har gått 60 sekunder.
    distanceSixty = distanceTotal - distanceSixty; //Regner total distanse siden starten av 60-sekundersperioden, basert på differansen mellom den totale distansen over 60 sekunder.
    avgSpeedSixty = speedSixty / numSixty; //Gjennomsnittshastigheten over 60 sekunder.

    //Tar vare på verdier før man nullstiller og gjør klar for en ny måling
    speedSixtyFinal = avgSpeedSixty;
    distanceSixtyFinal = distanceSixty;

    //nullstiller tellere for å gjøre klart for en ny måling
    speedSixty = 0;
    distanceSixty = 0;
    seventyTime = 0;
    numSixty = 0;

    sixtyCalc = false; //Bool for at ny timestamp skal kunne gjennomføres (Starten av denne funksjonen)

    writeToESP(3, "BLYNK speedSixtyFinal", speedSixtyFinal);
    writeToESP(4, "BLYNK distanceSixtyFinal", float(distanceSixtyFinal));
    //EEPROM.write(0, batteryChargeCycles);//Lagrer antall batterisykluser i EEPROM en gang i minuttet. Gjør dette sjeldent for å ikke "slite ut" minnet, som her levetid på ca 100,000 skrive/slette-runder
  }
}

//----------------seventyCheck()-----------
void seventyCheck() {
  if (speedo >= SEVENTY_LIMIT && seventyCalc == false) {
    seventyCalc = true;
    seventyCalcTime = millis();
  }
  if (speedo < SEVENTY_LIMIT && seventyCalc == true) {
    seventyCalc = false;
    seventyTime += millis() - seventyCalcTime;
  }
}

void chargeBattery() {
  /*Kjøres når "charge"-knappen i blynk blir holdt inne, og lagrer antall sekunder knappen er holdt inne for å regne ut ladningsprosent*/
  charged = (ceil((millis() - chargeTime) / 1000)) * 10; //Runder opp til nærmeste antall sek kjørt. 1 sek = 10% batteri oppladet

  charged = constrain(charged, 0, BATTERY_MAX - (BATTERY_MAX * (ceil(batteryChargeCycles)) / 10));
  Serial.print("charged: ");
  Serial.println(charged);
  newCharge = true;
}

void checkIfLowBattery() {
  if (batteryPercent <= 95 && lowBattery == false) {
    lowBattery = true;
    lowBatteryToESP = true;
  }
  if (batteryPercent > 95 && lowBattery == true) {
    lowBattery = false;
    lowBatteryToESP = true;
  }
}

void batteryHealth() {
  if (batteryChargeCycles >= 2 && batteryChargeCycles < 3 && batteryServiceNeeded == false) {
    batteryServiceNeeded = true;
    Serial.println("SERVICE NEEDED");
  }
  if (batteryChargeCycles >= 3 && batteryChangeNeeded == false) {
    batteryChangeNeeded = true;
    Serial.println("CHANGE NEEDED");
  }
}



//--------------batteryCheck---------
void batteryCheck() {
  if (newSpeedo == true) {
    batteryLeft -= (speedo * (movementTime / 1000)); //Samme som distansen, men med egen teller. Oppgitt i meter.
    newBatteryPercent = true;
  }
  if (newCharge == true) {
    batteryLeft += charged;
    batteryChargedTotal += charged; //Teller hvor mye batteriet totalt har blitt oppladet. "Overlading" tærer også på batteriet, som i virkeligheten (Altså om man forsøker å lade over 100%)
    batteryChargeCycles = batteryChargedTotal / (BATTERY_MAX - (BATTERY_MAX * (ceil(batteryChargeCycles)) / 10)); //Sjekker hvor mye batteriet har blitt oppladet totalt siden programmet startet
    batteryLeft = constrain(batteryLeft, 0, BATTERY_MAX - (BATTERY_MAX * (ceil(batteryChargeCycles)) / 10));

    Serial.print("batteryChargeCycles: ");
    Serial.println(batteryChargeCycles);
    newCharge = false;
    newBatteryPercent = true;
  }
  batteryCapasity = constrain(batteryCapasity, 0, BATTERY_MAX - (BATTERY_MAX * (ceil(batteryChargeCycles)) / 10)); //Begrenser maks batterikapasitet med 10% for hver hele ladesyklus.
  batteryPercent = batteryLeft / batteryCapasity * 100;

  checkIfLowBattery();
}


//speedometer? dette oppdaterer jo mye mer enn bare hastigheten...
//-----------------------Speedometer------------------
void speedometer() {
  oneCheck();     //Oppdaterer hastighet hvert sekund
  seventyCheck(); //Sjekker om man kjører over 70% eller mer av max, og lagrer tiden
  sixtyCheck();   //Sjekker om der har gått 60 sekunder og regner ut gj.snitts hastighet og distanse kjørt på den tiden
  batteryCheck(); //Regner ut batteri brukt, eller oppladet
  batteryHealth(); //Gir beskjed om service eller bytte av batteri
  writeToESP();   //Skriver til ESP som deretter oppdaterer blynk
}
