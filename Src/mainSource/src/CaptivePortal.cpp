#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <Preferences.h>

#define AP_SSID "EME-Config"
#define AP_PASSWORD "12345678"
#define DNS_PORT 53

extern Preferences preferences;
DNSServer dnsServer;
WebServer captiveServer(80);  // Renombrado para evitar conflictos

String getPortalHTML() {
  return "<!DOCTYPE html>"
         "<html lang='es'>"
         "<head>"
         "  <meta charset='UTF-8'>"
         "  <meta name='viewport' content='width=device-width, initial-scale=1.0'>"
         "  <title>Configuración WiFi</title>"
         "  <style>"
         "    * { margin: 0; padding: 0; box-sizing: border-box; }"
         "    body { font-family: Arial, sans-serif; background: #f0f4f8; color: #333; display: flex; align-items: center; justify-content: center; min-height: 100vh; padding: 20px; }"
         "    .container { background: #fff; padding: 20px 30px; border-radius: 8px; box-shadow: 0 2px 10px rgba(0, 0, 0, 0.1); max-width: 400px; width: 100%; }"
         "    h2 { margin-bottom: 20px; text-align: center; color: #2a7ae2; }"
         "    form { display: flex; flex-direction: column; gap: 15px; }"
         "    label { font-size: 14px; }"
         "    input[type='text'], input[type='password'] { padding: 10px; border: 1px solid #ccc; border-radius: 4px; font-size: 14px; }"
         "    input[type='submit'] { padding: 10px; border: none; background: #2a7ae2; color: white; border-radius: 4px; cursor: pointer; font-size: 16px; }"
         "    input[type='submit']:hover { background: #225bb5; }"
         "    .admin-banner { margin-top: 20px; text-align: center; font-size: 12px; color: #777; }"
         "    .admin-banner a { color: #2a7ae2; text-decoration: none; }"
         "    .admin-banner a:hover { text-decoration: underline; }"
         "    @media (max-width: 480px) {"
         "      .container { padding: 15px 20px; }"
         "      h2 { font-size: 18px; }"
         "    }"
         "  </style>"
         "</head>"
         "<body>"
         "  <div class='container'>"
         "    <h2>Configura tu WiFi</h2>"
         "    <form action='/save' method='POST'>"
         "      <label for='ssid'>SSID:</label>"
         "      <input type='text' name='ssid' id='ssid' placeholder='Nombre de la red' required>"
         "      <label for='pass'>Contraseña:</label>"
         "      <input type='password' name='pass' id='pass' placeholder='Contraseña' required>"
         "      <input type='submit' value='Guardar y Conectar'>"
         "    </form>"
         "    <div class='admin-banner'>"
         "    </div>"
         "  </div>"
         "</body>"
         "</html>";
}

void handleSaveCreds() {
  String ssid = captiveServer.arg("ssid");
  String pass = captiveServer.arg("pass");

  preferences.begin("wifi", false);
  preferences.putString("ssid", ssid);
  preferences.putString("pass", pass);
  preferences.end();

  captiveServer.send(200, "text/html", "<html><body><h2>Guardado! Reiniciando...</h2></body></html>");
  delay(3000);
  ESP.restart();
}

void handleRoot() {
  captiveServer.send(200, "text/html", getPortalHTML());
}

void startCaptivePortal() {
  WiFi.mode(WIFI_AP);  // Si solo necesitás el portal, este modo anda. Si luego requieres escanear, usá WIFI_AP_STA.
  WiFi.softAP(AP_SSID, AP_PASSWORD);
  delay(100);

  IPAddress apIP = WiFi.softAPIP();
  dnsServer.start(DNS_PORT, "*", apIP);

  captiveServer.on("/", handleRoot);
  captiveServer.on("/save", HTTP_POST, handleSaveCreds);
  captiveServer.begin();

  Serial.println("⚠️ Modo configuración iniciado.");
  Serial.println("📶 Conéctate a: " + String(AP_SSID));
  Serial.println("🌐 Abre cualquier sitio para configurar.");
}

void handleCaptivePortal() {
  dnsServer.processNextRequest();
  captiveServer.handleClient();
}
