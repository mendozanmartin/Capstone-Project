#define signalPin A0
float VREF = 5.0; // analog reference
int pulseSignal = 0;
int pulseCount;
unsigned long time1 = 0;

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  pinMode(signalPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  pulseSignal = digitalRead(signalPin);
  if (pulseSignal == 1) {
    pulseCount++;
    Serial.println("pulsed");
  }
  
}
