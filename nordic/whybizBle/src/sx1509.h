#ifndef SX1509__H__
#define SX1509__H__

void initSx1509(void);

uint8_t getSx1509(void);
bool checkSx1509In(void);

void writeOutSx(uint8_t bitNum, bool output);

void testRelay(void);
void testSw(void);

#endif 

