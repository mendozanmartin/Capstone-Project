/*
    FlowMeter.cpp is a wrapper for flow meter reading conversion
    Javaughn Smith
*/

#include "FlowMeter.h"

FlowMeter::FlowMeter(uint8_t digitalPin)
{
    this->digitalPin = digitalPin;
}

FlowMeter::flowValue()
{
    bool lastPulseSignal = 0;
    unsigned long delta_t, final_t, initial_t = 0;

    this->pulseSignal = digitalRead(this->digitalPin);
    if (pulseSignal != lastPulseSignal && pulseSignal == true)
    {
        pulseCount++;
    }
    lastPulseSignal = pulseSignal;

    final_t = millis();
    delta_t = final_t - initial_t;

    if (delta_t >= 2000)
    {

        countArray[j] = pulseCount;
        Serial.println(countArray[j]);
        pulseCount = 0;

        if (j == 4)
        {
        j = 0;
        }

        j++;

    initial_t = final_t;
  } 
}