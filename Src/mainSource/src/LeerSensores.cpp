#include "../lib/LeerSensores.h"


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

LeerSensoresControlador::LeerSensoresControlador()
    : dht(), bmp(), pressure(), lightMeter(BH1750_ADDRESS), mq135_sensor(PIN_MQ135)
{
    //  Método constructor de clase controlador de sensores

    // Defino los pines de ground que controlan los mosfet cómo salidas digitales
    pinMode(PIN_GROUND_1, OUTPUT);
    pinMode(PIN_GROUND_2, OUTPUT);

    // Defino los pines de ground que controlan los mosfet cómo salidas digitales
    pinMode(S0, INPUT);
    pinMode(S1, INPUT);
    pinMode(S2, INPUT);
    
    // Defino los pines de ground que controlan los mosfet cómo salidas digitales
    pinMode(SHM_Dx, INPUT);

    pinMode(HALL_Sensor, INPUT_PULLUP); // Configurar el pin como entrada con resistencia pull-up interna
    attachInterrupt(digitalPinToInterrupt(HALL_Sensor), LeerSensoresControlador::contadorPulsos, RISING); // Configurar la interrupción

    //Una vez definidos los pines se procede a levantarlos
    this->alimentaciones("all");
    // agregamos un pequeño delay para dartiempos a los sensores
    delay(150);
}

// Método destructor de la clase
LeerSensoresControlador::~LeerSensoresControlador()
{
}

//  Método inicializador de comunicación y varios
void LeerSensoresControlador::initControlador(const char *tipoDeBMP, const char *tipoDeDHT)
{
    // Initialize the I2C bus (BH1750 library doesn't do this automatically)
    Wire.begin();
    //  Le asigno al atributo de la instancia el valor tomado del sensor
    this->BMP_SELECTED = tipoDeBMP;
    this->DHT_SELECTED = tipoDeDHT;

    if (strcmp(tipoDeBMP, BMP_TYPE_280) == 0)
    {
        while (!Serial)
        delay(100); // wait for native usb
        unsigned status;
        status = bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
        // status = bmp.begin();
        if (!status)
        {
            Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                             "try a different address!"));
            Serial.print("SensorID was: 0x");
            Serial.println(bmp.sensorID(), 16);
            Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
            Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
            Serial.print("        ID of 0x60 represents a BME 280.\n");
            Serial.print("        ID of 0x61 represents a BME 680.\n");
            while (1)
                delay(10);
        }
        Serial.println("BMP280 init success");
        /* Default settings from datasheet. */
        bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                        Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                        Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                        Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                        Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
    }
    else if (strcmp(tipoDeBMP, BMP_TYPE_180) == 0)
    {
        if (pressure.begin())
            Serial.println("BMP180 init success");
        else
        {
            // Oops, something went wrong, this is usually a connection problem,
            // see the comments at the top of this sketch for the proper connections.

            Serial.println("BMP180 init fail\n\n");
            // while(1); // Pause forever.
        }
    }

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
    if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE))
    {
        Serial.println(F("BH1750 Advanced begin"));
    }
    else
    {
        Serial.println(F("Error initialising BH1750"));
    }

    //  inicializo DHT //
    // Verificar que tipo de DHT se está utilizando
    if (strcmp(DHT_SELECTED, DHT_TYPE_11) == 0){
        DHT_TYPE = DHTesp::DHT11;
    } else if (strcmp(DHT_SELECTED, DHT_TYPE_22) == 0){
        DHT_TYPE = DHTesp::DHT22;
    } else if (strcmp(DHT_SELECTED, DHT_TYPE_AM2302) == 0){
        DHT_TYPE = DHTesp::AM2302;
    } else {
        DHT_TYPE = DHTesp::AUTO_DETECT;
    }
    
    Serial.printf("DHT sensor selected: %s\n", DHT_SELECTED);
    
    this->dht.setup(DHT_PIN, DHT_TYPE);
}
//  Método para leer sensor DHT22
LeerSensoresControlador::datosDHT LeerSensoresControlador::leerDHT()
{
    delay(dht.getMinimumSamplingPeriod());

    //   const char* estado = dht.getStatusString();
    float humidity = dht.getHumidity();
    float temperature = dht.getTemperature();
    float sensacion = dht.computeHeatIndex(temperature, humidity, false);

    // Serial.print(dht.getStatusString());
    String tipo_de_DHT;
    if (DHT_TYPE == DHTesp::DHT11) tipo_de_DHT = "DHT11";
    else if (DHT_TYPE == DHTesp::DHT22) tipo_de_DHT = "DHT22";
    else if (DHT_TYPE == DHTesp::AM2302) tipo_de_DHT = "AM2302";
    else if (DHT_TYPE == DHTesp::AUTO_DETECT) tipo_de_DHT = "AUTO_DETECT";
    Serial.printf("\nA continuación: Datos leidos del sensor %s\n", tipo_de_DHT);
    
    Serial.print("Humedad de: ");
    Serial.print(humidity);
    Serial.print("\t\tTemperatura de:");
    Serial.print(temperature, 1);
    Serial.print("\t\tSensación Térmica calculada de:");
    Serial.println(sensacion, 1);
    return {humidity, temperature, sensacion};
}
//  Métodos para leer sensor MQ135
LeerSensoresControlador::datosMQ LeerSensoresControlador::leerMQ(float temperature, float humidity)
{
    float rzero = mq135_sensor.getRZero();
    float correctedRZero = mq135_sensor.getCorrectedRZero(temperature, humidity);
    float resistance = mq135_sensor.getResistance();
    float ppm = mq135_sensor.getPPM();
    float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity);

    Serial.print("\nA continuación: Datos leidos del sensor MQ135\n");

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

