
#include "LevelSensor.h"

LevelSensor::LevelSensor(uint8_t analogPin, float calibrationFactor)
{
    this->analogPin = analogPin;
    this->calibrationFactor = calibrationFactor;
}

float LevelSensor::getReading()
{
    float pressureSensorValue = 0;

    pressureSensorValue = analogRead(this->analogPin);
    this->level = (pressureSensorValue - 99) * this->calibrationFactor; // Convert the analog reading (which goes from 0 - 1023) to a pressureSensorVoltage (0 - 10PSI)
    Serial.print("Pressure Sensor Level:");
    Serial.print(this->level, 0);
    Serial.println("cm");

    return this->level;
}

float LevelSensor::getReading(int offset)
{
    float pressureSensorValue = 0;

    pressureSensorValue = analogRead(this->analogPin);
    this->level = (pressureSensorValue - 99 - offset) * this->calibrationFactor; // Convert the analog reading (which goes from 0 - 1023) to a pressureSensorVoltage (0 - 10PSI)
    Serial.print("Pressure Sensor Level:");
    Serial.print(this->level, 0);
    Serial.println("cm");

    return this->level;
}