int signalPin = 7;
float VREF = 5.0; // analog reference
bool pulseSignal, lastPulseSignal = false;
int pulseCount = 0;
float flowRate = 0;
int countArray[10];
unsigned long delta_t, final_t;
unsigned long initial_t = 0;

void setup() {
  Serial.begin(115200);
  // put your setup code here to run once:
  pinMode(signalPin, INPUT);
}

void loop() {
  // put your main code here to run repeatedly:
  
  pulseSignal = digitalRead(signalPin);
  if (pulseSignal != lastPulseSignal && pulseSignal == true) {
    pulseCount++;
  }
  lastPulseSignal = pulseSignal;

  final_t = millis();
  delta_t = final_t - initial_t;

  if (delta_t >= 2000){
  //Serial.println("Pulse Count: " + String(pulseCount));
  //Serial.println("Flow Rate delta t: " + String(delta_t) + "ms");
  //flowRate = (pulseCount/425.0) / (delta_t * pow(10, -3) / 60.0);
  //Serial.println("Flow Rate: " + String(flowRate) + " L/min");
  
  if (j == 10)
    j = 0;
    
  j++;
  countArray[j] = pulseCount;
  pulseCount = 0;
  
  //initial_t = final_t;
  }
}
