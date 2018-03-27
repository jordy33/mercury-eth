#include <ETH.h>

/* 
   * ETH_CLOCK_GPIO0_IN   - default: external clock from crystal oscillator
   * ETH_CLOCK_GPIO0_OUT  - 50MHz clock from internal APLL output on GPIO0 - possibly an inverter is needed for LAN8720
   * ETH_CLOCK_GPIO16_OUT - 50MHz clock from internal APLL output on GPIO16 - possibly an inverter is needed for LAN8720
   * ETH_CLOCK_GPIO17_OUT - 50MHz clock from internal APLL inverted output on GPIO17 - tested with LAN8720
*/
#define ETH_CLK_MODE    ETH_CLOCK_GPIO0_IN

// Pin# of the enable signal for the external crystal oscillator (-1 to disable for internal APLL source)
#define ETH_POWER_PIN   17

// Type of the Ethernet PHY (LAN8720 or TLK110)
#define ETH_TYPE        ETH_PHY_LAN8720

// I²C-address of Ethernet PHY (0 or 1 for LAN8720, 31 for TLK110)
#define ETH_ADDR        1

// Pin# of the I²C clock signal for the Ethernet PHY
#define ETH_MDC_PIN     23

// Pin# of the I²C IO signal for the Ethernet PHY
#define ETH_MDIO_PIN    18

const char *url = "c4neza.dwim.space";
//const char *url = "189.225.70.109";
uint16_t  port=5000;
static bool eth_connected = false;
const int ethernetConnectPin = 2;

#include <Adafruit_NeoPixel.h>
#define PIN 12
Adafruit_NeoPixel strip = Adafruit_NeoPixel(3, PIN, NEO_GRB + NEO_KHZ800);



byte longKeyPressCountMax = 80;    // 80 * 25 = 2000 ms
const unsigned long keySampleIntervalMs = 25;
const int buzzerPin = 5;

unsigned long keyPrevMillis_0 = 0;
byte KeyPressCount_0 = 0;
byte prevKeyState_0 = HIGH;         // button is active low
byte currKeyState_0 =false;
bool once_0= false;

unsigned long keyPrevMillis_8 = 0;
byte KeyPressCount_8 = 0;
byte prevKeyState_8 = HIGH;         // button is active low
byte currKeyState_8 =false;
bool once_8= false;


unsigned long keyPrevMillis_9 = 0;
byte KeyPressCount_9 = 0;
byte prevKeyState_9 = HIGH;         // button is active low
byte currKeyState_9 =false;
bool once_9= false;

unsigned long currentMillis;

void WiFiEvent(WiFiEvent_t event) {
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
      digitalWrite(ethernetConnectPin, HIGH);
      break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Disconnected");
      digitalWrite(ethernetConnectPin, LOW);
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

void sendMedical(const char * host, uint16_t port) {
  Serial.println("Medical button pressed\n");
  Serial.print("\nconnecting to ");
  Serial.println(host);

  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }
  client.printf("GET /listener/three?name=Jorge_Macias&source=5548885144&origin=ALARMA_MERCURY&latitude=19.5&longitude=-99.5&button=1&event=11 HTTP/1.1\r\nHost: %s\r\n\r\n", host);
  while (client.connected() && !client.available());
  while (client.available()) {
    Serial.write(client.read());
  }
  Serial.println("");
  Serial.println("closing connection\n");
  client.stop();
}

void sendFire(const char * host, uint16_t port) {
  Serial.println("Fire button pressed\n");
  Serial.print("\nconnecting to ");
  Serial.println(host);

  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }
  client.printf("GET /listener/three?name=Jorge_Macias&source=5548885144&origin=ALARMA_MERCURY&latitude=19.5&longitude=-99.5&button=1&event=12 HTTP/1.1\r\nHost: %s\r\n\r\n", host);
  while (client.connected() && !client.available());
  while (client.available()) {
    Serial.write(client.read());
  }
  Serial.println("");
  Serial.println("closing connection\n");
  client.stop();
}


// called when button is kept pressed for 2 seconds or more
void longKeyPress_0() {
    digitalWrite(buzzerPin, HIGH);
    if (!once_0){
      once_0=true;
      if (eth_connected){
        Serial.println("Send to platform fire");
        strip.setPixelColor(0,strip.Color(255, 0, 0)); //Red
        strip.show();
        strip.setPixelColor(1, 0, 0, 0);
        strip.show();
        strip.setPixelColor(2, 0, 0, 0);
        strip.show(); 
        sendFire(url, port);
      }
      else {
        strip.setPixelColor(0,strip.Color(127, 127, 127)); //WHITE
        strip.show();
        strip.setPixelColor(1, strip.Color(127, 127, 127));
        strip.show();
        strip.setPixelColor(2,strip.Color(127, 127, 127));
        strip.show(); 
        }
     }
}
// called when key goes from not pressed to pressed
void keyPress_0() {
    KeyPressCount_0 = 0;
}

// called when key goes from pressed to not pressed
void keyRelease_0() {
    digitalWrite(buzzerPin, LOW);
    once_0=false;

}


