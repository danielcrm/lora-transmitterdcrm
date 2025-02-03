#include <SPI.h>
#include <LoRa.h>

#define ss 15
#define rst 5
#define dio0 4

int counter = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Transmitator LoRa");
  LoRa.setPins(ss,rst, dio0);

  if (!LoRa.begin(868E6)) {
    Serial.println("Transmitatorul nu a pornit");
    while (1);
  }
  LoRa.setSyncWord(0xF3);
  LoRa.setTxPower(10);
}

void loop() 
{
  Serial.print("Se trimite pachetul cu numarul: ");
  Serial.println(counter);
  Serial.print(" cu RSSI ");
  Serial.println(LoRa.packetRssi());
  // send packet
  LoRa.beginPacket();
  LoRa.print("Acest mesaj a fost transmis prin intermediul LoRa ");
  LoRa.print(counter);
  LoRa.endPacket();
  counter++;

  delay(3000);
}
