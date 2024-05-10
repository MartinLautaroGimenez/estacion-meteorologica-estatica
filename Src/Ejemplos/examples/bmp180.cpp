#include <Wire.h>
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(115200);

  if (!bmp.begin()) {
    Serial.println("No se pudo encontrar el sensor BMP180. Conecta correctamente o revisa las conexiones.");
    while (true);
  }
}

void loop() {
  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0; // Convertir a hectopascales (hPa)

  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Presión: ");
  Serial.print(pressure);
  Serial.println(" hPa");

  delay(1000);
}

// //Conexiones
//     Conecta el pin VCC del sensor BH1750 a 3.3V del ESP32.
//     Conecta el pin GND del sensor BH1750 a GND del ESP32.
//     Conecta el pin SCL del sensor BH1750 a un pin GPIO del ESP32, por ejemplo, pin 22.
//     Conecta el pin SDA del sensor BH1750 a otro pin GPIO del ESP32, por ejemplo, pin 21.
