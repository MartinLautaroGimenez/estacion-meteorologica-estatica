#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "../lib/LeerSensores.h"
#include "../lib/config.h"
// #include "../lib/WiFiController.h"
#include "esp_sleep.h"
#include <WiFiClientSecure.h>
#include <ArduinoOTA.h>  // Añadido para la funcionalidad OTA

// Instanciar objetos de tipos.
WiFiClient esp_EME;
// ControladorWiFi controlerWiFi;
// ManejoDatosWifi dataHandler;
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
bool connectToWiFi();
String jsonMaker(
    LeerSensoresControlador::datosDHT dhtData,
    LeerSensoresControlador::datosMQ mqData,
    LeerSensoresControlador::datosBMP bmpData,    
    float bhData,
    float velViento,
    String dirViento,
    LeerSensoresControlador::datosYL lluvia
);

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

  esp_sleep_enable_timer_wakeup(15*60*1000000);

  Serial.begin(115200);
  String thisBoard = ARDUINO_BOARD;
  Serial.println(thisBoard);

  digitalWrite(PIN_GROUND_1, true);
  digitalWrite(PIN_GROUND_2, true);

  // Conectar a la red WiFi
  WiFi.begin(ssid, pass);
  Serial.print("Conectando a WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConectado a la red WiFi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP()); // Muestra la IP asignada
  
  
  //  Inicializar controlador de sensores
  controladorSensores.initControlador(BMP_TYPE_280, DHT_TYPE_22);

  //  Inicializar conexión a la red
  // setupWifi();

  leerSensores();

  // **Configuración de OTA**
  ArduinoOTA.setHostname("ESP32_OTA"); // Nombre del dispositivo
  ArduinoOTA.setPassword("emetec2024*"); // Contraseña de OTA
  ArduinoOTA.begin(); // Inicia el proceso de OTA
  
  Serial.println("Esperando 1 minuto para entrar en modo deepsleep...");
  delay(60000);

  Serial.print("Entrando a modo Deep Sleep\n");
  esp_deep_sleep_start();
}

void loop()
{
  ArduinoOTA.handle(); // Para manejar las solicitudes OTA mientras se ejecuta el loop
  
  // Si estamos en modo punto de acceso, atendemos las peticiones web
  // if (controlerWiFi.WiFi.getMode() == WIFI_AP) {
  //   controlerWiFi.server.handleClient();
  // }

  //leerSensores();
  
  // ESP.deepSleep(15*60*1000000);
  //delay(2 * 60 * 1000);
  //delay(TIME_TO_SLEEP_5_SEG * 1000);
}


void leerSensores(){
  
  //  Declaración de variables de lectura para sensores tomando cómo estructura de datos la de la clase LeerSensoresControlador
  LeerSensoresControlador::datosBMP bmpData;
  LeerSensoresControlador::datosDHT dhtData;
  LeerSensoresControlador::datosMQ mqData;
  float bh1750Data;
  float velViento;
  String dirViento;
  LeerSensoresControlador::datosYL lluvia;

  //  Lectura y asignación de párametros de los sensores
  dhtData = controladorSensores.leerDHT();
  bmpData = controladorSensores.leerBMP();
  bh1750Data = controladorSensores.leerBH();
  mqData = controladorSensores.leerMQ(dhtData.temperatura, dhtData.humedadRelativa);
  dirViento = controladorSensores.leerVeleta();
  lluvia = controladorSensores.leerHojaMojada();
  velViento = controladorSensores.leerAnemometro();


  // Crear cliente seguro para HTTPS
  WiFiClientSecure client;
  // Si estás en un entorno de pruebas y no te importa la verificación del certificado:
  client.setInsecure();

  // Conectar al servidor
  if (client.connect(SERVIDOR_ETEC, 443)) {
    Serial.printf("\nConectado al servidor HTTPS: %s\n", SERVIDOR_ETEC);

    //  Función para formatear a un dato tipo JSON valido
    String datazo = jsonMaker(dhtData,mqData,bmpData,bh1750Data,velViento,dirViento,lluvia);

    // Enviar solicitud HTTP POST
    client.println("POST /emeapi HTTP/1.1");
    client.println("Host: emetec.wetec.um.edu.ar");
    client.println("User-Agent: ESP32");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(datazo.length());
    client.println();  // Línea vacía para indicar fin de encabezados
    client.println(datazo);  // Cuerpo de la solicitud (datos)

    // Leer la respuesta del servidor
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("Cuerpo de la respuesta:");
        break;
      }
    }

    // Imprimir el cuerpo de la respuesta
    String response = client.readString();
    Serial.println(response);

  // Caso contrario indicar falla al servidor
  } else {
    Serial.println("Fallo al conectar al servidor HTTPS");
  }

  // Finalizar la conexión
  client.stop();
}

