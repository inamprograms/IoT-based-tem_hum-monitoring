
This directory is intended for microcontroller-side code.


- [server](/server/README.md)
- [IoT Based Temperature and Humidity Monitoring](/README.md)

The `include` directory contains header file `database.hpp` that contain the declaration of class `database` and macro definitions.

In database class I declared the following methods:
-    String getMacAddress()<br> &emsp;->> to get the mac address of the device (microcontroller) 
-    void connectToWifi()<br> &emsp;->> to connect microcontroller with the WiFi 
-    void connectToServer()<br> &emsp;->> to connect microcontroller with the node js server running 
-    void sendData(uint8_t option, float temperature, float humidity, String macAddress)<br> &emsp;->> after successful connection this method is used to send data to mysql database through the API
-    void fireBaseSetup()<br> &emsp;->> method to setup the firebase database for SignUp to the firebase database 
-    void postToFirebaseDatabase(float temperature, float humidity)<br> &emsp;->> After successful SighUp send data to the firebase database in real time
  
The `src` folder contains the source files `main.cpp` and `database.cpp`. 

`database.cpp` contains the definitions of member functions defined in the class. 

`main.cpp` contains the code to collect temperature and humidity data. And it calls the member functions of the class by creating its object.
