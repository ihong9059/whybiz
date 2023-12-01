#include <Wire.h>           // Include the I2C library (required)
#include <SparkFunSX1509.h>

#include "uttec.h"
#include "EEPROM.h"

// SX1509 I2C address (set by ADDR1 and ADDR0 (00 by default):
const byte SX1509_ADDRESS = 0x3E; // SX1509 I2C address
SX1509 io;                        // Create an SX1509 object to be used throughout
// SX1509 Pin definition:
const byte SX1509_LED_PIN = 15; // LED to SX1509's pin 15


#define SEL1            22 //SCL(32) 
#define SEL2            21 //SDA(33)
#define SX_RESET        17
#define RS485_EN        19
#define PWR_CTL         27
#define LORA_RST        18

void testSub(void){
    Serial.printf("testUttec\r\n");
}

void setPort(void){
  pinMode(SEL1, OUTPUT);
  pinMode(SEL2, OUTPUT);
  pinMode(SX_RESET, OUTPUT);
  pinMode(RS485_EN, OUTPUT);
  pinMode(PWR_CTL, OUTPUT);
  pinMode(LORA_RST, OUTPUT);
}

void testPort(void){
  static bool toggle = false;

  // digitalWrite(SEL1, toggle);
  // digitalWrite(SEL2, toggle);
  // digitalWrite(SX_RESET, toggle);
  // digitalWrite(RS485_EN, toggle);
  // digitalWrite(PWR_CTL, toggle);
  digitalWrite(LORA_RST, toggle);

  toggle = !toggle;
}

void initI2c(void){
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
  // io.blink(SX1509_LED_PIN, 1000, 500);
  // The timing parameters are in milliseconds, but they
  // aren't 100% exact. The library will estimate to try to
  // get them as close as possible. Play with the clock
  // divider to maybe get more accurate timing.
  io.analogWrite(SX1509_LED_PIN, 128);
}

factor_t myFactor = {0,};

#define EEPROM_MAX  1024

void initEeprom(void){
  EEPROM.begin(EEPROM_MAX);
  Serial.printf("EEPROM Size: %d\r\n", EEPROM.length());

}

void testEeprom(void){
  initEeprom();
  myFactor.version = 1;
  myFactor.rssi = -108;
  myFactor.temp = 1234;
  myFactor.test = 43.21;

  EEPROM.writeBytes(0, &myFactor, sizeof(myFactor));
  factor_t newFactor = {0, };
  EEPROM.readBytes(0, &newFactor, sizeof(factor_t));
  Serial.printf("version: %d\r\n", newFactor.version);
  Serial.printf("rssi: %d\r\n", newFactor.rssi);
  Serial.printf("temp: %d\r\n", newFactor.temp);
  Serial.printf("test: %0.2f\r\n", newFactor.test);
}





