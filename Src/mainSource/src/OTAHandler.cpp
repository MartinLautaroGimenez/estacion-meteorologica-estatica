#include "../lib/config.h"

#ifdef ota
#include <ArduinoOTA.h>
#include <WiFi.h>

void setupOTA(const char* hostname) {
  ArduinoOTA.setHostname(hostname);

  ArduinoOTA.onStart([]() {
    String type = (ArduinoOTA.getCommand() == U_FLASH) ? "sketch" : "filesystem";
    Serial.println("Inicio de OTA. Tipo: " + type);
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nOTA finalizado.");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progreso: %u%%\r", (progress * 100) / total);
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error [%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Fallo de autenticación");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Fallo al comenzar");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Fallo de conexión");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Fallo en recepción");
    else if (error == OTA_END_ERROR) Serial.println("Fallo al finalizar");
  });

  ArduinoOTA.begin();
  Serial.println("OTA listo ✨ Esperando actualizaciones...");
}

void handleOTA() {
  ArduinoOTA.handle();
}
#endif
