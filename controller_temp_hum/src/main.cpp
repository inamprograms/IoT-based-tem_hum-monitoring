#include <Arduino.h>
#include "database.hpp"
#include <DHT.h>

DHT dht(DHT_PIN, DHT_TYPE);

database device;
String macAddress = "";

void setup()
{

  Serial.begin(9600);
  pinMode(DHT_PIN, INPUT);
  dht.begin();

  device.connectToWifi();
  macAddress = device.getMacAddress();

  // device.fireBaseSetup();
}

void loop()
{

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  Serial.print("Temperature: ");
  Serial.println(temp);
  Serial.print("Humudity: ");
  Serial.println(hum);

  device.connectToServer();
  device.sendData(POST_DATA, temp, hum, macAddress);
  // device.postToFirebaseDatabase(temp, hum);
  delay(15000);
}
