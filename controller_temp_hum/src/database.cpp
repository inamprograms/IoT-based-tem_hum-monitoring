#include<ESP8266WiFi.h>
#include"database.hpp"

WiFiClient client;

void database::connect_to_wifi(){
    
    WiFi.begin(SSID, PASSWORD);
    Serial.println("Connecting to wifi...");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        Serial.println("connecting");
    }
    Serial.println("Connected to the wifi.");
    
}
