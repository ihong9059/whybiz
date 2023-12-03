#include <WiFi.h>       // standard library
#include <WebServer.h>  // standard library
#include "SuperMon.h"   // .h file that stores your html page code

#include "uttec.h"
#include "myBle.h"
#include "myWifi.h"
#include "sx1509Lib.h"
#include "myJson.h"

#define BLE_MODE 1

#define RXD2 16
#define TXD2 17
// #define RXD2 10
// #define TXD2 9

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  // Serial1.begin(115200);
  Serial.printf("Now start Whybiz project. 2023.12.03\r\n");
  Serial2.printf("Now start Whybiz project. 2023.12.03\r\n");
  // testParse();

#ifdef BLE_MODE  
  initBle();
#else
  initWifi();
#endif
  while(Serial.available() > 0){
    // read the incoming byte:
    char temp = Serial.read();
  }
  Serial.printf("end----\r\n");
  generateJsonSerial();
  initSx1509();
  randomSeed(analogRead(0));
}

void loop() {
  static uint32_t count = 0;
  uint16_t value = random(300);
#ifdef BLE_MODE  
  loop_ble();
#else
  loop_wifi();
#endif  
  testReceive();
  // Serial.printf("Loop: %ld\r\n", count++);
  delay(200);
  Serial2.printf("{\"device\":%d,\"value\":%d}\r\n", count++, value);
  // delay(200);
}
