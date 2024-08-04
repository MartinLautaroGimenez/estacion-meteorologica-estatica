#include <Arduino.h>
#include "../lib/LeerSensores.h"
LeerSensoresControlador controlador("bmp180");

#include <DHTesp.h>
DHTesp dht;
#define DHT_PIN 18
#define DHT_TYPE DHTesp::DHT22

#include <MQ135.h>
#define PIN_MQ135 35 // MQ135 Analog Input Pin
MQ135 mq135_sensor(PIN_MQ135);


#include <SFE_BMP180.h>
#include <Wire.h>  //GPIO22 es el SCL  y el GPIO21 es el SDA en el DOIT esp32 DEVkit
// You will need to create an SFE_BMP180 object, here called "pressure":
//  Estructura de datos para el BMP180
SFE_BMP180 pressure;
struct datosBMP180{
  // char estado;
  double temperatura;
  double presionAbsoluta;
  double presionAlNivelDelMar;
  double altitud;
};

//  Estructura de datos para el DHT22
struct datosDHT22{
//  char *estado;
 float humedadRelativa;
 float temperatura;
 float sensacionTermica;
};

//  Estructura de datos para el MQ135
struct datosMQ135{
  float rzero;
  float zeroCorregido;
  float resistencia;
  float ppmCO2;
  float ppmCorregidas;
};

#define ALTITUDE 1655.0 // Altitude of SparkFun's HQ in Boulder, CO. in meters
/*

Advanced BH1750 library usage example

This example has some comments about advanced usage features.

Connections

  - VCC to 3V3 or 5V
  - GND to GND
  - SCL to SCL (A5 on Arduino Uno, Leonardo, etc or 21 on Mega and Due, on
    esp8266 free selectable)
  - SDA to SDA (A4 on Arduino Uno, Leonardo, etc or 20 on Mega and Due, on
    esp8266 free selectable)
  - ADD to (not connected) or GND

ADD pin is used to set sensor I2C address. If it has voltage greater or equal
to 0.7VCC voltage (e.g. you've connected it to VCC) the sensor address will be
0x5C. In other case (if ADD voltage less than 0.7 * VCC) the sensor address
will be 0x23 (by default).

*/
#include <BH1750.h>

/*
  BH1750 can be physically configured to use two I2C addresses:
    - 0x23 (most common) (if ADD pin had < 0.7VCC voltage)
    - 0x5C (if ADD pin had > 0.7VCC voltage)

  Library uses 0x23 address as default, but you can define any other address.
  If you had troubles with default value - try to change it to 0x5C.

*/
BH1750 lightMeter(0x23);

unsigned long lastMillis = 0;

#include <WiFi.h>
#include <PubSubClient.h>
const char* ssid = "momantai";
const char* pass = "42425640";
const char* brokerUser = "EMM";
const char* brokerPass = "public";
const char* broker = "192.168.100.12";
const char* senBH1750 = "EMM/bh1750";
const char* senBMP180 = "EMM/bmp180";
const char* senDHT11 = "EMM/dht11";
const char* senMQ135 = "EMM/mq135";

const char* chau = "EMM/test";
char mensaje[80];

WiFiClient esp_EMM;
PubSubClient client(esp_EMM);

void setupWifi(){
  delay(100);
  Serial.print("\nConectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while(WiFi.status() != WL_CONNECTED){
    delay(200);
    Serial.print(".-");
  }

  Serial.print("\nConectado a la red: ");
  Serial.println(ssid);
  Serial.print("\nCon la siguiente IP: ");
  Serial.println(WiFi.localIP());
  
}

