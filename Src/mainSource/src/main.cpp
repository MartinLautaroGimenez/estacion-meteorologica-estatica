#include <Arduino.h>
#include <ArduinoOTA.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

#include "../lib/LeerSensores.h"
#include "../lib/config.h"
#include "esp_sleep.h"

// Instanciar objetos de sensores
WiFiClient esp_EME;
LeerSensoresControlador controladorSensores;

// Servidor HTTP para recibir solicitudes POST que activan deep sleep
WiFiServer server(80);

// Definir las credenciales por defecto (estas se usan para conectar en modo STA)
#define DEFAULT_SSID RED_SSID_WIFI
#define DEFAULT_PASS PASSWORD_WIFI

// Factores de tiempo y configuración para deep sleep
#define uS_TO_S_FACTOR 1000000  // Conversión de segundos a microsegundos
#define TIME_TO_SLEEP_15_MIN (15 * 60)  // 15 minutos (en segundos)
#define TIME_TO_SLEEP_5_SEG 5           // 5 segundos (en segundos)
#define TIME_TO_SLEEP_2_MIN (2 * 60)    // 2 minutos (en segundos)

// Declaración de funciones
void leerSensores();
String jsonMaker(LeerSensoresControlador::datosDHT dhtData,
                 LeerSensoresControlador::datosMQ mqData,
                 LeerSensoresControlador::datosBMP bmpData, float bhData,
                 float velViento, String dirViento,
                 LeerSensoresControlador::datosYL lluvia);

void setup() {
    Serial.begin(115200);
    String thisBoard = ARDUINO_BOARD;
    Serial.println(thisBoard);

    // Configurar pines (por ejemplo, activar salidas para sensores)
    pinMode(PIN_GROUND_1, OUTPUT);
    pinMode(PIN_GROUND_2, OUTPUT);
    digitalWrite(PIN_GROUND_1, HIGH);
    digitalWrite(PIN_GROUND_2, HIGH);

    // ---- Conexión WiFi directa (SIN portal cautivo) ----
    WiFi.begin(DEFAULT_SSID, DEFAULT_PASS);
    Serial.println("Conectando al WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi conectado correctamente.");
    Serial.print("Dirección IP: ");
    Serial.println(WiFi.localIP());
    // ----------------------------------------------------

    // Si se logró conectar a la red (modo STA), iniciamos el servidor HTTP para
    // recibir comandos
    if (WiFi.status() == WL_CONNECTED) {
        server.begin();
        Serial.println("Servidor HTTP iniciado en el puerto 80 🚀");
    }

    // Inicializar el controlador de sensores
    controladorSensores.initControlador(BMP_TYPE_280, DHT_TYPE_22);

    // Leer sensores y enviar los datos al servidor remoto
    leerSensores();

    // Configurar OTA (Actualización Over The Air)
    ArduinoOTA.setHostname("ESP32_OTA");
    ArduinoOTA.setPassword("emetec2024*");
    ArduinoOTA.begin();

    // Activar las alimentaciones necesarias para sensores u otros módulos
    controladorSensores.alimentaciones("on");
}

void loop() {
    // Manejar actualizaciones OTA
    ArduinoOTA.handle();

    // Procesar solicitudes HTTP para deep sleep
    WiFiClient client = server.available();
    if (client) {
        Serial.println("¡Nuevo cliente HTTP conectado!");
        String request = "";
        // Esperar a que haya datos disponibles
        while (client.connected() && client.available() == 0) {
            delay(1);
        }
        request = client.readStringUntil('\r');
        Serial.print("Solicitud recibida: ");
        Serial.println(request);

        if (request.indexOf("POST") >= 0) {
            // Leer el cuerpo de la solicitud que contendrá la duración de deep sleep
            String body = client.readString();
            Serial.print("Cuerpo de la solicitud: ");
            Serial.println(body);

            int sleepDuration = body.toInt();
            if (sleepDuration > 0) {
                // Responder al cliente y activar deep sleep
                client.println("HTTP/1.1 200 OK");
                client.println("Content-Type: text/plain");
                client.println("Connection: close");
                client.println();
                client.println("Deep Sleep activado por solicitud POST 😴");
                delay(10);
                client.stop();

                Serial.printf(
                    "Deep Sleep activado por solicitud POST. "
                    "Duración: %d segundos. ¡A descansar se ha dicho! 😎\n",
                    sleepDuration
                );
                delay(100);
                esp_sleep_enable_timer_wakeup(sleepDuration * uS_TO_S_FACTOR);
                esp_deep_sleep_start();
            } else {
                // Responder con error en caso de datos inválidos
                client.println("HTTP/1.1 400 Bad Request");
                client.println("Content-Type: text/plain");
                client.println("Connection: close");
                client.println();
                client.println("Duración de Deep Sleep no válida.");
                delay(10);
                client.stop();
                Serial.println("Duración de Deep Sleep no válida.");
            }
        }
    }
}

