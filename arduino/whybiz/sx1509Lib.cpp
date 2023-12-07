#include <Wire.h>           // Include the I2C library (required)
#include <SparkFunSX1509.h> //Click here for the library: http://librarymanager/All#SparkFun_SX1509

#include "sx1509Lib.h"
#include "myJson.h"

sx1509_t mySx1509 = {0, };
// SX1509 I2C address (set by ADDR1 and ADDR0 (00 by default):
const byte SX1509_ADDRESS = 0x3E; // SX1509 I2C address
SX1509 io;                        // Create an SX1509 object to be used throughout

void initSx1509(void){
  Serial.println("SX1509 Example");
  Wire.begin();
  if (io.begin(SX1509_ADDRESS) == false)
  {
    Serial.println("Failed to communicate. Check wiring and address of SX1509.");
    while (1);
  }
  for(uint8_t i = 0; i < 8; i++){
    mySx1509.relayPin[i] = i + 8;
    io.pinMode(mySx1509.relayPin[i], OUTPUT); // Set LED pin to OUTPUT
    mySx1509.swPin[i] = i;
    io.pinMode(mySx1509.swPin[i], INPUT_PULLUP); // Set LED pin to OUTPUT
  }
}

void procSwitch(uttecJson_t data){
  Serial.printf("procSwitch in sx1509Lib\r\n");
}

void procRelay(uttecJson_t){
  Serial.printf("procRelay in sx1509Lib\r\n");
}

void testRelay(void){
  static uint16_t count = 0;
  static bool toggle = false;
  if(toggle){
    setRelay(count % 8, 1);
    count++;
  }
  else{
    setRelay(count % 8, 0);
  }
  toggle = !toggle;
}

void testSwitch(void){
  static uint16_t count = 0;
  uint8_t i = count++ % 8;
  // if(io.digitalRead(mySx1509.swPin[i])){
  if(getSwitch(i)){
    Serial.printf("pin: %d, High\r\n", i);
  }
  else{
    Serial.printf("pin: %d, Low\r\n", i);
  }
}

void setRelay(uint8_t pin, uint8_t set){
  io.digitalWrite(mySx1509.relayPin[pin], set);
}

uint8_t getSwitch(uint8_t pin){
  return io.digitalRead(mySx1509.swPin[pin]);
}