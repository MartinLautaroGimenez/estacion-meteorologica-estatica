#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <Preferences.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>
#include <ESPmDNS.h>  // Para mDNS

// —— Añadidos para forzar hostname —— 
#include <esp_netif.h>
// ————————————————————————————————

#include "config.h"
#include "../lib/LeerSensores.h"

#ifdef USE_PANTALLA
  #include "../lib/pantallacontroller.h"
  PantallaController pantalla;
  unsigned long tiempoUltimaPantalla = 0;
  uint8_t indicePantalla = 0;
#endif

#ifdef ota
  #include <ArduinoOTA.h>
#endif

#ifdef WEBAPP
  #include "../lib/WebController.h"
#endif

WiFiClientSecure client;
LeerSensores sensores;
Preferences preferences;
AsyncWebServer server(2020); // Web server asincrónico

#ifdef WEBAPP
WebController webapp(80, &sensores, &preferences);
#endif

String jsonValue(float value) {
  return (isnan(value) || value < 0) ? "0" : String(value, 2);
}

void startCaptivePortal();
void handleCaptivePortal();

void conectarWiFi() {
  // Inicializa la interfaz de red y fuerza modo estación
  esp_netif_init();
  WiFi.mode(WIFI_STA);

  preferences.begin("wifi", true);
  String ssid = preferences.getString("ssid", "");
  String pass = preferences.getString("pass", "");
  preferences.end();

  if (ssid == "") {
    #ifdef RED_SSID_WIFI
      ssid = RED_SSID_WIFI;
      pass = PASSWORD_WIFI;
      Serial.println("Usando credenciales predeterminadas.");
      WebSerial.println("Usando credenciales predeterminadas.");
    #else
      Serial.println("❌ Sin credenciales y sin predeterminadas.");
      WebSerial.println("❌ Sin credenciales y sin predeterminadas.");
    #endif
  }

  if (ssid != "") {
    Serial.print("Conectando a: "); Serial.println(ssid);
    WebSerial.print("Conectando a: "); WebSerial.println(ssid);

    // —— Forzamos el hostname ANTES de conectar —— 
    esp_netif_t* sta_netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    esp_netif_set_hostname(sta_netif, hostdom);
    // ————————————————————————————————

    WiFi.begin(ssid.c_str(), pass.c_str());

    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
      delay(500); Serial.print("."); WebSerial.print(".");
    }
    Serial.println(); WebSerial.println();

    if (WiFi.status() == WL_CONNECTED) {
      String ip = WiFi.localIP().toString();
      Serial.println("✅ Conectado. IP: " + ip);
      WebSerial.println("✅ Conectado. IP: " + ip);
      digitalWrite(ledverde, HIGH);
      #ifdef USE_PANTALLA
        pantalla.mostrarEstadoWiFi(ssid, ip);
      #endif

      // —— Iniciar mDNS para "eme.local" y HTTP ——
      if (MDNS.begin(hostdom)) {
        MDNS.addService("http", "tcp", 80);
        Serial.println("✅ mDNS iniciado: http://" + String(hostdom) + ".local");
        WebSerial.println("✅ mDNS iniciado: http://" + String(hostdom) + ".local");
      } else {
        Serial.println("❌ Error iniciando mDNS");
        WebSerial.println("❌ Error iniciando mDNS");
      }
      // ——————————————————————————

    } else {
      Serial.println("❌ Falló conexión. Activando portal cautivo.");
      WebSerial.println("❌ Falló conexión. Activando portal cautivo.");
      String mac = WiFi.macAddress();
      Serial.println("MAC: " + mac);
      WebSerial.println("MAC: " + mac);
      #ifdef USE_PANTALLA
        pantalla.mostrarError("Config. wifi activo");
      #endif
      startCaptivePortal();
    }
  } else {
    Serial.println("📡 Sin credenciales. Activando portal cautivo.");
    WebSerial.println("📡 Sin credenciales. Activando portal cautivo.");
    #ifdef USE_PANTALLA
      pantalla.mostrarError("Config. wifi activo");
    #endif
    startCaptivePortal();
  }
}

