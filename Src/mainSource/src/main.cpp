#include <Arduino.h>
#include <WiFiClientSecure.h>
#include "config.h"
#include "../lib/LeerSensores.h"

#ifdef online_deepsleep
  #include <WebServer.h>  // Usamos la librería estándar del ESP para el servidor HTTP
#endif

// Función auxiliar para validar valores numéricos (puedes seguir usando jsonValue si quieres)
String jsonValue(float value) {
  return (isnan(value) || value < 0) ? "0" : String(value);
}

WiFiClientSecure client;
LeerSensores sensores;

#ifdef online_deepsleep
// Se instancia el servidor web en el puerto 2020
WebServer server(2020);

// Handler para la ruta raíz: espera el parámetro "duration" (duración en minutos)
void handleSleepRequest() {
  if (server.hasArg("duration")) {
    int minutes = server.arg("duration").toInt();
    if (minutes <= 0) {
      minutes = 15;  // Si el valor es inválido, se utiliza 15 minutos por defecto.
    }
    String response = "Configurado deep sleep por " + String(minutes) + " minutos. Reiniciando para dormir...";
    server.send(200, "text/html", response);
    Serial.println(response);
    delay(1000);  // Pequeña pausa para que se envíe la respuesta
    Serial.println("Entrando en deep sleep por " + String(minutes) + " minutos.");
    esp_sleep_enable_timer_wakeup(minutes * 60ULL * 1000000ULL);
    esp_deep_sleep_start();
  } else {
    // Si no se envía el parámetro se muestra una página simple para que el usuario conozca la sintaxis
    server.send(200, "text/html",
      "<html><head><meta http-equiv='refresh' content='5'></head>"
      "<body><h1>Esperando configuración de deep sleep</h1>"
      "<p>Indique la duración en minutos mediante <b>?duration=XX</b> en la URL.</p></body></html>");
  }
}
#endif

void setup() {
  Serial.begin(115200);
  Serial.println("Arrancando estación meteorológica... 🌦️");

  Serial.print("Conectando a la red ");
  Serial.println(RED_SSID_WIFI);
  WiFi.begin(RED_SSID_WIFI, PASSWORD_WIFI);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi conectado!");
  Serial.print("IP asignada: ");
  Serial.println(WiFi.localIP());

  // Deshabilitamos la verificación del certificado (solo para pruebas)
  client.setInsecure();

  sensores.begin();
  delay(2000);
  sensores.leerTodos();

  // Construimos la cadena JSON con los datos de los sensores
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
    client.print("Host: ");
    client.println(SERVIDOR_ETEC);
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(postData.length());
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
    Serial.println("Error: No se pudo conectar al servidor.");
  }

#ifdef online_deepsleep
  // Iniciamos el servidor web para configurar el deep sleep.
  server.on("/", handleSleepRequest);
  server.begin();
  Serial.println("Servidor web iniciado en el puerto 2020.");
  Serial.println("Esperando configuración de deep sleep (duración en minutos) durante 30 segundos...");
  
  unsigned long start_wait = millis();
  while (millis() - start_wait < 30000) {
    server.handleClient();
    delay(10);
  }
  Serial.println("No se recibió configuración. Entrando en deep sleep automáticamente por 15 minutos.");
  esp_sleep_enable_timer_wakeup(15 * 60ULL * 1000000ULL);
  esp_deep_sleep_start();
#else
  Serial.println("Entrando en deep sleep automáticamente por 15 minutos.");
  esp_sleep_enable_timer_wakeup(15 * 60ULL * 1000000ULL);
  esp_deep_sleep_start();
#endif
}

void loop() {
  // Nada que hacer aquí porque después de enviar los datos se entra en deep sleep.
}
