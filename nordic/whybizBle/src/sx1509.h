#ifndef SX1509__H__
#define SX1509__H__

void initSx1509(void);
void testI2c(void);

uint8_t readIn(void);
bool checkSx1509In(uint8_t sw);

#endif 

