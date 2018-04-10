/*
    This sketch shows the Ethernet event usage

*/

#include <ETH.h>

static bool eth_connected = false;
const char *url = "c4neza.dwim.space";
uint16_t  port=80;

void WiFiEvent(WiFiEvent_t event)
{
  switch (event) {
    case SYSTEM_EVENT_ETH_START:
      Serial.println("ETH Started");
      //set eth hostname here
      ETH.setHostname("esp32-ethernet");
      break;
    case SYSTEM_EVENT_ETH_CONNECTED:
      Serial.println("ETH Connected");
      break;
    case SYSTEM_EVENT_ETH_GOT_IP:
      Serial.print("ETH MAC: ");
      Serial.print(ETH.macAddress());
      Serial.print(", IPv4: ");
      Serial.print(ETH.localIP());
      if (ETH.fullDuplex()) {
        Serial.print(", FULL_DUPLEX");
      }
      Serial.print(", ");
      Serial.print(ETH.linkSpeed());
      Serial.println("Mbps");
      eth_connected = true;
      break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Disconnected");
      eth_connected = false;
      break;
    case SYSTEM_EVENT_ETH_STOP:
      Serial.println("ETH Stopped");
      eth_connected = false;
      break;
    default:
      break;
  }
}

void sendPolice(const char * host, uint16_t port) {
  Serial.println("Police Button policePressed\n");
  Serial.print("\nconnecting to ");
  Serial.println(host);

  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }
  client.printf("GET /listener/three?name=Jorge_Macias&source=5548885144&origin=ALARMA_MERCURY&latitude=19.5&longitude=-99.5&button=1&event=10 HTTP/1.1\r\nHost: %s\r\n\r\n", host);
  while (client.connected() && !client.available());
  while (client.available()) {
    Serial.write(client.read());
  }
  Serial.println("");
  Serial.println("closing connection\n");
  client.stop();
}

const int buttonPin = 34;     // the number of the pushbutton pin
int buttonState = 0;         // variable for reading the pushbutton status
bool once= false;

void setup()
{
  pinMode(buttonPin, INPUT);
  Serial.begin(115200);
  WiFi.onEvent(WiFiEvent);
  delay(1000); // give me time to bring up serial monitor
  Serial.println("PANIC BUTTON");
  ETH.begin();
}


void loop()
{
  if (eth_connected) {
    buttonState = digitalRead(buttonPin);
      if (buttonState == LOW) {    
        if (once==false){
          Serial.println("Police button pressed\n");
          sendPolice(url,port);
          once=true;
        }
      } else {
    once=false;
      }    
  }
}
