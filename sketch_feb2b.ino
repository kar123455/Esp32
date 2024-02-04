#include <SPI.h>  //library for SPI communication
#include <EthernetENC.h>  //library for LAN module (we are using ENC28J60 LAN module ).These library also known as UIPEthernet.h .it is suitable for EN28j60.
#include <PubSubClient.h>  //library for MQTT connection
#include <DHT.h>  //library for DHT sensor

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Set the static IP address to use if the DHCP fails to assign
#define MYIPADDR 192,168,1,28
#define MYIPMASK 255,255,255,0
#define MYDNS 192,168,1,1
#define MYGW 192,168,1,1

//MQTT broker credentials
const char *mqtt_broker = "broker.emqx.io";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;
const char *client_id = "your-CLIENT-ID";

#define DHT_PIN 14 // Change this to the pin where your DHT sensor is connected
#define DHT_TYPE DHT22
DHT dht(DHT_PIN, DHT_TYPE);

// MQTT topics
const char *temperature_topic = "your-topic/temperature";
const char *humidity_topic = "your-topic/humidity";

//this code sets up an Ethernet client (espClient) for network communication, and then a PubSubClient (client) is instantiated using the Ethernet client. 
//This PubSubClient is likely to be used for publishing and subscribing to MQTT topics over the Ethernet connection. Make sure you have the necessary libraries installed and the appropriate hardware connected to your Arduino board for Ethernet communication.
EthernetClient espClient;
PubSubClient client(espClient); 

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Begin Ethernet");
    // You can use Ethernet.init(pin) to configure the CS pin
    Ethernet.init(15);   // MKR ETH Shield
    if (Ethernet.begin(mac)) { // Dynamic IP setup
        Serial.println("DHCP OK!");}
    else{
        Serial.println("Failed to configure Ethernet using DHCP");
        // Check for Ethernet hardware present
        if (Ethernet.hardwareStatus() == EthernetNoHardware) {
          Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
          while (true) {
            delay(1); // do nothing, no point running without Ethernet hardware
          }
        }
    if (Ethernet.linkStatus() == LinkOFF) {
          Serial.println("Ethernet cable is not connected.");}
 
          IPAddress ip(MYIPADDR);
          IPAddress dns(MYDNS);
          IPAddress gw(MYGW);
          IPAddress sn(MYIPMASK);
          Ethernet.begin(mac, ip, dns, gw, sn);
          Serial.println("STATIC OK!");
    }
    delay(5000);
 
 
    Serial.print("Local IP : ");
    Serial.println(Ethernet.localIP());
    Serial.print("Subnet Mask : ");
    Serial.println(Ethernet.subnetMask());
    Serial.print("Gateway IP : ");
    Serial.println(Ethernet.gatewayIP());
    Serial.print("DNS Server : ");
    Serial.println(Ethernet.dnsServerIP());
 
   Serial.println("Ethernet Successfully Initialized");
   Serial.println("MQTT Successfully Initialized");
  // if you get a connection, report back via serial:

   // Set MQTT broker and credentials
   client.setServer(mqtt_broker, mqtt_port);
   //client.setCredentials(mqttUsername, mqttPassword);
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
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (digitalRead(DHT_PIN) == HIGH) {
  Serial.println("Sensor is connected!");
} else{
      Serial.println("DHT is connected.");
      if((temperature<(-40) && temperature>(80)) ||(humidity<(0) && humidity>(100))){
        Serial.println("readings outof range.");

      }
    }
    ESP.restart();
   
  

    while (1);
  }
    
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
