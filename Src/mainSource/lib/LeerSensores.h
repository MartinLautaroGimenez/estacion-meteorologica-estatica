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
#define DHT_PIN 15
#define DHT_TYPE_22 "DHT22"
#define DHT_TYPE_11 "DHT11"
#define DHT_TYPE_AM2302 "AM2302"
//  Definiciones del sensor MQ135
#define PIN_MQ135 35 // MQ135 Analog Input Pin
//  Definiciones del sensor BMP's
#define ALTITUDE 1655.0 // Altitude of SparkFun's HQ in Boulder, CO. in meters
#define BMP_TYPE_180 "BMP180"
#define BMP_TYPE_280 "BMP280"
// #define BMP280_ADDRESS (0x77) /**< The default I2C address for the sensor. */
// #define BMP280_ADDRESS_ALT (0x76)    /**< Alternative I2C address for the sensor. */
// #define BMP280_CHIPID (0x58)  /**< Default chip ID. */
#define BMP_SCK (18)
#define BMP_MISO (19)
#define BMP_MOSI (23)
#define BMP_CS (5)
//  Definiciones del sensor BH1750
#define BH1750_ADDRESS 0X23
// Definición de pines para Sensor de Hoja Mojada
#define SHM_Ax 12
#define SHM_Dx 13
// Definición de pines para Veleta
#define S0 32
#define S1 33
#define S2 25
// Definición de pines para Anemometro
#define HALL_Sensor 2
// Definición de pines para Ground 1 y Ground 2
#define PIN_GROUND_1 27
#define PIN_GROUND_2 14

//  Clase controladora de leer los sensores de la estación
class LeerSensoresControlador
{
public:
    // Estructuras de datos //  
    
    /*!
    *  Estructura de datos para el DHT.
    *  @param humedadRelativa
    *           Float: Valor de humedad relativa.
    *  @param temperatura
    *           Float: Valor de Temperatura.
    *  @param sensacionTermica
    *           Float: Valor de Sensación térmica.
    *  @warning 
    *           Tener en cuenta el orden en que aparecen!
    *  @author Mario-dango
    */
    struct datosDHT
    {
        //  char *estado;
        float humedadRelativa;
        float temperatura;
        float sensacionTermica;
    };
    
    /*!
    *  Estructura de datos para el BMP180.
    *  @param temperatura
    *           double: Valor de humedad Temperatura.
    *  @param presionAbsoluta
    *           double: Valor de Presipon Absoluta.
    *  @param presionAlNivelDelMar
    *           double: Valor de Presipon a Nivel del Mar.
    *  @param altitud
    *           double: Valor de Altiitud.
    *  @warning 
    *           Tener en cuenta el orden en que aparecen!
    *  @author Mario-dango
    */
    struct datosBMP
    {
        // char estado;
        double temperatura;
        double presionAbsoluta;
        double presionAlNivelDelMar;
        double altitud;
    };
    
    /*!
    *  Estructura de datos para el MQ135.
    *  @param rzero
    *           Float: valor de Resistencia Zero.
    *  @param zeroCorregido
    *           Float: valor de Resistencia Zero Corregida.
    *  @param resistencia
    *           Float: valor de Resistencia del Sensor.
    *  @param ppmCO2
    *           Float: valor de Partes Por Millón de CO2.
    *  @param ppmCorregidas
    *           Float: valor de Partes Por Millón de CO2 Corregidas.
    *  @warning 
    *           Tener en cuenta el orden en que aparecen!
    *  @author Mario-dango
    */
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
    const char *DHT_SELECTED;
    DHTesp::DHT_MODEL_t DHT_TYPE;

    float temperatura, humedadRelativa;
    //  Declaración de Métodos //
    
    /*!
    *  Constructor de la clase LeerSensoresControlador.
    *  @brief   
    *           Constructor 
    *  @warning 
    *           Necesaria para crear el objeto controlador.
    */
    LeerSensoresControlador();
    
