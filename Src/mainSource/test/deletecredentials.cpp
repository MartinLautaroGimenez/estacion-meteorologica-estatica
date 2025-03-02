#include <Arduino.h>
#include <Preferences.h>

#define LED_PIN 2  // Cambia este valor si tu placa utiliza otro pin para el LED

void clearCredentials() {
  Preferences preferences;
  // Abrir el namespace "wifi" en modo escritura
  preferences.begin("wifi", false);
  // Eliminar todas las claves en este namespace
  preferences.clear();
  // Cerrar el namespace
  preferences.end();
  Serial.println("Credenciales eliminadas.");
}

void setup() {
  Serial.begin(115200);
  while (!Serial);  // Espera conexión serial si es necesario
  delay(1000);
  
  Serial.println("Iniciando limpieza de credenciales...");

  // Inicializar y encender el LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // Enciende el LED
  Serial.println("LED encendido.");

  // Eliminar credenciales guardadas
  clearCredentials();

  Serial.println("Proceso completado.");
}

void loop() {
  // El loop se deja vacío o se puede agregar otra lógica
  delay(1000);
}