LeerSensoresControlador::datosMQ LeerSensoresControlador::leerMQ()
{
    this->temperatura = this->leerDHT().temperatura;
    this->humedadRelativa = this->leerDHT().humedadRelativa;
    float rzero = mq135_sensor.getRZero();
    float correctedRZero = mq135_sensor.getCorrectedRZero(temperatura, humedadRelativa);
    float resistance = mq135_sensor.getResistance();
    float ppm = mq135_sensor.getPPM();
    float correctedPPM = mq135_sensor.getCorrectedPPM(temperatura, humedadRelativa);

    Serial.print("\nA continuación: Datos leidos del sensor MQ135\n");

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
//  Método para leer sensor BMP280
LeerSensoresControlador::datosBMP LeerSensoresControlador::leerBMP()
{
    Serial.printf("\nA continuación: Datos leidos del sensor %s\n", this->BMP_SELECTED);
    if (strcmp(this->BMP_SELECTED, BMP_TYPE_280) == 0)
    {
        float T, P, p0, a;
        T = bmp.readTemperature();
        P = bmp.readPressure()/100;
        p0 = (P / pow(1 - (ALTITUDE / 44330.0), 5.255))/100;
        a = bmp.readAltitude(1013.25);

        Serial.print(F("Temperature = "));
        Serial.print(T);
        Serial.print(" *C \t\t");

        Serial.print(F("Pressure = "));
        Serial.print(P/100);
        Serial.print(" hPa \t\t");

        Serial.print(F("Approx altitude = "));
        Serial.print(a); /* Adjusted to local forecast! */
        Serial.print(" m \t\t");

        Serial.print(F("Presión a nivel del mar = "));
        Serial.print(p0/100);
        Serial.println(" hPa \t\t");

        return {T, P, p0, a};
    }
    else if (strcmp(this->BMP_SELECTED, BMP_TYPE_180) == 0)
    {
        char status;
        double T, P, p0, a;
        Serial.print("altitud proporcionada: ");
        Serial.print(ALTITUDE, 0);
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
                Serial.print(T, 2);
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

                    /*
                    Retrieve the completed pressure measurement:
                    Note that the measurement is stored in the variable P.
                    Note also that the function requires the previous temperature measurement (T).
                    (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
                    Function returns 1 if successful, 0 if failure.
                    */

                    status = pressure.getPressure(P, T);
                    if (status != 0)
                    {
                        // Print out the measurement:
                        Serial.print("Presión absoluta: ");
                        Serial.print(P, 2);
                        Serial.print(" hPa \t\t");
                        // Serial.print(P*0.0295333727,2);
                        // Serial.println(" inHg");

                        // The pressure sensor returns abolute pressure, which varies with altitude.
                        // To remove the effects of altitude, use the sealevel function and your current altitude.
                        // This number is commonly used in weather reports.
                        // Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
                        // Result: p0 = sea-level compensated pressure in mb

                        p0 = pressure.sealevel(P, ALTITUDE); // we're at 1655 meters (Boulder, CO)
                        Serial.print("Presión relativa (a nivel del mar): ");
                        Serial.print(p0, 2);
                        Serial.print(" hPa \t\t");
                        // Serial.print(p0, 2);
                        // Serial.print(" mb \t\t");
                        // Serial.print(p0*0.0295333727,2);
                        // Serial.println(" inHg");

                        // On the other hand, if you want to determine your altitude from the pressure reading,
                        // use the altitude function along with a baseline pressure (sea-level or other).
                        // Parameters: P = absolute pressure in mb, p0 = baseline pressure in mb.
                        // Result: a = altitude in m.

                        a = pressure.altitude(P, p0);
                        Serial.print("Altitud calculada: ");
                        Serial.print(a, 0);
                        Serial.println(" m");
                        // Serial.print(a*3.28084,0);
                        // Serial.println(" feet");
                        float Tf = (float)T;
                        float Pf = (float)P;
                        float p0f = (float)p0;
                        float af = (float)a;
                        return {Tf, Pf, p0f, af};
                    }
                    else
                        Serial.println("error retrieving pressure measurement\n");
                    return {0, 0, 0, 0};
                }
                else
                    Serial.println("error starting pressure measurement\n");
                return {0, 0, 0, 0};
            }
            else
                Serial.println("error retrieving temperature measurement\n");
            return {0, 0, 0, 0};
        }
        else
            Serial.println("error starting temperature measurement\n");
        return {0, 0, 0, 0};
    }
    else
    {
        return {0, 0, 0, 0};
    }
}
//  Método para leer sensor BH1750
float LeerSensoresControlador::leerBH()
{
    Serial.print("\nA continuación: Datos leidos del sensor BH1750\n");
    float lux = 0;
    if (lightMeter.measurementReady())
    {
        lux = lightMeter.readLightLevel();
        Serial.print("Valor de luminosidad: ");
        Serial.print(lux);
        Serial.println(" lx");
    }

    return lux;
}

