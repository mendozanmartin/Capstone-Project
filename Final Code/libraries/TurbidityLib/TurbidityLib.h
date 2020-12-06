/*
  TurbidityLib.h - Header file that contains class declarations for Turbidity.cpp
  Created by Ernest WOng
  Released into the public domain.
*/

#include "Arduino.h"
#include <stdio.h> //for printing to the serial monitor. Use key word 'Serial'

#define SCOUNT 30
#define VREF 5.0 // analog reference voltage(Volt) of the ADC
#define TEMPERATURE 25

class TurbidityLib
{
public:
  TurbidityLib(uint8_t analogPin, float calibrationFactor);
  float getReading(void);
  int getMedianNum(int bArray[], int iFilterLen);
  void startSampling(void);

private:
  int analogBuffer[SCOUNT];
  int analogBufferTemp[SCOUNT];
  uint8_t analogPin;
  float calibrationFactor;
};
