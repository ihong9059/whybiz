#ifndef UTTEC_JSON_H
#define UTTEC_JSON_H

typedef struct{
  uint16_t node;
  uint8_t category;
  uint8_t device;
  int16_t value;
  uint16_t crc;
} uttecJson_t;

#define MAX_DOC 100

void testReceive(void);

void testReceiveJson(void);
void testSendJson(void);

void procCmdLine(uttecJson_t);
void procSwitch(uttecJson_t);
void procRelay(uttecJson_t);
void procAdc(uttecJson_t);

#endif 
