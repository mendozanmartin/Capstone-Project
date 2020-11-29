/*
  Wifi.cpp - A simple client for MQTT.
  Nick O'Leary
  http://knolleary.net
*/

#include "SimpleWifi.h"

static int status = WL_IDLE_STATUS;

SimpleWifi::SimpleWifi()
{

  WiFiEspClient wifiClient;
  WiFiEspClass wifiEsp;

  this->wifiClient = wifiClient;
  this->wifiEsp = wifiEsp;
}

void SimpleWifi::init(Stream *espSerial, const char *domain, uint16_t port, void (*callback)(char *, uint8_t *, unsigned int))
{
  PubSubClient mqttClient(domain, port, callback, this->wifiClient);
  this->mqttClient = mqttClient;
  this->wifiEsp.init(espSerial);
}

void SimpleWifi::connectToWifi(char *ssid, char *pass)
{
  // check for the presence of the shield
  if (this->wifiEsp.status() == WL_NO_SHIELD)
  {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true)
      ;
  }

  // attempt to connect to WiFi network
  while (status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = this->wifiEsp.begin(ssid, pass);
  }

  // you're connected now, so print out the data
  Serial.println("You're connected to the network");
  this->printWifiStatus();
}

void SimpleWifi::printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(this->wifiEsp.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = this->wifiEsp.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = this->wifiEsp.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void SimpleWifi::connectToMqtt(const char *adafruitUsername, const char *adafruitKey, const char *server)
{
  Serial.print("Attempting MQTT connection...");
  String clientId = "ESP8266Client-";
  clientId += String(random(0xffff), HEX);
  if (this->mqttClient.connect(clientId.c_str(), adafruitUsername, adafruitKey))
  {
    Serial.print("Connected to: ");
    Serial.println(server);
  }
  else
  {
    Serial.print("failed, rc=");
    Serial.print(this->mqttClient.state());
    Serial.println(" try again in 2 seconds");
    delay(2000);
  }
}

boolean SimpleWifi::mqttConnected()
{
  return this->mqttClient.connected();
}

void SimpleWifi::mqttLoop()
{
  this->mqttClient.loop();
}

boolean SimpleWifi::mqttSubscribe(const char *topic, uint8_t qos)
{
  return this->mqttClient.subscribe(topic, qos);
}

void SimpleWifi::mqttPublish(const char *topic, const char *payload)
{
  this->mqttClient.publish(topic, payload);
}