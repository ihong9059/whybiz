#ifndef NVM__H__
#define NVM__H__

typedef struct{
    uint16_t version;
    uint16_t temp0;
    uint8_t data[10];
} uttec_t;


void initNvm(void);

void nvmTest(void);




#endif 
