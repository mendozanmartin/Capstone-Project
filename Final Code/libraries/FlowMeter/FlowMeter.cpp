/*
    FlowMeter.cpp is a wrapper for flow meter reading conversion
    Javaughn Smith
*/

#include "FlowMeter.h"

FlowMeter::FlowMeter(uint8_t digitalPin)
{
    this->digitalPin = digitalPin;
    this->arrayCount = 0;
    this->pulseCount = 0;
    this->lastPulseSignal = 0;
}

void FlowMeter::flowSampling()
{
    unsigned long delta_t, final_t, initial_t = 0;

    this->pulseSignal = digitalRead(this->digitalPin);
    if (this->pulseSignal != this->lastPulseSignal && this->pulseSignal == true)
    {
        this->pulseCount++;
    }
    this->lastPulseSignal = this->pulseSignal;

    final_t = millis();
    delta_t = final_t - initial_t;

    if (delta_t >= 2000)
    {

        this->countArray[this->arrayCount] = this->pulseCount;
        Serial.println(this->countArray[this->arrayCount]);
        this->pulseCount = 0;

        if (this->arrayCount == 4)
        {
            this->arrayCount = 0;
        }

        this->arrayCount++;

        initial_t = final_t;
    }
}

float FlowMeter::getReading()
{
    int totalPulses = 0;
    for (int i = 0; i < 4; i++)
    {
        pulseCount += this->countArray[i];
    }
    Serial.println("Pulse Count: " + String(totalPulses));
    Serial.println("Flow Rate delta t: " + String(10000) + "ms"); // 10 elements in array, each containing 2 seconds of pulse data
    this->flowRate = (totalPulses / 425.0) / (10000 * pow(10, -3) / 60.0);
    Serial.println("Flow Rate: " + String(flowRate) + " L/min");
    return this->flowRate;
}