#include "Arduino.h"
#include "../lib/LeerSensores.h"
#include "config.h"
#include <DHT.h>
#define DHTTYPE DHT22  // Usamos DHT22 para DHT22
#include <Adafruit_BMP280.h>
#ifdef USE_BH_SENSOR
  #include <BH1750.h>
#endif

#ifdef USE_ANEMOMETER
volatile unsigned long anemometerPulseCount = 0;
#endif

#ifdef USE_ANEMOMETER
void IRAM_ATTR anemometerISR() {
  anemometerPulseCount++;
}
#endif

// Constructor: usamos el constructor por defecto para DHT
LeerSensores::LeerSensores() : dht(DHT_PIN, DHTTYPE) {
  tempDHT = 0;
  humDHT = 0;
  sensDHT = 0;
  tempBMP = 0;
  presBMP = 0;
  altBMP = 0;
  presNMBMP = 0;
  bh1750 = 0;
  ppmCO2 = 0;
  lluviaPosibilidad = 0;
  vientoVel = 0;
  vientoDir = 0;
}

void LeerSensores::begin() {
  // Configuramos el DHT con su pin y tipo usando setup()
  DHT dht(DHT_PIN, DHTTYPE);

  // Iniciar BMP280 (prueba con 0x76 o 0x77)
  if (!bmp.begin(0x76)) {
    Serial.println("Error al iniciar BMP280!");
  }

#ifdef USE_BH_SENSOR
  // Iniciar BH1750 sin parámetros
  if (!lightMeter.begin()) {
    Serial.println("Error al iniciar BH1750!");
  }
#endif

#ifdef USE_ANEMOMETER
  pinMode(PIN_ANEMOMETER, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_ANEMOMETER), anemometerISR, FALLING);
#endif

#ifdef USE_HOJA_MOJADA
  pinMode(HOJA_MOJADA_PIN, INPUT);
#endif
}

void LeerSensores::leerTodos() {
  // --- DHT: Temperatura y Humedad ---
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if (isnan(t) || isnan(h)) {
    Serial.println("Error leyendo DHT!");
    tempDHT = -100;
    humDHT = -100;
    sensDHT = 0;
  } else {
    tempDHT = t;
    humDHT = h;
    sensDHT = 1;
  }

  // --- BMP280: Temperatura, Presión y Altitud ---
  tempBMP = bmp.readTemperature();
  presBMP = bmp.readPressure() / 100.0;
  altBMP = bmp.readAltitude(1013.25);
  presNMBMP = presBMP - 1;

#ifdef USE_BH_SENSOR
  // --- BH1750: Nivel de luz (lux) ---
  bh1750 = lightMeter.readLightLevel();
#else
  bh1750 = -1;
#endif

#ifdef USE_MQ_SENSOR
  // --- MQ135: CO2 (valor analógico a ppm) ---
  int mqValue = analogRead(PIN_MQ135);
  ppmCO2 = (mqValue / 4095.0) * 5000.0;
#else
  ppmCO2 = -1;
#endif

#ifdef USE_ANEMOMETER
  noInterrupts();
  unsigned long pulseCount = anemometerPulseCount;
  anemometerPulseCount = 0;
  interrupts();
  float windSpeedKmh = (pulseCount * 2.4);
  vientoVel = windSpeedKmh / 3.6;
#else
  vientoVel = -1;
#endif

#ifdef USE_VELETA
  int analogVal = analogRead(VELETA_Y);
  vientoDir = map(analogVal, 0, 4095, 0, 360);
#else
  vientoDir = -1;
#endif

#ifdef USE_HOJA_MOJADA
  lluviaPosibilidad = digitalRead(HOJA_MOJADA_PIN);
#else
  lluviaPosibilidad = 0;
#endif
}
