#include "../lib/WiFiController.h"
#include "../lib/RootHTML.h"

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


ControladorWiFi::ControladorWiFi()
{
  // Credenciales WiFi predeterminadas (se usarán si no hay nada guardado en las preferencias)
  const char* defaultSSID = "ETEC";
  const char* defaultPassword = "ETec2024*";

  // Creamos un objeto Preferences para manejar la configuración en la memoria flash
  Preferences preferences;

  // Creamos una instancia del servidor web en el puerto 80
  WebServer server(80);

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

bool ControladorWiFi::connectToWiFi() {
  // Leemos las credenciales WiFi de las preferencias
  String ssid = preferences.getString("ssid", defaultSSID);
  String password = preferences.getString("password", defaultPassword);

  Serial.println("Intentando conectar a: " + ssid);

  // Intentamos conectarnos al WiFi
  this->WiFi.begin(ssid.c_str(), password.c_str());

  // Esperamos hasta 10 segundos por la conexión
  for (int i = 0; i < 20; i++) {
    if (this->WiFi.status() == WL_CONNECTED) {
      Serial.println("Conectado a WiFi");
      Serial.println("Dirección IP: " + this->WiFi.localIP().toString());
      return true;
    }
    delay(500);
    Serial.print(".");
  }

  Serial.println("Fallo en la conexión WiFi");
  return false;
}

void ControladorWiFi::handleRoot() {
  // la variable html la saca del RootHTML.h
  this->server.send(200, "text/html", html);
}

void ControladorWiFi::handleSave() {
  // Obtenemos el nuevo SSID y contraseña del formulario
  String newSSID = this->server.arg("ssid");
  String newPassword = this->server.arg("password");

  // Guardamos las nuevas credenciales en las preferencias
  this->preferences.putString("ssid", newSSID);
  this->preferences.putString("password", newPassword);

  // Enviamos una respuesta al cliente
  this->server.send(200, "text/plain", "Configuración guardada. Reiniciando...");
  delay(1000);
  ESP.restart();  // Reiniciamos el ESP32 para aplicar la nueva configuración
}

void ControladorWiFi::clearPreferences() {
  // Borramos todas las preferencias almacenadas en el espacio de nombres "wifi"
  this->preferences.clear();
  this->preferences.end();
}


ControladorWiFi::~ControladorWiFi()
{
}

void ControladorWiFi::setupAP() {
    // Configuramos el ESP32 como punto de acceso
  this->WiFi.mode(WIFI_AP);
  this->WiFi.softAP("ESP32_Config", "password");

  // Configuramos las rutas del servidor web
  const Uri root = "/";
  this->server.on("/", std::bind(&ControladorWiFi::handleRoot, this));
  this->server.on("/save", std::bind(&ControladorWiFi::handleSave, this));
  this->server.begin();

  Serial.println("Modo punto de acceso iniciado");
  Serial.println("SSID: ESP32_Config");
  Serial.println("Password: password");
  Serial.println("Dirección IP: " + WiFi.softAPIP().toString());
}


ManejoDatosWifi::ManejoDatosWifi() 
{
  // Crear cliente seguro para HTTPS
  WiFiClientSecure client;

  // Si estás en un entorno de pruebas y no te importa la verificación del certificado:
  client.setInsecure();
}

String ManejoDatosWifi::jsonMaker(
    LeerSensoresControlador::datosDHT dhtData,
    LeerSensoresControlador::datosMQ mqData,
    LeerSensoresControlador::datosBMP bmpData,    
    float bhData,
    float velViento,
    String dirViento,
    float lluvia
)
{
  String postData;
  if (lluvia < 0 || velViento < 0)
  {
    // Crear los datos JSON para enviar (temperatura y humedad)
    postData = "{\"DHT_temperatura\":" + String(dhtData.temperatura) + 
                      ",\"DHT_humedad\":" + String(dhtData.humedadRelativa) + 
                      ",\"DHT_sensasionTerm\":" + String(dhtData.sensacionTermica) + 
                      ",\"MQ_ppmCO2\":" + String(mqData.ppmCO2) + 
                      ",\"BMP_presion\":" + String(bmpData.presionAbsoluta) + 
                      ",\"BMP_temperatura\":" + String(bmpData.temperatura) + 
                      ",\"BMP_altitud\":" + String(bmpData.altitud) + 
                      ",\"BH_lumines\":" + String(bhData) + 
                      ",\"VelocidadViento\":" + "Proximamente" + 
                      ",\"DireccionViento\":" + "Proximamente" + 
                      ",\"Lluvia\":" + "Proximamente" + 
                      "}";
  } else {
    // Crear los datos JSON para enviar (temperatura y humedad)
    postData = "{\"DHT_temperatura\":" + String(dhtData.temperatura) + 
                      ",\"DHT_humedad\":" + String(dhtData.humedadRelativa) + 
                      ",\"DHT_sensasionTerm\":" + String(dhtData.sensacionTermica) + 
                      ",\"MQ_ppmCO2\":" + String(mqData.ppmCO2) + 
                      ",\"BMP_presion\":" + String(bmpData.presionAbsoluta) + 
                      ",\"BMP_temperatura\":" + String(bmpData.temperatura) + 
                      ",\"BMP_altitud\":" + String(bmpData.altitud) + 
                      ",\"BH_lumines\":" + String(bhData) + 
                      ",\"VelocidadViento\":" + String(velViento) + 
                      ",\"DireccionViento\":" + dirViento + 
                      ",\"Lluvia\":" + String(lluvia) + 
                      "}";
    
  }
  return postData;
}

//  Destructor de la clase
ManejoDatosWifi::~ManejoDatosWifi()
{
}

//  Método para el envío de datos pro medio de HTTP
int ManejoDatosWifi::enviarData(String postData)
{
  // Conectar al servidor
  if (client.connect("emetec.wetec.um.edu.ar", 443)) {
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
    return 0;
  } else {
    Serial.println("Fallo al conectar al servidor HTTPS");
    return 1;
  }

  // Finalizar la conexión
  client.stop();
}
