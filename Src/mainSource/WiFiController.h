#ifndef WIFI_CONTROLLER_H
#define WIFI_CONTROLLER_H

#include <WiFi.h>
// #include <ESPAsyncWebServer.h>
// #include <DNSServer.h>

class WiFiController {
public:
  // Constructor que recibe las credenciales por defecto.
  WiFiController(const char* defaultSSID, const char* defaultPassword);

  // Inicializa la conexión. Si falla, inicia el portal cautivo.
  void begin();

  // Procesa las peticiones del DNS (para redirigir a nuestro portal).
  void handleClient();

private:
  const char* _defaultSSID;
  const char* _defaultPassword;
  bool _connected;
  AsyncWebServer _server;
  DNSServer _dnsServer;

  // Intenta conectarse a la red por defecto.
  bool connectToDefaultWiFi();

  // Inicia el modo AP y configura el portal cautivo.
  void startCaptivePortal();

  // Configura las rutas del servidor (formulario de configuración, etc.).
  void setupWebServer();
};

#endif
