#include <Arduino.h>
const int sensor = 22;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(sensor, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int value = digitalRead(sensor);
  if (value == 0){
    Serial.println("Hay shuvia");
  }
  if (value == 1){
    Serial.println("No hay shuvia");
  }
  delay(2000);
}
