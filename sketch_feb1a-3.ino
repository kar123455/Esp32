//#include <UIPEthernet.h>

#include <UIPEthernet.h>
#include <PubSubClient.h>
#include <DHT.h>

byte mac[] = {0x54, 0x55, 0x58, 0x10, 0x00, 0x24};  // Replace with your unique MAC address
IPAddress ip(192, 168, 1, 100);  // Replace with the desired IP address
const char *mqtt_broker = "broker.emqx.io";
//const char *topic = "sensor/+/temperature";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;
const char *client_id = "your-CLIENT-ID";

#define DHT_PIN 4 // Change this to the pin where your DHT sensor is connected
#define DHT_TYPE DHT22
DHT dht(DHT_PIN, DHT_TYPE);

const char *temperature_topic = "your-topic/temperature";
const char *humidity_topic = "your-topic/humidity";

UIPClient espClient;
PubSubClient client(espClient);


void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  while(!Ethernet.linkStatus()){
    delay(250);
    Serial.println("hiii");
  }
  Serial.println("ethernet connected.");
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
  // Your main loop code...
  Ethernet.maintain();
  //Serial.println("Failed to configure Ethernet using DHCP");
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

