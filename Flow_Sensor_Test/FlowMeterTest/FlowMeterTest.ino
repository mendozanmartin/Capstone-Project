int signalPin = 7;
float VREF = 5.0; // analog reference
bool pulseSignal = false;
int pulseCount = 0;
float flowRate = 0;
unsigned long delta_t = 0;
unsigned long initial_t = 0;
unsigned long final_t;

void setup() {
  Serial.begin(115200);
  // put your setup code here to run once:
  pinMode(signalPin, INPUT);
}

void loop() {
  // put your main code here to run repeatedly:
  
  pulseSignal = digitalRead(signalPin);
  if (pulseSignal == true) {
    pulseCount++;
  }

  final_t = millis();
  delta_t = final_t - initial_t;

  if (delta_t >= 1000){
  Serial.println("Pulse Count: " + String(pulseCount));
  Serial.println("Flow Rate delta t: " + String(delta_t) + "ms");
  flowRate = (pulseCount/450) / (delta_t * pow(10, -3) / 60);
  Serial.println("Flow Rate: " + String(flowRate) + " L/min");
  pulseCount = 0;
  initial_t = final_t;
  }
}
