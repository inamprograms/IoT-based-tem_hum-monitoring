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

//Method to post data from esp8266 to the database through api
void database::sendData(uint8_t myChoice, float temp, float hum){
    char packet[1000];
    char payload[500];
    int contentLength;
    String device_id = "device1";
    switch (myChoice)
    {
    case POST_DATA:
        contentLength = sprintf(payload, "{ \"device_id\" : \"%s\", \"temperature\" : \"%f\", \"humidity\" : \"%f\" }", device_id, temp,hum);
        sprintf(packet, "POST %s HTTP/1.1\r\nHOST:%s\r\nContent-type:%sr\r\nContent-length:%i\r\n\r\n%s", POST_TEMP_HUM, HOST, CONTENT_TYPE, contentLength, payload);
        Serial.print(packet);
        client.print(packet);
        while (client.available())
        {
            String line = client.readStringUntil('\r');
            Serial.print(line);
        }
        Serial.println();
        Serial.println("Closing connection");
        break;
    
    default: 
        break;
    }
}
