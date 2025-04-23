#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  // Espera a que se inicie el serial
  delay(1000);

  // Obtiene y muestra la MAC
  Serial.print("Dirección MAC del ESP32: ");
  Serial.println(WiFi.macAddress());
}

void loop() {
  // No hace falta repetir esto
}
