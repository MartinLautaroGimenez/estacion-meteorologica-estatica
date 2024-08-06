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
// #define WAKEUP_PIN GPIO_NUM_33


// RTC_DATA_ATTR int bootCount = 0;

// void print_wakeup_reason() {
//     esp_sleep_wakeup_cause_t wakeup_reason;  
//     wakeup_reason = esp_sleep_get_wakeup_cause();

//     switch (wakeup_reason) {
//         case ESP_SLEEP_WAKEUP_TIMER:
//             Serial.println("Wakeup caused by timer");
//             break;
//         case ESP_SLEEP_WAKEUP_EXT0:
//             Serial.println("Wakeup caused by external signal using RTC_IO");
//             break;
//         case ESP_SLEEP_WAKEUP_EXT1:
//             Serial.println("Wakeup caused by external signal using RTC_CNTL");
//             break;
//         case ESP_SLEEP_WAKEUP_TOUCHPAD:
//             Serial.println("Wakeup caused by touchpad");
//             break;
//         case ESP_SLEEP_WAKEUP_ULP:
//             Serial.println("Wakeup caused by ULP program");
//             break;
//         default:
//             Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
//             break;
//     }
// }


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
  delay(TIME_TO_SLEEP_15_MIN * 1000 * 60);
}


void envioMQTT(){
//  Reconectar si se ha desconectado del Broker
  if (!client.connected())
  {
    reconectar();
  }
  client.loop();

  //  Declaración de variables de lectura para sensores tomando cómo estructura de datos la de la clase LeerSensoresControlador
  LeerSensoresControlador::datosBMP bmpData;
  LeerSensoresControlador::datosDHT dhtData;
  LeerSensoresControlador::datosMQ mqData;
  float bh1750Data;

  //  Lectura y asignación de párametros de los sensores
  dhtData = controlador.leerDHT();
  bmpData = controlador.leerBMP();
  bh1750Data = controlador.leerBH();
  mqData = controlador.leerMQ(dhtData.temperatura, dhtData.humedadRelativa);
  // mqData = controlador.leerMQ();

  bool local = false;
  // bool local = true;

  if (!local){
    // Publicar temperatura ETec_broker
    snprintf(mensaje, 20, "%.2f °C", (bmpData.temperatura + dhtData.temperatura)/2 );
    client.publish(temperaturaDHT, mensaje);
    // Publicar humedad ETec_broker
    snprintf(mensaje, 20, "%.2f %%HR", dhtData.humedadRelativa);
    client.publish(humedadDHT, mensaje);
    // Publicar presion ETec_broker
    snprintf(mensaje, 20, "%.2f Pa", bmpData.presionAbsoluta);
    client.publish(presionBMP, mensaje);
    // Publicar luz ETec_broker
    snprintf(mensaje, 20, "%.2f lux", bh1750Data);
    client.publish(luminosidad, mensaje);
    // Publicar ppmCO2 ETec_broker
    snprintf(mensaje, 20, "%.2f ppmCO2", mqData.ppmCO2);
    client.publish(calidadAire, mensaje);
    // Publicar velocidad del Viento ETec_broker
    snprintf(mensaje, 20, "Proximamente");
    client.publish(velocidadViento, mensaje);
    // Publicar dirección del viento ETec_broker
    snprintf(mensaje, 20, "Proximamente");
    client.publish(direccionViento, mensaje);
    // Publicar lluvia ETec_broker
    snprintf(mensaje, 20, "Proximamente");
    client.publish(sensorDeLluvia, mensaje);

  } else {
      // Publicar datos del BMP180
    snprintf(mensaje, 75,
            "estado:OK temperatura:%.2f presAbs:%.2f presNivlMar:%.2f altit:%.2f",
            // bmpData.estado,
            bmpData.temperatura,
            bmpData.presionAbsoluta,
            bmpData.presionAlNivelDelMar,
            bmpData.altitud);
    client.publish(senBMP180, mensaje);

    // Publicar datos del BH1750
    snprintf(mensaje, 20, "Lux:%.2f", bh1750Data);
    client.publish(senBH1750, mensaje);

    // Publicar datos del DHT11
    snprintf(mensaje, 40, "hum:%.2f temp:%.2f sensacionTerm:%.2f",
            // dhtData.estado,
            dhtData.humedadRelativa,
            dhtData.temperatura,
            dhtData.sensacionTermica);
    client.publish(senDHT11, mensaje);

    // Publicar datos del MQ135
    snprintf(mensaje, 75, "rzero:%.2f correctRZero:%.2f res:%.2f ppmCO2:%.2f ppmCorreg:%.2f",
            mqData.rzero,
            mqData.zeroCorregido,
            mqData.resistencia,
            mqData.ppmCO2,
            mqData.ppmCorregidas);
    client.publish(senMQ135, mensaje);
  }
}

