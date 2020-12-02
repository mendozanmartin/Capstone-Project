/*
  TurbidityLib.h - Header file that contains class declarations for Turbidity.cpp
  Created by Ernest WOng
  Released into the public domain.
*/

#include "Arduino.h"
#include <stdio.h> //for printing to the serial monitor. Use key word 'Serial'

class ValveCtrl
{
  public:
    ValveCtrl(uint8_t outputPin);
    void openValve(void);
    void closeValve(void);

  private:
    uint8_t outputPin;
};

