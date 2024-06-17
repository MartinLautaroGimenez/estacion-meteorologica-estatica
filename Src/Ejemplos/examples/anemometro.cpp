#include <Arduino.h>

#define pinAnemometer 5

void interruption();
float velocidad();
long t0,t1 = millis();

void setup(){
    pinMode(pinAnemometer, INPUT);
    attachInterrupt(digitalPinToInterrupt(pinAnemometer), interruption, RISING);
}

void loop(){
    printf("hola, ando verificando el anemómetro!");
    printf("la velocidad es %f.2", velocidad());
    delay(1000);
}

float velocidad(){
    int brazo = 7;
    return ((brazo*2*PI)/(t1-t0));  // se devuelve el resultado de velocidad tangencial
}

// Lectura de tiempos para deltaT
void interruption(){
    t0 = t1;        //  Refresco el valor del tiempo anterior
    t1 = millis();  //  Cargo el nuevo tiempo transcurrido
}