    /*!
    *  Destructor de la clase LeerSensoresControlador.
    *  @brief   
    *           Destructor 
    *  @warning 
    *           Necesaria para Destruir el objeto controlador.
    */
    ~LeerSensoresControlador();
    
    /*!
    *  Inicialización del controlador y sus comunicaciones a los sensores.
    *  @param tipoDeBMP
    *           Pasa por String el tipo de Sensor BMP que se utiliza, opcioens (DHT11/DHT22/AM2302/AUTO_DETECT)
    *  @param tipoDeDHT
    *           Pasa por String el tipo de Sensor DHT que se utiliza, opcioens (DHT11/DHT22/AM2302/AUTO_DETECT)
    *  @return 
    *           Devuelve nada.
    *  @warning 
    *           Especificar tipo de seosnres a usar, si no se inicializa no habrá comunicación con sensores.
    */
    void initControlador(const char *tipoDeBMP, const char *tipoDeDHT);
    
    /*!
    *  Método Lectura del DHT22.
    *  @return 
    *           Devuelve lectura en una estructura de dato tipo "datosDHT".
    *  @warning 
    *           Si no se activan G1 y G2 no van a funcionar los sensores, ni las comunicaciones
    */
    datosDHT leerDHT();

    /*!
    *  Método Lectura del MQ135.
    *  @return 
    *           Devuelve lectura en una estructura de dato tipo "datosMQ".
    */
    datosMQ leerMQ();

    /*!
    *  Método Lectura del MQ135.
    *  @param temperature
    *           Pasa el valor de temperatura para tener una lectura precisa.
    *  @param humidity
    *           Pasa el valor de huemdad para tener una lectura precisa
    *  @return 
    *           Devuelve lectura en una estructura de dato tipo "datosMQ".
    */
    datosMQ leerMQ(float temperature, float humidity);

    /*!
    *  Método Lectura del BMP.
    *  @return 
    *           Devuelve lectura en una estructura de dato tipo "datosBMP".
    */
    datosBMP leerBMP();

    /*!
    *  Método Lectura del BH1750.
    *  @return 
    *           Devuelve lectura en una estructura de dato tipo "float".
    */
    float leerBH();
    
    /*!
    *  Método Lectura del Anemometro.
    *  @return 
    *           Devuelve lectura en una estructura de dato tipo "float".
    */
    float leerAnemometro();
    
    /*!
    *  Método Lectura de la veleta.
    *  @return 
    *           Devuelve lectura en una estructura de dato tipo "float".
    */
    float leerVeleta();
    
    /*!
    *  Método Lectura del Sensor de Hoja Mojada.
    *  @return 
    *           Devuelve lectura en una estructura de dato tipo "float".
    */
    float leerHojaMojada();
    
    /*!
    *  Método Lectura del BH1750.
    *  @return 
    *           Devuelve lectura en una estructura de dato tipo "float".
    */
    float leerBH();
    
    /*!
    *  Alimentaciones.
    *  @param g1
    *           Pasa directamente el estado booleano al MOSFET G1.
    *  @param g2
    *           Pasa directamente el estado booleano al MOSFET G2.
    *  @return 
    *           Devuelve nada.
    *  @warning 
    *           Si no se activan G1 y G2 no van a funcionar los sensores, ni las comunicaciones
    */
    void alimentaciones(boolean g1, boolean g2);

    /*!
    *  Alimentaciones.
    *  @param state
    *         Se evalua un String, siendo 
    *           on: encendido total (G1 y G2)
    *           off: Apagado total (G1 y G2)
    *           g1: Encendido de G1
    *           g2: Encendido de G2
    *           default: devuelve argumento invalido.
    *  @return 
    *           Devuelve El resultado del estado actual de los MOSFET.
    *  @warning 
    *           Si no se activan G1 y G2 no van a funcionar los sensores, ni las comunicaciones
    */
    String alimentaciones(String state);

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