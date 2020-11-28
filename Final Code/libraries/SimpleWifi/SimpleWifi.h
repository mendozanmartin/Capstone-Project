/*
  Configuration.h - Header file for Storing Configuration Variables for the WaSh SMART Water Tank.
  Created by Martin Mendoza, November 26, 2020
  Released into the public domain.
*/

#ifndef SimpleWifi_h
#define SimpleWifi_h

#include "Arduino.h"
#include "../WiFiEsp/src/WiFiEsp.h"
#include "../PubSubClient/src/PubSubClient.h"
#include <stdio.h> //for printing to the serial monitor. Use key word 'Serial'

class SimpleWifi
{

public:
  SimpleWifi();
  void init(void);

private:
  static WiFiEspClient wifiClient;
  static PubSubClient mqttClient;
};

#endif