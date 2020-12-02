
#ifndef TdsSensor_h
#define TdsSensor_h

#include "Arduino.h"

#define SCOUNT 30
#define VREF 5.0 // analog reference voltage(Volt) of the ADC
#define TEMPERATURE 25

class TdsSensor
{

public:
    TdsSensor(uint8_t analogPin);
    float getReading(void);
    int getMedianNum(int bArray[], int iFilterLen);
    void startSampling();

private:
    int analogBuffer[SCOUNT];
    int analogBufferTemp[SCOUNT];
    float reading;
    float tdsValue;
    uint8_t analogPin;
};
#endif