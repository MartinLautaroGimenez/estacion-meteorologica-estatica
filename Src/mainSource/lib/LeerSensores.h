#ifndef LeerSensores_h
#define LeerSensores_h

// Inclusión de librerías
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

// Definir aquí la versión de la PCB
#define PCB_VERSION_2_3_7 // Comentar o cambiar por PCB_VERSION_2 según corresponda

// Configuración de pines según la versión de la PCB
#ifdef PCB_VERSION_1
    /**DEFINICIONES**/
    //  Definiciones del sensor DHT22
    #define DHT_PIN 15
    // #define DHT_PIN 12 //PCB vers
    #define DHT_TYPE_22 "DHT22"
    #define DHT_TYPE_11 "DHT11"
    #define DHT_TYPE_AM2302 "AM2302"
    //  Definiciones del sensor MQ135
    #define PIN_MQ135 35 // MQ135 Analog Input Pin
    #define PIN_MQ135_D0 34 // MQ135 Analog Input Pin
    //  Definiciones del sensor BMP's
    #define ALTITUDE 1655.0 // Altitude of SparkFun's HQ in Boulder, CO. in meters
    #define BMP_TYPE_180 "BMP180"
    #define BMP_TYPE_280 "BMP280"
    // #define BMP280_ADDRESS (0x77) /< The default I2C address for the sensor. */
    // #define BMP280_ADDRESS_ALT (0x76)    /< Alternative I2C address for the sensor. /
    // #define BMP280_CHIPID (0x58)  /**< Default chip ID./
    #define BMP_SCK (18)
    #define BMP_MISO (19)
    #define BMP_MOSI (23)
    #define BMP_CS (5)
    //  Definiciones del sensor BH1750
    #define BH1750_ADDRESS 0X23
    // Definición de pines para Sensor de Hoja Mojada
    #define SHM_Ax 39
    #define SHM_Dx 36
    // Definición de pines para Veleta
    #define S0 32
    #define S1 33
    #define S2 25
    // Definición de pines para Anemometro
    #define HALL_Sensor 2
    // Definición de pines para Ground 1 y Ground 2
    #define PIN_GROUND_1 27
    #define PIN_GROUND_2 14
#elif defined(PCB_VERSION_2_3_7)
    /**DEFINICIONES**/
    //  Definiciones del sensor DHT22
    #define DHT_PIN 12
    //  Definiciones del sensor MQ135
    #define PIN_MQ135 35 // MQ135 Analog Input Pin
    #define PIN_MQ135_D0 34 // MQ135 Analog Input Pin
    // #define BMP280_ADDRESS (0x77) /< The default I2C address for the sensor. */
    // #define BMP280_ADDRESS_ALT (0x76)    /< Alternative I2C address for the sensor. /
    // #define BMP280_CHIPID (0x58)  /**< Default chip ID./
    // #define BMP_SCK (18)
    // #define BMP_MISO (19)
    // #define BMP_MOSI (23)
    // #define BMP_CS (5)
    //  Definiciones del sensor BH1750
    #define BH1750_ADDRESS 0X23
    // Definición de pines para Sensor de Hoja Mojada
    #define SHM_Ax 39
    #define SHM_Dx 36
    // Definición de pines para Veleta
    #define S0 32
    #define S1 33
    #define S2 13
    #define Y 26
    // Definición de pines para Anemometro
    #define HALL_Sensor 13
    // Definición de pines para Ground 1 y Ground 2
    #define PIN_GROUND_1 14
    #define PIN_GROUND_2 15
#else
    #error "Debe definir una versión de PCB (PCB_VERSION_1 o PCB_VERSION_2)"
#endif

// Definiciones del sensor DHT
#define DHT_TYPE_22 "DHT22"
#define DHT_TYPE_11 "DHT11"
#define DHT_TYPE_AM2302 "AM2302"

// Definiciones del sensor BMP's
#define ALTITUDE 1655.0
#define BMP_TYPE_180 "BMP180"
#define BMP_TYPE_280 "BMP280"
#define BH1750_ADDRESS 0x23

// Definición de estructura para el MQ135
struct datosVELOCIDADES
{
    float velocidadVientoAngular;
    float velocidadVientoTangencial;
};

// Clase controladora de lectura de sensores
class LeerSensoresControlador
{
public:
    struct datosDHT
    {
        float humedadRelativa;
        float temperatura;
        float sensacionTermica;
    };

    struct datosBMP
    {
        double temperatura;
        double presionAbsoluta;
        double presionAlNivelDelMar;
        double altitud;
    };

    struct datosMQ
    {
        float rzero;
        float zeroCorregido;
        float resistencia;
        float ppmCO2;
        float ppmCorregidas;
    };

    struct datosYL
    {
        int analogico;
        boolean digital;
    };

    const char *BMP_SELECTED;
    const char *DHT_SELECTED;
    DHTesp::DHT_MODEL_t DHT_TYPE;

    float temperatura, humedadRelativa;

    LeerSensoresControlador();
    ~LeerSensoresControlador();

    void initControlador(const char *tipoDeBMP, const char *tipoDeDHT);
    datosDHT leerDHT();
    datosMQ leerMQ();
    datosMQ leerMQ(float temperature, float humidity);
    datosBMP leerBMP();
    float leerBH();
    float leerAnemometro();
    String leerVeleta();
    datosYL leerHojaMojada();
    void alimentaciones(boolean g1, boolean g2);
    String alimentaciones(String state);

private:
    static datosVELOCIDADES velocidades;
    DHTesp dht;
    Adafruit_BMP280 bmp;
    SFE_BMP180 pressure;
    BH1750 lightMeter;
    MQ135 mq135_sensor;

    static void contadorPulsos();
    static void calcularVelocidad(unsigned long pulsos);
    static volatile unsigned long pulseCount;
};

#endif
