//modules
var express = require('express');
var http = require('http');
var mqtt = require('mqtt');
var socket = require('socket.io');

var app = express();
var server = http.createServer(app);
var io = socket(server);

app.use(express.static('public'));

server.listen(3000, function(){
      console.log("listening on port 3000...")
});

// Create an instance of the mqttclient and the socketioserver and define their interactions

/**********************************/
/*********      MQTT      *********/
/**********************************/

var mqttOptions = {
      port: "your_port",
      clientId: "your_clientID",
      username: "your_username",
      password: "your_password",
};
var mqttClient = mqtt.connect('mqtt://tailor.cloudmqtt.com', mqttOptions);
mqttClient.on('connect', function () {
      console.log("node server connected to mqtt broker");
      mqttClient.publish("announcements", "MQTT client ready");
});


/**********************************/
/*********   SOCKET.IO    *********/
/**********************************/
io.on('connection', function (socket) {
      console.log('user ' + socket.id + ' connected');

      socket.on("burst", function(data){
            mqttClient.publish("burst", "move air");
            console.log("burst");
      });

      socket.on('disconnect', function () {
        console.log('user ' + socket.id + ' disconnected');
    });
});
