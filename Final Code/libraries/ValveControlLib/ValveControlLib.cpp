/*
  Turbidity.cpp lol is a thing idk
  Ernest Wong
*/

#include "ValveControlLib.h"

ValveCtrl::ValveCtrl(uint8_t outputPin)
{
  this->outputPin = outputPin;
}

void ValveCtrl::openValve()
{
  digitalWrite(this->outputPin, LOW);
  Serial.println("Opening Valve");
}

void ValveCtrl::closeValve()
{
  digitalWrite(this->outputPin, HIGH);
  Serial.println("closing Valve");
}