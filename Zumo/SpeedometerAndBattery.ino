/*
   Disse funksjoner skal kjøres sammen som beskrevet. De funksjoner "spedoometer()" består av bør ikke kjøres individuelt.
   Funksjonen skal loope i main og kjøres i hver iterasjon. Hvis det mer mye delay blir målingene mindre presise.
   Denne funksjonen tar vare på:
    - Hastigheten, meter/sek. Oppdatert hvert sekund.
    - Høyeste hastighet.
    - Total distanse kjørt siden programmet startet Oppdateres hvert sekund.
    - gjennomsnittshastighet, oppdatert hvert 60. sekund
    - Distanse kjørt hvert 60. sekund
    - Antall sekunder bilen har kjørt over 70% av maks hastighet, utregnes hvert 60. sekund.
    - Batteri forbrukt
    - Batteri gjenstående
    - Antall ladesykluser på batteriet
  Programmet begrenser også batterikapasiteten utfra antall ladinger og overlading, slik som med et ekte batteri.

  todo:
  - Rydde opp i alle dårlige globale verdien.
  - ???
  - Profit
*/

#include <Zumo32U4.h>
#include <EEPROM.h>

Zumo32U4Encoders encoders;

//--------Global-----
const int SEVENTY_LIMIT = 50; //Grensen for 70% av bilens maxhastighet. LEGG INN SPEEDOMETERVETDIEN(M/S)! 50 ER ARBITRÆRT!

int speedSixtyFinal;      //Verdien tas vare på for å printe til BLYNK, selv om en ny måling blir gjennomført
int distanceSixtyFinal;   //Verdien tas vare på for å printe til BLYNK, selv om en ny måling blir gjennomført
int distanceTotal = 0;    //Distansen kjørt, oppgitt i meter
int numSixty = 0;         //Teller for hvor mange summer speedSixty består av. Brukes for å regne ut gjennomsnittet.
int avgSpeedSixty;        //Gjennomsnittshastighet hvert 60. sekund

float speedo;             //Utregnet meter/sekund. Faktiske speedometerverdien.
float maxSpeed = 0;       //Historisk mål for høyeste hastighet
int speedLeft;            //Encoderverdi
int speedRight;           //Encoderverdi
int movementTime;         //Tiden for en bevegelse. Brukes for å regne ut hastigheten.

bool newSpeedo = false;             //Brukes for å si om det finnes en ny verdi å skrive til ESP
bool newDistanceTotal = false;      //Brukes for å si om det finnes en ny verdi å skrive til ESP
bool newSpeedSixtyFinal = false;    //Brukes for å si om det finnes en ny verdi å skrive til ESP
bool newDistanceSixtyFinal = false; //Brukes for å si om det finnes en ny verdi å skrive til ESP
bool newMaxSpeed = false;           //Brukes for å si om det finnes en ny verdi å skrive til ESP
bool newBatteryPercent = false;     //Brukes for å si om det finnes en ny verdi å skrive til ESP

float batteryChargeCycles = 0;  //Antall totale ladesykluser for batteriet. Brukes også til å begrense toppverdien for batteriet. Flere sykluser gir lavere batterikapasietet.
bool squareDriven = false;      //Om ladefunksjonen er kjørt.
int batteryPercent;             //Batterinivå i prosent.



//-----------------------Speedometer------------------

void speedometer() {
  oneCheck();     //Kaller funksjon for å sjekke kontinuerlig hastighet (sjekker hvert sekund)
  seventyCheck(); //Kaller funksjon for å sjekk om man er over 70% av max
  sixtyCheck();   //Kaller funksjon for å sjekke om der har gått 60 sekunder og gjøre nødvendig utregninger
  batteryCheck(); //Kaller funksjon som sjekker batteriforbruk
  writeToESP();   //Kaller funksjon for å oppdatere blynk
}


//-----------------------oneCheck--------------------

void oneCheck() {
  bool oneCalc = false;   //For å sjekke om man er i en beregning av 1 sekunder
  long int oneCalcTime;   //Timestamp for start av en måling av 1 sekund

  if (oneCalc == false) { //Sjekke om man er i en "sekundtelling" eller ikke. Starter en måling
    oneCalc = millis();   //Timestamp
    speedLeft = encoders.getCountsLeft();     //Sjekker encoders i starten av en måling
    speedRight = encoders.getCountsRight();   //Sjekker encoders i starten av en måling
    oneCalc = true;       //Markerer at man er i en måling
  }
  if (millis() - oneCalcTime >= 1000) {     //Når det har gått ett sekund siden sist måling
    movementTime = millis() - oneCalcTime;  //Regner ut nøyaktig tid for bevegelsen.
    speedLeft = abs(speedLeft - encoders.getCountsLeft());    //Bruker positiv verdi av dekoderdifferensen side sist måling.
    speedRight = abs(speedRight - encoders.getCountsRight()); //Bruker positiv verdi av dekoderdifferensen side sist måling.
    speedCheck();     //Kaller speedCheck()-funksjonen. Se under
    oneCalc = false;  //Ferdig med en hastighetsmåling.
    distanceTotal = speedo * movementTime / 1000;             //Nullstilles aldri. Historisk mål for total distanse kjørt, målt i meter
    newDistanceTotal = true;                //Markerer at en ny verdi er tilgjengelig for å printe til Blynk
  }
}


