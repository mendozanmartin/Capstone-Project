#define sensorPin A0
int sensorValue = 0;
float voltage = 0;

//pressure sensor starts arount 0.5V and goes up by 0.01V about every 1cm of water height

void setup() {
  Serial.begin(9600); // initialize serial communication
}

void loop() {
  sensorValue = analogRead(sensorPin);            // read the input on sensor pin
  voltage = sensorValue * (5.0 / 1023.0);         // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V)
  Serial.println(voltage);                         // print out the value 
  delay(200);
}
