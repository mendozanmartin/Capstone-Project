
#ifndef TdsSensor_h
#define TdsSensor_h

#include "Arduino.h"
#include <stdio.h> //for printing to the serial monitor. Use key word 'Serial'

#define SCOUNT 30
#define VREF 5.0 // analog reference voltage(Volt) of the ADC

class TdsSensor
{

public:
    TdsSensor(uint8_t analogPin, float TEMPERATURE);
    float getReading(void);
    int getMedianNum(int bArray[], int iFilterLen);
    void startSampling(void);

private:
    int analogBuffer[SCOUNT];
    int analogBufferTemp[SCOUNT];
    float reading;
    float tdsValue;
    uint8_t analogPin;
    float TEMPERATURE;
};
#endif