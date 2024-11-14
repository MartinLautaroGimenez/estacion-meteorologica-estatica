#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "../lib/LeerSensores.h"
#include "../lib/config.h"
#include "../lib/WiFiController.h"
#include "esp_sleep.h"
#include <WiFiClientSecure.h>

// Instanciar objetos de tipos.
WiFiClient esp_EME;
ControladorWiFi controlerWiFi;
ManejoDatosWifi dataHandler;
LeerSensoresControlador controladorSensores;

// Definición de las credenciales de WiFi
#define ssid RED_SSID_WIFI
#define pass PASSWORD_WIFI

// Definición de factores de tiempo a usar
#define uS_TO_S_FACTOR 1000000  // Conversión de segundos a microsegundos
#define TIME_TO_SLEEP_15_MIN  15 * 60  // Tiempo en segundos (15 minutos)
#define TIME_TO_SLEEP_5_SEG  5   // Tiempo en segundos (5seg
#define TIME_TO_SLEEP_2_MIN  2 * 60   // Tiempo en segundos (2min)

// Función para leer sensores
void leerSensores();

void setup()
{
  ////****Para ESP8266****////
  // ESP.deepSleep(15*60*1000000);    // DEEP sleep 15 minutos
  // ESP.deepSleep(5 * 1000000); // DEEP sleep de 5 segundos
  ////****Para ESP32*****////
  // Configurar el temporizador RTC para que despierte el ESP32 después de TIME_TO_SLEEP segundos
  // esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP_15_MIN * uS_TO_S_FACTOR);
  // esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP_5_SEG * uS_TO_S_FACTOR);

    // ++bootCount;
    // Serial.printf("Boot number: %d\n", bootCount);

    // print_wakeup_reason();

  Serial.begin(115200);
  String thisBoard = ARDUINO_BOARD;
  Serial.println(thisBoard);

  if (controlerWiFi.connectToWiFi()){
    Serial.printf("Se conecto a la red WiFi %s, contraseña: %s", RED_SSID_WIFI, PASSWORD_WIFI);
  }
  // Conectar a la red WiFi
  WiFi.begin(ssid, pass);
  Serial.print("Conectando a WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConectado a la red WiFi");

  //  Inicializar controlador de sensores
  controladorSensores.initControlador(BMP_TYPE_280, DHT_TYPE_22);

  //  Inicializar conexión a la red
  // setupWifi();
  
  // Serial.print("Entrando a modo Deep Sleep\n");
  // esp_deep_sleep_start();
}

void loop()
{
  // Si estamos en modo punto de acceso, atendemos las peticiones web
  // if (controlerWiFi.WiFi.getMode() == WIFI_AP) {
  //   controlerWiFi.server.handleClient();
  // }

  leerSensores();
  // digitalWrite(27, LOW);
  // digitalWrite(14, LOW);
  // ESP.deepSleep(15*60*1000000);
  delay(2 * 60 * 1000);
  //delay(TIME_TO_SLEEP_5_SEG * 1000);
}


void leerSensores(){
  
  //  Declaración de variables de lectura para sensores tomando cómo estructura de datos la de la clase LeerSensoresControlador
  LeerSensoresControlador::datosBMP bmpData;
  LeerSensoresControlador::datosDHT dhtData;
  LeerSensoresControlador::datosMQ mqData;
  float bh1750Data;
  float velViento = -1;
  String dirViento = "";
  float lluvia = -1;

  //  Lectura y asignación de párametros de los sensores
  dhtData = controladorSensores.leerDHT();
  bmpData = controladorSensores.leerBMP();
  bh1750Data = controladorSensores.leerBH();
  mqData = controladorSensores.leerMQ(dhtData.temperatura, dhtData.humedadRelativa);

  String datazo = dataHandler.jsonMaker(
      dhtData,mqData,bmpData,bh1750Data,velViento,dirViento,lluvia
  );

  dataHandler.enviarData(datazo);


  // // Crear cliente seguro para HTTPS
  // WiFiClientSecure client;
  // // Si estás en un entorno de pruebas y no te importa la verificación del certificado:
  // client.setInsecure();

  // // Conectar al servidor
  // if (client.connect("emetec.wetec.um.edu.ar", 443)) {
  //   Serial.println("Conectado al servidor HTTPS");

  //   // String datazo = dataHandler.jsonMaker(dhtData,mqData,bmpData,bh1750Data,velViento,dirViento,lluvia);
  //   String datazo = "pepe";

  //   // Enviar solicitud HTTP POST
  //   client.println("POST /weather HTTP/1.1");
  //   client.println("Host: emetec.wetec.um.edu.ar");
  //   client.println("User-Agent: ESP32");
  //   client.println("Content-Type: application/json");
  //   client.print("Content-Length: ");
  //   client.println(datazo.length());
  //   client.println();  // Línea vacía para indicar fin de encabezados
  //   client.println(datazo);  // Cuerpo de la solicitud (datos)

  //   // Leer la respuesta del servidor
  //   while (client.connected()) {
  //     String line = client.readStringUntil('\n');
  //     if (line == "\r") {
  //       Serial.println("Cuerpo de la respuesta:");
  //       break;
  //     }
  //   }

  //   // Imprimir el cuerpo de la respuesta
  //   String response = client.readString();
  //   Serial.println(response);

  // // Caso contrario indicar falla al servidor
  // } else {
  //   Serial.println("Fallo al conectar al servidor HTTPS");
  // }

  // // Finalizar la conexión
  // client.stop();
}

/*
⣿⣿⡟⡹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⢱⣶⣭⡻⢿⠿⣛⣛⣛⠸⣮⡻⣿⣿⡿⢛⣭⣶⣆⢿⣿
⣿⡿⣸⣿⣿⣿⣷⣮⣭⣛⣿⣿⣿⣿⣶⣥⣾⣿⣿⣿⡷⣽⣿
⣿⡏⣾⣿⣿⡿⠿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⣿⣿
⣿⣧⢻⣿⡟⣰⡿⠁⢹⣿⣿⣿⣋⣴⠖⢶⣝⢻⣿⣿⡇⣿⣿
⠩⣥⣿⣿⣴⣿⣇⠀⣸⣿⣿⣿⣿⣷⠀⢰⣿⠇⣿⣭⣼⠍⣿
⣿⡖⣽⣿⣿⣿⣿⣿⣿⣯⣭⣭⣿⣿⣷⣿⣿⣿⣿⣿⡔⣾⣿
⣿⡡⢟⡛⠻⠿⣿⣿⣿⣝⣨⣝⣡⣿⣿⡿⠿⠿⢟⣛⣫⣼⣿
⣿⣿⣿⡷⠝⢿⣾⣿⣿⣿⣿⣿⣿⣿⣿⣾⡩⣼⣿⣿⣿⣿⣿
⣿⣿⣯⡔⢛⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣭⣍⣨⠿⢿⣿⣿⣿
⣿⡿⢫⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣶⣝⣿
*/