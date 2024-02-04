#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// WiFi credentials
const char *ssid = "Airtel_SenseLive"; // Enter your Wi-Fi name
const char *password = "SL123456";  

// EMQ X Cloud MQTT credentials
const char *mqtt_broker = "broker.emqx.io";
//const char *topic = "sensor/+/temperature";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;
const char *client_id = "your-CLIENT-ID";

// DHT sensor setup
#define DHT_PIN 4 // Change this to the pin where your DHT sensor is connected
#define DHT_TYPE DHT22
DHT dht(DHT_PIN, DHT_TYPE);

// MQTT topics
const char *temperature_topic = "your-topic/temperature";
const char *humidity_topic = "your-topic/humidity";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(250);
        Serial.print(".");
    }
    Serial.println("WiFi connected");

    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);

    while (!client.connected()) {
        if (client.connect( client_id, mqtt_username, mqtt_password)) {
            Serial.println("MQTT connected");
        } else {
            Serial.print("Failed, rc=");
            Serial.print(client.state());
            Serial.println(" Retrying in 5 seconds...");
            delay(5000);
        }
    }

    dht.begin();
}

void loop() {
    client.loop();

    // Read DHT sensor data
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // Publish data to MQTT topics
    if (!isnan(temperature)) {
        client.publish(temperature_topic, String(temperature).c_str());
    }

    if (!isnan(humidity)) {
        client.publish(humidity_topic, String(humidity).c_str());
    }

    // Wait for a moment before publishing again
    delay(5000);
}

void callback(char *topic, byte *payload, unsigned int length) {
    // Handle incoming MQTT messages if needed
}

