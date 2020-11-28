void setup() {
  Serial.begin(115200); //Baud rate: 115200
}
void loop() {
  int sensorValue = analogRead(A0);// read the input on analog pin 0:
  float voltage = sensorValue * (5.0 / 1024.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  Serial.println("Voltage: " + String(voltage) + "V");

  float NTU = (-1120.4 * pow(voltage, 2)) + (5742.3*voltage) - 4352.9;
  Serial.println("NTU: " + String(NTU));
  delay(500);
}
