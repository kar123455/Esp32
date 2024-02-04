#include <ESP32-ENC28J60.h>



// ENC28J60 SPI pins
#define SDA_PIN 19 // SPI MISO
#define SCL_PIN 23 // SPI MOSI
#define CS_PIN 5  // Chip Select
#define INT_PIN 2   // Interrupt
byte mac[] = {0x02, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE};
// Replace with your MAC address


EthernetClient client;

void setup() {
    Serial.begin(115200);
    Serial.println("hiiii");
    while (!Serial);

    // Initialize Ethernet library
    Ethernet.begin(mac // Replace with your MAC address
, CS_PIN);
    Serial.println("Attempting to connect to server...");

    // Your setup code here
}

void loop() {
    // Your main loop code here
}