void enviarDatos() {
  sensores.leerTodos();
  client.setInsecure();

  String postData = "{\"EME_n0/dht/temp\":" + jsonValue(sensores.tempDHT) + 
                    ",\"EME_n0/dht/hum\":"  + jsonValue(sensores.humDHT) + 
                    ",\"EME_n0/dht/senst\":"+ jsonValue(sensores.sensDHT) + 
                    ",\"EME_n0/bmp/temp\":"+ jsonValue(sensores.tempBMP) +
                    ",\"EME_n0/bmp/pres\":"+ jsonValue(sensores.presBMP) +
                    ",\"EME_n0/bmp/alt\":" + jsonValue(sensores.altBMP) +
                    ",\"EME_n0/bmp/presnm\":"+ jsonValue(sensores.presNMBMP) +
                    ",\"EME_n0/bh/luz\":" + jsonValue(sensores.bh1750) +
                    ",\"EME_n0/mq/ppmco2\":"+ jsonValue(sensores.ppmCO2) +
                    ",\"EME_n0/yl/lluv\":"+ jsonValue(-1) +
                    ",\"EME_n0/viento/vel\":"+ jsonValue(sensores.vientoVel) +
                    ",\"EME_n0/viento/dir\":"+ jsonValue(sensores.vientoDir) +
                    "}";

  Serial.println("📤 Datos a enviar:");
  Serial.println(postData);
  WebSerial.println(postData);

  if (client.connect(SERVIDOR_ETEC, 443)) {
    Serial.println("Conectado al servidor!");
    WebSerial.println("Conectado al servidor!");

    client.println("POST " apiendpoint " HTTP/1.1");
    client.print("Host: "); client.println(SERVIDOR_ETEC);
    client.println("Content-Type: application/json");
    client.print("Content-Length: "); client.println(postData.length());
    client.println(); client.println(postData);

    while (client.connected()) {
      if (client.readStringUntil('\n') == "\r") break;
    }

    String response = client.readString();
    Serial.println("📥 Respuesta del servidor:");
    Serial.println(response);
    WebSerial.println(response);

    client.stop();
    digitalWrite(ledverde, LOW);
    for (int i = 0; i < 3; i++) {
      digitalWrite(ledverde, HIGH); delay(100);
      digitalWrite(ledverde, LOW);  delay(100);
    }
    digitalWrite(ledverde, HIGH);
  } else {
    Serial.println("❌ No se pudo conectar al servidor.");
    WebSerial.println("❌ No se pudo conectar al servidor.");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_GROUND_1, OUTPUT);
  pinMode(PIN_GROUND_2, OUTPUT);
  pinMode(ledverde, OUTPUT);
  digitalWrite(PIN_GROUND_1, HIGH);
  digitalWrite(PIN_GROUND_2, HIGH);

  sensores.begin();
  delay(2000);

  #ifdef USE_PANTALLA
    pantalla.begin();
    pantalla.mostrarInicio();
  #endif

  conectarWiFi();

  WebSerial.begin(&server);
  WebSerial.println("🌐 WebSerial activo. http://<IP>:2020/webserial");
  server.begin();

  #ifdef ota
    ArduinoOTA.setHostname(hostdom);
    ArduinoOTA.begin();
  #endif
  #ifdef WEBAPP
    webapp.begin();
  #endif
}

void loop() {
  #ifdef ota
    ArduinoOTA.handle();
  #endif

  if (WiFi.getMode() == WIFI_AP) {
    handleCaptivePortal();
    delay(10);
    return;
  }

  enviarDatos();

  Serial.println("⏳ Esperando 15 minutos...");
  WebSerial.println("⏳ Esperando 15 minutos...");

  for (int i = 0; i < 900; i++) {
    #ifdef ota
      ArduinoOTA.handle();
    #endif

    if (!WiFi.isConnected()) {
      Serial.println("❌ Desconectado.");
      WebSerial.println("❌ Desconectado.");
      digitalWrite(ledverde, LOW);
      conectarWiFi();
    }

    #ifdef USE_PANTALLA
      if (millis() - tiempoUltimaPantalla > 4000) {
        pantalla.mostrarPantallaCiclo(indicePantalla, sensores);
        indicePantalla = (indicePantalla + 1) % 2;
        tiempoUltimaPantalla = millis();
      }
    #endif

    delay(1000);
  }
}
