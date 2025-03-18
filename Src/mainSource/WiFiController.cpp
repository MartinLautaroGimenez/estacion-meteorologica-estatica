#include "../lib/WiFiController.h"

WiFiController::WiFiController(const char* defaultSSID, const char* defaultPassword)
    : _defaultSSID(defaultSSID), _defaultPassword(defaultPassword), _server(80), _connected(false) {
}

void WiFiController::begin() {
  // Primero, intenta conectarse en modo estación (STA).
  WiFi.mode(WIFI_STA);
  _connected = connectToDefaultWiFi();
  if (!_connected) {
    Serial.println("No se pudo conectar a la red por defecto. Iniciando portal cautivo...");
    startCaptivePortal();
  } else {
    Serial.println("Conectado a la red por defecto.");
    Serial.print("Dirección IP: ");
    Serial.println(WiFi.localIP());
  }
}

bool WiFiController::connectToDefaultWiFi() {
  WiFi.begin(_defaultSSID, _defaultPassword);
  unsigned long startAttemptTime = millis();
  // Intentar conectarse durante 10 segundos.
  while (WiFi.status() != WL_CONNECTED && (millis() - startAttemptTime) < 10000) {
    delay(500);
    Serial.print(".");
  }
  return (WiFi.status() == WL_CONNECTED);
}

void WiFiController::startCaptivePortal() {
  // Cambiar a modo AP (Access Point)
  WiFi.mode(WIFI_AP);
  // Iniciar un AP con nombre "ConfigAP" y clave "12345678" (puedes modificar estos valores)
  WiFi.softAP("ConfigAP", "12345678");
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  // Iniciar el servidor DNS para redirigir todas las solicitudes al AP
  _dnsServer.start(53, "*", WiFi.softAPIP());

  // Configurar el servidor web con el portal cautivo
  setupWebServer();
  _server.begin();
}

void WiFiController::setupWebServer() {
  // Ruta para servir el formulario de configuración
  _server.on("/", HTTP_GET, [&](AsyncWebServerRequest *request) {
    String html = "<!DOCTYPE html><html><head><meta charset='utf-8'><title>Configurar WiFi</title></head><body>";
    html += "<h1>Configuración WiFi</h1>";
    html += "<form action='/save' method='POST'>";
    html += "SSID: <input type='text' name='ssid'><br>";
    html += "Password: <input type='password' name='password'><br>";
    html += "<input type='submit' value='Guardar'>";
    html += "</form></body></html>";
    request->send(200, "text/html", html);
  });

  // Ruta para procesar el formulario y guardar la nueva configuración.
  _server.on("/save", HTTP_POST, [&](AsyncWebServerRequest *request) {
    String newSSID, newPassword;
    if (request->hasParam("ssid", true) && request->hasParam("password", true)) {
      newSSID = request->getParam("ssid", true)->value();
      newPassword = request->getParam("password", true)->value();

      // Aquí podrías guardar los nuevos datos en memoria no volátil o un archivo.
      // Por simplicidad, se reinicia la conexión con las nuevas credenciales.
      request->send(200, "text/html", "<h1>Configuración guardada. Reiniciando...</h1>");
      delay(2000);
      // Apagar el modo AP
      WiFi.softAPdisconnect(true);
      // Reiniciar en modo STA e intentar conectar con los nuevos datos
      WiFi.mode(WIFI_STA);
      WiFi.begin(newSSID.c_str(), newPassword.c_str());
    } else {
      request->send(400, "text/html", "<h1>Error: faltan datos</h1>");
    }
  });
}

void WiFiController::handleClient() {
  // Procesa las solicitudes DNS para redirigir al portal
  _dnsServer.processNextRequest();
}
