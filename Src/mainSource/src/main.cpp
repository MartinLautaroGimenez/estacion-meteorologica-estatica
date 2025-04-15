#include <Arduino.h>
#include <WiFiClientSecure.h>
#include "config.h"            // Asegurate de definir aquí RED_SSID_WIFI y PASSWORD_WIFI
#include "../lib/LeerSensores.h"
#include <Preferences.h>

#ifdef online_deepsleep
  #include <WebServer.h>
#endif

#ifdef ota
void setupOTA(const char* hostname = "esp32");
void handleOTA();
#else
void setupOTA(const char* hostname = nullptr) {}
void handleOTA() {}
#endif

WiFiClientSecure client;
LeerSensores sensores;
Preferences preferences;

#ifdef online_deepsleep
WebServer server(2020);

void handleSleepRequest() {
  if (server.hasArg("duration")) {
    int minutes = server.arg("duration").toInt();
    if (minutes <= 0) minutes = 15;
    server.send(200, "text/html", "Deep sleep por " + String(minutes) + " minutos.");
    delay(1000);
    esp_sleep_enable_timer_wakeup(minutes * 60ULL * 1000000ULL);
    esp_deep_sleep_start();
  } else {
    server.send(200, "text/html",
      "<html><head><meta http-equiv='refresh' content='5'></head>"
      "<body><h1>Esperando configuración de deep sleep</h1>"
      "<p>Usá <b>?duration=15</b> en la URL para setear duración.</p></body></html>");
  }
}
#endif

String jsonValue(float value) {
  return (isnan(value) || value < 0) ? "0" : String(value);
}
void startCaptivePortal();
void handleCaptivePortal();

void setup() {
  Serial.begin(115200);
  Serial.println("Arrancando estación meteorológica... 🌦️");

  // Leer credenciales almacenadas
  preferences.begin("wifi", true);
  String ssid = preferences.getString("ssid", "");
  String pass = preferences.getString("pass", "");
  preferences.end();

  // Si no hay credenciales en las preferencias, utilizamos las predeterminadas (definidas en config.h)
  if (ssid == "") {
    #ifdef RED_SSID_WIFI
      ssid = RED_SSID_WIFI;
      pass = PASSWORD_WIFI;
      Serial.println("No se encontraron credenciales guardadas. Usando las predeterminadas.");
    #else
      Serial.println("No hay credenciales guardadas y no se han definido predeterminadas.");
    #endif
  }
  
  // Intentar conectarse si hay un SSID definido
  if (ssid != "") {
    Serial.print("Intentando conectar a la red: ");
    Serial.println(ssid);
    WiFi.begin(ssid.c_str(), pass.c_str());

    unsigned long startAttemptTime = millis();
    // Esperar hasta 10 segundos para conectarse
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
      delay(500);
      Serial.print(".");
    }
    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("✅ Conectado. IP: " + WiFi.localIP().toString());
    } else {
      Serial.println("❌ Falló la conexión. Activando portal cautivo.");
      startCaptivePortal();
    }
  } else {
    Serial.println("📡 No hay credenciales definidas. Iniciando portal cautivo.");
    startCaptivePortal();
  }

  // Si WiFi está en modo AP (portal), salimos de setup
  if (WiFi.getMode() == WIFI_AP) return;

  client.setInsecure();
  sensores.begin();
  delay(2000);
  sensores.leerTodos();

  String postData = "{\"EME_n0/dht/temp\":" + String(sensores.tempDHT) + 
    ",\"EME_n0/dht/hum\":" + String(sensores.humDHT) + 
    ",\"EME_n0/dht/senst\":" + String(sensores.sensDHT) + 
    ",\"EME_n0/bmp/temp\":" + String(sensores.tempBMP) + 
    ",\"EME_n0/bmp/pres\":" + String(sensores.presBMP) + 
    ",\"EME_n0/bmp/alt\":" + String(sensores.altBMP) + 
    ",\"EME_n0/bmp/presnm\":" + String(sensores.presNMBMP) + 
    ",\"EME_n0/bh/luz\":" + String(sensores.bh1750) + 
    ",\"EME_n0/mq/ppmco2\":" + String(sensores.ppmCO2) + 
    ",\"EME_n0/yl/lluv\":" + String(-1) + 
    ",\"EME_n0/viento/vel\":" + String(sensores.vientoVel) +  
    ",\"EME_n0/viento/dir\":" + String(sensores.vientoDir) + "}";

  Serial.println("Datos a enviar:");
  Serial.println(postData);

  if (client.connect(SERVIDOR_ETEC, 443)) {
    Serial.println("Conectado al servidor!");
    client.println("POST /emeapi HTTP/1.1");
    client.print("Host: "); client.println(SERVIDOR_ETEC);
    client.println("Content-Type: application/json");
    client.print("Content-Length: "); client.println(postData.length());
    client.println();
    client.println(postData);

    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") break;
    }

    String response = client.readString();
    Serial.println("Respuesta del servidor:");
    Serial.println(response);
    client.stop();
  } else {
    Serial.println("❌ No se pudo conectar al servidor.");
  }

#ifdef online_deepsleep
  server.on("/", handleSleepRequest);
  server.begin();
  Serial.println("Servidor web iniciado en el puerto 2020.");
  Serial.println("Esperando configuración de deep sleep (30 seg)...");

  #ifdef ota
    setupOTA("EME_esp32");
  #endif

  unsigned long start_wait = millis();
  while (millis() - start_wait < 30000) {
    server.handleClient();
    #ifdef ota
      handleOTA();
    #endif
    delay(10);
  }

  Serial.println("No se recibió configuración. Esperando OTA (120 seg)...");

  unsigned long ota_wait = millis();
  while (millis() - ota_wait < 120000) {
    #ifdef ota
      handleOTA();
    #endif
    delay(10);
  }

  Serial.println("Entrando en deep sleep por 15 minutos.");
  esp_sleep_enable_timer_wakeup(15 * 60ULL * 1000000ULL);
  esp_deep_sleep_start();
#else
  Serial.println("Entrando en deep sleep por 15 minutos.");
  esp_sleep_enable_timer_wakeup(15 * 60ULL * 1000000ULL);
  esp_deep_sleep_start();
#endif
}

void loop() {
  if (WiFi.getMode() == WIFI_AP) {
    handleCaptivePortal();
  }
}
