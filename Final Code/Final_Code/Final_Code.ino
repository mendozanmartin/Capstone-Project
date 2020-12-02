
#include "Configuration.h"
#include "SimpleWifi.h"
#include "TdsSensor.h"
#include "LevelSensor.h"
#include "TurbidityLib.h"

#include <Timer.h>

#define pressureSensorPin A0
#define tdsSensorPin A1
#define turbiditySensorPin A2
#define flowSensorPin A3

#define VREF 5.0  // analog reference voltage(Volt) of the ADC
#define SCOUNT 30 // sum of sample point

char ssid[] = WIFI_SSID;     // your network SSID (name)
char pass[] = WIFI_PASSWORD; // your network password
char adafruitUsername[] = ADAFRUIT_IO_USERNAME;
char adafruitKey[] = ADAFRUIT_IO_KEY;
char server[] = MQTT_SERVER;
int port = MQTT_PORT;

// int status = WL_IDLE_STATUS; // the Wifi radio's status

bool pulseSignal, lastPulseSignal = false;
float tdsValue = 0;
float levelSensorValue = 0;

int pulseCount = 0;
float flowRate = 0;
int countArray[5] = {0, 0, 0, 0, 0};
unsigned long delta_t, final_t;
unsigned long initial_t = 0;

void callback(char *topic, byte *payload, unsigned int length);
void reconnect();

int j = 0;

Timer mqttLoop;
Timer sensorLoop;
int PUBLISH_INTERVAL = 2000;
SimpleWifi simpleWifi;

//Sensors Declaration
TdsSensor tdsSensor(tdsSensorPin);
LevelSensor levelSensor(pressureSensorPin);
TurbidityLib turbiditySensor(turbiditySensor);

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
  sensorLoop.every((PUBLISH_INTERVAL * 4 + 3), sensorPublish); // give it extra 2 seconds so that function calls do not overlap
}

void mqttConnect()
{

  while (!simpleWifi.mqttConnected())
  {
    simpleWifi.connectToMqtt(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY, server);

    boolean isSubscribed = simpleWifi.mqttSubscribe("mendozamartin/feeds/outlet-valve", 1);
    boolean isSubscribed2 = simpleWifi.mqttSubscribe("mendozamartin/feeds/inlet-valve", 1);

    if (isSubscribed)
    {
      Serial.println("MQTT client is subscribed to topic");
    }
    else
    {
      Serial.println("MQTT client is not subscribed to topic");
    }
  }

  simpleWifi.mqttLoop();
}

int getMedianNum(int bArray[], int iFilterLen)
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

void sensorPublish()
{
  char msgBuffer[10]; // make sure this is big enough to hold your string

  levelSensorValue = levelSensor.getReading();
  simpleWifi.mqttPublish("mendozamartin/feeds/collection-tank-level", dtostrf(levelSensorValue, 6, 2, msgBuffer));

  /////////////////////////////////////////////////////////////////////////////////////////
  delay(PUBLISH_INTERVAL);
  turbiditySensor.startSampling();
  float NTU = turbiditySensor.getReading(); // read the input on analog pin 0:
  simpleWifi.mqttPublish("mendozamartin/feeds/collection-tank-turbidity", dtostrf(NTU, 6, 2, msgBuffer));

  ////////////////////////////////////////////////////////////////////////////////////////
  delay(PUBLISH_INTERVAL);

  tdsSensor.startSampling();
  tdsValue = tdsSensor.getReading();
  simpleWifi.mqttPublish("mendozamartin/feeds/collection-tank-tds", dtostrf(tdsValue, 6, 2, msgBuffer));

  /////////////////////////////////////////////////////////////////////////////////////////
  delay(PUBLISH_INTERVAL);
  int pulseCount = 0;
  Serial.println("This executed");
  for (int i = 0; i < 4; i++)
  {
    pulseCount += countArray[i];
  }
  Serial.println("Pulse Count: " + String(pulseCount));
  Serial.println("Flow Rate delta t: " + String(10000) + "ms"); // 10 elements in array, each containing 2 seconds of pulse data
  flowRate = (pulseCount / 425.0) / (10000 * pow(10, -3) / 60.0);
  Serial.println("Flow Rate: " + String(flowRate) + " L/min");

  simpleWifi.mqttPublish("mendozamartin/feeds/inlet-flowrate", dtostrf(flowRate, 6, 2, msgBuffer));
}

void loop()
{
  mqttLoop.update();
  sensorLoop.update();

  pulseSignal = digitalRead(flowSensorPin);
  if (pulseSignal != lastPulseSignal && pulseSignal == true)
  {
    pulseCount++;
  }
  lastPulseSignal = pulseSignal;

  final_t = millis();
  delta_t = final_t - initial_t;

  if (delta_t >= 2000)
  {

    countArray[j] = pulseCount;
    Serial.println(countArray[j]);
    pulseCount = 0;

    if (j == 4)
    {
      j = 0;
    }

    j++;

    initial_t = final_t;
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");

  char message[length];
  for (int i = 0; i < length; i++)
  {
    message[i] = payload[i];
  }
  Serial.println(message[1]);
  if (message[1] == 'N')
  {
    Serial.println(message);
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else if (message[1] == 'F')
  {
    Serial.println(message);
    digitalWrite(LED_BUILTIN, LOW);
  }
}
