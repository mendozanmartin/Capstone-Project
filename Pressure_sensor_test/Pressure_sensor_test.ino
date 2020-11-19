#define pressureSensorPin A0
int pressureSensorValue = 0;
float pressureSensorVoltage = 0;

//pressure sensor starts arount 0.5V and goes up by 0.01V about every 1cm of water height

void setup() {
  Serial.begin(115200); // initialize serial communication
}

void loop() {
  pressureSensorValue = analogRead(pressureSensorPin);            // read the input on sensor pin
  pressureSensorVoltage = pressureSensorValue * (5.0 / 1023.0);         // Convert the analog reading (which goes from 0 - 1023) to a pressureSensorVoltage (0 - 5V)
  Serial.println(pressureSensorVoltage);                         // print out the value 
  delay(200);
}