bool connectToWiFi()
{
  WiFi.begin(ssid, pass);

  // Esperamos hasta 10 segundos por la conexión
  for (int i = 0; i < 20; i++) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Conectado a WiFi");
      Serial.println("Dirección IP: " + WiFi.localIP().toString());
      return true;
    }
    delay(500);
    Serial.print(".");
  }

  Serial.println("Fallo en la conexión WiFi");
  return false;
}

String jsonMaker(
    LeerSensoresControlador::datosDHT dhtData,
    LeerSensoresControlador::datosMQ mqData,
    LeerSensoresControlador::datosBMP bmpData,    
    float bhData,
    float velViento,
    String dirViento,
    LeerSensoresControlador::datosYL lluvia
)
{
  String lluviaPosibilidad;
  
  // Evaluar la humedad relativa para determinar el mensaje
  if (dhtData.humedadRelativa > 85) {
    lluviaPosibilidad = "\"Hay posibilidades de que llueva\"";
  } else {
    lluviaPosibilidad = "\"No hay posibilidades de que llueva\"";
  }

  String postData;
  if (velViento < 0 
  || strcmp(dirViento.c_str(), "default") == 0 || strcmp(dirViento.c_str(), "Pendiente") == 0)
  {
    // Crear los datos JSON para enviar (sin valores de viento y hoja mojada)
    postData =  "{\"EME_n0/dht/temp\":" + String(dhtData.temperatura) + 
                ",\"EME_n0/dht/hum\":" + String(dhtData.humedadRelativa) + 
                ",\"EME_n0/dht/senst\":" + String(dhtData.sensacionTermica) + 
                ",\"EME_n0/bmp/temp\":" + String(bmpData.temperatura) + 
                ",\"EME_n0/bmp/pres\":" + String(bmpData.presionAbsoluta) + 
                ",\"EME_n0/bmp/alt\":" + String(bmpData.altitud) + 
                ",\"EME_n0/bmp/presnm\":" + String(bmpData.presionAlNivelDelMar) + 
                ",\"EME_n0/bh/presnm\":" + String(bhData) + 
                ",\"EME_n0/mq/ppmco2\":" + String(mqData.ppmCO2) + 
                ",\"EME_n0/yl/lluv\":" + lluviaPosibilidad + 
                ",\"EME_n0/viento/vel\":\"Proximamente\"" + 
                ",\"EME_n0/viento/dir\":\"Proximamente\"" + 
                "}";
  } else {
    // Crear los datos JSON para enviar (con valores completos)
    postData =  "{\"EME_n0/dht/temp\":" + String(dhtData.temperatura) + 
                ",\"EME_n0/dht/hum\":" + String(dhtData.humedadRelativa) + 
                ",\"EME_n0/dht/senst\":" + String(dhtData.sensacionTermica) + 
                ",\"EME_n0/bmp/temp\":" + String(bmpData.temperatura) + 
                ",\"EME_n0/bmp/pres\":" + String(bmpData.presionAbsoluta) + 
                ",\"EME_n0/bmp/alt\":" + String(bmpData.altitud) + 
                ",\"EME_n0/bmp/presnm\":" + String(bmpData.presionAlNivelDelMar) + 
                ",\"EME_n0/bh/presnm\":" + String(bhData) + 
                ",\"EME_n0/mq/ppmco2\":" + String(mqData.ppmCO2) + 
                ",\"EME_n0/yl/lluv\":" + lluviaPosibilidad + 
                ",\"EME_n0/viento/vel\":" +  String(velViento) + 
                ",\"EME_n0/viento/dir\":" + dirViento + 
                "}";
  }
  return postData;
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