
#include "TdsSensor.h"

TdsSensor::TdsSensor(uint8_t analogPin)
{
    this->analogPin = analogPin;
}

void TdsSensor::startSampling()
{
    int analogBufferIndex = 0;
    while (analogBufferIndex != SCOUNT)
    {
        this->analogBuffer[analogBufferIndex] = analogRead(this->analogPin); //read the analog value and store into the buffer
        analogBufferIndex++;
        delay(40); // sample every 40 milliseconds
    }
    analogBufferIndex = 0;
}

float TdsSensor::getReading()
{
    float averageVoltage = 0;
    averageVoltage = this->getMedianNum(this->analogBuffer, SCOUNT) * (float)VREF / 1024.0;                                                                                                // read the analog value more stable by the median filtering algorithm, and convert to voltage value
    float compensationCoefficient = 1.0 + 0.02 * (TEMPERATURE - 25.0);                                                                                                                     //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
    float compensationVoltage = averageVoltage / compensationCoefficient;                                                                                                                  //temperature compensation
    this->tdsValue = (133.42 * compensationVoltage * compensationVoltage * compensationVoltage - 255.86 * compensationVoltage * compensationVoltage + 857.39 * compensationVoltage) * 0.5; //convert voltage value to tds value
                                                                                                                                                                                           //Serial.print("voltage:");
                                                                                                                                                                                           //Serial.print(averageVoltage,2);
                                                                                                                                                                                           //Serial.print("V ");
    Serial.print("TDS Value:");
    Serial.print(this->tdsValue, 0);
    Serial.println("ppm");
    return this->tdsValue;
}

int TdsSensor::getMedianNum(int bArray[], int iFilterLen)
{
    int bTab[iFilterLen];
    for (byte i = 0; i < iFilterLen; i++)
        bTab[i] = bArray[i];
    int i, j, bTemp;
    for (j = 0; j < iFilterLen - 1; j++)
    {
        for (i = 0; i < iFilterLen - j - 1; i++)
        {
            if (bTab[i] > bTab[i + 1])
            {
                bTemp = bTab[i];
                bTab[i] = bTab[i + 1];
                bTab[i + 1] = bTemp;
            }
        }
    }
    if ((iFilterLen & 1) > 0)
        bTemp = bTab[(iFilterLen - 1) / 2];
    else
        bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
    return bTemp;
}