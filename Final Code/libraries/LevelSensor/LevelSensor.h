
#ifndef LevelSensor_h
#define LevelSensor_h

#include "Arduino.h"
#include <stdio.h> //for printing to the serial monitor. Use key word 'Serial'

#define SCOUNT 30
#define VREF 5.0 // analog reference voltage(Volt) of the ADC
#define TEMPERATURE 25

class LevelSensor
{

public:
    LevelSensor(uint8_t analogPin, float calibrationFactor);
    float getReading(void);
    float getReading(float offset);

private:
    uint8_t analogPin;
    float level;
    float calibrationFactor;
};
#endif