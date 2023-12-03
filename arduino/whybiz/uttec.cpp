
#include "uttec.h"
#include "EEPROM.h"

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





