// Define Trig and Echo pin:
#define trigPin 2
#define echoPin 3
// Define variables:
int reading = 0;

void setup() {
  // Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Begin Serial communication at a baudrate of 9600:
  Serial.begin(9600);
}

//MAIN LOOP HERE --------------------------------------------------------------------------------------------
void loop() {
  reading = getDistance();
  Serial.print("Distance: ");
  Serial.print(reading);
  Serial.println("cm");
}

//FUNCTIONS HERE --------------------------------------------------------------------------------------------
int takeMeasurement() { //this function just takes a reading from the sensor
  long duration;
  int distance;
  // Clear the trigPin by setting it LOW:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  
  // Trigger the sensor by setting the trigPin high for 10 microseconds:
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Read the echoPin. pulseIn() returns the duration (length of the pulse) in microseconds:
  duration = pulseIn(echoPin, HIGH);
  delay(100);
  
  // Calculate the distance:
  distance = duration*0.034/2; //distance is in cm
  return distance;
}

int getDistance() {
  int sensorReadings[7];
  double filteredVal = 0;
  int divisor = 0;
  
  for (int i = 0; i <= 6; i++) {
    sensorReadings[i] = takeMeasurement();
    if (sensorReadings[i] != 0) {
      filteredVal = filteredVal + sensorReadings[i];
      divisor++;
    }
  }
  filteredVal = filteredVal/divisor;
  return filteredVal;
}
