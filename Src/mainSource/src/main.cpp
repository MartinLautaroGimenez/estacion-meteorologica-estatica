#include <Arduino.h>
#include <WiFiClientSecure.h>
#include "config.h"
#include "../lib/LeerSensores.h"
#include <Preferences.h>

#ifdef ota
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

#ifdef ota
WebServer server(2020);
#endif

String jsonValue(float value) {
  return (isnan(value) || value < 0) ? "0" : String(value, 2);
}

void startCaptivePortal();
void handleCaptivePortal();

void conectarWiFi() {
  preferences.begin("wifi", true);
  String ssid = preferences.getString("ssid", "");
  String pass = preferences.getString("pass", "");
  preferences.end();

  if (ssid == "") {
    #ifdef RED_SSID_WIFI
      ssid = RED_SSID_WIFI;
      pass = PASSWORD_WIFI;
      Serial.println("No se encontraron credenciales guardadas. Usando las predeterminadas.");
    #else
      Serial.println("No hay credenciales guardadas y no se han definido predeterminadas.");
    #endif
  }

  if (ssid != "") {
    Serial.print("Intentando conectar a la red: ");
    Serial.println(ssid);
    WiFi.begin(ssid.c_str(), pass.c_str());

    unsigned long startAttemptTime = millis();
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
}

void enviarDatos() {
  sensores.leerTodos();
  client.setInsecure();

  String postData = "{";
  postData += "\"EME_n0/dht/temp\":"     + jsonValue(sensores.tempDHT) +
              ",\"EME_n0/dht/hum\":"      + jsonValue(sensores.humDHT) +
              ",\"EME_n0/dht/senst\":"    + jsonValue(sensores.sensDHT) +
              ",\"EME_n0/bmp/temp\":"     + jsonValue(sensores.tempBMP) +
              ",\"EME_n0/bmp/pres\":"     + jsonValue(sensores.presBMP) +
              ",\"EME_n0/bmp/alt\":"      + jsonValue(sensores.altBMP) +
              ",\"EME_n0/bmp/presnm\":"   + jsonValue(sensores.presNMBMP) +
              ",\"EME_n0/bh/luz\":"       + jsonValue(sensores.bh1750) +
              ",\"EME_n0/mq/ppmco2\":"    + jsonValue(sensores.ppmCO2) +
              ",\"EME_n0/yl/lluv\":"      + jsonValue(sensores.lluviaPosibilidad) +
              ",\"EME_n0/viento/vel\":"   + jsonValue(sensores.vientoVel) +
              ",\"EME_n0/viento/dir\":"   + jsonValue(sensores.vientoDir) +
              "}";

  Serial.println("📤 Datos a enviar:");
  Serial.println(postData);

  if (client.connect(SERVIDOR_ETEC, 443)) {
    Serial.println("🔗 Conectado al servidor!");
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
    Serial.println("📥 Respuesta del servidor:");
    Serial.println(response);
    client.stop();
  } else {
    Serial.println("❌ No se pudo conectar al servidor.");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Arrancando estación meteorológica... 🌦️");

  Serial.print("Dirección MAC del ESP32: ");
  Serial.println(WiFi.macAddress());

  pinMode(PIN_GROUND_1, OUTPUT);
  pinMode(PIN_GROUND_2, OUTPUT);
  digitalWrite(PIN_GROUND_1, HIGH);
  digitalWrite(PIN_GROUND_2, HIGH);

  sensores.begin();
  delay(2000);

  conectarWiFi();

  #ifdef ota
    server.begin();
    setupOTA("EME_esp32");
  #endif
}

void loop() {
  if (WiFi.getMode() == WIFI_AP) {
    handleCaptivePortal();
    delay(10);
    return;
  }

  #ifdef ota
    server.handleClient();
    handleOTA();
  #endif

  enviarDatos();

  Serial.println("🕒 Esperando 15 minutos para el próximo envío...");
  for (int i = 0; i < 900; i++) {
    #ifdef ota
      server.handleClient();
      handleOTA();
    #endif
    delay(1000);
  }
}
