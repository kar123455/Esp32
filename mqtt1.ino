#include <WiFi.h>
#include <PubSubClient.h>

// Update these with your EMQX MQTT broker and Wi-Fi credentials
const char* ssid = "Redmi Note 9 Pro";
const char* password = "Samyak16";
const char* mqttServer = "y1a1d1d1.ap-east-1.emqx.cloud";
const int mqttPort = 15654;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  // Connect to MQTT broker
  Serial.print("Connecting to MQTT broker");
  client.setServer(mqttServer, mqttPort);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Publish a message
  Serial.println("Publishing message...");
  client.publish("topic/test", "Hello from ESP32");

  // Wait for 1 second before publishing the next message
  delay(1000);
}

void reconnect() {
  // Loop until reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (client.connect("ESP32Client")) {
      Serial.println("Connected to MQTT broker");
      // Subscribe to a topic if needed
      // client.subscribe("topic/subscribe");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
}
