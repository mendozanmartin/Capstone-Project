
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
#define inletValvePin 10
#define manualValvePin 52

char ssid[] = WIFI_SSID;     // your network SSID (name)
char pass[] = WIFI_PASSWORD; // your network password
char adafruitUsername[] = ADAFRUIT_IO_USERNAME;
char adafruitKey[] = ADAFRUIT_IO_KEY;
char server[] = MQTT_SERVER;
int port = MQTT_PORT;

void callback(char *topic, byte *payload, unsigned int length);
void reconnect();

int UPPER_LEVEL_THRESHOLD = 6;
int LOWER_LEVEL_THRESHOLD = 2.5;
int PRESSURE_CALIBRATION = 0;
int PUBLISH_INTERVAL = 2500;
int valveState = 1;

// wiring for the ESP module can be found in this tuturial below
// https://www.instructables.com/noobs-guide-to-ESP8266-with-Arduino-Mega-2560-or-U/
// Please use Serial1 which is pin 19 (RX) and pin 18 (TX)

//Timer declaration
Timer mqttLoop;
Timer sensorLoop;
Timer controlLoop;

SimpleWifi simpleWifi;

//Sensors Declaration
TdsSensor tdsSensor(tdsSensorPin, 25);
LevelSensor levelSensor(pressureSensorPin, 0.75);
TurbidityLib turbiditySensor(turbiditySensorPin, 0.65);
FlowMeter flowSensor(flowSensorPin);
ValveCtrl valve(inletValvePin);

int AUTOMATIC_MODE = 1; // set arduino to automatic mode first

void setup()
{
  pinMode(tdsSensorPin, INPUT);
  pinMode(turbiditySensorPin, INPUT);
  pinMode(flowSensorPin, INPUT);
  pinMode(pressureSensorPin, INPUT);
  pinMode(inletValvePin, OUTPUT);
  pinMode(manualValvePin, INPUT);

  valve.closeValve();

  // initialize serial for debugging
  Serial.begin(115200);
  // initialize serial for ESP module
  Serial1.begin(115200); // Serial1 is pin 19 (RX) and pin 18 (TX)

  simpleWifi.init(&Serial1, server, port, callback);
  simpleWifi.connectToWifi(ssid, pass);

  pinMode(LED_BUILTIN, OUTPUT);
  mqttLoop.every(1000, mqttConnect);
  controlLoop.every(1500, controlValve);
  sensorLoop.every((PUBLISH_INTERVAL * 5), publishSensorReadings); // give it extra 2 seconds so that function calls do not overlap
}

void mqttConnect()
{
  simpleWifi.mqttUnsubscribe("mendozamartin/feeds/inlet-valve");
  while (!simpleWifi.mqttConnected())
  {
    simpleWifi.connectToMqtt(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY, server);
    boolean valveSubscription = 0;
    valveSubscription = simpleWifi.mqttSubscribe("mendozamartin/feeds/inlet-valve", 1);

    if (AUTOMATIC_MODE == 1)
    { // this will prevent users from operating the valves in the dashboard
      simpleWifi.mqttPublish("mendozamartin/feeds/inlet-valve", "Auto");
    }
    else
    { // this will allow the user to operate the valves in the dashboard
      simpleWifi.mqttPublish("mendozamartin/feeds/inlet-valve", "Manual");
    }

    if (valveSubscription) // check if arduino client is subscribed to valve topic
    {
      Serial.println("MQTT client is subscribed to valve topic");
    }
    else
    {
      Serial.println("MQTT client is not subscribed to valve topic");
    }
  }
  simpleWifi.mqttLoop();
}

void controlValve()
{
  // if (AUTOMATIC_MODE == 1)
  // { // execute logic when system is in automatic mode
  //   if (levelSensor.getReading() >= UPPER_LEVEL_THRESHOLD)
  //   {
  //     valve.closeValve(PRESSURE_CALIBRATION);
  //   }
  //   else if (levelSensor.getReading() <= LOWER_LEVEL_THRESHOLD)
  //   {
  //     valve.openValve(PRESSURE_CALIBRATION);
  //   }
  // }

  if (digitalRead(manualValvePin) == 1)
  {
    valve.closeValve();
  }
  else if (digitalRead(manualValvePin) == 0)
  {
    valve.openValve();
  }
}

void publishSensorReadings()
{
  char msgBuffer[20]; // make sure this is big enough to hold your string
  delay(PUBLISH_INTERVAL);
  float levelSensorValue = levelSensor.getReading(PRESSURE_CALIBRATION);
  simpleWifi.mqttPublish("mendozamartin/feeds/filter-level", dtostrf(levelSensorValue, 6, 2, msgBuffer));
  ///////////////////////////////////////////////////////////////////////////////////////
  delay(PUBLISH_INTERVAL);
  turbiditySensor.startSampling();
  float NTU = turbiditySensor.getReading(); // read the input on analog pin 0:
  simpleWifi.mqttPublish("mendozamartin/feeds/final-tank-turbidity", dtostrf(NTU, 6, 2, msgBuffer));
  //////////////////////////////////////////////////////////////////////////////////////
  delay(PUBLISH_INTERVAL);

  tdsSensor.startSampling();
  float tdsValue = tdsSensor.getReading();
  simpleWifi.mqttPublish("mendozamartin/feeds/final-tank-tds", dtostrf(tdsValue, 6, 2, msgBuffer));
  ///////////////////////////////////////////////////////////////////////////////////////
  delay(PUBLISH_INTERVAL);
  float flowRate = flowSensor.getReading();
  simpleWifi.mqttPublish("mendozamartin/feeds/outlet-flowrate", dtostrf(flowRate, 6, 2, msgBuffer));
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
  Serial.println(message);

  if (message[0] == 'O') // open
  {
    valve.openValve();
  }
  else if (message[0] == 'C') // close
  {
    valve.closeValve();
  }
  else if (message[0] == 'A')
  { //automatic
    AUTOMATIC_MODE = 1;
  }
  else if (message[0] == 'M')
  { // manual
    AUTOMATIC_MODE = 0;
  }
}
