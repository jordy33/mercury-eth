#define MACADDRESS 0x00,0x01,0x02,0x03,0x04,0x05
#define MYIPADDR 192,168,1,6
#define MYIPMASK 255,255,255,0
#define MYDNS 192,168,1,254
#define MYGW 192,168,1,254
#define LISTENPORT 1000
#define UARTBAUD 115200

#if defined(__MBED__)
  #include <mbed.h>
  #include "mbed/millis.h"
  #define delay(x) wait_ms(x)
  #define PROGMEM
  #include "mbed/Print.h"
#endif

#include <UIPEthernet.h>
// The connection_data struct needs to be defined in an external file.
#include <UIPServer.h>
#include <UIPClient.h>
#include "utility/logging.h"

uint8_t mac[6] = {MACADDRESS};
uint8_t myIP[4] = {MYIPADDR};
uint8_t myMASK[4] = {MYIPMASK};
uint8_t myDNS[4] = {MYDNS};
uint8_t myGW[4] = {MYGW};

EthernetServer server = EthernetServer(LISTENPORT);
#if defined(ARDUINO)
void setup() {
#endif  
#if defined(__MBED__)
int main() {
#endif
  #if ACTLOGLEVEL>LOG_NONE
    #if defined(ARDUINO)
      LogObject.begin(UARTBAUD);
    #endif
    #if defined(__MBED__)
      Serial LogObject(SERIAL_TX,SERIAL_RX);
      LogObject.baud(UARTBAUD);
    #endif
  #endif

// initialize the ethernet device
//Ethernet.begin(mac,myIP);
//pinMode(4, OUTPUT);
Ethernet.begin(mac,myIP,myDNS,myGW,myMASK);
// start listening for clients
server.begin();
#if defined(ARDUINO)
}

void loop() {
#endif  

#if defined(__MBED__)
while(true) {
#endif
  size_t size;
  //digitalWrite(4, HIGH);   // turn the LED on (HIGH is the voltage level)
  //delay(1000);              // wait for a second
  ////digitalWrite(4, LOW);    // turn the LED off by making the voltage LOW
  //delay(1000);  
  if (EthernetClient client = server.available())
    {

      if (client)
        {
          while((size = client.available()) > 0)
            {
              uint8_t* msg = (uint8_t*)malloc(size);
              size = client.read(msg,size);
              #if ACTLOGLEVEL>=LOG_INFO
                LogObject.uart_send_buf_len(msg,size);
              #endif
              client.write(msg,size);
              free(msg);
            }
        }
    }
}
#if defined(__MBED__)
}
#endif

