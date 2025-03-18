#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  lightMeter.begin();
}

void loop() {
  float luz = lightMeter.readLightLevel();
  Serial.print("Nivel de luz: ");
  Serial.print(luz);
  Serial.println(" lx");
  delay(1000);
}

// //Conexiones
//     Conecta el pin VCC del sensor BH1750 a 3.3V del ESP32.
//     Conecta el pin GND del sensor BH1750 a GND del ESP32.
//     Conecta el pin SCL del sensor BH1750 a un pin GPIO del ESP32, por ejemplo, pin 22.
//     Conecta el pin SDA del sensor BH1750 a otro pin GPIO del ESP32, por ejemplo, pin 21.