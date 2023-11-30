#include <SparkFunSX1509.h>

#include "uttec.h"


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