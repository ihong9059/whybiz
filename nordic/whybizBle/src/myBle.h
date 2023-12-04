#ifndef MYBLE__H__
#define MYBLE__H__


void initBle(void);
void procRxBle(uint8_t* data, uint16_t len);
void procTxBle(void);

#endif 
