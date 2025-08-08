#ifndef PANTALLACONTROLLER_H
#define PANTALLACONTROLLER_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "../lib/LeerSensores.h"

class PantallaController {
  public:
    PantallaController(uint8_t direccion = 0x27);

    void begin();
    void mostrarInicio();
    void mostrarLecturas(float temp, float hum, float pres);
    void mostrarEstadoWiFi(const String& ssid, const String& ip);
    void mostrarError(const String& mensaje);
    void mostrarPantallaCiclo(uint8_t indice, const LeerSensores& sensores);

  private:
    LiquidCrystal_I2C lcd;
};

#endif
