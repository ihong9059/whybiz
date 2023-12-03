#ifndef UTTEC_H
#define UTTEC_H

#include <Arduino.h>

typedef struct{
  uint16_t  version;
  int16_t   rssi;
  uint16_t  temp;
  float     test;
} factor_t;

void setPort(void);

void testSub(void);
void testPort(void);

void testEeprom(void);


#endif 

