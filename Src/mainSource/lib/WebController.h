// ../lib/WebController.h
#ifndef WEBCONTROLLER_H
#define WEBCONTROLLER_H

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Preferences.h>
#include "LeerSensores.h"

// Prototipo de la función de envío de datos (definido en tu main.cpp)
extern void enviarDatos();

class WebController {
public:
    // Constructor: puerto HTTP, puntero a sensores y puntero a Preferences
    WebController(uint16_t port, LeerSensores* sensores, Preferences* preferences);
    // Inicia el servidor y rutas
    void begin();

private:
    uint16_t webPort;
    AsyncWebServer server;
    LeerSensores* sensoresPtr;
    Preferences* prefPtr;

    // Configura rutas HTTP
    void setupRoutes();
    // Helpers para IP/MAC
    String getIp();
    String getMac();
    // Handlers de rutas
    void handleMainPage(AsyncWebServerRequest* request);
    void handleReset(AsyncWebServerRequest* request);
    void handleSendData(AsyncWebServerRequest* request);
    void handleChangeWifi(AsyncWebServerRequest* request);
};

#endif // WEBCONTROLLER_H
