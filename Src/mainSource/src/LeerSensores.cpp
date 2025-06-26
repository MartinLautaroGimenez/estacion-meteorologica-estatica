#include "Arduino.h"
#include "../lib/LeerSensores.h"

#ifdef USE_DHT
  #include <DHT.h>
  #define DHTTYPE DHT22
#endif

#ifdef USE_BH_SENSOR
  #include <BH1750.h>
#endif

#ifdef USE_ANEMOMETER
  volatile unsigned long anemometerPulseCount = 0;

  void IRAM_ATTR anemometerISR() {
    anemometerPulseCount++;
  }
#endif

// Constructor
LeerSensores::LeerSensores()
#ifdef USE_DHT
  : dht(DHT_PIN, DHTTYPE)
#endif
{
  tempDHT = humDHT = sensDHT = 0;
  tempBMP = presBMP = altBMP = presNMBMP = -1;
  bh1750 = 0;
  ppmCO2 = 0;
  lluviaPosibilidad = 0;
  vientoVel = 0;
  vientoDir = 0;
}

void LeerSensores::begin() {
  Serial.println("---- Estado de Sensores ----");

#if defined(USE_BMP680)
  Serial.println("BMP680: ACTIVADO");
  if (!bmp680.begin()) {
    Serial.println("❌ Error al iniciar BMP680!");
  } else {
    Serial.println("✅ BMP680 listo");
  }

#elif defined(USE_BMP280)
  Serial.println("BMP280: ACTIVADO");
  if (!bmp280.begin(0x76)) {
    Serial.println("❌ Error al iniciar BMP280!");
  } else {
    Serial.println("✅ BMP280 listo");
  }
#else
  Serial.println("BMPs: DESACTIVADO");
#endif

#ifdef USE_DHT
  Serial.println("DHT: ACTIVADO");
  dht.begin();
#else
  Serial.println("DHT: DESACTIVADO");
#endif

#ifdef USE_BH_SENSOR
  Serial.println("BH1750: ACTIVADO");
  if (!lightMeter.begin()) {
    Serial.println("❌ Error al iniciar BH1750!");
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

#if defined(USE_BMP680)

  if (bmp680.performReading()) {
    tempDHT = bmp680.temperature;
    humDHT = bmp680.humidity;
    sensDHT = 0;

    tempBMP = bmp680.temperature;
    presBMP = bmp680.pressure / 100.0;
    altBMP = bmp680.readAltitude(1013.25);
    presNMBMP = presBMP / pow(1.0 - (750.0 / 44330.0), 5.255);

    ppmCO2 = bmp680.gas_resistance / 1000.0;  // Valor estimado
  } else {
    Serial.println("❌ Falló la lectura del BMP680");
    tempDHT = humDHT = sensDHT = -1;
    tempBMP = presBMP = altBMP = presNMBMP = -1;
    ppmCO2 = -1;
  }

#else

  #ifdef USE_DHT
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    float st = dht.computeHeatIndex(t, h, false);
    if (isnan(t) || isnan(h)) {
      Serial.println("Error leyendo DHT!");
      tempDHT = humDHT = -100;
      sensDHT = 0;
    } else {
      tempDHT = t;
      humDHT = h;
      sensDHT = st;
    }
  #else
    tempDHT = humDHT = -1;
    sensDHT = 0;
  #endif

  #ifdef USE_BMP280
    tempBMP = bmp280.readTemperature();
    presBMP = bmp280.readPressure() / 100.0;
    altBMP = bmp280.readAltitude(1013.25);
    presNMBMP = presBMP / pow(1.0 - (750.0 / 44330.0), 5.255);
  #else
    tempBMP = presBMP = altBMP = presNMBMP = -1;
  #endif

  #ifdef USE_MQ_SENSOR
    int mqValue = analogRead(PIN_MQ135);
    ppmCO2 = (mqValue / 4095.0) * 5000.0;
  #else
    ppmCO2 = -1;
  #endif

#endif // USE_BMP680

  #ifdef USE_BH_SENSOR
    bh1750 = lightMeter.readLightLevel();
  #else
    bh1750 = -1;
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

  #ifdef pantallita
    serial.print("Temperatura DHT: ");
  #endif
}
