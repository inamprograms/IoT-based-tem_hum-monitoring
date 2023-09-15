#include <Arduino.h>
#include"database.hpp"
#include<DHT.h>

#define DHT_PIN D7
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);

database device;



void setup() {
 
  Serial.begin(9600);
  pinMode(DHT_PIN, INPUT);
  dht.begin();

  device.connectToWifi();
  device.connectToServer();
  
}

void loop() {
   
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  Serial.print("Temperature: ");
  Serial.println(temp);
  Serial.print("Humudity: ");
  Serial.println(hum);

  delay(2000);
   
}