// called when button is kept pressed for 2 seconds or more
void longKeyPress_8() {
    digitalWrite(buzzerPin, HIGH);
    if (!once_8){
      once_8=true;
      if (eth_connected){
        Serial.println("Send to platform medic");
        strip.setPixelColor(1,strip.Color(0, 255, 0)); //GReen
        strip.show();
        strip.setPixelColor(0, 0, 0, 0);
        strip.show();
        strip.setPixelColor(2, 0, 0, 0);
        strip.show(); 
        sendMedical(url,port);
      }
      else {
        strip.setPixelColor(0,strip.Color(127, 127, 127)); //WHITE
        strip.show();
        strip.setPixelColor(1, strip.Color(127, 127, 127));
        strip.show();
        strip.setPixelColor(2,strip.Color(127, 127, 127));
        strip.show(); 
        }
      
     }
}
// called when key goes from not pressed to pressed
void keyPress_8() {
    KeyPressCount_8 = 0;
}

// called when key goes from pressed to not pressed
void keyRelease_8() {
    digitalWrite(buzzerPin, LOW);
    once_8=false;

}

// called when button is kept pressed for 2 seconds or more
void longKeyPress_9() {
    digitalWrite(buzzerPin, HIGH);
    if (!once_9){
      once_9=true;
      if (eth_connected){
        Serial.println("Send to platform Theft");
        strip.setPixelColor(2,strip.Color(0, 0, 255)); //GReen
        strip.show();
        strip.setPixelColor(0, 0, 0, 0);
        strip.show();
        strip.setPixelColor(1, 0, 0, 0);
        strip.show(); 
        sendPolice(url, port);
      }
      else {
        strip.setPixelColor(0,strip.Color(127, 127, 127)); //WHITE
        strip.show();
        strip.setPixelColor(1, strip.Color(127, 127, 127));
        strip.show();
        strip.setPixelColor(2,strip.Color(127, 127, 127));
        strip.show(); 
        }
      
     }
      
     
}
// called when key goes from not pressed to pressed
void keyPress_9() {
    KeyPressCount_9 = 0;
}

// called when key goes from pressed to not pressed
void keyRelease_9() {
    digitalWrite(buzzerPin, LOW);
    once_9=false;

}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000); // give me time to bring up serial monitor
  Serial.println("ESP32 Touch Test");
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  //
  strip.begin();
  colorWipe(strip.Color(255, 0, 0), 50); // Red
  colorWipe(strip.Color(0, 255, 0), 50); // Green
  colorWipe(strip.Color(0, 0, 255), 50); // Blue
  colorWipe(strip.Color(0, 0, 0), 50); // off
  strip.show(); // Initialize all pixels to 'off'
  pinMode(ethernetConnectPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  WiFi.onEvent(WiFiEvent);
  ETH.begin(ETH_ADDR, ETH_POWER_PIN, ETH_MDC_PIN, ETH_MDIO_PIN, ETH_TYPE, ETH_CLK_MODE);
  //Neopixel

}


void loop() {
    // key management section
    currentMillis = millis();
    if (currentMillis - keyPrevMillis_0 >= keySampleIntervalMs) {
        keyPrevMillis_0 = millis();
        if (touchRead(T0)<30){
            currKeyState_0 = false;
        }
        else{
            currKeyState_0 = true;
        }
        
        if ((prevKeyState_0 == HIGH) && (currKeyState_0 == LOW)) {
            keyPress_0();
        }
        else if ((prevKeyState_0 == LOW) && (currKeyState_0 == HIGH)) {
            keyRelease_0();
        }
        else if (currKeyState_0 == LOW) {
            KeyPressCount_0++;
        if (KeyPressCount_0 >= longKeyPressCountMax) {
            longKeyPress_0();
        }
    }
    prevKeyState_0 = currKeyState_0;
    }

    if (currentMillis- keyPrevMillis_8 >= keySampleIntervalMs) {
        keyPrevMillis_8 = millis();
        if (touchRead(T8)<40){
            currKeyState_8 = false;
        }
        else{
            currKeyState_8 = true;
        }
        
        if ((prevKeyState_8 == HIGH) && (currKeyState_8 == LOW)) {
            keyPress_8();
        }
        else if ((prevKeyState_8 == LOW) && (currKeyState_8 == HIGH)) {
            keyRelease_8();
        }
        else if (currKeyState_8 == LOW) {
            KeyPressCount_8++;
        if (KeyPressCount_8 >= longKeyPressCountMax) {
            longKeyPress_8();
        }
    }
    prevKeyState_8 = currKeyState_8;
    }

    if (currentMillis- keyPrevMillis_9 >= keySampleIntervalMs) {
        keyPrevMillis_9 = millis();
        if (touchRead(T9)<40){
            currKeyState_9 = false;
        }
        else{
            currKeyState_9 = true;
        }
        
        if ((prevKeyState_9 == HIGH) && (currKeyState_9 == LOW)) {
            keyPress_9();
        }
        else if ((prevKeyState_9 == LOW) && (currKeyState_9 == HIGH)) {
            keyRelease_9();
        }
        else if (currKeyState_9 == LOW) {
            KeyPressCount_9++;
        if (KeyPressCount_9 >= longKeyPressCountMax) {
            longKeyPress_9();
        }
    }
    prevKeyState_9 = currKeyState_9;
    }
    // other code goes here
}
