/*
  Projet d'apprentissage d'un objet connecté (IoT)  pour réaliser une sonde de température
  ESP8266 + LED + MQTT + BUZZER
  Projets DIY (http://www.projetsdiy.fr) - Mai 2016
  Licence : MIT

  Copyright (c) 2019 Ignazio Binanti

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//*#include "DHT.h"          // Librairie des capteurs DHT

//todo: configuration of the wifi shall be read from ESP EEPROM
//todo: add webpage for the configuration of WiFi parameters 
#define wifi_ssid "778116"
#define wifi_password "260237470"

//todo: configuration of the mqtt server shall be read from ESP EEPROM
//todo: add webpage for the configuration of MQTT server parameters 
#define mqtt_server "m24.cloudmqtt.com"
#define mqtt_user "pgswncfj"      
#define mqtt_password "9deOwCVQLJXX"
#define mqtt_port 11436
#define temperature_topic "sensor/temperature"  //Topic temperature
//*#define humidity_topic "sensor/humidity"        //Topic humidity

//Buffer to decode MQTT messages
char message_buff[100];

long lastMsg = 0;   
long lastRecu = 0;
bool debug = false;  //Display log message if True

// Create abjects
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);     
  pinMode(BUILTIN_LED,OUTPUT); //GPIO2 is builtin LED
  digitalWrite(BUILTIN_LED,HIGH);  
  setup_wifi();                //Connect to Wifi network
  client.setServer(mqtt_server, mqtt_port);// Configure MQTT connexion
  client.setCallback(callback);            // callback function to execute when a MQTT message   
}

//Connexion au réseau WiFi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

//*todo:add error message tracking
//* LOLIN D1 has a led comnnected to GPIO2
//* LED ON stable, not blinking = normal operation
//* LED blinking "1" coded as 750ms on / 250ms OFF
//* LED blinking "0" coded as 250ms on / 750ms off
//* START = 1s OFF
//* LED blinging duty cicle 50% error
//* LED 3 fast + 1 long blinking = WiFi connecting
//* LED 4 fast + 1 long blinking = MQTT connecting
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi OK ");
  Serial.print("=> ESP8266 IP address: ");
  Serial.print(WiFi.localIP());
}

//Reconnexion
void reconnect() {

//*todo:add error message tracking
  while (!client.connected()) {
    Serial.print("Connecting to MQTT broker ...");
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("OK");
    } else {
      Serial.print("KO, error : ");
      Serial.print(client.state());
      Serial.println(" Wait 5 secondes before to retry");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  // Send a message every minute
  if (now - lastMsg > 1000 * 60) {
    lastMsg = now;

    // Read temperature in Celcius
    // to be implemented
    float t = 25.0;
  
    if ( debug ) {
      Serial.print("Temperature : ");
      Serial.println(t);
    }  
    client.publish(temperature_topic, String(t).c_str(), true);   // Publish temperature on temperature_topic
  }
  if (now - lastRecu > 100 ) {
    lastRecu = now;
    client.subscribe("bell");
  }
}

// MQTT callback function
// D'après http://m2mio.tumblr.com/post/30048662088/a-simple-example-arduino-mqtt-m2mio
void callback(char* topic, byte* payload, unsigned int length) {

  int i = 0;
  if ( debug ) {
    Serial.println("Message recu =>  topic: " + String(topic));
    Serial.print(" | longueur: " + String(length,DEC));
  }
  // create character buffer with ending null terminator (string)
  for(i=0; i<length; i++) {
    message_buff[i] = payload[i];
    if (message_buff[i] == ',') 
      break;
  }
  message_buff[i] = '\0';
  
  String msgString = String(message_buff);
  if ( debug ) {
    Serial.println("Payload: " + msgString);
  }
  
  if ( msgString == "cuceglio" ) {
    Serial.print("driuiiing !");
    digitalWrite(BUILTIN_LED,LOW);  
    tone(D5, 1000);
    delay(1000);
    noTone(D5);
    delay(1000);
    digitalWrite(BUILTIN_LED,HIGH);  
  }
}
