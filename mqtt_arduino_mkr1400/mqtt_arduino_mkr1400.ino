// This example uses an Arduino MKR GSM 1400 board
// The MKR1400 connects to CloudMQTT and exchanges messages. 
// If CloudMQTT sends a message with the topic "arduino", then the MKR1400 turns on its LED
// Dimitris Papanikolaou
// Based on code by Sandeep Mistry / modified by Dimitris Papanikolaou
// https://github.com/256dpi/arduino-mqtt

#include <MKRGSM.h>
#include <MQTT.h>
#include <Arduino_MKRGPS.h>


const char boardIDString[] = "arduino4"; //has to be unique
const int boardID = 4; //has to be unique

const char pin[]      = "";
const char apn[]      = "fast.t-mobile.com";
const char login[]    = "";
const char password[] = "";

GSMSSLClient net;
GPRS gprs;
GSM gsmAccess;
MQTTClient client;

unsigned long lastMillis = 0;

const int fanPin =  1;      // the number of the source fan realy pin


void connect() {
  // connection state
  bool connected = false;

  Serial.print("connecting to cellular network ...");

  // After starting the modem with gsmAccess.begin()
  // attach to the GPRS network with the APN, login and password
  while (!connected) {
    if ((gsmAccess.begin(pin) == GSM_READY) &&
        (gprs.attachGPRS(apn, login, password) == GPRS_READY)) {
      connected = true;
    } else {
      Serial.print(".");
      delay(1000);
    }
  }

  Serial.print("\nconnecting...");
  //first argument in .connect is the ID of the board and has to be unique for each board eg:arduino4 where 4 is no.assigned to this arduino
  while (!client.connect(boardIDString, "ozzhcvbr", "1t9Q28WXW4o2")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");
 
  client.subscribe("burst");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);

  if(topic=="burst"){
      digitalWrite(fanPin, HIGH); 
      delay(2000);
      digitalWrite(fanPin, LOW);
  }
  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, changRe a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(fanPin, OUTPUT);
  
  
  Serial.begin(115200);

  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported
  // by Arduino. You need to set the IP address directly.
  client.begin("tailor.cloudmqtt.com", 24612, net);
  client.onMessage(messageReceived);

  connect();
}

void loop() {
  client.loop();

  if (!client.connected()) {
    connect();
  }

}
