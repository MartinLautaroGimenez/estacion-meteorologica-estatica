#include <Arduino.h>
#include <WiFiClientSecure.h>
#include "config.h"
#include "../lib/LeerSensores.h"

// Función auxiliar que convierte un valor flotante a una cadena JSON válida:
// Si el valor es NaN o menor a 0 (negativo o ilógico), devuelve "0".
String jsonValue(float value) {
  return (isnan(value) || value < 0) ? "0" : String(value);
}

WiFiClientSecure client;
LeerSensores sensores;

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
}

void loop() {
  delay(2000);
  sensores.leerTodos();
  String postData;
  // Usamos jsonValue para cada valor numérico y String() para los enteros ya que se espera que sean 0 o 1.
  postData =  "{\"EME_n0/dht/temp\":" + String(sensores.tempDHT) + 
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
  ",\"EME_n0/viento/dir\":" + String(sensores.vientoDir) + 
  "}";
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

  delay(60000);
}
