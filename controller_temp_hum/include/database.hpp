#ifndef DATABASE_HPP
#define DATABASE_HPP

// #define WIFI_SSID "Tenda_0F1FF0"
// #define PASSWORD "Envi3691"

#define HOST "192.168.0.110"
#define HTTP_PORT 5000

#define POST_DATA 0

#define DHT_PIN D7
#define DHT_TYPE DHT11

// #define DEVICE_ID ""
#define POST_TEMP_HUM "/weatherData"
#define CONTENT_TYPE "application/json"

#include <Arduino.h>
#include <stdint.h>
class database
{
private:
    /* data */
public:
    String getMacAddress();
    void connectToWifi();
    void connectToServer();
    void sendData(uint8_t option, float temperature, float humidity, String macAddress);
    void fireBaseSetup();
    void postToFirebaseDatabase(float temperature, float humidity);
    void sendOTA();
};

#endif