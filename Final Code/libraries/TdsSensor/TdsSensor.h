
#ifndef TdsSensor_h
#ifndef TdsSensor_h

#include "Arduino.h"

class TdsSensor
{

public:
    TdsSensor(uint8_t analogPin);
    float getReading(void);
    int getMedianNum(int bArray[], int iFilterLen);

private:
    float reading;
    uint8_t analogPin;
}