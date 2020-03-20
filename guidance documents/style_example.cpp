/*
*
* This is a style example.
* This example shows layout and formatting of code for zumo project.
* more description of code here ...
*
*/

//constants
const  SOMEVALUE = 100;
const OTHERVALUE = 2;

//global variables
uint8_t someVariable = 100;
bool   otherVariable = true;

bool isGreater(uint8_t someNumber, uint8_t otherNumber) {
  /*
  takes two numbers and returns true if first number (someNumber)
  is greater than second number (otherNumber)
  */
  if (someNumber > otherNumber) {
    return true;
  }
  else  {
    return false;
  }
}

void doNothing()  {
  /*
  this function does nothing
  */
  bool nothing = true;  //this variable is not used,
                       // this function does nothing :)
}

void loop() {
  /*
  loop function
  */
  if isGreater(SOMEVALUE, OTHERVALUE) {
    doNothing();
  }
}
