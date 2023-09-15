#include<ESP8266WiFi.h>
#include"database.hpp"

WiFiClient client;

//Method to connect esp8266 to the wifi 
void database::connectToWifi(){

    WiFi.begin(SSID, PASSWORD);
    Serial.println("Connecting to wifi...");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        Serial.println("connecting");
    }
    Serial.println("Connected to the wifi."); 
}

//Method to connect esp8266 with the server
void database::connectToServer(){

if (!client.connect(HOST, HTTP_PORT))
    {
        Serial.println("Connection failed");
        return;
    }
    Serial.println("Connected With server.");
}
