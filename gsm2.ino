#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#define LED_IND D6// opto D6=DATA ENTRY INDICATOR HTTP, new gsm device D5
//#define LED_BLUE D5// network HTTP INDICATOR, new gsm device D6

#define rxPin D1
#define txPin D7
SoftwareSerial sim800L(rxPin, txPin);

//GPS Module RX pin to Arduino 9
//GPS Module TX pin to Arduino 8

TinyGPSPlus gps;

unsigned long previousMillis = 0;
long interval = 3000;

 uint8_t x = 0,  answer = 0;
  char response[100];
  unsigned long previous;
 
#include "DHT.h"
#define DHTPIN D2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
const int transistor = D4;
String apiKeyValue = "SL-2024";


#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

// select which pin will trigger the configuration portal when set to LOW
#define TRIGGER_PIN 0

int timeout = 100; // seconds to run for


void setup()
{
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("\n Starting");
  pinMode(TRIGGER_PIN, INPUT_PULLUP);

  // initialize digital pin LED_BUILTIN as an output.
  //pinMode(LED_BUILTIN, OUTPUT);


  pinMode (transistor,  OUTPUT);
  pinMode(LED_IND, OUTPUT);
//  pinMode(LED_BLUE, OUTPUT);

  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(115200);
  dht.begin();
  //Begin serial communication with Arduino and SIM800L
  sim800L.begin(9600);


  Serial.println("Initializing...");
  //delay(10000);

  //Once the handshake test is successful, it will back to OK
  sendATcommand("AT", "OK", 2000);
  
  sendATcommand("AT+CMGF=1", "OK", 2000);
  //sim800L.print("AT+CMGR=40\r");
 Serial.println("Transistor high");
  digitalWrite(transistor, HIGH);
  delay(1000);
  Serial.println("Transistor low");
  digitalWrite(transistor, LOW);
  delay(1000);
  Serial.println("Transistor high");
  digitalWrite(transistor, HIGH);
  delay(1000);
}

