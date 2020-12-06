/*
  Turbidity.cpp lol is a thing idk
  Ernest Wong
*/

#include "TurbidityLib.h"

TurbidityLib::TurbidityLib(uint8_t analogPin, float calibrationFactor)
{
  this->analogPin = analogPin;
  this->calibrationFactor = calibrationFactor;
}

float TurbidityLib::getReading()
{
  float averageSensorValue = this->getMedianNum(this->analogBuffer, SCOUNT);        // read the input on analog pin 0:
  float voltRead = (averageSensorValue * (5.0 / 1024.0)) + this->calibrationFactor; // Convert the analog reading (which goes from 0 - 1023) to a voltRead (0 - 5V):
  float ntu = (-1120.4 * pow(voltRead, 2)) + (5742.3 * voltRead) - 4352.9;

  if (ntu <= 0)
  {
    ntu = 0;
  }
  Serial.println("NTU: " + String(ntu));
  return ntu;
}

void TurbidityLib::startSampling()
{
  int analogBufferIndex = 0;
  while (analogBufferIndex != SCOUNT)
  {
    this->analogBuffer[analogBufferIndex] = analogRead(this->analogPin); //read the analog value and store into the buffer
    //Serial.println("This is the turbidity sample tingz: ");
    //Serial.print(analogRead(this->analogPin));
    analogBufferIndex++;
    delay(40); // sample every 40 milliseconds
  }
  analogBufferIndex = 0;
}

int TurbidityLib::getMedianNum(int bArray[], int iFilterLen)
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