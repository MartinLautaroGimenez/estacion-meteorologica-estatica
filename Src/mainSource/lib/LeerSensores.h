#ifndef LeerSensores_h
#define LeerSensores_h

//  Inclusión de librerías
#include <DHTesp.h>
#include <MQ135.h>
#include <Wire.h>
#include <SPI.h>
#include <BH1750.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <SFE_BMP180.h>
#include <math.h>

/*****************DEFINICIONES*************/
//  Definiciones del sensor DHT22
#define DHT_PIN 18
#define DHT_TYPE DHTesp::DHT22
//  Definiciones del sensor MQ135
#define PIN_MQ135 35 // MQ135 Analog Input Pin
//  Definiciones del sensor BMP's
#define ALTITUDE 1655.0 // Altitude of SparkFun's HQ in Boulder, CO. in meters
#define BMP_TYPE_180 "BMP180"
#define BMP_TYPE_280 "BMP280"
// #define BMP280_ADDRESS (0x77) /**< The default I2C address for the sensor. */
// #define BMP280_ADDRESS_ALT (0x76)    /**< Alternative I2C address for the sensor. */
// #define BMP280_CHIPID (0x58)  /**< Default chip ID. */
#define BMP_SCK (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS (10)
//  Definiciones del sensor BH1750
#define BH1750_ADDRESS 0X23

//  Clase controladora de leer los sensores de la estación
class LeerSensoresControlador
{
public:
    // Estructuras de datos
    //  Estructura de datos para el DHT22
    struct datosDHT
    {
        //  char *estado;
        float humedadRelativa;
        float temperatura;
        float sensacionTermica;
    };
    //  Estructura de datos para el BMP180
    struct datosBMP
    {
        // char estado;
        double temperatura;
        double presionAbsoluta;
        double presionAlNivelDelMar;
        double altitud;
    };
    //  Estructura de datos para el MQ135
    struct datosMQ
    {
        float rzero;
        float zeroCorregido;
        float resistencia;
        float ppmCO2;
        float ppmCorregidas;
    };
    //  Declaración de atributos
    const char *BMP_SELECTED;
    float temperatura, humedadRelativa;
    //  Declaración de Métodos
    LeerSensoresControlador();
    void initControlador(const char *sensor);
    datosDHT leerDHT();
    datosMQ leerMQ();
    datosMQ leerMQ(float temperature, float humidity);
    datosBMP leerBMP();
    float leerBH();

private:
    DHTesp dht;
    Adafruit_BMP280 bmp; // I2C
    // Adafruit_BMP280 bmp(BMP_CS); // hardware SPI
    // Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);
    SFE_BMP180 pressure;
    BH1750 lightMeter;
    MQ135 mq135_sensor;
};

#endif