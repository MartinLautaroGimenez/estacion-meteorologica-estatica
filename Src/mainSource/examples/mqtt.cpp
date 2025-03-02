#include <WiFi.h>
#include <PubSubClient.h>

// Replace with your network credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Replace with your MQTT broker IP address
const char* mqttServer = "your_MQTT_broker_IP";
const int mqttPort = 1883;

// Initialize the WiFi and MQTT client objects
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    // Connect to MQTT broker
    client.setServer(mqttServer, mqttPort);
    while (!client.connected()) {
        Serial.println("Connecting to MQTT broker...");
        if (client.connect("ESP32Client")) {
            Serial.println("Connected to MQTT broker");
        } else {
            Serial.print("Failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }
}

void loop() {
    // Reconnect if connection is lost
    if (!client.connected()) {
        reconnect();
    }
    
    // Publish a message to a topic
    client.publish("topic", "Hello from ESP32");

    // Subscribe to a topic
    client.subscribe("topic");

    // Handle incoming messages
    client.loop();
}

void reconnect() {
    // Loop until reconnected
    while (!client.connected()) {
        Serial.println("Attempting MQTT connection...");
        if (client.connect("ESP32Client")) {
            Serial.println("Connected to MQTT broker");
            client.subscribe("topic");
        } else {
            Serial.print("Failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }
}