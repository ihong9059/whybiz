#include <WiFi.h>       // standard library
#include <WebServer.h>  // standard library
#include "SuperMon.h"   // .h file that stores your html page code

#include "uttec.h"
#include "myBle.h"
#include "myWifi.h"
#include "sx1509Lib.h"
#include "myJson.h"

#define BLE_MODE 1

#define RXD2 4  //ok
#define TXD2 5  //ok
// #define RXD2 16 //ok
// #define TXD2 17 //ok
// #define RXD2 10
// #define TXD2 9

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  Serial.printf("Now start Whybiz project. 2023.12.03\r\n");
  initUttec();
  
#ifdef BLE_MODE  
  initBle();
#else
  initWifi();
#endif
}

void loop() {
  static uint32_t count = 0;
  uint16_t value = random(300);
#ifdef BLE_MODE  
  loop_ble();
#else
  loop_wifi();
#endif  
  if(!(count % 5)){
    loop_uttec();
    parseReceiveJson();
    // testRelay();
    // testSwitch();
    signal();
    // Serial2.printf("AT=U");
    digitalWrite(RS485_EN, 1);
    // Serial2.printf("Lora test: %d\r\n", count);
    delay(5);
    digitalWrite(RS485_EN, 0);
    // testRelay();
    // Serial.printf("Lora test: %d\r\n", count);
    // testReceive();
    // testEeprom();
    // uint8_t temp = readSxRelay();
    // Serial.printf("relay: %x\r\n", temp);

    // temp = readSxSw();
    // Serial.printf("switch: %x\r\n", temp);
  }  
  delay(100); //for loop delay
  count++;
}
