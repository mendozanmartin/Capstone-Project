
#include "Configuration.h"
#include "SimpleWifi.h"
#include "TdsSensor.h"
#include "LevelSensor.h"
#include "TurbidityLib.h"
#include "ValveControlLib.h"
#include "FlowMeter.h"

#include <Timer.h>

#define pressureSensorPin A0
#define tdsSensorPin A1
#define turbiditySensorPin A2
#define flowSensorPin 8
#define inletValvePin 9

char ssid[] = WIFI_SSID;     // your network SSID (name)
char pass[] = WIFI_PASSWORD; // your network password
char adafruitUsername[] = ADAFRUIT_IO_USERNAME;
char adafruitKey[] = ADAFRUIT_IO_KEY;
char server[] = MQTT_SERVER;
int port = MQTT_PORT;

void callback(char *topic, byte *payload, unsigned int length);
void reconnect();

float LEVEL_THRESHOLD = 10;
int PUBLISH_INTERVAL = 2000;
int valveState = 1;

//Timer declaration
Timer mqttLoop;
Timer sensorLoop;
Timer controlLoop;

SimpleWifi simpleWifi;

//Sensors Declaration
TdsSensor tdsSensor(tdsSensorPin);
LevelSensor levelSensor(pressureSensorPin);
TurbidityLib turbiditySensor(turbiditySensorPin);
FlowMeter flowSensor(flowSensorPin);
ValveCtrl valve(inletValvePin);

int AUTOMATIC_MODE = 1; // set arduino to automatic mode first

void setup()
{
  pinMode(tdsSensorPin, INPUT);
  pinMode(turbiditySensorPin, INPUT);
  pinMode(flowSensorPin, INPUT);
  pinMode(pressureSensorPin, INPUT);

  // initialize serial for debugging
  Serial.begin(115200);
  // initialize serial for ESP module
  Serial1.begin(115200);

  simpleWifi.init(&Serial1, server, port, callback);
  simpleWifi.connectToWifi(ssid, pass);

  pinMode(LED_BUILTIN, OUTPUT);
  mqttLoop.every(1000, mqttConnect);
  controlLoop.every(1500, controlValve);
  sensorLoop.every((PUBLISH_INTERVAL * 4 + 3), publishSensorReadings); // give it extra 2 seconds so that function calls do not overlap
  // sensorLoop.every((PUBLISH_INTERVAL), sensorPublish); // give it extra 2 seconds so that function calls do not overlap
}

void mqttConnect()
{

  while (!simpleWifi.mqttConnected())
  {
    simpleWifi.connectToMqtt(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY, server);
    boolean valveSubscription = 0;
    boolean modeSubscription = 0;

    if (AUTOMATIC_MODE == 1) { // this will prevent users from operating the valves in the dashboard
      modeSubscription = simpleWifi.mqttSubscribe("mendozamartin/feeds/automatic-mode", 1);
      simpleWifi.mqttPublish("mendozamartin/feeds/automatic-mode", "ON");
    } else { // this will allow the user to operate the valves in the dashboard
      valveSubscription = simpleWifi.mqttSubscribe("mendozamartin/feeds/outlet-valve", 1);
      modeSubscription = simpleWifi.mqttSubscribe("mendozamartin/feeds/automatic-mode", 1);
      simpleWifi.mqttPublish("mendozamartin/feeds/automatic-mode", "OFF");
    }

    if (valveSubscription) // check if arduino client is subscribed to valve topic
    {
      Serial.println("MQTT client is subscribed to valve topic");
    }
    else
    {
      Serial.println("MQTT client is not subscribed to valve topic");
    }

    if (modeSubscription) // check if arduino client is subscribed to valve topic
    {
      Serial.println("MQTT client is subscribed to mode topic");
    }
    else
    {
      Serial.println("MQTT client is not subscribed to mode topic");
    }
  }

  simpleWifi.mqttLoop();
}

void controlValve()
{ 
  if (AUTOMATIC_MODE == 1) { // execute logic when system is in automatic mode
    if (levelSensor.getReading() >= LEVEL_THRESHOLD)
    {
      valve.closeValve();
      simpleWifi.mqttPublish("mendozamartin/feeds/outlet-valve", "OFF");
    }
    else
    {
      valve.openValve();
      simpleWifi.mqttPublish("mendozamartin/feeds/outlet-valve", "ON");
    }
  }
}

void publishSensorReadings()
{
  char msgBuffer[10]; // make sure this is big enough to hold your string

  float levelSensorValue = levelSensor.getReading();
  simpleWifi.mqttPublish("mendozamartin/feeds/collection-tank-level", dtostrf(levelSensorValue, 6, 2, msgBuffer));

  ///////////////////////////////////////////////////////////////////////////////////////
  delay(PUBLISH_INTERVAL);
  turbiditySensor.startSampling();
  float NTU = turbiditySensor.getReading(); // read the input on analog pin 0:
  simpleWifi.mqttPublish("mendozamartin/feeds/collection-tank-turbidity", dtostrf(NTU, 6, 2, msgBuffer));

  //////////////////////////////////////////////////////////////////////////////////////
  delay(PUBLISH_INTERVAL);

  tdsSensor.startSampling();
  float tdsValue = tdsSensor.getReading();
  simpleWifi.mqttPublish("mendozamartin/feeds/collection-tank-tds", dtostrf(tdsValue, 6, 2, msgBuffer));

  ///////////////////////////////////////////////////////////////////////////////////////
  delay(PUBLISH_INTERVAL);
  float flowRate = flowSensor.getReading();
  simpleWifi.mqttPublish("mendozamartin/feeds/inlet-flowrate", dtostrf(flowRate, 6, 2, msgBuffer));
}

void loop()
{
  mqttLoop.update();
  sensorLoop.update();
  controlLoop.update();
  flowSensor.flowSampling();
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");

  char message[length];
  for (int i = 0; i < length; i++) // loop bytes from payload and place into char array
  {
    message[i] = payload[i];
  }
  if (topic == "mendozamartin/feeds/outlet-valve") {
    if (message[1] == 'N')
    {
      Serial.println(message);
      valve.openValve();
    }
    else if (message[1] == 'F')
    {
      Serial.println(message);
      valve.closeValve();
    }
  } else if (topic == "mendozamartin/feeds/automatic-mode") {
    if (message[1] == 'N')
    {
      Serial.println(message);
      AUTOMATIC_MODE = 1;
    }
    else if (message[1] == 'F')
    {
      Serial.println(message);
      AUTOMATIC_MODE = 0;
    }
  }

}
