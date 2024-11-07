#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "../lib/LeerSensores.h"
#include "../lib/config.h"
#include "../lib/FormWiFi.h"
#include "esp_sleep.h"
#include <WiFiClientSecure.h>

char mensaje[80];

WiFiClient esp_EME;
//ControladorWiFi controlerWiFi;
// ManejoDatosWifi dataHandler;
LeerSensoresControlador controladorSensores;



#define ssid RED_SSID_WIFI
#define pass PASSWORD_WIFI


#define uS_TO_S_FACTOR 1000000  // Conversión de segundos a microsegundos
#define TIME_TO_SLEEP_15_MIN  15 * 60  // Tiempo en segundos (15 minutos)
#define TIME_TO_SLEEP_5_SEG  5   // Tiempo en segundos (5seg
#define TIME_TO_SLEEP_2_MIN  2 * 60   // Tiempo en segundos (2min)

void leerSensores();



String jsonMaker(
    LeerSensoresControlador::datosDHT dhtData,
    LeerSensoresControlador::datosMQ mqData,
    LeerSensoresControlador::datosBMP bmpData,    
    float bhData,
    float velViento,
    String dirViento,
    float lluvia
)
{
  String postData;
  if (lluvia < 0 || velViento < 0)
  {
    // Crear los datos JSON para enviar (temperatura y humedad)
    postData = "{\"DHT_temperatura\":" + String(dhtData.temperatura) + 
                      ",\"DHT_humedad\":" + String(dhtData.humedadRelativa) + 
                      ",\"DHT_sensasionTerm\":" + String(dhtData.sensacionTermica) + 
                      ",\"MQ_ppmCO2\":" + String(mqData.ppmCO2) + 
                      ",\"BMP_presion\":" + String(bmpData.presionAbsoluta) + 
                      ",\"BMP_temperatura\":" + String(bmpData.temperatura) + 
                      ",\"BMP_altitud\":" + String(bmpData.altitud) + 
                      ",\"BH_lumines\":" + String(bhData) + 
                      ",\"VelocidadViento\":" + "Proximamente" + 
                      ",\"DireccionViento\":" + "Proximamente" + 
                      ",\"Lluvia\":" + "Proximamente" + 
                      "}";
  } else {
    // Crear los datos JSON para enviar (temperatura y humedad)
    postData = "{\"DHT_temperatura\":" + String(dhtData.temperatura) + 
                      ",\"DHT_humedad\":" + String(dhtData.humedadRelativa) + 
                      ",\"DHT_sensasionTerm\":" + String(dhtData.sensacionTermica) + 
                      ",\"MQ_ppmCO2\":" + String(mqData.ppmCO2) + 
                      ",\"BMP_presion\":" + String(bmpData.presionAbsoluta) + 
                      ",\"BMP_temperatura\":" + String(bmpData.temperatura) + 
                      ",\"BMP_altitud\":" + String(bmpData.altitud) + 
                      ",\"BH_lumines\":" + String(bhData) + 
                      ",\"VelocidadViento\":" + String(velViento) + 
                      ",\"DireccionViento\":" + dirViento + 
                      ",\"Lluvia\":" + String(lluvia) + 
                      "}";
    
  }
  return postData;
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
  controladorSensores.initControlador(BMP_TYPE_180);

  //  Inicializar conexión a la red
  // setupWifi();
  
  // Serial.print("Entrando a modo Deep Sleep\n");
  // esp_deep_sleep_start();
}

void loop()
{
  envioMQTT();
  delay(2 * 60 * 1000);
  //delay(TIME_TO_SLEEP_5_SEG * 1000);
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
  float velViento = -1;
  String dirViento = "";
  float lluvia = -1;

  //  Lectura y asignación de párametros de los sensores
  dhtData = controlador.leerDHT();
  bmpData = controlador.leerBMP();
  bh1750Data = controlador.leerBH();
  mqData = controlador.leerMQ(dhtData.temperatura, dhtData.humedadRelativa);
  // mqData = controlador.leerMQ();

  bool local = false;

  if (!local){
    // Publicar temperatura ETec_broker
    snprintf(mensaje, 20, "%.2f °C", dhtData.temperatura);
    client.publish(TOPIC_DHT_TEMP, mensaje);
    // Publicar humedad ETec_broker
    snprintf(mensaje, 20, "%.2f %%HR", dhtData.humedadRelativa);
    client.publish(TOPIC_DHT_HUM, mensaje);
    // Publicar sensación térmica ETec_broker
    snprintf(mensaje, 20, "%.2f °C", dhtData.sensacionTermica);
    client.publish(TOPIC_DHT_SENST, mensaje);

    // Publicar temperatura bmp ETec_broker
    snprintf(mensaje, 20, "%.2f °C", bmpData.temperatura);
    client.publish(TOPIC_BMP_TEMP, mensaje);
    // Publicar presion ETec_broker
    snprintf(mensaje, 20, "%.2f hPa", bmpData.presionAbsoluta);
    client.publish(TOPIC_BMP_PRES, mensaje);
    // Publicar altitud ETec_broker
    snprintf(mensaje, 20, "%.2f m", bmpData.altitud);
    client.publish(TOPIC_BMP_ALT, mensaje);
    // Publicar presion a nivel del mar ETec_broker
    snprintf(mensaje, 20, "%.2f hPa", bmpData.presionAlNivelDelMar);
    client.publish(TOPIC_BMP_PRESNM, mensaje);

    // Publicar luz ETec_broker
    snprintf(mensaje, 20, "%.2f lm", bh1750Data);
    client.publish(TOPIC_BH_LUZ, mensaje);

    // Publicar ppmCO2 ETec_broker
    snprintf(mensaje, 20, "%.2f ppmCO2", mqData.ppmCO2);
    client.publish(TOPIC_MQ_PPMCO2, mensaje);

    // Publicar velocidad del Viento ETec_broker
    snprintf(mensaje, 20, "Proximamente");
    client.publish(TOPIC_VIENTO_VELOCIDAD, mensaje);
    // Publicar dirección del viento ETec_broker
    snprintf(mensaje, 20, "Proximamente");
    client.publish(TOPIC_VIENTO_DIRECCION, mensaje);

    // Publicar lluvia ETec_broker
    snprintf(mensaje, 20, "Proximamente");
    client.publish(TOPIC_YL_LLUVIA, mensaje);
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

