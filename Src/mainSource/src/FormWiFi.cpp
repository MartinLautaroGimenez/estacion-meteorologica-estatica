#include "../lib/FormWiFi.h"

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

FormularioWiFi::FormularioWiFi()
{
  // Credenciales WiFi predeterminadas (se usarán si no hay nada guardado en las preferencias)
  const char* defaultSSID = "TuSSIDPredeterminado";
  const char* defaultPassword = "TuContraseñaPredeterminada";

  // Creamos una instancia del servidor web en el puerto 80
  WebServer server(80);

  // Creamos un objeto Preferences para manejar la configuración en la memoria flash
  Preferences preferences;


  // Iniciamos la comunicación serial para depuración
  Serial.begin(115200);

  // Iniciamos las preferencias en modo lectura/escritura
  preferences.begin("wifi", false);

  // Ofrecemos la opción de borrar las preferencias al inicio
  Serial.println("Presiona 'C' en los próximos 5 segundos para borrar las preferencias...");
  unsigned long startTime = millis();
  while (millis() - startTime < 5000) {
    if (Serial.available() && Serial.read() == 'C') {
      clearPreferences();
      Serial.println("Preferencias borradas. Reiniciando...");
      delay(1000);
      ESP.restart();
    }
  }

  // Intentamos conectarnos al WiFi
  if (!connectToWiFi()) {
    // Si la conexión falla, configuramos el modo punto de acceso
    setupAP();
  }
}

bool FormularioWiFi::connectToWiFi() {
  // Leemos las credenciales WiFi de las preferencias
  String ssid = preferences.getString("ssid", defaultSSID);
  String password = preferences.getString("password", defaultPassword);

  Serial.println("Intentando conectar a: " + ssid);

  // Intentamos conectarnos al WiFi
  WiFi.begin(ssid.c_str(), password.c_str());

  // Esperamos hasta 10 segundos por la conexión
  for (int i = 0; i < 20; i++) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Conectado a WiFi");
      Serial.println("Dirección IP: " + WiFi.localIP().toString());
      return true;
    }
    delay(500);
    Serial.print(".");
  }

  Serial.println("Fallo en la conexión WiFi");
  return false;
}

void FormularioWiFi::setupAP() {
  // Configuramos el ESP32 como punto de acceso
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESP32_Config", "password");

  // Configuramos las rutas del servidor web
  this->server.on('/', handleRoot);
  server.on("/save", handleSave);
  server.begin();

  Serial.println("Modo punto de acceso iniciado");
  Serial.println("SSID: ESP32_Config");
  Serial.println("Password: password");
  Serial.println("Dirección IP: " + WiFi.softAPIP().toString());
}

void FormularioWiFi::handleRoot() {
  String html = "<!DOCTYPE html><html lang='es'>"
                "<head>"
                "<meta name='viewport' content='width=device-width, initial-scale=1'>"
                "<title>Configuraci&oacute;n WiFi</title>"
                "<style>"
                "body{font-family:system-ui,-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,sans-serif;color:#1d1d1f;background-color:#f5f5f7;margin:0;padding:20px;display:flex;align-items:center;justify-content:center;min-height:100vh}"
                ".container{background-color:#fff;padding:30px;border-radius:12px;box-shadow:0 4px 6px rgba(0,0,0,0.1);max-width:300px;width:100%}"
                "h1{font-size:24px;text-align:center;margin-bottom:20px;color:#1d1d1f}"
                "form{display:flex;flex-direction:column}"
                "input{width:100%;padding:10px;margin-bottom:15px;border:1px solid #d2d2d7;border-radius:6px;font-size:14px}"
                "input[type='submit']{background-color:#0071e3;color:white;border:none;font-weight:bold;cursor:pointer}"
                "input[type='submit']:hover{background-color:#0077ed}"
                "</style>"
                "</head>"
                "<body>"
                "<div class='container'>"
                "<h1>EMM ETEC</h1>"
                "<h1>Configuraci&oacute;n WiFi</h1>"
                "<form action='/save' method='post'>"
                "<input type='text' name='ssid' placeholder='Nombre de la red (SSID)' required>"
                "<input type='password' name='password' placeholder='Contrase&ntilde;a' required>"
                "<input type='submit' value='Guardar'>"
                "</form>"
                "</div>"
                "</body>"
                "</html>";
  server.send(200, "text/html", html);
}

void FormularioWiFi::handleSave() {
  // Obtenemos el nuevo SSID y contraseña del formulario
  String newSSID = server.arg("ssid");
  String newPassword = server.arg("password");

  // Guardamos las nuevas credenciales en las preferencias
  preferences.putString("ssid", newSSID);
  preferences.putString("password", newPassword);

  // Enviamos una respuesta al cliente
  server.send(200, "text/plain", "Configuración guardada. Reiniciando...");
  delay(1000);
  ESP.restart();  // Reiniciamos el ESP32 para aplicar la nueva configuración
}

void FormularioWiFi::clearPreferences() {
  // Borramos todas las preferencias almacenadas en el espacio de nombres "wifi"
  preferences.clear();
  preferences.end();
}

ManejoDatosWifi::ManejoDatosWifi()
{
  /*
  Serial.begin(115200);
  // Conectar a la red WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConectado a la red WiFi");

*/
  // Crear cliente seguro para HTTPS
  WiFiClientSecure client;

  // Si estás en un entorno de pruebas y no te importa la verificación del certificado:
  client.setInsecure();
}

String ManejoDatosWifi::jsonMaker(
    LeerSensoresControlador::datosDHT dhtData,
    LeerSensoresControlador::datosMQ mqData,
    LeerSensoresControlador::datosBMP bmpData,    
    float bhData
)
{
    // Crear los datos JSON para enviar (temperatura y humedad)
    String postData = "{\"DHT_temperatura\":" + String(dhtData.temperatura) + 
                      ",\"DHT_humedad\":" + String(dhtData.humedadRelativa) + 
                      ",\"DHT_sensasionTerm\":" + String(dhtData.sensacionTermica) + 
                      ",\"MQ_ppmCO2\":" + String(mqData.ppmCO2) + 
                      ",\"BMP_presion\":" + String(bmpData.presionAbsoluta) + 
                      ",\"BMP_temperatura\":" + String(bmpData.temperatura) + 
                      ",\"BMP_altitud\":" + String(bmpData.altitud) + 
                      ",\"BH_lumines\":" + String(bhData) + 
                      ",\"aaaaaaaa\":" + String() + 
                      ",\"aaaaaaaa\":" + String() + 
                      ",\"aaaaaaaa\":" + String() + 
                      "}";
    
    return postData;
}

int ManejoDatosWifi::enviarData(String postData)
{
  // Conectar al servidor
  if (this->client.connect("emetec.wetec.um.edu.ar", 443)) {
    Serial.println("Conectado al servidor HTTPS");

    // Enviar solicitud HTTP POST
    client.println("POST /weather HTTP/1.1");
    client.println("Host: emetec.wetec.um.edu.ar");
    client.println("User-Agent: ESP32");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(postData.length());
    client.println();  // Línea vacía para indicar fin de encabezados
    client.println(postData);  // Cuerpo de la solicitud (datos)

    // Leer la respuesta del servidor
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("Cuerpo de la respuesta:");
        break;
      }
    }

    // Imprimir el cuerpo de la respuesta
    String response = client.readString();
    Serial.println(response);

  } else {
    Serial.println("Fallo al conectar al servidor HTTPS");
  }

  // Finalizar la conexión
  client.stop();
}
