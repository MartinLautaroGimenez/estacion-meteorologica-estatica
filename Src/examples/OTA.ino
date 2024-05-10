#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncOTA.h>

const char* ssid = "";
const char* password = "";

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Estableciendo conexión a la red WiFi...");
    }
    Serial.println("Conectado al internet!");
    Serial.print("Dirección IP: ");
    Serial.println(WiFi.localIP());
    
    //INICIAR OTA
    ArduinoOTA.setHostname("OTA-EJEMPLO");
    ArduinoOTA.setPassword("1234");

    ArduinoOTA.onStart([]() {
        Serial.println("INICIANDO ACTUALIZACIÓN VIA OTA");
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nOTA finalizada exitosamente");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("OTA EN PROCESO: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("OTA update error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) {
            Serial.println("Autenticación fallida");
        } else if (error == OTA_BEGIN_ERROR) {
            Serial.println("Error al iniciar OTA");
        } else if (error == OTA_CONNECT_ERROR) {
            Serial.println("Error al conectar OTA");
        } else if (error == OTA_RECEIVE_ERROR) {
            Serial.println("Error al recibir datos OTA");
        } else if (error == OTA_END_ERROR) {
            Serial.println("Error al finalizar OTA");
        }
    });

    ArduinoOTA.begin();

    // ACA VAN LAS DEMAS CONFIGURACIONES
}

void loop() {
    ArduinoOTA.handle();
}

