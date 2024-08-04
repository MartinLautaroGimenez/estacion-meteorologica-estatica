#include "../lib/LeerSensores.h"

LeerSensoresControlador::LeerSensoresControlador(const char* sensor)
:   mq135_sensor(DHT_PIN) , lightMeter(BH1750_ADDRESS){
    
    // Initialize the I2C bus (BH1750 library doesn't do this automatically)
    Wire.begin();


    if(strcmp(sensor,BMP_TYPE_280) == 0){
        while ( !Serial ) delay(100);   // wait for native usb
        Serial.println(F("BMP280 test"));
        unsigned status;
        status = bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
        // status = bmp.begin();
        if (!status) {
            Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                            "try a different address!"));
            Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(),16);
            Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
            Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
            Serial.print("        ID of 0x60 represents a BME 280.\n");
            Serial.print("        ID of 0x61 represents a BME 680.\n");
            while (1) delay(10);
        }
        /* Default settings from datasheet. */
        bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                        Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                        Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                        Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                        Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
    } else if (strcmp(sensor,BMP_TYPE_180) == 0){
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
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println(F("BH1750 Advanced begin"));
  } else {
    Serial.println(F("Error initialising BH1750"));
  }

}

datosDHT LeerSensoresControlador::leerDHT(){
  delay(dht.getMinimumSamplingPeriod());

  const char* estado = dht.getStatusString();
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  float sensacion = dht.computeHeatIndex(temperature, humidity, false);

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

//  Metodo para leer sensor MQ135
datosMQ LeerSensoresControlador::leerMQ(float temperature, float humidity){
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

//  Metodo para leer sensor BMP280
datosBMP LeerSensoresControlador::leerBMP(const char* sensor){
    if(strcmp(sensor,BMP_TYPE_280) == 0){
        Serial.print(F("Temperature = "));
        Serial.print(bmp.readTemperature());
        Serial.println(" *C");

        Serial.print(F("Pressure = "));
        Serial.print(bmp.readPressure());
        Serial.println(" Pa");

        Serial.print(F("Approx altitude = "));
        Serial.print(bmp.readAltitude(1013.25)); /* Adjusted to local forecast! */
        Serial.println(" m");

        Serial.println();
    } else if (strcmp(sensor,BMP_TYPE_180) == 0){
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

            /* 
            Retrieve the completed pressure measurement:
            Note that the measurement is stored in the variable P.
            Note also that the function requires the previous temperature measurement (T).
            (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
            Function returns 1 if successful, 0 if failure.
            */

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
}

//  Metodo para leer sensor BH1750
float LeerSensoresControlador::leerBH(){    
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