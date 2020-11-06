///*
// WiFiEsp example: WebClient
// This sketch connects to google website using an ESP8266 module to
// perform a simple web search.
// For more details see: http://yaab-arduino.blogspot.com/p/wifiesp-example-client.html
//*/
//
#include "WiFiEsp.h"

// Emulate Serial1 on pins 10/11 if not present
//#include "SoftwareSerial.h"
//#include "HardwareSerial.h"
//SoftwareSerial Serial1(50,51); // RX, TX

char ssid[] = "martin06m";            // your network SSID (name)
char pass[] = "mathea06m";        // your network password
const String ADAFRUIT_IO_USERNAME = "mendozanmartin";
const String ADAFRUIT_IO_KEY = "aio_xYpn92R9hsfHwDcEAFckkMD07VpX";
int status = WL_IDLE_STATUS;     // the Wifi radio's status

char server[] = "io.adafruit.com";

// Initialize the Ethernet client object
WiFiEspClient client;

void setup()
{
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

  Serial.println();
  Serial.println("Starting connection to server...");
  // if you get a connection, report back via serial
  if (client.connect(server, 8883)) {
    Serial.println("Connected to server");
    // Make a HTTP request
    String content = "{\"value\": \"6\"}";
    client.println("POST /api/v2/mendozanmartin/feeds/outlet-flowrate/data HTTP/1.1");
    client.println("X-AIO-Key: " + ADAFRUIT_IO_KEY);
    client.println("Host: io.adafruit.com:8883");
    client.println("Accept: */*");
    client.println("Content-Length: " + String(content.length()));
    client.println("Content-Type: application/json");
    client.println();
    client.println(content);
  }
}

void loop()
{
//  // if there are incoming bytes available
//  // from the server, read them and print them
//  while (client.available()) {
//    char c = client.read();
//    Serial.write(c);
//  }
//
//  // if the server's disconnected, stop the client
//  if (!client.connected()) {
//    Serial.println();
//    Serial.println("Disconnecting from server...");
//    client.stop();
//
//    // do nothing forevermore
//    while (true);
//  }
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
