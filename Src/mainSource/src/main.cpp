#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "../lib/LeerSensores.h"
#include "../lib/config.h"
#include "esp_sleep.h"

char mensaje[80];

WiFiClient esp_EME;
PubSubClient client(esp_EME);
LeerSensoresControlador controlador;

#define ssid RED_SSID_WIFI
#define pass PASSWORD_WIFI
#define brokerUser USUARIO_BROKER
#define brokerPass PASSWORD_BROKER
#define broker DIRECCION_BROKER

//  Topicos del Broker Mosquitto MQTT
#define senBH1750 "EMM/bh1750"
#define senBMP180 "EMM/bmp180"
#define senDHT11 "EMM/dht11"
#define senMQ135 "EMM/mq135"
#define chau "EMM/test"
//  Topicos Del broker ETec
#define temperaturaDHT "temp"
#define humedadDHT "hum"
#define presionBMP "bp"
#define luminosidad "luz"
#define calidadAire "aire"
#define velocidadViento "velocidad"
#define direccionViento "direccion"
#define sensorDeLluvia "lluvia"


#define uS_TO_S_FACTOR 1000000  // Conversión de segundos a microsegundos
#define TIME_TO_SLEEP_15_MIN  15 * 60  // Tiempo en segundos (15 minutos)
#define TIME_TO_SLEEP_5_SEG  5   // Tiempo en segundos (5seg
#define TIME_TO_SLEEP_2_MIN  2 * 60   // Tiempo en segundos (2min)

void envioMQTT();

void setupWifi()
{
  delay(100);
  Serial.print("\nConectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print(".-");
  }

  Serial.print("\nConectado a la red: ");
  Serial.println(ssid);
  Serial.print("\nCon la siguiente IP: ");
  Serial.println(WiFi.localIP());
}
void reconectar()
{
  Serial.print("\nConectando al broker: ");
  Serial.println(broker);
  while (!client.connected())
  {
    if (client.connect("EME"))
    {
      Serial.print("\nConectado al broker: ");
      Serial.println(broker);
      client.subscribe(chau);
    }
    else
    {
      Serial.print("Error de conexión, rc=");
      Serial.print(client.state());
      Serial.println(".-");
      delay(1000);
    }
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Mensaje recibido: ");
  Serial.println(topic);
  for (unsigned int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

unsigned long delayTime;
void setup()
{
  // //****Para ESP8266****/
  // // ESP.deepSleep(15*60*1000000);    // DEEP sleep 15 minutos
  // // ESP.deepSleep(5 * 1000000); // DEEP sleep de 5 segundos
  // //****Para ESP32*****/
  // // Configurar el temporizador RTC para que despierte el ESP32 después de TIME_TO_SLEEP segundos
  // // esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP_15_MIN * uS_TO_S_FACTOR);
  // esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP_5_SEG * uS_TO_S_FACTOR);

    // ++bootCount;
    // Serial.printf("Boot number: %d\n", bootCount);

    // print_wakeup_reason();


  Serial.begin(115200);
  String thisBoard = ARDUINO_BOARD;
  Serial.println(thisBoard);

  digitalWrite(27, HIGH);
  digitalWrite(14, HIGH);



  //  Inicializar controlador de sensores
  controlador.initControlador(BMP_TYPE_180);

  //  Inicializar conexión a la red
  setupWifi();
  client.setServer(broker, 1883);
  client.setCallback(callback);

  // envioMQTT();

  // Serial.print("Entrando a modo Deep Sleep\n");
  // esp_deep_sleep_start();
}

void loop()
{
  envioMQTT();
  digitalWrite(27, LOW);
  digitalWrite(14, LOW);
  ESP.deepSleep(15*60*1000000);
  //delay(2 * 60 * 1000);
  //delay(TIME_TO_SLEEP_5_SEG * 1000);
}


void envioMQTT(){
  // Reconectar si se ha desconectado del Broker
  if (!client.connected())
  {
    reconectar();
  }
  client.loop();

  // Declaración de variables de lectura para sensores
  LeerSensoresControlador::datosBMP bmpData;
  LeerSensoresControlador::datosDHT dhtData;
  LeerSensoresControlador::datosMQ mqData;
  float bh1750Data;

  // Lectura y asignación de parámetros de los sensores
  dhtData = controlador.leerDHT();
  bmpData = controlador.leerBMP();
  bh1750Data = controlador.leerBH();
  mqData = controlador.leerMQ(dhtData.temperatura, dhtData.humedadRelativa);

  bool local = false;

  if (!local){
    // Crear mensaje JSON
    snprintf(mensaje, 256, 
      "{"
      "\"temperaturaDHT\": %.2f, "
      "\"humedadRelativa\": %.2f, "
      "\"sensacionTermica\": %.2f, "
      "\"temperaturaBMP\": %.2f, "
      "\"presionAbsoluta\": %.2f, "
      "\"altitud\": %.2f, "
      "\"presionAlNivelDelMar\": %.2f, "
      "\"luminosidad\": %.2f, "
      "\"ppmCO2\": %.2f, "
      "\"vientoVelocidad\": \"Proximamente\", "
      "\"vientoDireccion\": \"Proximamente\", "
      "\"lluvia\": \"Proximamente\""
      "}", 
      dhtData.temperatura, dhtData.humedadRelativa, dhtData.sensacionTermica, 
      bmpData.temperatura, bmpData.presionAbsoluta, bmpData.altitud, 
      bmpData.presionAlNivelDelMar, bh1750Data, mqData.ppmCO2);

    // Publicar datos en formato JSON
    client.publish("TOPIC_GENERAL", mensaje);
  } else {
    // Crear mensaje JSON para el modo local
    snprintf(mensaje, 512,
      "{"
      "\"BMP180\": {\"estado\": \"OK\", \"temperatura\": %.2f, \"presAbs\": %.2f, \"presNivlMar\": %.2f, \"altit\": %.2f}, "
      "\"BH1750\": {\"lux\": %.2f}, "
      "\"DHT11\": {\"humedad\": %.2f, \"temperatura\": %.2f, \"sensacionTermica\": %.2f}, "
      "\"MQ135\": {\"rzero\": %.2f, \"correctRZero\": %.2f, \"res\": %.2f, \"ppmCO2\": %.2f, \"ppmCorreg\": %.2f}"
      "}",
      bmpData.temperatura, bmpData.presionAbsoluta, bmpData.presionAlNivelDelMar, bmpData.altitud, 
      bh1750Data, 
      dhtData.humedadRelativa, dhtData.temperatura, dhtData.sensacionTermica, 
      mqData.rzero, mqData.zeroCorregido, mqData.resistencia, mqData.ppmCO2, mqData.ppmCorregidas);

    // Publicar datos en formato JSON
    client.publish("TOPIC_LOCAL", mensaje);
  }
}


