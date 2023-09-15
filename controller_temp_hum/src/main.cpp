#include <Arduino.h>
#include<DHT.h>

#define DHT_PIN D7
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);


// put function declarations here:
// int myFunction(int, int);

void setup() {
  // put your setup code here, to run once: 
  Serial.begin(9600);
  pinMode(DHT_PIN, INPUT);
  dht.begin();
  
  // int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:
   
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  Serial.print("Temperature: ");
  Serial.println(temp);
  Serial.print("Humudity: ");
  Serial.println(hum);

  delay(2000);
   
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}