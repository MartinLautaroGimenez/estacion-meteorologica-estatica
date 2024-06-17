#include <Arduino.h>

int pines[8] = {
    9       // pinNorte
    , 8     // pinNorEste
    , 7     // pinNorOeste
    , 6     // pinOeste
    , 5     // pinEste
    , 4     // pinSur
    , 3     // pinSurOeste
    , 2     // pinSurEste
};

boolean direction[8];
char* ultimaPos = "Narnia";

void setup(){    
    Serial.begin(9600);
    for (size_t i = 0; i < sizeof(direction); i++){
        pinMode(pines[i], INPUT);
    }
}

void loop(){
    for (size_t i = 0; i < sizeof(direction); i++){
        direction[i] = digitalRead(pines[i]);
        if (direction[i] == 1){
            // switch-case para cargar las últimas lecturas
            switch (i){
            case 0:
                ultimaPos = "Norte";
                break;        
            case 1:
                ultimaPos = "NorEste";
                break;        
            case 2:
                ultimaPos = "NorOeste";
                break;        
            case 3:
                ultimaPos = "Oeste";
                break;        
            case 4:
                ultimaPos = "Este";
                break;        
            case 5:
                ultimaPos = "Sur";
                break;        
            case 6:
                ultimaPos = "SurOeste";
                break;            
            case 7:
                ultimaPos = "SurEste";
                break;        
            default: // en default notificar que se debe de ver la última posición
                Serial.println("revisar el último valor registrado!");
                break;
            }
        }
    }
    delay(200);    
}