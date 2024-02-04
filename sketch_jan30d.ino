#include <Ethernet.h>
#include <SPI.h>

// ENC28J60 SPI pins
#define SDA_PIN 19 // SPI MISO
#define SCL_PIN 23 // SPI MOSI
#define CS_PIN 5  // Chip Select
#define INT_PIN 2   // Interrupt
byte mac[] = {0x02, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE};


// MAC address of your ENC28J60 module{0x02, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE};

//byte mac[] = {0x54, 0x55, 0x58, 0x10, 0x00, 0x24};  

EthernetClient client;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // Start the SPI communication
  SPI.begin(SDA_PIN, SCL_PIN, CS_PIN);

  // Start the Ethernet connection
  ///if (Ethernet.begin(mac) == 0) {
  //Serial.println("Failed to configure Ethernet using DHCP");
  //Serial.println("Trying static IP configuration...");
  //IPAddress ip(192, 168, 1, 10);
  //Ethernet.begin(mac, ip);
//}
IPAddress ip(192, 168, 1, 13); // Replace with your desired IP address
Ethernet.begin(mac, ip);



  Serial.println("Ethernet initialized");
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
