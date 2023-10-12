# Node Js Server For Temperature and Humidity data management

This server serves the data(temperature and humidity) collected by the microcontroller from the environment. It servers a simple static web page that shows the graph of the data collected when the user clicks on the `Get Graph` button the Geraph will be shown on web page. It provides the service to post data to the MySql database and send data to the Telegram user(using ChatId) via Telegram Bot.

The following URIs are used in API:

GET `/` ->> serves the static web page<br>
GET `/createTable` ->> to create a table (Not completed yet)<br>
POST  `/weatherData` ->> to send data into database<br>
GET `/getData` ->> to get data from the database<br>
POST `/sendDataToTelegramBot` ->> to send data to user via telegram bot<br> 
GET `/plotData` and GET `/plot` are temporarily created just for testing purpose<br> 

- [controller_temp_hum](/controller_temp_hum/README.md)
- [IoT Based Temperature and Humidity Monitoring](/README.md)