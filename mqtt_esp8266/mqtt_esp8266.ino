///*
// WiFiEsp example: WebClient
// This sketch connects to google website using an ESP8266 module to
// perform a simple web search.
// For more details see: http://yaab-arduino.blogspot.com/p/wifiesp-example-client.html
//*/
//s

#define TdsSensorPin A1
#define VREF 5.0 // analog reference voltage(Volt) of the ADC
#define SCOUNT 30 // sum of sample point
int analogBuffer[SCOUNT]; // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0, copyIndex = 0;
float averageVoltage = 0, tdsValue = 0, temperature = 25;

#define pressureSensorPin A0
int pressureSensorValue = 0;
float pressureSensorVoltage = 0;

#include "WiFiEsp.h"
//#include "SAM32WiFiEsp.h"


#include <PubSubClient.h>


char ssid[] = "Martin!";            // your network SSID (name)
char pass[] = "martinmendoza";        // your network password
char ADAFRUIT_IO_USERNAME[] = "mendozanmartin";
char ADAFRUIT_IO_KEY[] = "aio_HpFX47bwUsrNTWi7Ba2JgnRYFWhe";
int status = WL_IDLE_STATUS;     // the Wifi radio's status

//char server[] = "io.adafruit.com";
char server[] = "broker.hivemq.com";
int port = 1883;

void callback(char* topic, byte* payload, unsigned int length);


// Initialize the Ethernet client object
WiFiEspClient client;
PubSubClient mqttClient(server, port, callback,client);
void setup()
{
  pinMode(TdsSensorPin, INPUT);

  // initialize serial for debugging
  Serial.begin(115200);
  // initialize serial for ESP module
  Serial1.begin(115200);
  // initialize ESP module
  WiFi.init(&Serial1);
    
  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data
  Serial.println("You're connected to the network");

  printWifiStatus();

  mqttClient.setBufferSize(1024);

//  mqttClient.setServer(server, port);
//  mqttClient.setCallback(callback);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  if (!mqttClient.connected()) {
    reconnect();
  }

  mqttClient.loop();
  delay(1000);

   static unsigned long analogSampleTimepoint = millis();
  if (millis() - analogSampleTimepoint > 40U) //every 40 milliseconds,read the analog value from the ADC
  {
    analogSampleTimepoint = millis();
    analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin); //read the analog value and store into the buffer
    analogBufferIndex++;
    if (analogBufferIndex == SCOUNT)
      analogBufferIndex = 0;
  }
  static unsigned long tdsTimepoint = millis();
  static unsigned long pressureTimepoint = millis();

  char msgBuffer[20];           // make sure this is big enough to hold your string    

  if (millis() - tdsTimepoint > 5000U) {
    tdsTimepoint = millis();
    for (copyIndex = 0; copyIndex < SCOUNT; copyIndex++)
      analogBufferTemp[copyIndex] = analogBuffer[copyIndex];
    averageVoltage = getMedianNum(analogBufferTemp, SCOUNT) * (float) VREF / 1024.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
    float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0); //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
    float compensationVoltage = averageVoltage / compensationCoefficient; //temperature compensation
    tdsValue = (133.42 * compensationVoltage * compensationVoltage * compensationVoltage - 255.86 * compensationVoltage * compensationVoltage + 857.39 * compensationVoltage) * 0.5; //convert voltage value to tds value
    //Serial.print("voltage:");
    //Serial.print(averageVoltage,2);
    //Serial.print("V ");
    mqttClient.publish("TDS-collection", dtostrf(tdsValue, 6, 2, msgBuffer));
    Serial.print("TDS Value:");
    Serial.print(tdsValue, 0);
    Serial.println("ppm");
  } 
  
  if (millis() - pressureTimepoint > 7000U) {
    pressureTimepoint = millis();

    pressureSensorValue = analogRead(pressureSensorPin);            // read the input on sensor pin
    pressureSensorVoltage = pressureSensorValue * (5.0 / 1023.0);         // Convert the analog reading (which goes from 0 - 1023) to a pressureSensorVoltage (0 - 5V)
    mqttClient.publish("Pressure-collection", dtostrf(pressureSensorVoltage, 6, 2, msgBuffer));
    Serial.print("Pressure Sensor Value:");
    Serial.print(pressureSensorVoltage, 0);
    Serial.println("V");  
  }
  
}


void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");

}

void connectToMQTT() {
  String clientId = "ESP8266Client-";
  clientId += String(random(0xffff), HEX);
//  if(mqttClient.connect(clientId.c_str(), ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)) {
  if (mqttClient.connect(clientId.c_str())) {
    Serial.print("Connected to: ");
    Serial.println(server);
  } else {
    Serial.print("failed, rc=");
    Serial.print(mqttClient.state());
    Serial.println(" try again in 2 seconds");
    delay(2000);
  }

//  boolean isSubscribed = mqttClient.subscribe("mendozanmartin/feeds/outlet-valve", 1);
  boolean isSubscribed = mqttClient.subscribe("outlet-valve", 1);
  boolean isSubscribed2 = mqttClient.subscribe("inlet-valve", 1);

  mqttClient.publish("TDS-collection", "40");
  if (isSubscribed) {
    Serial.println("MQTT client is subscribed to topic");
  } else {
    Serial.println("MQTT client is not subscribed to topic");
  }
}

void reconnect() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    connectToMQTT();
  }
//  return mqttClient.connected();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");

  char message[length];
  for(int i = 0; i < length; i++)
  {
    message[i] = payload[i];
  }
  Serial.println(message[1]);  
 if (message[1] == 'N') {
  Serial.println(message);
  digitalWrite(LED_BUILTIN, HIGH);
 } else if (message[1] == 'F') {
  Serial.println(message);
  digitalWrite(LED_BUILTIN, LOW);
  }

}

int getMedianNum(int bArray[], int iFilterLen) {
  int bTab[iFilterLen];
  for (byte i = 0; i < iFilterLen; i++)
    bTab[i] = bArray[i];
  int i, j, bTemp;
  for (j = 0; j < iFilterLen - 1; j++) {
    for (i = 0; i < iFilterLen - j - 1; i++) {
      if (bTab[i] > bTab[i + 1]) {
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
