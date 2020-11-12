///*
// WiFiEsp example: WebClient
// This sketch connects to google website using an ESP8266 module to
// perform a simple web search.
// For more details see: http://yaab-arduino.blogspot.com/p/wifiesp-example-client.html
//*/
//
#include "WiFiEsp.h"
#include <PubSubClient.h>


char ssid[] = "Martin!";            // your network SSID (name)
char pass[] = "martinmendoza";        // your network password
char ADAFRUIT_IO_USERNAME[] = "mendozanmartin";
char ADAFRUIT_IO_KEY[] = "aio_wwQH757UghxuMx3TXIfjEUICj0DU";
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
  // initialize serial for debugging
  Serial.begin(9600);
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
//  mqttClient.setServer(server, port);
//  mqttClient.setCallback(callback);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  if (!mqttClient.connected()) {
    reconnect();
  }

  delay(1000);
  mqttClient.loop();
  
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
  boolean isSubscribed = mqttClient.subscribe("hello", 1);

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

  char message[20];
  for(int i = 0; i < length; i++)
  {
    message[i] = payload[i];
  }
  Serial.println(message[1]);  
 if (message[1] == 'N') {
  Serial.println("LED ON");
  digitalWrite(LED_BUILTIN, HIGH);
 } else if (message[1] == 'F') {
  Serial.println("LED OFF");
  digitalWrite(LED_BUILTIN, LOW);
  }

}
