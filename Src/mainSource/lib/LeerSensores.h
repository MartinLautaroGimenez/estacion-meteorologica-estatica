#ifndef LEERSENSORES_H
#define LEERSENSORES_H

#include "Arduino.h"
#include "config.h"

#ifdef USE_DHT
  #include <DHT.h>
#endif

#ifdef USE_BMP
  #include <Adafruit_BMP280.h>
#endif

#ifdef USE_BH_SENSOR
  #include <BH1750.h>
#endif

class LeerSensores {
  public:
    LeerSensores();
    void begin();
    void leerTodos();

    // Variables para almacenar las lecturas
    float tempDHT = 0;
    float humDHT = 0;
    int sensDHT = 0;

    float tempBMP = 0;
    float presBMP = 0;
    float altBMP = 0;
    float presNMBMP = 0;

    float bh1750 = 0;
    float ppmCO2 = 0;
    int lluviaPosibilidad = 0;
    float vientoVel = 0;
    int vientoDir = 0;

    int nivelBateria = 0;
  private:
#ifdef USE_DHT
    DHT dht;
#endif

#ifdef USE_BMP
    Adafruit_BMP280 bmp;
#endif

#ifdef USE_BH_SENSOR
    BH1750 lightMeter;
#endif
};

#endif