void leerSensores() {
    // Declaración de variables para almacenar los datos de los sensores
    LeerSensoresControlador::datosBMP bmpData;
    LeerSensoresControlador::datosDHT dhtData;
    LeerSensoresControlador::datosMQ mqData;
    float bh1750Data;
    float velViento;
    String dirViento;
    LeerSensoresControlador::datosYL lluvia;

    // Realizar la lectura de cada sensor
    dhtData = controladorSensores.leerDHT();
    bmpData = controladorSensores.leerBMP();
    bh1750Data = controladorSensores.leerBH();
    mqData = controladorSensores.leerMQ(dhtData.temperatura,
                                        dhtData.humedadRelativa);
    dirViento = controladorSensores.leerVeleta();
    lluvia = controladorSensores.leerHojaMojada();
    velViento = controladorSensores.leerAnemometro();

    // Crear un cliente seguro para la conexión HTTPS
    WiFiClientSecure client;
    client.setInsecure();  // Para pruebas, deshabilitamos la verificación del certificado

    // Conectar al servidor remoto mediante HTTPS
    if (client.connect(SERVIDOR_ETEC, 443)) {
        Serial.printf("\nConectado al servidor HTTPS: %s\n", SERVIDOR_ETEC);

        // Crear el string JSON con los datos obtenidos
        String datazo = jsonMaker(dhtData, mqData, bmpData, bh1750Data,
                                  velViento, dirViento, lluvia);

        // Enviar la solicitud HTTP POST
        client.println("POST /emeapi HTTP/1.1");
        client.println("Host: emetec.wetec.um.edu.ar");
        client.println("User-Agent: ESP32");
        client.println("Content-Type: application/json");
        client.print("Content-Length: ");
        client.println(datazo.length());
        client.println();        // Fin de las cabeceras HTTP
        client.println(datazo);  // Enviar el cuerpo de la solicitud

        // Leer y mostrar la respuesta del servidor
        while (client.connected()) {
            String line = client.readStringUntil('\n');
            if (line == "\r") {
                Serial.println("Cuerpo de la respuesta:");
                break;
            }
        }
        String response = client.readString();
        Serial.println(response);
    } else {
        Serial.println("Fallo al conectar al servidor HTTPS");
    }

    // Finalizar la conexión
    client.stop();
}

String jsonMaker(LeerSensoresControlador::datosDHT dhtData,
                 LeerSensoresControlador::datosMQ mqData,
                 LeerSensoresControlador::datosBMP bmpData, float bhData,
                 float velViento, String dirViento,
                 LeerSensoresControlador::datosYL lluvia) {
    String lluviaPosibilidad;
    if (dhtData.humedadRelativa > 85) {
        lluviaPosibilidad = "\"Hay posibilidades de que llueva\"";
    } else {
        lluviaPosibilidad = "\"No hay posibilidades de que llueva\"";
    }

    String postData;
    // Si no se obtuvieron datos válidos de viento, se envían valores por defecto
    if (velViento < 0 || strcmp(dirViento.c_str(), "default") == 0 ||
        strcmp(dirViento.c_str(), "Pendiente") == 0) {
        postData =
            "{\"EME_n0/dht/temp\":" + String(dhtData.temperatura) +
            ",\"EME_n0/dht/hum\":" + String(dhtData.humedadRelativa) +
            ",\"EME_n0/dht/senst\":" + String(dhtData.sensacionTermica) +
            ",\"EME_n0/bmp/temp\":" + String(bmpData.temperatura) +
            ",\"EME_n0/bmp/pres\":" + String(bmpData.presionAbsoluta) +
            ",\"EME_n0/bmp/alt\":" + String(bmpData.altitud) +
            ",\"EME_n0/bmp/presnm\":" + String(bmpData.presionAlNivelDelMar) +
            ",\"EME_n0/bh/presnm\":" + String(bhData) +
            ",\"EME_n0/mq/ppmco2\":" + String(mqData.ppmCO2) +
            ",\"EME_n0/yl/lluv\":" + lluviaPosibilidad +
            ",\"EME_n0/viento/vel\":\"Proximamente\"" +
            ",\"EME_n0/viento/dir\":\"Proximamente\"" +
            "}";
    } else {
        postData =
            "{\"EME_n0/dht/temp\":" + String(dhtData.temperatura) +
            ",\"EME_n0/dht/hum\":" + String(dhtData.humedadRelativa) +
            ",\"EME_n0/dht/senst\":" + String(dhtData.sensacionTermica) +
            ",\"EME_n0/bmp/temp\":" + String(bmpData.temperatura) +
            ",\"EME_n0/bmp/pres\":" + String(bmpData.presionAbsoluta) +
            ",\"EME_n0/bmp/alt\":" + String(bmpData.altitud) +
            ",\"EME_n0/bmp/presnm\":" + String(bmpData.presionAlNivelDelMar) +
            ",\"EME_n0/bh/presnm\":" + String(bhData) +
            ",\"EME_n0/mq/ppmco2\":" + String(mqData.ppmCO2) +
            ",\"EME_n0/yl/lluv\":" + lluviaPosibilidad +
            ",\"EME_n0/viento/vel\":" + String(velViento) +
            ",\"EME_n0/viento/dir\":" + dirViento +
            "}";
    }
    return postData;
}

/*
   ¡Listo! Toda la lógica relacionada con el portal cautivo ha sido removida,
   incluyendo la inclusión y el uso de WiFiController.
*/
