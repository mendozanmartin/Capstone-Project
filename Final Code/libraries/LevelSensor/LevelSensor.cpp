
#include "LevelSensor.h"

LevelSensor::LevelSensor(uint8_t analogPin, float calibrationFactor)
{
    this->analogPin = analogPin;
    this->calibrationFactor = calibrationFactor;
}

float LevelSensor::getReading()
{
    int pressureSensorValue = 0;
    float level;

    pressureSensorValue = analogRead(this->analogPin);
    level = (pressureSensorValue - 99 - 6); // Convert the analog reading (which goes from 0 - 1023) to a pressureSensorVoltage (0 - 10PSI)
    Serial.print("Pressure Sensor Level:");
    Serial.print(level, 0);
    Serial.println("cm");

    Serial.print("Raw Pressure Level Reading:");
    Serial.print(pressureSensorValue);
    Serial.println(" units");

    return level;
}

float LevelSensor::getReading(float offset)
{
    float pressureSensorValue = 0;

    pressureSensorValue = analogRead(this->analogPin);
    this->level = (pressureSensorValue - 99 - offset) * this->calibrationFactor; // Convert the analog reading (which goes from 0 - 1023) to a pressureSensorVoltage (0 - 10PSI)
    Serial.print("Pressure Sensor Level:");
    Serial.print(this->level, 2);
    Serial.println("cm");

    Serial.print("Raw Pressure Level Reading:");
    Serial.print(pressureSensorValue);
    Serial.println(" units");

    return this->level;
}