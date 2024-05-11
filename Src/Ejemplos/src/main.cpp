#include <WiFi.h>
#include <ArduinoOTA.h>

const char* ssid = "ETEC";
const char* password = "ETec2024*";
const char* OTA_password = "bob";

void setup() {
  Serial.begin(115200);

  // Conexión WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conexión exitosa a WiFi");
  
  // Configuración OTA
  ArduinoOTA.setHostname("ESP32-OTA");
  ArduinoOTA.setPassword(OTA_password);

  // Iniciar el servicio OTA
  ArduinoOTA.begin();
}

void loop() {
  // Manejo de actualizaciones OTA
  ArduinoOTA.handle();
  
  // Tu código loop aquí
}
