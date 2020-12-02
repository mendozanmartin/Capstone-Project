
#ifndef LevelSensor_h
#define LevelSensor_h

#include "Arduino.h"

#define SCOUNT 30
#define VREF 5.0 // analog reference voltage(Volt) of the ADC
#define TEMPERATURE 25

class LevelSensor
{

public:
    LevelSensor(uint8_t analogPin);
    float getReading(void);

private:
    uint8_t analogPin;
    float level;
};
#endif