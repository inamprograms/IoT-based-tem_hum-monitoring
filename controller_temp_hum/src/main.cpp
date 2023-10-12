#include <Arduino.h>
#include "database.hpp"
#include <DHT.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

DHT dht(DHT_PIN, DHT_TYPE);

database device;
String macAddress = "";

void setup()
{

  Serial.begin(9600);

  device.connectToWifi();
  // device.sendOTA();

  pinMode(DHT_PIN, INPUT);
  dht.begin();

  macAddress = device.getMacAddress();
  // device.fireBaseSetup();
  // device.fireBaseSetup();
}

void loop()
{

  // ArduinoOTA.handle();

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  // Serial.print("Temperature: ");
  // Serial.println(temp);
  // Serial.print("Humudity: ");
  // Serial.println(hum);

  device.connectToServer();
  device.sendData(POST_DATA, temp, hum, macAddress);
  // device.postToFirebaseDatabase(temp, hum);

  delay(5000);
}
