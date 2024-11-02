#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define ssid "ETEC" // Reemplaza con tu SSID
#define pass "ETec2024*" // Reemplaza con tu contraseña Wi-Fi

char mensaje[512]; // Aumenté el tamaño del buffer para el mensaje

// Declaración de funciones
void setupWifi();
void enviarDatosFicticios();

void setup() {
  Serial.begin(115200);
  setupWifi();
}

void loop() {
  enviarDatosFicticios();
  delay(60000); // Espera 60 segundos antes de enviar nuevamente
}

void setupWifi() {
  delay(100);
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }

  Serial.println("\nConectado a la red: " + String(ssid));
  Serial.print("Con la siguiente IP: ");
  Serial.println(WiFi.localIP());
}

void enviarDatosFicticios() {
  HTTPClient http;
  http.begin("https://emetec.wetec.um.edu.ar/emeapi");
  http.addHeader("Content-Type", "application/json");

  // Asignar valores ficticios
  float temperaturaDHT = 25.0; // Valor ficticio
  float humedadRelativa = 50.0; // Valor ficticio
  float sensacionTermica = 24.5; // Valor ficticio
  float temperaturaBMP = 22.0; // Valor ficticio
  float presionAbsoluta = 1010.0; // Valor ficticio
  float altitud = 100.0; // Valor ficticio
  float presionAlNivelDelMar = 1013.0; // Valor ficticio
  float luminosidad = 300.0; // Valor ficticio
  float ppmCO2 = 400.0; // Valor ficticio

  // Crear mensaje JSON con valores ficticios
  snprintf(mensaje, sizeof(mensaje), 
    "{"
    "\"temperaturaDHT\": %.2f, "
    "\"humedadRelativa\": %.2f, "
    "\"sensacionTermica\": %.2f, "
    "\"temperaturaBMP\": %.2f, "
    "\"presionAbsoluta\": %.2f, "
    "\"altitud\": %.2f, "
    "\"presionAlNivelDelMar\": %.2f, "
    "\"luminosidad\": %.2f, "
    "\"ppmCO2\": %.2f, "
    "\"vientoVelocidad\": \"Proximamente\", "
    "\"vientoDireccion\": \"Proximamente\", "
    "\"lluvia\": \"Proximamente\""
    "}", 
    temperaturaDHT, humedadRelativa, sensacionTermica, 
    temperaturaBMP, presionAbsoluta, altitud, 
    presionAlNivelDelMar, luminosidad, ppmCO2);

  // Envía el POST
  int httpResponseCode = http.POST(mensaje);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Código de respuesta: " + String(httpResponseCode));
    Serial.println("Respuesta: " + response);
  } else {
    Serial.print("Error en la conexión: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}
