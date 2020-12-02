/*
    Flowmeter.h - Header file containing a class declaration for FlowMeter.cpp
    Created by Javaughn Smith, December 1, 2020
    Released into the public domain.
*/

#ifndef FlowMeter_h
#define FlowMeter_h

#include "Arduino.h"

class FlowMeter
{

    public:
        FlowMeter(uint8_t digitalPin);
        float flowValue(void);

    private:
        uint8_t digitalPin;
        bool pulseSignal, lastPulseSignal;
        unsigned long delta_t, final_t, initial_t;
        int pulseCount, countArray[5];
};

#endif