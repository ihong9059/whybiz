#ifndef SX1509_LIB_H
#define SX1509_LIB_H

typedef struct{
  uint8_t relayPin[8];
  uint8_t relayValue[8];
  uint8_t swPin[8];
  uint8_t swValue[8];
} sx1509_t;

void initSx1509(void);
void testRelay(void);
void testSwitch(void);
void relay(uint8_t pin, uint8_t set);

#endif

