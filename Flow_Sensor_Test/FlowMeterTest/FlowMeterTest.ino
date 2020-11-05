int signalPin = 7;
float VREF = 5.0; // analog reference
int pulseSignal = 0;
int pulseCount;
int volume;
unsigned long time1 = millis();
unsigned long time2 = 0;
float flowRate = 0;

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
    time2 = time1 - time2;
    Serial.println("pulsed");
  }
  if (pulseCount == 450) {
    volume = 1;
    Serial.println(millis());
    Serial.println(time1);
    Serial.println(time2);
    flowRate = volume / (time2 * pow(10, -3) / 60);
    pulseCount = 0;
    volume = 0;
    Serial.println("Flow Rate: " + String(flowRate) + " L/s");
    }
}
