#include <Wire.h>           // Include the I2C library (required)
#include <SparkFunSX1509.h> //Click here for the library: http://librarymanager/All#SparkFun_SX1509

#include "sx1509Lib.h"
#include "myJson.h"

// SX1509 I2C address (set by ADDR1 and ADDR0 (00 by default):
const byte SX1509_ADDRESS = 0x3E; // SX1509 I2C address
SX1509 io;                        // Create an SX1509 object to be used throughout

// SX1509 Pin definition:
const byte SX1509_LED_PIN = 15; // LED to SX1509's pin 15

void initSx1509(void){
  Serial.println("SX1509 Example");

  Wire.begin();

  // Call io.begin(<address>) to initialize the SX1509. If it
  // successfully communicates, it'll return 1.
  if (io.begin(SX1509_ADDRESS) == false)
  {
    Serial.println("Failed to communicate. Check wiring and address of SX1509.");
    while (1)
      ; // If we fail to communicate, loop forever.
  }

  // Set up the SX1509's clock to use the internal 2MHz
  // oscillator. The second parameter divides the oscillator
  // clock to generate a slower LED clock. 4 divides the 2MHz
  // clock by 2 ^ (4-1) (8, ie. 250kHz). The divider parameter
  // can be anywhere between 1-7.
  io.clock(INTERNAL_CLOCK_2MHZ, 4);

  io.pinMode(SX1509_LED_PIN, OUTPUT); // Set LED pin to OUTPUT

  // Blink the LED pin -- ~1000 ms LOW, ~500 ms HIGH:
  io.blink(SX1509_LED_PIN, 1000, 500);
  // The timing parameters are in milliseconds, but they
  // aren't 100% exact. The library will estimate to try to
  // get them as close as possible. Play with the clock
  // divider to maybe get more accurate timing.  
}

void procSwitch(uttecJson_t data){
  Serial.printf("procSwitch in sx1509Lib\r\n");
}

void procRelay(uttecJson_t){
  Serial.printf("procRelay in sx1509Lib\r\n");
}
