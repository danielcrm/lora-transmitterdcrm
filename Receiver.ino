#include <ESP8266WiFi.h>
#include <SPI.h>
#include <LoRa.h>

//define the pins used by the transceiver module
#define ss 15
#define rst 5
#define dio0 4

/* Initializare client Wi-Fi si API Thingspeak */
String apiKey = "G7GRGL8DKBLMZAOE";       // Enter your Write API key from ThingSpeak

const char* ssid = "HUAWEI-apwC";           // Give your wifi network name
const char* password = "caramidadani_SV";   // Give your wifi network password
const char* server = "api.thingspeak.com";
WiFiClient client;
/*///////////////////////////////////////*/
bool rx=false;
unsigned long pulseTime,lastTime;
double puterea, kWh;
long Counter = 0;
void setup() {
  //initialize Serial Monitor
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Receptor LoRa");

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  if(!LoRa.begin(868E6)) {
    Serial.println("Initiere esuata");
    while (1);
  }
   // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("Initiere reusita");
   //Modul WiFi
       Serial.println("Connecting to ");
       Serial.println(ssid);
 
 
       WiFi.begin(ssid, password);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
}

void loop() 
{
  
  String LoRaData; 
  int packetSize = LoRa.parsePacket();
  if (packetSize) 
  {
    // read packet
    while (LoRa.available())
    {
      LoRaData = LoRa.readString();
      Serial.println(LoRaData); 
       lastTime = pulseTime;
       pulseTime = micros();
       puterea = (3600000000.0 / (pulseTime - lastTime));
       Counter++;
       kWh = (1.0*Counter/(ppwh*1000));
       Serial.print(putere,4);
       Serial.print(" ");
       Serial.println(kWh,3);
    }
    if (client.connect(server, 80)) // "184.106.153.149" or api.thingspeak.com
   {
      String postStr = apiKey;
      postStr += "&field1=";
      postStr += String(puterea);
      postStr += "&field2=";
      postStr += String(kWh);
      
      postStr += "\r\n\r\n\r\n\r\n";
    
      client.print("POST /update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(postStr.length());
      client.print("\n\n");
      client.print(postStr);
 
    }    
    //delay(30000);

   }
}
