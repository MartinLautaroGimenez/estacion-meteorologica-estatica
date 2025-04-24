#include "Arduino.h"
#include "../lib/LeerSensores.h"

#ifdef USE_DHT
  #include <DHT.h>
  #define DHTTYPE DHT22  // Usamos DHT22
#endif

#ifdef USE_BMP
  #include <Adafruit_BMP280.h>
#endif

#ifdef USE_BATERRY_SENSOR
  #include <Adafruit_BMP280.h>
#endif

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

// Constructor: Inicializa dht solo si USE_DHT está definido
LeerSensores::LeerSensores()
#ifdef USE_DHT
  : dht(DHT_PIN, DHTTYPE)
#endif
{
  tempDHT = 0;
  humDHT = 0;
  sensDHT = 0;
#ifdef USE_BMP
  tempBMP = 0;
  presBMP = 0;
  altBMP = 0;
  presNMBMP = 0;
#else
  tempBMP = -1;
  presBMP = -1;
  altBMP = -1;
  presNMBMP = -1;
#endif
  bh1750 = 0;
  ppmCO2 = 0;
  lluviaPosibilidad = 0;
  vientoVel = 0;
  vientoDir = 0;
}

void LeerSensores::begin() {
  Serial.println("---- Estado de Sensores ----");

#ifdef USE_DHT
  Serial.println("DHT: ACTIVADO");
  dht.begin(); // Inicializamos el DHT
#else
  Serial.println("DHT: DESACTIVADO");
#endif

#ifdef USE_BMP
  Serial.println("BMP280: ACTIVADO");
  if (!bmp.begin(0x76)) {
    Serial.println("Error al iniciar BMP280!");
  }
#else
  Serial.println("BMP280: DESACTIVADO");
#endif

#ifdef USE_BH_SENSOR
  Serial.println("BH1750: ACTIVADO");
  if (!lightMeter.begin()) {
    Serial.println("Error al iniciar BH1750!");
  }
#else
  Serial.println("BH1750: DESACTIVADO");
#endif

#ifdef USE_ANEMOMETER
  Serial.println("Anemómetro: ACTIVADO");
  pinMode(PIN_ANEMOMETER, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_ANEMOMETER), anemometerISR, FALLING);
#else
  Serial.println("Anemómetro: DESACTIVADO");
#endif

#ifdef USE_HOJA_MOJADA
  Serial.println("Hoja Mojada: ACTIVADO");
  pinMode(HOJA_MOJADA_PIN, INPUT);
#else
  Serial.println("Hoja Mojada: DESACTIVADO");
#endif

#ifdef USE_MQ_SENSOR
  Serial.println("MQ Sensor (MQ135): ACTIVADO");
#else
  Serial.println("MQ Sensor (MQ135): DESACTIVADO");
#endif

#ifdef USE_VELETA
  Serial.println("Velocímetro (VELETA): ACTIVADO");
#else
  Serial.println("Velocímetro (VELETA): DESACTIVADO");
#endif

  Serial.println("---- Fin Estado de Sensores ----");
}

void LeerSensores::leerTodos() {
#ifdef USE_DHT
  // --- DHT: Temperatura y Humedad ---
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  float st = dht.computeHeatIndex(t, h, false);
  if (isnan(t) || isnan(h)) {
    Serial.println("Error leyendo DHT!");
    tempDHT = -100;
    humDHT = -100;
    sensDHT = 0;
  } else {
    tempDHT = t;
    humDHT = h;
    sensDHT = st;
  }
#else
  tempDHT = -1;
  humDHT = -1;
  sensDHT = 0;
#endif

#ifdef USE_BMP
  // --- BMP280: Temperatura, Presión y Altitud ---
  tempBMP = bmp.readTemperature();
  presBMP = bmp.readPressure() / 100.0;
  altBMP = bmp.readAltitude(1013.25);
  float presNMBMP = presBMP / pow(1.0 - (750.0 / 44330.0), 5.255);

#else
  tempBMP = -1;
  presBMP = -1;
  altBMP = -1;
  presNMBMP = -1;
#endif

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
