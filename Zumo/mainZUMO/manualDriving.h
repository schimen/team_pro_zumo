
#ifndef manualDriving_h
#define manualDriving_h

#include <Arduino.h>
#include "zumoObjects.h"
#include "speedometerBattery.h"

//endret dette fra å bruke global inChar til å
//ta et argument istedet. Dette er MYE bedre kode practice
//hvis ikke det er absolutt nødvendig å bruke
//den globale variablen
//Jeg ser at dette kan skape problem siden inChar endrer
//verdi i funksjonen. dette kan være må fikses
void manualDriving(char input);

#endif
