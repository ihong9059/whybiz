#ifndef UTTEC_H
#define UTTEC_H

#include <Arduino.h>

typedef struct{
  uint16_t  version;
  int16_t   rssi;
  uint16_t  temp;
  float     test;
} factor_t;

typedef struct{
  int adc0;
  int adc1;
  uint8_t relay[8];
  uint8_t sw[8];
} device_t;

device_t* getMyDevice(void);
void setMyDevice(device_t data);

void setPort(void);
void testPort(void);
void testEeprom(void);

void loop_uttec(void);

#endif 

