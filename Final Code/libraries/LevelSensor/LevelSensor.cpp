
#include "LevelSensor.h"

LevelSensor::LevelSensor(uint8_t analogPin)
{
    this->analogPin = analogPin;
}

float LevelSensor::getReading()
{
    float pressureSensorValue = 0;

    pressureSensorValue = analogRead(this->analogPin);
    this->level = (pressureSensorValue - 99) * 1.3; // Convert the analog reading (which goes from 0 - 1023) to a pressureSensorVoltage (0 - 10PSI)
    Serial.print("Pressure Sensor Level:");
    Serial.print(this->level, 0);
    Serial.println("cm");

    return this->level;
}