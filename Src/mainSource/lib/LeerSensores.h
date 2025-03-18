#ifndef LEER_SENSORES_H
#define LEER_SENSORES_H

#include "Arduino.h"
#include "config.h"

// Librerías reales para los sensores
#include <DHT.h>
#include <Adafruit_BMP280.h>
#ifdef USE_BH_SENSOR
  #include <BH1750.h>
#endif

class LeerSensores {
public:
    // Variables públicas para almacenar lecturas
    float tempDHT;
    float humDHT;
    float sensDHT;     // 1: lectura exitosa, 0: error
    float tempBMP;
    float presBMP;
    float altBMP;
    float presNMBMP;
    float bh1750;
    float ppmCO2;
    int   lluviaPosibilidad;
    float vientoVel;
    int   vientoDir;

    // Constructor
    LeerSensores();

    // Inicialización de sensores
    void begin();

    // Lectura de todos los sensores
    void leerTodos();

private:
    // Objetos de sensores
    DHT dht;  // Se usará DHT22 definido por la librería
    Adafruit_BMP280 bmp;
#ifdef USE_BH_SENSOR
    BH1750 lightMeter;
#endif
};

#endif // LEER_SENSORES_H
