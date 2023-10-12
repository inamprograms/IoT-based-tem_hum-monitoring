#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <WiFiManager.h>
#include "config.hpp"
#include "database.hpp"

WiFiClient client;
WiFiManager wifiManager;

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool signupOK = false;
unsigned long sendDataPrevMillis = 0;
int count = 0;

// Method to connect esp8266 to the wifi
void database::connectToWifi()
{

    wifiManager.autoConnect("AutoConnectAp");
    WiFi.begin(WiFi.SSID(), WiFi.psk());
    // WiFi.begin(WIFI_SSID, PASSWORD);
    Serial.println("Connecting to wifi...");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        Serial.println("connecting");
    }
    Serial.println("Connected to the wifi.");
}

// Method to connect esp8266 with the server
void database::connectToServer()
{

    if (!client.connect(HOST, HTTP_PORT))
    {
        Serial.println("Connection failed");
        return;
    }
    Serial.println("Connected With server.");
}

// Method to post data from esp8266 to the Mysql database through api
void database::sendData(uint8_t myChoice, float temp, float hum, String device_id)
{
    char packet[1000];
    char payload[500];
    int contentLength;
    switch (myChoice)
    {
    case POST_DATA:

        contentLength = sprintf(payload, "{ \"device_id\" : \"%s\", \"temperature\" : \"%f\", \"humidity\" : \"%f\" }", device_id.c_str(), temp, hum);
        sprintf(packet, "POST %s HTTP/1.1\r\nHOST:%s\r\nContent-type:%s\r\nContent-length:%i\r\n\r\n%s", POST_TEMP_HUM, HOST, CONTENT_TYPE, contentLength, payload);
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

// Method to get Mac address of device
String database::getMacAddress()
{

    uint8_t macBytes[6];
    WiFi.macAddress(macBytes);

    String macAddress;
    for (int i = 0; i < 6; i++)
    {
        macAddress += String(macBytes[i], HEX);
        if (i < 5)
        {
            macAddress += ":";
        }
    }
    return macAddress;
}

// Method to setup Firebase real time database
void database::fireBaseSetup()
{

    config.api_key = Config::FIREBASE_API_KEY;
    config.database_url = Config::FIREBASE_DATABASE_URL;

    if (Firebase.signUp(&config, &auth, "", ""))
    {
        Serial.println("ok");
        signupOK = true;
    }
    else
    {
        Serial.printf("%s\n", config.signer.signupError.message.c_str());
    }
    config.token_status_callback = tokenStatusCallback;
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
}

// Method to post data to Firebase database
void database::postToFirebaseDatabase(float temperature, float humidity)
{

    if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
    {
        sendDataPrevMillis = millis();

        // Write an Float number on the database path test/float
        if (Firebase.RTDB.setFloat(&fbdo, "roomData/temperature", temperature))
        {
            Serial.println("PASSED, Temperature");
            Serial.println("PATH: " + fbdo.dataPath());
            Serial.println("TYPE: " + fbdo.dataType());
        }
        else
        {
            Serial.println("FAILED");
            Serial.println("REASON: " + fbdo.errorReason());
        }
        if (Firebase.RTDB.setFloat(&fbdo, "roomData/humidity", humidity))
        {
            Serial.println("PASSED, humidity");
            Serial.println("PATH: " + fbdo.dataPath());
            Serial.println("TYPE: " + fbdo.dataType());
        }
        else
        {
            Serial.println("FAILED");
            Serial.println("REASON: " + fbdo.errorReason());
        }
    }
}

// Method to send Over the Air Update
void database::sendOTA()
{
    ArduinoOTA.onStart([]()
                       {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else {  // U_FS
      type = "filesystem";
    }
    Serial.println("Start updating " + type); });
    // ArduinoOTA.onStart([]()
    //                    { Serial.println("Start"); });
    
    ArduinoOTA.onEnd([]()
                     { Serial.println("\nEnd"); });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                          { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });

    ArduinoOTA.onError([](ota_error_t error)
                       {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    } });

    ArduinoOTA.begin();
    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}