//-----------------------SpeedCheck------------------

void speedCheck() {                   //Brukes til å beregne hastigheten til det "raskeste" hjulet.
  const int SNURRE_TERSKEL = 20;      //Ved snurring om egen akse vil dekoderne fortsatt lese avlitt ulikt pga høy følsomhet. Har en terskel som tar høyde for dette.
  const int SPEED_CONVERSION = 7750;  //omregning fra dekodertellinger til m.

  if (speedLeft + SNURRE_TERSKEL > speedRight) {    //Bruker bare verdi fra det hjulet som bevegde seg raskest. Om det andre ble "dratt etter" teller det ikke. Ved sving teller hjulet som bevegde seg raskest.
    speedo = speedLeft / SPEED_CONVERSION / movementTime * 1000; // hastighet for bevegelsen, hensyntatt konvertering fra "dekodertellinger per millisekund" til "meter/sek"
  }
  if (speedLeft < speedRight + SNURRE_TERSKEL) {    //SNURRE_TERSKEL fordi dekoderen er så følsom at selv ved snurring med samme hastighet på begge hjul, vil dekoderverdiene avvike noe. Fanger dette opp med en SNURRE_TERSKEL
    speedo = speedRight / SPEED_CONVERSION / movementTime * 1000; // hastighet for bevegelsen, hensyntatt konvertering fra "dekodertellinger per millisekund" til "meter/sek"
  }
  else {    //Hvis man snurrer om sin egen akse har man ikke fart, og det skal ikke påvirke snittfarta.
    speedo = 0;
    numSixty--;
  }
  if (speedo >  maxSpeed) { //Historisk mål for høyeste hastighet
    maxSpeed = speedo;
    newMaxSpeed = true;     //Markerer at ny verdi er klar til å sendes til Blynk
  }
  newSpeedo = true;         //Markerer at et ny speedometerverdi er oppdatert, så kan den sendes til ESP
}



//-----------------------sixtyCheck-----------------
long int seventyTime; //Total tid over 70% av max
int speedSixty;       //Mellomregningsmål for å beregne gjennomsnittshastighet
int distanceSixty;    //Total distanse innen 60 sekunder

void sixtyCheck() {
  bool sixtyCalc = false; // For å sjekke om man er i en beregning av 60 sekunder
  long int sixtyCalcTime; //Timestamp for start av en måling av 1 sekund

  speedSixty += speedo; //Mellomregning for å regne ut gjennomsnittshastighet hvert 60. sekund. Tar med summen av alle fartsberegninger.
  numSixty++;           //Teller for antall summer i speedSixty. Brukes for å senere regne ut gjennomsnittet av dem.
  if (sixtyCalc == false) {         //Starter på en ny telling dersom en ikke eksisterer
    sixtyCalcTime = millis();       //Timestamp ved starten av en telling
    distanceSixty = distanceTotal;  //Midlertidig verdi for beregning av total distanse over 60 sekunder
    sixtyCalc = true;   //Bool for at timestamp ikke skal sjekke igjen
  }
  if (millis() - sixtyCalcTime >= 60000) {          //Sjekker om det har gått 60 sekunder.
    distanceSixty = distanceTotal - distanceSixty;  //Regner total distanse siden starten av 60-sekundersperioden, basert på differansen mellom den totale distansen over 60 sekunder.
    avgSpeedSixty = speedSixty / numSixty;          //Gjennomsnittshastigheta over 60 sekunder.
    speedSixtyFinal = speedSixty;                   //Tar vare på verdier før man nullstiller og gjør klar for en ny måling
    distanceSixtyFinal = distanceSixty;             //Tar vare på verdier før man nullstiller og gjør klar for en ny måling
    speedSixty = 0;         //nullstiller tellere for å gjøre klart for en ny måling
    distanceSixty = 0;      //nullstiller tellere for å gjøre klart for en ny måling
    seventyTime = 0;        //nullstiller tellere for å gjøre klart for en ny måling
    numSixty = 0;           //nullstiller tellere for å gjøre klart for en ny måling
    sixtyCalc = false;            //Bool for at ny timestamp skal kunne gjennomføres (Starten av denne funksjonen)
    newSpeedSixtyFinal = true;    //Markerer at nye verdier er klare for å sendes til BLYNK
    newDistanceSixtyFinal = true; //Markerer at nye verdier er klare for å sendes til BLYNK
    //EEPROM.write(0, batteryChargeCycles); //Lagrer antall batterisykluser i EEPROM en gang i minuttet. Gjør dette sjeldent for å ikke "slite ut" minnet, som her levetid på ca 100,000 skrive/slette-runder
  }
}



