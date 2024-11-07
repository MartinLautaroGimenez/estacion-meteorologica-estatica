#ifndef FormWifi_h
#define FormWifi_h

#include <WiFi.h>
#include <Preferences.h>
#include <WebServer.h>
#include "LeerSensores.h"
#include <WiFiClientSecure.h>

class ControladorWiFi 
{
    public:
        // Credenciales WiFi predeterminadas (se usarán si no hay nada guardado en las preferencias)
        const char* defaultSSID = "TuSSIDPredeterminado";
        const char* defaultPassword = "TuContraseñaPredeterminada";

        // Creamos una instancia del servidor web en el puerto 80
        // WebServer server(80);
        // Métodos
        ControladorWiFi();
        // Declaración de funciones
        void setupAP();
        void handleRoot();
        void handleSave();
        bool connectToWiFi();
        void clearPreferences();

    // private:
        // Creamos un objeto Preferences para manejar la configuración en la memoria flash
        Preferences preferences;
        WebServer server;
        WiFiClass WiFi;

};

class ManejoDatosWifi
{
  public:

    ManejoDatosWifi();

    // URL del servidor con HTTPS (asegúrate que el endpoint de Node-RED no incluya /admin)
    const char* serverName = "https://emetec.wetec.um.edu.ar/weather";

    WiFiClientSecure client;
  // métodos
    int enviarData(String postData);
     
    String jsonMaker(
        LeerSensoresControlador::datosDHT dhtData,
        LeerSensoresControlador::datosMQ mqData,
        LeerSensoresControlador::datosBMP bmpData,  
        float bhData,
        float velViento,
        String dirViento,
        float lluvia
        );
};


#endif


