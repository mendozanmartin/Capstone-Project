/*
  Turbidity.cpp lol is a thing idk
  Ernest Wong
*/

#include "TurbidityLib.h"

TurbidityLib::TurbidityLib(uint8_t analogPin)
{
    this->analogPin = analogPin;
}

float TurbidityLib::getReading()
{
    int sensorValue = analogRead(analogPin);             // read the input on analog pin 0:
    float voltRead = sensorValue * (5.0 / 1024.0); // Convert the analog reading (which goes from 0 - 1023) to a voltRead (0 - 5V):
    float ntu = (-1120.4 * pow(voltRead, 2)) + (5742.3 * voltRead) - 4352.9;

    Serial.println("NTU: " + String(ntu));

    return ntu
}