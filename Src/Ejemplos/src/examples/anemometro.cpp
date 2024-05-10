#include <Arduino.h>

#define pinAnemometer 5

void interruption();

void setup(){
    pinMode(pinAnemometer, INPUT_PULLDOWN);
    attachInterrupt(pinAnemometer, interruption, RISING);
}

void loop(){
    
}

void interruption(){
    
}