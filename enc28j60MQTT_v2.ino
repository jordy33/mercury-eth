// GOOD ONE
#include <UIPEthernet.h>
#include "PubSubClient.h"
#define CLIENT_ID   "Hal"
//#define SERVER      "mercury.dwim.mx"
//#define USERNAME    "dwim"
//#define PASSWORD    "gpscontrol1"
#define SERVER      "c4neza.dwim.space"
#define USERNAME    "c4neza"
#define PASSWORD    "Neza1234"
#define PUBLISH_DELAY   3000
#define OPTO_COUPLER_PIN 4
#define LED_PIN 5
#define BUTTON_PIN1 16

uint8_t mac[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
//IPAddress mqttServer(192,168,1,4);
//For  Local 
EthernetClient ethClient;
PubSubClient mqttClient;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println("");
  if (length>0){
      if (payload[0]==48)
      {
        //do stuff here because it was a match
        Serial.println("off");
        digitalWrite(OPTO_COUPLER_PIN,LOW);
      }         
      if(payload[0]==49){
        Serial.println("on");
        digitalWrite(OPTO_COUPLER_PIN,HIGH);
      }
      if(payload[0]==82){
        Serial.println("Ring");
        digitalWrite(OPTO_COUPLER_PIN,HIGH);
        delay(2000);
        digitalWrite(OPTO_COUPLER_PIN,LOW);
      }
  }
  Serial.println();
}

void setup() {
  pinMode(OPTO_COUPLER_PIN,OUTPUT);
  pinMode(LED_PIN,OUTPUT);
  pinMode(BUTTON_PIN1,INPUT);
  digitalWrite(LED_PIN,LOW);



  // setup serial communication
  Serial.begin(115200);
  while(!Serial) {};
  Serial.println();
  Serial.println(F("MQTT Arduino Demo"));
  Serial.println();
  Serial.println(digitalRead(BUTTON_PIN1));
  // setup ethernet communication using DHCP
  if (Ethernet.begin(mac) == 0) {
    //Serial.println(F("Unable to configure Ethernet using DHCP"));
    for (;;);
  }
  Serial.println(F("Ethernet configured via DHCP"));
  Serial.print("IP address: ");
  Serial.println(Ethernet.localIP());
  Serial.println();
  // setup mqtt client
  mqttClient.setClient(ethClient);
  mqttClient.setServer(SERVER, 1883);
  mqttClient.setCallback(callback);
  Serial.println(F("MQTT client configured"));
}

void loop() {

   
  //Serial.println(); 
 if (!mqttClient.connected()) {
   if (mqttClient.connect(CLIENT_ID,USERNAME,PASSWORD)) {
    mqttClient.subscribe("alarm");
    Serial.println("Subscribed to alarm");
    digitalWrite(LED_PIN,HIGH);
  }
 }
  mqttClient.loop();
}