//----------------seventyCheck()-----------
// Funksjonen beregner tiden man er over 70% av max hastighet, etter en terskel man har lagt inn.
bool seventyCalc = false; //For å sjekke når bilen kjører over 70% av maxfart
long int seventyCalcTime; //Timestamp for start av periode med kjøring over 70% av max

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


//--------------batteryCheck---------
void batteryCheck() {
  const int BATTERY_SIZE = 100; //Max batterikapasitet, i meter
  int charged;                  //Teller hvor mye som lades
  int batteryLeft;              //Gjenstående batteri, i meter
  int batteryChargedTotal;      //Totalt opplading av batteriet siden programmets start

  constrain(batteryLeft, 0, BATTERY_SIZE - (BATTERY_SIZE * (ceil(batteryChargeCycles)) / 10)); //Begrenser maks batterikapasitet med 10% for hver hele ladesyklus.
  batteryLeft -= (speedo * movementTime);   //Samme som distansen, men med egen teller. Oppgitt i meter.
  if (squareDriven == true) {               //Lader 20% per runde med firkantkjøring. Arbitrært valg om lading, kan endres.
    charged  = BATTERY_SIZE / 20;           // sette squareDriven til true når man har kjørt firkantmønster, så lader den for hver runde.
    batteryLeft += charged;
    squareDriven = false;
  }
  batteryPercent = batteryLeft / BATTERY_SIZE * 100;


  batteryChargedTotal += charged;           //Teller hvor mye batteriet totalt har blitt oppladet. "Overlading" tærer også på batteriet, som i virkeligheten (Altså om man forsøker å lade over 100%)
  batteryChargeCycles = batteryChargedTotal / BATTERY_SIZE; //Sjekker hvor mye batteriet har blitt oppladet totalt siden programmet startet
  if (batteryChargeCycles >= 3) {           //Hvis batteriet har blitt ladet opp totalt med enn 3 ganger, trenger man service. Da er 70% max
    bool batteryServiceNeeded = true;   //Har ikke gjort noe valg om hva en service eller batteribytte innebærer
  }
  if (batteryChargeCycles >= 5) {           //Hvis batteriet har blitt ladet opp totalt med enn 5 ganger, trenger man nytt batteri. Da er 50% max.
    bool batteryChangeNeeded = true;    //Har ikke gjort noe valg om hva en service eller batteribytte innebærer
  }
}


//-----------------writeToESP----------
//Skriver alle nye verdier til ESP. Samsvarer med switch case i "ESP Code".
// Ingenting her er testet. Mistenker kluss med datatyper. Må sees på når de faktisk er kobla.
void writeToESP() {
  if (newSpeedo == true) {
    Serial1.write(1);
    delay(2);
    Serial1.write(int(speedo));
    newSpeedo = false;
  }
  if (newDistanceTotal == true) {
    Serial1.write(2);
    delay(2);
    Serial1.write(distanceTotal);
    newDistanceTotal = false;
  }
  if (newSpeedSixtyFinal == true) {
    Serial1.write(3);
    delay(2);
    Serial.write(speedSixtyFinal);
    newSpeedSixtyFinal = false;
  }
  if (newDistanceSixtyFinal == true) {
    Serial1.write(4);
    delay(2);
    Serial.write(distanceSixtyFinal);
    newDistanceSixtyFinal = false;
  }
  if (newMaxSpeed == true) {
    Serial1.write(5);
    delay(2);
    Serial1.write(int(maxSpeed));
    newMaxSpeed = false;
  }
  if (newBatteryPercent == true) {
    Serial1.write(6);
    delay(2);
    Serial1.write(batteryPercent);
    newBatteryPercent = false;
  }
}

void setup() {
  /* Fjern kommentar for å aktivere EEPROM-lesing. Fjerne også kommentar på linje 143. Ikke testet noe her ang datatyper og hvordan det lagres, men syntakser stemmer
    if (EEPROM.read(0) != 255){ //Sjekker batterisyklusene i EEPROM ved oppstart av programmet. Ubrukte har verdi 255. Må unngå å lese 255 ved første gangs bruk.
    batteryChargeCycles = EEPROM.read(0);
  */
}
void loop() {
  // put your main code here, to run repeatedly:

}
