#include <WiFi.h>
#include <ArduinoOTA.h>

const char* ssid = "quemirabobo";
const char* password = "12345678";
const char* otaPassword = "BOB";

void setup() {
  Serial.begin(115200);

  // Inicializar WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }

  // Inicializar OTA
  ArduinoOTA.setPort(8266);
  ArduinoOTA.setPassword(otaPassword);
  ArduinoOTA.begin();

  Serial.println("Listo");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  ArduinoOTA.handle();
  // Tu código aquí
}