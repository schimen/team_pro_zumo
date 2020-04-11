/* Koden skal være på ESP'en som er et bindeledd mellom BLYNK og Zumoen
   Programmet tar imot instrukser fre BLYNK, og sender de videre til ZUMO via
   seriemonitoren.

*/


#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
char auth[] = "AUTH";   //ENDRE VERDIER HER FOR BLYNK OG NETT
char ssid[] = "SSID";   //ENDRE VERDIER HER FOR BLYNK OG NETT
char pass[] = "PASS";   //ENDRE VERDIER HER FOR BLYNK OG NETT
const int W = 12;   //Sett inn pinneverdier. Samsvar med Blynk-app.
const int A = 13;   //Sett inn pinneverdier. Samsvar med Blynk-app.
const int S = 14;   //Sett inn pinneverdier. Samsvar med Blynk-app.
const int D = 15;   //Sett inn pinneverdier. Samsvar med Blynk-app.
bool moved;   //Bool som passer på at ESP bare printer hvert tegn en gang
int maxSpeed = 1400; //Maxhastigheten fra 0 - 400 skal representeres som verdi fra 1000 - 1400.

int inByte; //Brukes for å ta imot kommunikasjon FRA Zumo som skal TIL Blynk

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(W, INPUT); //WASD i Blynk setter fysiske pinner høy og lav. Dette for å få anvendt både fysiske og virtuelle pinner
  pinMode(A, INPUT); //WASD i Blynk setter fysiske pinner høy og lav
  pinMode(S, INPUT); //WASD i Blynk setter fysiske pinner høy og lav
  pinMode(D, INPUT); //WASD i Blynk setter fysiske pinner høy og lav
  Serial.flush();
}

BLYNK_WRITE(V0) { //Når man endrer verdien på slideren i Blynk, sendes verdien som råtekst til Zumo.
  int maxSpeed = param.asInt();
  Serial.print(maxSpeed);
}

void loop() {
  Blynk.run();

//--------------Sending---
  if (digitalRead(W) == 1 && moved == false) { //Printer "W" i Serial hvis fysisk pinne er satt høy av Blynk
    Serial.write("W");
    moved = true; //Boolske verdi påser at strengen blir printet kun én gang
  }
  if (digitalRead(A) == 1 &&  moved == false) { //Printer "A" i Serial hvis fysisk pinne er satt høy av Blynk
    Serial.write("A");
    moved = true;//Boolske verdi påser at strengen blir printet kun én gang
  }
  if (digitalRead(S) == 1 && moved == false) { //Printer "S" i Serial hvis fysisk pinne er satt høy av Blynk
    Serial.write("S");
    moved = true;//Boolske verdi påser at strengen blir printet kun én gang
  }
  if (digitalRead(D) == 1 && moved == false) { //Printer "D" i Serial hvis fysisk pinne er satt høy av Blynk
    Serial.write("D");
    moved = true;//Boolske verdi påser at strengen blir printet kun én gang
  }
  if (digitalRead(W) == 0 && digitalRead(A) == 0 && digitalRead(S) == 0 && digitalRead(D) == 0) { //Denne burde kunne være en "Else", men da printa den gjentatte ganger. Feilsøkte ikke mer. Det finner du ut av hvis du vil
    if (moved == true) { //Denne løkka kan og bør gjøres enklere. Ble litt frankenstein på slutten. Den gjør i grunn bare at det sendes en '0' dersom alle angitte fysiske pinner er lave.
      Serial.write("0");
      moved = false;
    }
  }

//--------- Mottak
//De ulike tallene representerer nye verdier som hastighet, batterinivå, snitthastighet etc.
  if (Serial.available() > 0) { //Tar imot verdier FRA Zumo som skal TIL Blynk
    inByte = Serial.read();     //Bruker her tall istedet for strenger, for å bruke ulike datatyper
    switch (inByte) {
      case '1':                 //Zumo sender et siffer, delayer et par millis, så sender den verdien
        while (inByte == 1);    //Venter på ny verdi i Serial
        Blynk.virtualWrite(V5, inByte); //Skriver nye verdien til respektive virtualPin.
        break;
      case '2':
        while (inByte == 2);
        Blynk.virtualWrite(V6, inByte);
        break;
      case '3':
        while (inByte == 3);
        Blynk.virtualWrite(V7, inByte);
        break;
      case '4':
        while (inByte == 4);
        Blynk.virtualWrite(V8, inByte);
        break;
      case '5':
        while (inByte == 5);
        Blynk.virtualWrite(V9, inByte);
        break;
    }
  }
}
