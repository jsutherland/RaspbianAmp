#include <SPI.h>
#include <WiFiNINA.h>
#include "IRLibAll.h"

// Configure the pins used for the ESP32 connection
#if defined(ADAFRUIT_FEATHER_M4_EXPRESS) || \
  defined(ADAFRUIT_FEATHER_M0_EXPRESS) || \
  defined(ARDUINO_AVR_FEATHER32U4) || \
  defined(ARDUINO_NRF52840_FEATHER) || \
  defined(ADAFRUIT_ITSYBITSY_M0) || \
  defined(ADAFRUIT_ITSYBITSY_M4_EXPRESS) || \
  defined(ARDUINO_AVR_ITSYBITSY32U4_3V) || \
  defined(ARDUINO_NRF52_ITSYBITSY)
  // Configure the pins used for the ESP32 connection
  #define SPIWIFI       SPI  // The SPI port
  #define SPIWIFI_SS    13   // Chip select pin
  #define ESP32_RESETN  12   // Reset pin
  #define SPIWIFI_ACK   11   // a.k.a BUSY or READY pin
  #define ESP32_GPIO0   -1
#elif defined(ARDUINO_AVR_FEATHER328P)
  #define SPIWIFI       SPI  // The SPI port
  #define SPIWIFI_SS     4   // Chip select pin
  #define ESP32_RESETN   3   // Reset pin
  #define SPIWIFI_ACK    2   // a.k.a BUSY or READY pin
  #define ESP32_GPIO0   -1
#elif defined(TEENSYDUINO)
  #define SPIWIFI       SPI  // The SPI port
  #define SPIWIFI_SS     5   // Chip select pin
  #define ESP32_RESETN   6   // Reset pin
  #define SPIWIFI_ACK    9   // a.k.a BUSY or READY pin
  #define ESP32_GPIO0   -1
#elif defined(ARDUINO_NRF52832_FEATHER )
  #define SPIWIFI       SPI  // The SPI port
  #define SPIWIFI_SS    16   // Chip select pin
  #define ESP32_RESETN  15   // Reset pin
  #define SPIWIFI_ACK    7   // a.k.a BUSY or READY pin
  #define ESP32_GPIO0   -1
#elif !defined(SPIWIFI_SS)   // if the wifi definition isnt in the board variant
  // Don't change the names of these #define's! they match the variant ones
  #define SPIWIFI       SPI
  #define SPIWIFI_SS    10   // Chip select pin
  #define SPIWIFI_ACK    7   // a.k.a BUSY or READY pin
  #define ESP32_RESETN   5   // Reset pin
  #define ESP32_GPIO0   -1   // Not connected
#endif

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "4GEE-Router-FP8T";        // your network SSID (name)
char pass[] = "mqAifJm9NTxV";    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the Wifi radio's status

WiFiClient client;
IRrecvPCI myReceiver(11); 
IRdecode myDecoder;   

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  myReceiver.enableIRIn(); // Start the receiver
  Serial.println(F("Ready to receive IR signals"));

  WiFi.setPins(SPIWIFI_SS, SPIWIFI_ACK, ESP32_RESETN, ESP32_GPIO0, &SPIWIFI);
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the data:
  Serial.println("You're connected to the network");
}

void loop() {
  // IR Things
  if (myReceiver.getResults()) {
    myDecoder.decode();           //Decode it
    // myDecoder.dumpResults(true);  //Now print results. Use false for less detail
    if (myDecoder.protocolNum == NEC) {
      switch(myDecoder.value) {
        case 0xFF629D:  //Volume Up
          Serial.println("Volume Up");
          changeVolume("/vol/relative/5");
          break;
        case 0xFFA857:  //Volume Down
          Serial.println("Volume Down");
          changeVolume("/vol/relative/-5");
          break;
        case 0xFFA25D:  //Reset Amp
          Serial.println("Reset Amp");
          changeVolume("/vol/reset");
          break;
        case 0xFF02FD:  //Mute Amp
          Serial.println("Mute Amp");
          changeVolume("/vol/0");
          break;
        case 0xFF9867:  //Set Low Vol
          Serial.println("Set Volume to 30%");
          changeVolume("/vol/30");
          break;
        // Absolute Values
        case 0x0:  //Set Low Vol
          changeVolume("/vol/10");
          break;
        case 0x0:  //Set Low Vol
          changeVolume("/vol/20");
          break;
        case 0x0:  //Set Low Vol
          changeVolume("/vol/30");
          break;
        case 0x0:  //Set Low Vol
          changeVolume("/vol/40");
          break;
        case 0x0:  //Set Low Vol
          changeVolume("/vol/50");
          break;
        case 0x0:  //Set Low Vol
          changeVolume("/vol/60");
          break;
        case 0x0:  //Set Low Vol
          changeVolume("/vol/70");
          break;
        case 0x0:  //Set Low Vol
          changeVolume("/vol/80");
          break;
        case 0x0:  //Set Low Vol
          changeVolume("/vol/90");
          break;
      }
    }
    delay(100);
    myReceiver.enableIRIn();      //Restart receiver
  }
}

void changeVolume(char path[]) {
  Serial.println("Changing the volume...");
  // Connect to the HTTP server
  char server[] = "192.168.8.117";    // name address for adafruit test   

  if (client.connect(server, 5000)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.print("GET "); client.print(path); client.println(" HTTP/1.1");
    client.print("Host: "); client.println(server);
    client.println("Connection: close");
    client.println();
  }
}
