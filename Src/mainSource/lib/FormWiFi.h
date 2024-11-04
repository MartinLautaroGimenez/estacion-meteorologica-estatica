#ifndef FormWifi_h
#define FormWifi_h

#include <WiFi.h>
#include <Preferences.h>
#include <WebServer.h>
#include "LeerSensores.h"
#include <WiFiClientSecure.h>

class FormularioWiFi 
{
    public:
        // Credenciales WiFi predeterminadas (se usarán si no hay nada guardado en las preferencias)
        const char* defaultSSID = "TuSSIDPredeterminado";
        const char* defaultPassword = "TuContraseñaPredeterminada";

        // Creamos una instancia del servidor web en el puerto 80
        // WebServer server(80);
        // Métodos
        FormularioWiFi();
        // Declaración de funciones
        void setupAP();
        void handleRoot();
        void handleSave();
        bool connectToWiFi();
        void clearPreferences();

    private:
        // Creamos un objeto Preferences para manejar la configuración en la memoria flash
        Preferences preferences;
        WebServer server;

};

class ManejoDatosWifi
{
  public:

    ManejoDatosWifi();

    const char* ssid = "TU_SSID";                  // Tu nombre de red WiFi
    const char* password = "TU_PASSWORD";          // Tu contraseña de red WiFi

    // URL del servidor con HTTPS (asegúrate que el endpoint de Node-RED no incluya /admin)
    const char* serverName = "https://emetec.wetec.um.edu.ar/weather";

    WiFiClientSecure client;

    int enviarData(String postData);
     
    String jsonMaker(
        LeerSensoresControlador::datosDHT dhtData,
        LeerSensoresControlador::datosMQ mqData,
        LeerSensoresControlador::datosBMP bmpData,    
        float bhData
        );
};


#endif
