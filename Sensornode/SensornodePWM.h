/* * * * * * * * * * *     PWM class     * * * * * * * * * * */

#ifndef SensornodePWM_h
#define SensornodePWM_h

#include <Arduino.h>

class PWM {
  public:

    //constructor of PWM class.
    //       channel,        pin,        frequency,       resolution
    PWM(uint8_t chan, uint8_t pin, double freq = 50, uint8_t res = 8);

    //save new duty cycle and write it to channel
    void setDuty(uint16_t newDuty);

    //save new duty cycle and write it to channel
    //    (converted from degree)
    void setDegree(uint8_t degree);

    //set new duty cycle as sum of current duty cycle and change
    void changeDuty(int16_t change);

    //set new degree based on current degree and the change of degree
    void changeDegree(int8_t degreeChange);

    //return dutyCycle
    uint16_t getDuty();

    //maps duty cycle to degree end return current degree
    uint8_t getDegree();

    //save new frequency and write it to channel
    void setTone(double frequency);

    //turn off channel (duty cycle to zero)
    void setOff();

    //return true if duty cycle is more than zero
    bool isOn();

  private:
    uint16_t dutyCycle;
    uint8_t  channel;
    double   frequency;
};

#endif
