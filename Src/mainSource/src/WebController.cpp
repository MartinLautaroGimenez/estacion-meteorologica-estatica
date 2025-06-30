// ../src/WebController.cpp
#include "../lib/WebController.h"
#include "config.h"
#include <WebSerial.h>

// Helper para formatear valores a JSON
static String jsonValue(float value) {
    return (isnan(value) || value < 0) ? "0" : String(value, 2);
}

// HTML principal del WebController, usando Tailwind CSS y tu plantilla
static const char* mainPageHtml = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Estación Meteorológica EME</title>
    <script src="https://cdn.tailwindcss.com"></script>
    <link href="https://fonts.googleapis.com/css2?family=Inter:wght@400;500;600;700&display=swap" rel="stylesheet">
    <style>
        body { font-family: 'Inter', sans-serif; background-color: #f0f4f8; }
        ::-webkit-scrollbar { width: 8px; }
        ::-webkit-scrollbar-track { background: #e2e8f0; border-radius: 10px; }
        ::-webkit-scrollbar-thumb { background: #cbd5e1; border-radius: 10px; }
        ::-webkit-scrollbar-thumb:hover { background: #94a3b8; }
    </style>
</head>
<body class="min-h-screen flex items-center justify-center p-4">
    <div class="bg-white p-8 rounded-xl shadow-2xl w-full max-w-4xl border border-gray-200">
        <h1 class="text-3xl font-bold text-gray-800 mb-6 text-center">Estación Meteorológica EME</h1>
        <!-- Info red -->
        <div class="mb-8 p-6 bg-blue-50 rounded-lg shadow-inner border border-blue-200">
            <h2 class="text-xl font-semibold text-blue-800 mb-4">Información de Red</h2>
            <div class="grid grid-cols-1 md:grid-cols-3 gap-4 text-gray-700">
                <p><strong>SSID:</strong> <span id="network-ssid" class="font-medium text-blue-600">Cargando...</span></p>
                <p><strong>IP:</strong>   <span id="network-ip"  class="font-medium text-blue-600">Cargando...</span></p>
                <p><strong>MAC:</strong>  <span id="network-mac" class="font-medium text-blue-600">Cargando...</span></p>
            </div>
        </div>
        <!-- Botones -->
        <div class="flex flex-col sm:flex-row justify-center gap-4 mb-10">
            <button id="reset-button" class="flex-1 px-8 py-3 bg-red-600 text-white font-semibold rounded-lg shadow-lg hover:bg-red-700 transition transform hover:-translate-y-1 hover:scale-105 focus:outline-none focus:ring-4 focus:ring-red-300">Reset</button>
            <button id="send-data-button" class="flex-1 px-8 py-3 bg-green-600 text-white font-semibold rounded-lg shadow-lg hover:bg-green-700 transition transform hover:-translate-y-1 hover:scale-105 focus:outline-none focus:ring-4 focus:ring-green-300">Enviar Datos</button>
        </div>
        <!-- Datos sensores -->
        <div class="p-6 bg-gray-50 rounded-lg shadow-inner border border-gray-200">
            <h2 class="text-xl font-semibold text-gray-800 mb-4">Datos de Sensores</h2>
            <div id="sensor-cards-container" class="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-3 gap-6"></div>
        </div>
    </div>
    <script>
        // Definición de sensores y unidades
        const sensorsMeta = [
            { id: 'tempDHT',   label: 'Temperatura DHT',      unit: '°C' },
            { id: 'humDHT',    label: 'Humedad DHT',          unit: '%' },
            { id: 'sensDHT',   label: 'Sensación Térmica',    unit: '°C' },
            { id: 'tempBMP',   label: 'Temperatura BMP',      unit: '°C' },
            { id: 'presBMP',   label: 'Presión BMP',          unit: 'hPa' },
            { id: 'altBMP',    label: 'Altitud BMP',          unit: 'm' },
            { id: 'presNMBMP', label: 'Presión Nivel Mar',    unit: 'hPa' },
            { id: 'bh1750',    label: 'Luminosidad BH1750',   unit: 'lux' },
            { id: 'ppmCO2',    label: 'CO2 MQ',               unit: 'ppm' },
            { id: 'lluv',      label: 'Lluvia YL',            unit: '(0/1)' },
            { id: 'vientoVel', label: 'Velocidad Viento',     unit: 'km/h' },
            { id: 'vientoDir', label: 'Dirección Viento',     unit: '°' }
        ];

        // Crea tarjetas vacías para cada sensor
        function createSensorCards() {
            const container = document.getElementById('sensor-cards-container');
            sensorsMeta.forEach(s => {
                const card = document.createElement('div');
                card.className = 'bg-white p-5 rounded-lg shadow-md border border-gray-100';
                card.innerHTML = `
                  <h3 class='text-lg font-semibold text-gray-700 mb-2'>${s.label}</h3>
                  <p class='text-3xl font-bold text-purple-600'>
                    <span id='val-${s.id}'>--</span>
                    <span class='text-xl text-purple-500'>${s.unit}</span>
                  </p>`;
                container.appendChild(card);
            });
        }

        // Actualiza red (ahora con JSON)
        function fetchNetwork() {
            fetch('/info')
              .then(r => r.json())
              .then(data => {
                  document.getElementById('network-ssid').textContent = data.ssid;
                  document.getElementById('network-ip')  .textContent = data.ip;
                  document.getElementById('network-mac') .textContent = data.mac;
              })
              .catch(console.error);
        }

        // Obtiene datos reales
        function fetchSensors() {
            fetch('/sensors')
              .then(r => r.json())
              .then(data => {
                  Object.keys(data).forEach(id => {
                      const el = document.getElementById('val-' + id);
                      if (el) el.textContent = data[id];
                  });
              });
        }

        document.addEventListener('DOMContentLoaded', () => {
            createSensorCards();
            fetchNetwork();
            fetchSensors();
            setInterval(fetchNetwork, 5000);
            setInterval(fetchSensors, 5000);

            document.getElementById('reset-button')
                    .addEventListener('click', () => {
                fetch('/reset').then(() => alert('Reiniciando ESP'));
            });
            document.getElementById('send-data-button')
                    .addEventListener('click', () => {
                fetch('/send').then(() => alert('Datos enviados'));
            });
        });
    </script>
</body>
</html>
)rawliteral";

WebController::WebController(uint16_t port, LeerSensores* sensores, Preferences* preferences)
  : webPort(port), server(port), sensoresPtr(sensores), prefPtr(preferences) {}

void WebController::begin() {
    setupRoutes();
    server.begin();
    Serial.println("🌐 WebApp activo en puerto " + String(webPort));
}

void WebController::setupRoutes() {
    // Página principal
    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request){
        request->send(200, "text/html", mainPageHtml);
    });

    // Reset
    server.on("/reset", HTTP_GET, [this](AsyncWebServerRequest* request){
        request->send(200, "text/plain", "Reiniciando...");
        delay(100);
        ESP.restart();
    });

    // Enviar datos manual
    server.on("/send", HTTP_GET, [this](AsyncWebServerRequest* request){
        enviarDatos();
        request->send(200, "text/plain", "Datos enviados!");
    });

    // Información red — ahora devuelve JSON
    server.on("/info", HTTP_GET, [](AsyncWebServerRequest* request){
        String json = "{";
        json += "\"ssid\":\"" + WiFi.SSID() + "\",";
        json += "\"ip\":\""   + WiFi.localIP().toString() + "\",";
        json += "\"mac\":\""  + WiFi.macAddress() + "\"";
        json += "}";
        request->send(200, "application/json", json);
    });

    // Datos sensores reales
    server.on("/sensors", HTTP_GET, [this](AsyncWebServerRequest* request){
        sensoresPtr->leerTodos();
        String json = "{";
        json += "\"tempDHT\":"   + jsonValue(sensoresPtr->tempDHT) + ",";
        json += "\"humDHT\":"    + jsonValue(sensoresPtr->humDHT) + ",";
        json += "\"sensDHT\":"   + jsonValue(sensoresPtr->sensDHT) + ",";
        json += "\"tempBMP\":"   + jsonValue(sensoresPtr->tempBMP) + ",";
        json += "\"presBMP\":"   + jsonValue(sensoresPtr->presBMP) + ",";
        json += "\"altBMP\":"    + jsonValue(sensoresPtr->altBMP) + ",";
        json += "\"presNMBMP\":" + jsonValue(sensoresPtr->presNMBMP) + ",";
        json += "\"bh1750\":"    + jsonValue(sensoresPtr->bh1750) + ",";
        json += "\"ppmCO2\":"    + jsonValue(sensoresPtr->ppmCO2) + ",";
        json += "\"lluv\":"      + String(sensoresPtr->lluviaPosibilidad) + ",";
        json += "\"vientoVel\":" + jsonValue(sensoresPtr->vientoVel) + ",";
        json += "\"vientoDir\":" + jsonValue(sensoresPtr->vientoDir);
        json += "}";
        request->send(200, "application/json", json);
    });

    // Cambiar WiFi
    server.on("/wifi", HTTP_POST, [this](AsyncWebServerRequest* request){
        if (request->hasParam("ssid", true) && request->hasParam("pass", true)) {
            String ssid = request->getParam("ssid", true)->value();
            String pass = request->getParam("pass", true)->value();
            prefPtr->begin("wifi", false);
            prefPtr->putString("ssid", ssid);
            prefPtr->putString("pass", pass);
            prefPtr->end();
            request->send(200, "text/plain", "OK");
        } else {
            request->send(400, "text/plain", "Missing params");
        }
    });
}

void WebController::handleReset(AsyncWebServerRequest* request) {}
void WebController::handleSendData(AsyncWebServerRequest* request) {}
void WebController::handleChangeWifi(AsyncWebServerRequest* request) {}
