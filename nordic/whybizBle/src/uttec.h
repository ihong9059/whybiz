#ifndef UTTEC__H__
#define UTTEC__H__

#include <stdio.h>
#include <stdbool.h>


#define ADC_DEVICE		1
#define SWITCH_DEVICE 	2
#define RELAY_DEVICE	3

#define FLASH_FLAG      0xaaaa 

typedef struct{
    uint16_t flashFlag;
    uint8_t version;
    uint8_t node;
    uint8_t channel;
    uint8_t ssid;
    uint8_t ble;
    int8_t  adc1;
    int8_t  adc2;
    uint8_t sw;
    uint8_t relay;
    uint8_t power;
    int8_t  rssi;
} whybiz_t;

typedef struct{
    uint16_t src_pid;
    uint16_t src_addr;
    int rssi;
} loraFactor_t;

typedef struct{
    uint8_t relay;
    uint8_t sw;
} sx1509_t;

typedef struct{
    uint8_t node;
    uint8_t category;
    uint8_t sensor;
    uint8_t value;
    uint16_t crc;
} whybizFrame_t;

typedef struct{
    uint8_t frame[200];
    uint8_t start;
    uint8_t end;
    uint8_t flag;
} jsonFrame_t;

void initPort(void);
void setUartChannel(uint8_t channel);

whybiz_t* getWhybizFactor(void);

void putBuff(uint8_t*, uint8_t);
void uttecJsonTest(uint8_t* pData, uint8_t len);
void callPrint(void);
void clearUart(void);

sx1509_t* getBeforeSxReg(void);

void sendWhybizFrame(void);
whybizFrame_t* getWhybizFrame(void);
void testJsonOut(void);


void clearJsonData(void);
jsonFrame_t* getJsonFrame(void);
void dispJsonFrame(void);

bool* getConnectBleFlag(void);

#endif 

