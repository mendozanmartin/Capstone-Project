/*
  SimpleWifi.h - Header file that contains class declarations for SimpleWifi.cpp
  Created by Martin Mendoza, November 26, 2020
  Released into the public domain.
*/

#ifndef SimpleWifi_h
#define SimpleWifi_h

#include "Arduino.h"
#include "WiFiEsp.h"
#include "PubSubClient.h"
#include <stdio.h> //for printing to the serial monitor. Use key word 'Serial'

class SimpleWifi
{

public:
  SimpleWifi();
  void init(Stream *espSerial, const char *domain, uint16_t port, MQTT_CALLBACK_SIGNATURE);
  void connectToWifi(char *ssid, char *pass);
  boolean publishMessage(const char *topic, const char *payload);
  boolean mqttConnected(void);
  boolean mqttSubscribe(const char *topic, uint8_t qos);
  void mqttPublish(const char *topic, const char *payload);
  void printWifiStatus();
  void connectToMqtt(const char *adafruitUsername, const char *adafruitKey, const char *server);
  void mqttLoop(void);

private:
  WiFiEspClass wifiEsp;
  WiFiEspClient wifiClient;
  PubSubClient mqttClient;
};

#endif