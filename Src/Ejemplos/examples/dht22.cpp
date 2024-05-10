#include "DHT.h"

const int pinDatos =  22;


DHT dhtsensor (pinDatos, DHT22);    //Crea objeto sensorTH

void setup() {

 Serial.begin (115200);
 Serial.println ("***** Lectura DHT22 *****");

 dhtsensor.begin ();   //Inicializa pines del sensor
 
}

void loop() {

  //Lee valores de temperatura y humedad
  float humedad = dhtsensor.readHumidity();
  float temperatura = dhtsensor.readTemperature();
  
  //Mostrar valores leídos
  Serial.print ("Temperatura = ");
  Serial.print (temperatura);
  Serial.println (" ºC");
  Serial.print ("Humedad = ");
  Serial.print (humedad);
  Serial.println (" %");

  delay (2000);
  
}