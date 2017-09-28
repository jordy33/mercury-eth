#include <UIPEthernet.h>
#include "PubSubClient.h"
#include "DHT.h"
//kennelijk  geeft update van DHT sensor library boven 1.2.1 een fout
#define CLIENT_ID       "Hal"
//#define TOPIC           "temp"
#define PUBLISH_DELAY   3000
#define DHTPIN          3
#define DHTTYPE         DHT11
bool statusKD = HIGH;
bool statusBD = HIGH;
bool statusGD = HIGH;
bool pir = LOW;
int lichtstatus;
uint8_t mac[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
//IPAddress mqttServer(192,168,1,4);

EthernetClient ethClient;
PubSubClient mqttClient;
DHT dht(DHTPIN, DHTTYPE);

long previousMillis;
//pinMode(4,INPUT_PULLUP);

void setup() {
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  // setup serial communication
  /*
    Serial.begin(9600);
    while(!Serial) {};
    Serial.println(F("MQTT Arduino Demo"));
    Serial.println();
  */
  // setup ethernet communication using DHCP
  if (Ethernet.begin(mac) == 0) {
    //Serial.println(F("Unable to configure Ethernet using DHCP"));
    for (;;);
  }
  /*
    Serial.println(F("Ethernet configured via DHCP"));
    Serial.print("IP address: ");
    Serial.println(Ethernet.localIP());
    Serial.println();
  */
  // setup mqtt client
  mqttClient.setClient(ethClient);
  //mqttClient.setServer(mqttServer, 1883);
  mqttClient.setServer("test.mosquitto.org", 1883);
  //Serial.println(F("MQTT client configured"));

  // setup DHT sensor
  dht.begin();
  //Serial.println(F("DHT sensor initialized"));

  //Serial.println();
  // Serial.println(F("Ready to send data"));
  previousMillis = millis();
}

void loop() {
  statusBD = digitalRead(4);
  statusGD = digitalRead(5);
  statusKD = (digitalRead(6));
  lichtstatus = analogRead(A0);
  pir = digitalRead(7);

  // it's time to send new data?
  if (millis() - previousMillis > PUBLISH_DELAY) {
    sendData();
    previousMillis = millis();
  }

  mqttClient.loop();
}

void sendData() {

  char msgBuffer[20];
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  //  Serial.print("Temperature: ");
  // Serial.println(t);

  if (mqttClient.connect(CLIENT_ID)) {
    mqttClient.publish("hal/temp", dtostrf(t, 6, 2, msgBuffer));
    mqttClient.publish("hal/humid", dtostrf(h, 6, 2, msgBuffer));
    mqttClient.publish("hal/deur", (statusBD == HIGH) ? "OPEN" : "DICHT");
    mqttClient.publish("hal/garage", (statusGD == HIGH) ? "OPEN" : "DICHT");
    mqttClient.publish("hal/bel", (statusKD == HIGH) ? "OPEN" : "DICHT");
    mqttClient.publish("hal/l", dtostrf(lichtstatus, 4, 0, msgBuffer));
    mqttClient.publish("hal/p", (pir== HIGH) ? "OPEN" : "DICHT");
    //  mqttClient.publish(TOPIC, dtostrf(t, 6, 2, msgBuffer));

  }
}


