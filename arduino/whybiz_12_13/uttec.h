#ifndef UTTEC_H
#define UTTEC_H

#include <Arduino.h>

#define FLASH_FLAG  0xaaaa
#define VERSION     0.1
#define EEPROM_MAX  1024

#define SEL1            32 //SCL(32) 
#define SEL2            33 //SDA(33)
#define SX_RESET        17
#define RS485_EN        19
#define PWR_CTL         27
#define LORA_RST        18

typedef struct{
  uint16_t  flag;
  float     version;
  uint8_t   mode;
  uint8_t   channel;
  int16_t   rssi;
  uint8_t   uChannel;
  uint8_t   relay[8];
  uint16_t  uiTest;
  float     fTest;
} factor_t;

typedef struct{
  bool LED0;
  int adc0;
  int adc1;
  uint8_t relay[8];
  uint8_t sw[8];
} device_t;

/* initEeprom, initSx1509, initPort. 2023.12.06 */ 
void initUttec(void);

device_t* getMyDevice(void);
void setMyDevice(device_t data);

void initPort(void);
void testPort(void);

void loop_uttec(void);
void signal(void);
void setUartChannel(uint8_t channel);

#endif 