void reconectar(){
  Serial.print("\nConectando al broker: ");
  Serial.println(broker);
  while(!client.connected()){
    if (client.connect("EME")){
      Serial.print("\nConectado al broker: ");
      Serial.println(broker);
      client.subscribe(chau);
    } else {
      Serial.print("Error de conexión, rc=");
      Serial.print(client.state());
      Serial.println(".-");
      delay(1000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Mensaje recibido: ");
  Serial.println(topic);
  for(int i=0; i<length; i++){
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

datosDHT22 leerDHT22(){
  delay(dht.getMinimumSamplingPeriod());

  const char* estado = dht.getStatusString();
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  float sensacion = dht.computeHeatIndex(temperature, humidity, false);
/*
  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.print(dht.toFahrenheit(temperature), 1);
  Serial.print("\t\t");
  Serial.print(dht.computeHeatIndex(temperature, humidity, false), 1);
  Serial.print("\t\t");
  Serial.println(dht.computeHeatIndex(dht.toFahrenheit(temperature), humidity, true), 1);
  */
  // Serial.print(dht.getStatusString());
  Serial.print("\nA continuación: Datos leidos del sensor DHT11\n");
  Serial.print("Humedad de: ");
  Serial.print(humidity);
  Serial.print("\t\tTemperatura de:");
  Serial.print(temperature, 1);
  Serial.print("\t\tSensación Térmica calculada de:");
  Serial.print(dht.computeHeatIndex(temperature, humidity, false), 1);
  return {humidity, temperature, sensacion};
}

datosBMP180 leerBMP180(){

  Serial.print("\n\nA continuación: Datos leidos del sensor BMP180\n");
  char status;
  double T,P,p0,a;
  Serial.print("altitud proporcionada: ");
  Serial.print(ALTITUDE,0);
  Serial.print(" m \t\t");
  // Serial.print(ALTITUDE*3.28084,0);
  // Serial.println(" feet");
  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Print out the measurement:
      Serial.print("temperatura: ");
      Serial.print(T,2);
      Serial.println(" °C \t\t");
      // Serial.print((9.0/5.0)*T+32.0,2);
      // Serial.println(" deg F");
      
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          // Print out the measurement:
          Serial.print("Presión absoluta: ");
          Serial.print(P,2);
          Serial.print(" mb \t\t");
          // Serial.print(P*0.0295333727,2);
          // Serial.println(" inHg");

          // The pressure sensor returns abolute pressure, which varies with altitude.
          // To remove the effects of altitude, use the sealevel function and your current altitude.
          // This number is commonly used in weather reports.
          // Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
          // Result: p0 = sea-level compensated pressure in mb

          p0 = pressure.sealevel(P,ALTITUDE); // we're at 1655 meters (Boulder, CO)
          Serial.print("Presión relativa (a nivel del mar): ");
          Serial.print(p0,2);
          Serial.print(" mb \t\t");
          // Serial.print(p0*0.0295333727,2);
          // Serial.println(" inHg");

          // On the other hand, if you want to determine your altitude from the pressure reading,
          // use the altitude function along with a baseline pressure (sea-level or other).
          // Parameters: P = absolute pressure in mb, p0 = baseline pressure in mb.
          // Result: a = altitude in m.

          a = pressure.altitude(P,p0);
          Serial.print("Altitud calculada: ");
          Serial.print(a,0);
          Serial.println(" m");
          // Serial.print(a*3.28084,0);
          // Serial.println(" feet");

          return {T, P, p0, a};
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");
  
  return {0,0,0,0};
}

datosMQ135 leerMQ135(float temperature, float humidity){
  
  float rzero = mq135_sensor.getRZero();
  float correctedRZero = mq135_sensor.getCorrectedRZero(temperature, humidity);
  float resistance = mq135_sensor.getResistance();
  float ppm = mq135_sensor.getPPM();
  float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity);

  Serial.print("\n\nA continuación: Datos leidos del sensor MQ135\n");

  Serial.print("MQ135 RZero: ");
  Serial.print(rzero);
  Serial.print("\t Corrected RZero: ");
  Serial.print(correctedRZero);
  Serial.print("\t Resistance: ");
  Serial.print(resistance);
  Serial.print("\t PPM: ");
  Serial.print(ppm);
  Serial.print(" ppm");
  Serial.print("\t Corrected PPM: ");
  Serial.print(correctedPPM);
  Serial.println(" ppm");
  
  return {rzero, correctedRZero, resistance, ppm, correctedPPM};
}

float leerBH1750(){
  Serial.print("\nA continuación: Datos leidos del sensor BH1750\n");
  float lux = 0;
  if (lightMeter.measurementReady()) {
    lux = lightMeter.readLightLevel();
    Serial.print("Valor de luminosidad: ");
    Serial.print(lux);
    Serial.print(" lx");
  }
  return lux;
}

unsigned long delayTime;
void setup()
{
  Serial.begin(115200);
  String thisBoard= ARDUINO_BOARD;
  Serial.println(thisBoard);


  dht.setup(DHT_PIN, DHT_TYPE); // Connect DHT sensor to GPIO 18


  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.

    Serial.println("BMP180 init fail\n\n");
    // while(1); // Pause forever.
  }
  
  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  Wire.begin();
  // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);

  /*
    BH1750 has six different measurement modes. They are divided in two groups;
    continuous and one-time measurements. In continuous mode, sensor
    continuously measures lightness value. In one-time mode the sensor makes
    only one measurement and then goes into Power Down mode.

    Each mode, has three different precisions:

      - Low Resolution Mode - (4 lx precision, 16ms measurement time)
      - High Resolution Mode - (1 lx precision, 120ms measurement time)
      - High Resolution Mode 2 - (0.5 lx precision, 120ms measurement time)

    By default, the library uses Continuous High Resolution Mode, but you can
    set any other mode, by passing it to BH1750.begin() or BH1750.configure()
    functions.

    [!] Remember, if you use One-Time mode, your sensor will go to Power Down
    mode each time, when it completes a measurement and you've read it.

    Full mode list:

      BH1750_CONTINUOUS_LOW_RES_MODE
      BH1750_CONTINUOUS_HIGH_RES_MODE (default)
      BH1750_CONTINUOUS_HIGH_RES_MODE_2

      BH1750_ONE_TIME_LOW_RES_MODE
      BH1750_ONE_TIME_HIGH_RES_MODE
      BH1750_ONE_TIME_HIGH_RES_MODE_2

  */
  // begin returns a boolean that can be used to detect setup problems.
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println(F("BH1750 Advanced begin"));
  } else {
    Serial.println(F("Error initialising BH1750"));
  }

  //  Inicializar conexión a la red
  setupWifi();
  client.setServer(broker, 1883);
  client.setCallback(callback);
}

void loop()
{
  //  Reconectar si se ha desconectado del Broker
  if(!client.connected()){
    reconectar();
  }
  client.loop();

  //  Declaración de variables de lectura para sensores
  datosBMP180 bmpData;
  datosDHT22 dhtData;
  datosMQ135 mqData;
  float bh1750Data;

  //  Lectura y asignación de párametros de los sensores
  dhtData = leerDHT22();
  // bmpData = leerBMP180();
  bh1750Data = leerBH1750();
  mqData = leerMQ135(dhtData.temperatura, dhtData.humedadRelativa);

  // // Publicar datos del BMP180
  // snprintf(mensaje, 75, 
  // "estado:%c temperatura:%.2f presAbs:%.2f presNivlMar:%.2f altit:%.2f", 
  //   // bmpData.estado, 
  //   bmpData.temperatura, 
  //   bmpData.presionAbsoluta, 
  //   bmpData.presionAlNivelDelMar,
  //   bmpData.altitud
  // );  
  // client.publish(senBMP180, mensaje);
  
  // Publicar datos del BH1750
  snprintf(mensaje, 20, "Lux:%.2f", bh1750Data);  
  client.publish(senBH1750, mensaje);

  // Publicar datos del DHT11
  snprintf(mensaje, 20, "hum:%.2f temp:%.2f sensacionTerm:%.2f", 
    // dhtData.estado,
    dhtData.humedadRelativa, 
    dhtData.temperatura, 
    dhtData.sensacionTermica
  );  
  client.publish(senDHT11, mensaje);
  
  // Publicar datos del MQ135
  snprintf(mensaje, 75, "rzero:%.2f correctRZero:%.2f res:%.2f ppmCO2:%.2f ppmCorreg:%.2f", 
    mqData.rzero, 
    mqData.zeroCorregido, 
    mqData.resistencia, 
    mqData.ppmCO2, 
    mqData.ppmCorregidas
  );  
  client.publish(senMQ135, mensaje);

  delay(2000);
}

