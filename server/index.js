const express = require("express");
const path = require('path')
const mysql = require("mysql");
const cors = require('cors')
const bodyParser = require('body-parser');
require('dotenv').config();
const TelegramBot = require('node-telegram-bot-api')

var app = express();
const plotly = require('plotly')(process.env.PLOTLY_USERNAME, process.env.PLOTLY_API_KEY);
var temp = [], hum = [], time = [];

const token = process.env.WEATHER_BOT_TOKEN;
const chatId = 6639483412;
// const chatId = 6682457220;
const bot =  new TelegramBot(token, {polling: true});

//Middlewares
app.use(cors())
app.use(bodyParser.json())
app.use(bodyParser.urlencoded({extended:false}))
app.use(express.static(__dirname + '/public'))

const db = mysql.createConnection({
    host: process.env.DB_HOST,
    user: process.env.DB_USER, 
    password: process.env.DB_PASSWORD,
    database: process.env.DB_DATABASE,
    port: process.env.DB_PORT,
    // host: "86.1.199.8",
    // user: "root", 
    // password: "Asdbnm@12345",
    // database: "inamDb",
    // port: 3307,
});

//Routes
app.get('/', (req, res)=>{
    res.render('index.html')
});

app.get('/createTable', (req, res)=>{
                                                                       
    let sql = "CREATE TABLE temp_hum (Sr INT AUTO_INCREMENT NOT NULL, device_id VARCHAR(50), temperature INT, humidity INT, date TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP, PRIMARY KEY(Sr))";
    db.query(sql, (err)=>{
        if(err){
            throw(err)
        }
        res.send("Table created");
    });
});
//Controller sending data
app.post('/weatherData', (req, res)=>{
    var device_id = req.body.device_id;
    var temp = req.body.temperature;
    var hum = req.body.humidity;
    console.log(req.body);
    console.log(device_id);
    console.log(temp);
    console.log(hum);
    let sql = "INSERT INTO temp_hum (device_id, temperature, humidity) VALUES (?, ?, ?)";
    db.query(sql, [device_id, temp, hum], (err)=>{
        if(err){
            throw(err)
        }
        res.redirect("http://localhost:5000/");
    });
});
// Get data from database
app.get('/getData', (req, res)=>{
    let sql = "SELECT * FROM temp_hum";
    db.query(sql, (err, result)=>{
        if(err){
            throw err;
        }
        formatData(result);
        // for(const record in result){
        //     const row = result[record];
        //     for(const key in row){
        //         const value = row[key];
        //         console.log(value);
        //     }
        // }
        res.send(jsonArray);
    });
});
//Function to store data into json array
function formatData(dataArray) {
    for(var i = 0; i < dataArray.length; i++) {
      temp[i] = dataArray[i].temperature;
      hum[i] = dataArray[i].humidity;
      time[i] = dataArray[i].date;
    }
    jsonArray = [temp, hum, time];
    console.log("in FormatData()...\n");
    console.log(jsonArray);

  }

app.post('/sendDataToTelegramBot', (req, res)=>{
    
    var message = req.body.message;
    bot.sendMessage(chatId, message);
    bot.on('polling_error', (e)=>{
        console.log('Polling error: ', e);
    });
    res.send("Telegram message sent");
});
// For jsut testig 
app.get('/plotData', (req, res)=>{
    let sql = "SELECT temperature, humidity, date FROM temp_hum ";
    db.query(sql, (err, result)=>{
        if(err){
            throw err;
        }
        var data = [
            {
              x: [0],
              y: [0],
              type: "scatter"
            }
        ];
        const temp = result.temperature;
        for (const index in result){
            const row = result[index];
            data[0].x.push(row.date); 
            data[0].y.push(row.temperature);           
        }
        // res.send(data);
        // console.log(data);
        var graphOptions = {filename: "date-axes", fileopt: "overwrite"};
        plotly.plot(data, graphOptions, function (err, msg) {
            console.log(msg);
        });
    });
});
//Fot testing data is coming from data base
app.get('/plot', (req, res)=>{
    let sql = "SELECT temperature, humidity, date FROM temp_hum ";
    db.query(sql, (err, result)=>{
        if(err){
            throw err;
        }    
        res.send(result);
        
    });
});

app.listen(5000, ()=>{
    console.log("Server is running...");
});

