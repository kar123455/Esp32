#include <Ethernet.h>
#include <SPI.h>

// ENC28J60 SPI pins
#define SDA_PIN 19 // SPI MISO
#define SCL_PIN 23  // SPI MOSI
#define CS_PIN 5  // Chip Select
#define INT_PIN 2   // Interrupt
#define CLK_PIN 18  //clock

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

EthernetClient client;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // Start the SPI communication
  SPI.begin(SDA_PIN, SCL_PIN, CS_PIN);

  // Start the Ethernet connection with DHCP
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    delay(1000);
    ESP.restart();
  }

  Serial.println("Ethernet initialized");

  // Print the assigned IP address
  Serial.print("Assigned IP address: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // Your main loop code here
  // For example, check for network connectivity
  if (Ethernet.linkStatus() == LinkON) {
    Serial.println("Network is connected");
  } else {
    Serial.println("Network is not connected");
  }

  delay(1000);
}