void loop()
{
  digitalWrite (transistor, HIGH);
  Serial.println("Transistor ON");



  // is configuration portal requested?
  if ( digitalRead(TRIGGER_PIN) == LOW) {
    WiFiManager wm;

    //reset settings - for testing
    //wm.resetSettings();

    // set configportal timeout
    wm.setConfigPortalTimeout(timeout);

    if (!wm.startConfigPortal("Device (senselive.in)", "senselive.in")) {
      Serial.println("failed to connect and hit timeout");
      delay(3000);
      //reset and try again, or maybe put it to deep sleep
      ESP.restart();
      delay(5000);
    }

    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");

  }

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // delay(50);

  if (isnan(temperature) || isnan(humidity)) {

    digitalWrite (transistor, LOW);
    Serial.println("Transistor OFF");
    delay(5000);
    digitalWrite (transistor, HIGH);
    Serial.println("Transistor ON");
    //  Serial.println("Failed to read from DHT sensor!");
    digitalWrite(LED_IND, HIGH);  // turn the LED on (HIGH is the voltage level)
     return;
  }
  else{ ReadandSend();
  //delay(1000);
  //Retry();
  GSMRetry();

  }
}
/*void Retry(){
   if (sim800L.available() != 0) {
      response[x] = sim800L.read();
      Serial.print(response[x]);
      if (response[x] == 200) {
        Serial.println("200 succsess");
      }
      if(response[x]!=200){
        for(int i=0;i<2;i++){

          Serial.println("Retry and send");
         // ReadandSend();
          
        }
        if(sendATcommand("AT+CFUN=1,1", "OK", 200)){
          Serial.println("Retry and gsm restart");
          delay(1000);
          ESP.restart();

        }
        }
   }
   }*/
 void ReadandSend(){

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

 if (!isnan(temperature) || !isnan(humidity)) {
  digitalWrite (transistor, HIGH);
  Serial.println("Transistor ON");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("°C ");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  delay(2000);

  String auth = "SL-2024", deviceUID = "SL02202310";//SL23-CEAT-TEMP-01//SL02202301


 // String meter_URL = "http://technicalabcd.com/connection/Elkem/temp_elkem.php?";
   String meter_URL = "http://technicalabcd.com/connection/CEAT/temp_ceat.php?";
  //  String meter_URL = "http://technicalabcd.com/meter_connection/energyInsert_1.php?";

  meter_URL += "auth=" ;
  meter_URL += auth;
  meter_URL += "&device_uid=" ;
  meter_URL += deviceUID;

  meter_URL += "&temperature=" ;
  meter_URL += String(temperature);
  meter_URL += "&humidity=" ;
  meter_URL += String(humidity);


  while (sim800L.available()) {
    Serial.println(sim800L.readString());
  }
  while (Serial.available())  {
    sim800L.println(Serial.readString());
  }



  sendATcommand("AT+CFUN=1", "OK", 200);//
  delay(200);

  //AT+CGATT = 1 Connect modem is attached to GPRS to a network. AT+CGATT = 0, modem is not attached to GPRS to a network
  sendATcommand("AT+CGATT=1", "OK", 200);
  delay(200);

  //Connection type: GPRS - bearer profile 1
  sendATcommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"", "OK", 200);
  delay(200);

  //sets the APN settings for your network provider.
  sendATcommand("AT+SAPBR=3,1,\"airtelgprs.com\",\"internet\"", "OK", 200); 
  delay(200);

  //enable the GPRS - enable bearer 1
  sendATcommand("AT+SAPBR=1,1", "OK", 200);
  delay(200);

  //Init HTTP service
  sendATcommand("AT+HTTPINIT", "OK", 200);
  delay(200);

  sendATcommand("AT+HTTPPARA=\"CID\",1", "OK", 100);
  //Set the HTTP URL sim800.print("AT+HTTPPARA="URL","http://ahmadssd.000webhostapp.com/gpsdata.php?lat=222&lng=222"\r");
  sim800L.print("AT+HTTPPARA=\"URL\",\"");
  sim800L.print(meter_URL);
   Serial.println(meter_URL);
  sendATcommand("\"", "OK", 1000);
  //Set up the HTTP action
  sendATcommand("AT+HTTPACTION=0", "0,200", 100);
  delay(200);
 

  if (sim800L.find("OK"))
  {
    Serial.println("AT+HTTPREAD");
    delay(1000);
    String data = sim800L.readString();
    Serial.print(data);
  }


  //Terminate the HTTP service
  sendATcommand("AT+HTTPTERM", "OK", 1000);
  //shuts down the GPRS connection. This returns "SHUT OK".
  // sendATcommand("AT+CIPSHUT", "SHUT OK", 1000);
  delay(1000);
  digitalWrite(LED_IND, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(LED_IND, LOW);   // turn the LED off by making the voltage LOW
  delay(500);


      
        for(int i=0;i<2;i++){
          if (sim800L.find("200")) {
      Serial.println("HTTP Action successful");
      return; // Break out of the loop if successful response
    
      delay(1000);
          
        }
        
        
        if(sendATcommand("AT+CFUN=1,1", "OK", 200)){
          Serial.println("Retry and gsm restart");
          delay(1000);
          ESP.restart();

        }
        }
        
   
   }

 
 }

 void GSMRetry(){
  
  if(sim800L.find(200)){
    Serial.println("gotit");
    delay(1000);
  }
  else{
    for(int i=0;i<2;i++){
      GSMRetry();
    }delay(1000);
    ESP.restart();
  }
 }



int8_t sendATcommand(const char* ATcommand, const char* expected_answer, unsigned int timeout) {

  uint8_t x = 0,  answer = 0;
  char response[100];
  unsigned long previous;

  //Initialice the string
  memset(response, '\0', 100);
  delay(100);

  //Clean the input buffer
  while ( sim800L.available() > 0) sim800L.read();

  if (ATcommand[0] != '\0') {
    //Send the AT command
    sim800L.println(ATcommand);
  }

  x = 0;
  previous = millis();

  //this loop waits for the answer with time out
  do {
    //if there are data in the UART input buffer, reads it and checks for the asnwer
    if (sim800L.available() != 0) {
      response[x] = sim800L.read();
      Serial.print(response[x]);
      if (response[x] == 200) {
        Serial.println("200 succsess");
      }
      x++;
      // check if the desired answer (OK) is in the response of the module
      if (strstr(response, expected_answer) != NULL) {
        answer = 1;
      }
    }
  } while ((answer == 0) && ((millis() - previous) < timeout));

  //Serial.println(response);
  return answer;
}