//  Método "getter" de la velocidad de la clase
float LeerSensoresControlador::leerAnemometro()
{
    Serial.print("\nA continuación: Datos leidos del Anemómetro\n");
    Serial.printf("Velocidad Tangencial: %.3f \t\t", velocidades.velocidadVientoAngular);
    Serial.printf("Velocidad Angular: %.3f \n", velocidades.velocidadVientoTangencial);
    return velocidades.velocidadVientoTangencial;
}

//  Método para calcular la velocidad tangencial del viento
void LeerSensoresControlador::calcularVelocidad(unsigned long pulsos){
    velocidades.velocidadVientoAngular = (pulsos*60)/(2 * PI);      //  Revoluciones por minutos (RPM)
    velocidades.velocidadVientoTangencial = velocidades.velocidadVientoAngular * 0.08;          //  Vel angular pro el brazo de palanca da velocidad tangencial
}


//  Método para función de interrupción del anemómetro
void LeerSensoresControlador::contadorPulsos() {
        static volatile unsigned long previousMillis = 0;
        static volatile unsigned long pulseCount = 0;

        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= 1000) { // Cada segundo
            previousMillis = currentMillis;
            // Calcular la velocidad del viento basándose en pulseCount
            LeerSensoresControlador::calcularVelocidad(pulseCount);
            pulseCount = 0; // Reiniciar el contador
        }
        pulseCount++;
        
}


String LeerSensoresControlador::leerVeleta()
{
    String direction;
    int direccion = digitalRead(S2) *4 + digitalRead(S1) *2 + digitalRead(S0) *1;
    switch (direccion)
    {
    case 0:
        /* code */
        direction = "Pendiente";
        break;
    case 1:
        /* code */
        direction = "Norte";
        break;
    case 2:
        /* code */
        direction = "Noreste";
        break;
    case 3:
        /* code */
        direction = "Noroeste";
        break;
    case 4:
        /* code */
        direction = "Sur";
        break;
    case 5:
        /* code */
        direction = "Sureste";
        break;
    case 6:
        /* code */
        direction = "Suroeste";
        break;
    case 7:
        /* code */
        direction = "Oeste";
        break;
    case 8:
        /* code */
        direction = "Este";
        break;
    
    default:
        direction = "default";
        break;
    } 
    
    Serial.printf("\nLa dirección de la veleta apunta con dirección %s\n", direction);

    return direction;
}

LeerSensoresControlador::datosYL LeerSensoresControlador::leerHojaMojada()
{
    int analogSHM = analogRead(SHM_Ax);
    boolean boolSHM = digitalRead(SHM_Dx);

    Serial.print("\nA continuación: Datos leidos del Sensor de Hoja Mojada\n");
    Serial.printf("Analogico: %d \t\t", analogSHM);
    Serial.printf("Digital: %d \n", boolSHM);

    return {analogSHM, boolSHM};
}

void LeerSensoresControlador::alimentaciones(boolean g1, boolean g2){

    digitalWrite(PIN_GROUND_1, g1);
    digitalWrite(PIN_GROUND_2, g2);
    
    Serial.print("\nSe han habilitado los MOSFET de alimentación\n");
    Serial.printf("MOSFET de Sensores: %d \t\t", g1);
    Serial.printf("MOSFET de Comunicaciones: %d \n", g2);
}

String LeerSensoresControlador::alimentaciones(String state){

    Serial.print("\nSe han cambiado los estados de los MOSFET de alimentación\n");
    String respuesta;
    if (strcmp(state.c_str(), "on")){
        digitalWrite(PIN_GROUND_1, true);
        digitalWrite(PIN_GROUND_2, true);
        respuesta = "Ambos MOSFET's activados";
    } else if (strcmp(state.c_str(), "off")){
        digitalWrite(PIN_GROUND_1, false);
        digitalWrite(PIN_GROUND_2, false);
        respuesta = "Ambos MOSFET's desactivados";
    }else if (strcmp(state.c_str(), "g1")){
        digitalWrite(PIN_GROUND_1, true);
        digitalWrite(PIN_GROUND_2, false);
        respuesta = "MOSFET de Sensores: Activado \t\tMOSFET de Comunicaciones: Desactivado \n";
    }else if (strcmp(state.c_str(), "g2")){
        digitalWrite(PIN_GROUND_1, false);
        digitalWrite(PIN_GROUND_2, true);
        respuesta = "MOSFET de Sensores: Desactivado \t\tMOSFET de Comunicaciones: Activado \n";
    } else {
        respuesta = "Estado incorrecto para los MOSFET's.";
    }
    
    Serial.print(respuesta);

    return respuesta;
}