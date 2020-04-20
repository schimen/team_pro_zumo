/*Programmet brukes til å simulere batteriservice eller batteriskift ved å oppdage når bilen er snudd opp ned, hvor lenge den holdes der.
   Den markerer ved 3s etter snudd bil at batteriet er gjort service på, og ved 5s at batteriet er skiftet. Den utfører bare en av handlingene per gang.
*/
float batteryChargeCycles = 0;  //Antall totale ladesykluser for batteriet. Brukes også til å begrense toppverdien for batteriet. Flere sykluser gir lavere batterikapasietet.
const int BATTERY_MAX = 100; //Max batterikapasitet, i meter
float batteryLeft = BATTERY_MAX; //Gjenstående batteri, i meter
float batteryChargedTotal; //Totalt opplading av batteriet siden programmets start
bool newCharge = false;

unsigned long TimeInterval = 0; //Oppdaterer Y-rotasjon i et gitt intervall
int Y = 0;                      // Holder y-posisjonen
unsigned long startTime;        //teller
bool turnSense = false;         //Lagrer om Zumoen er opp ned eller ikke. (true = opp ned, false = rett vei)
int workDone;                   //Lagrer hvilket arbeid som er utført på batteriet (SERVICE / CHANGED)
const int SERVICE = 1;          //Utført service på batteriet
const int CHANGED = 2;          //Utført bytte av batteri

void checkIfTurned() {
  bool serviceSoundPlayed = false;
  bool changedSoundPlayed = false;
  updateGyro(); // Oppdaterer Y-posisjon

  if (turnSense == true) { //Hvis bilen har blitt snudd
    startTime = millis(); //Starter en teller

    while (turnSense == true) { //Så lengde bilen er snudd
      if (startTime + 3000 <= millis() < startTime + 5000) { //Hvis bilen har vært snudd 2s eller over, men mindre enn 5s
        if (serviceSoundPlayed == false) {
          Serial.println("SERVICE");
          workDone = SERVICE;
          buzzer.play("!T87 L8 V7 ce"); //Gir lydvarsel for service utført
          serviceSoundPlayed = true;
          while (buzzer.isPlaying());
        }
      }
      if (millis() >= startTime + 5000) { //Hvis bilen har vært snudd 5s eller over
        if (changedSoundPlayed == false) {
          Serial.println("CHANGED");
          workDone = CHANGED;
          buzzer.play("!T60 L8 V7 ged"); //Gir lydvarsel for bytte av batteri utført
          while (buzzer.isPlaying());
          changedSoundPlayed = true;
        }
      }
      updateGyro(); //Oppdatere Y-posisjon fer å sjekke om bilen er snudd igjen
    }

    if (workDone == SERVICE) {
      Serial.println("workDone is service");
      batteryChargeCycles = 1; // Fjerner 1 ladesyklus
      batteryChargedTotal = 100;
      batteryLeft = 100; // Resetter batteriprosent til 100
      newCharge == true; // Oppdaterer batteriutregninger
      workDone = 0; //  Resetter workDone variablen
      speedometer();

    }
    if (workDone == CHANGED) {
      batteryChargeCycles = 0; //Resetter ladesyklusene til 0
      Serial.println("workDone is changed");
      batteryLeft = 100; //Resetter batteriprosent til 100
      batteryChargedTotal = 0; //resetter batteryChargeCycles-utregningen
      workDone = 0;
      speedometer();
    }
  }
}

void updateGyro() { //Sjekker rotasjonsposisjon til Zumoen
  gyro.read(); //Leser gyroens verdi
  if ((millis() - TimeInterval) >= 100) { //Oppdaterer endringa av Y hvert 100ms
    TimeInterval = millis();
    Y = gyro.g.y; //Lagrer Y-posisjon
  }
  if (abs(Y) > 3000) { //Hvis bilen roterer rundt tverrgående-akse
    startTime = millis(); //Starter en teller
    bool OneSwitch = true; //Sikrer at turnSense kun endrer seg en gang per rotasjon
    while ( abs(Y) > 3000) { //Så læng Zumoen rotere
      if ((millis() >= startTime + 500) && (OneSwitch == true)) { //Hvis bilen har rotert i mer enn 0.5 sek
        turnSense = !turnSense; //Toggle turnSense slik at bilen er opp ned eller rett vei annen hver gang
        Serial.print("turnSense: ");
        Serial.println(turnSense);
        OneSwitch = false; // turnSense kan ikke toggles mer så lenge bilen roterer før neste runde igjen
      }
      gyro.read();// Sjekke om bilen har sluttet å rotere
      if ((millis() - TimeInterval) >= 100) { //Oppdaterer endringen av Y hvert 100ms
        TimeInterval = millis();
        Y = gyro.g.y;
      }
    }
  }
}
