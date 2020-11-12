#include <MsTimer2.h>

int signalPin = 7;
float VREF = 5.0; // analog reference
int pulseSignal = 0;
int pulseCount;
float flowRate = 0;
unsigned long delta_t;
unsigned long initial_t = 0;
unsigned long final_t;

void setup() {
  Serial.begin(115200);
  // put your setup code here to run once:
  pinMode(signalPin, INPUT);
    MsTimer2::set(5000, flow_Rate);
    MsTimer2::start();
    Serial.println("Timer Initialized");
}

void loop() {
  // put your main code here to run repeatedly:
  
  pulseSignal = digitalRead(signalPin);
  if (pulseSignal == 1) {
    pulseCount++;
    Serial.println("pulsed");
  }
}

void flow_Rate() {
  Serial.println("Hello, World!");
  final_t = millis();
  delta_t = final_t - initial_t;
  Serial.println(millis());
  Serial.println(String(final_t));
  flowRate = (pulseCount/450) / (delta_t * pow(10, -3) / 60);
  Serial.println("Flow Rate: " + String(flowRate) + " L/min");
  pulseCount = 0;
  initial_t = final_t;
  }
