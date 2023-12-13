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
    uint16_t flashFlag;
    uint8_t version;
    uint8_t node;
    uint8_t channel;
    uint8_t ble;
    int8_t  adc1;
    int8_t  adc2;
    uint8_t sw;
    uint8_t relay;
    uint8_t lora_ch;
    uint8_t power;
    int8_t  rssi;
} whybiz_t;

typedef struct{
    bool first;
    bool ble;
    bool wifi;
    bool ethernet;
    bool rs485;
} connectFlag_t;

typedef struct{
    uint8_t node;
    uint8_t category;
    uint8_t sensor;
    uint8_t value;
    uint16_t crc;
} whybizFrame_t;

typedef struct{
    uint8_t relay;
    uint8_t sw;
} sx1509_t;

/* initEeprom, initSx1509, initPort. 2023.12.06 */ 
void initUttec(void);

void initPort(void);
void setUartChannel(uint8_t channel);

sx1509_t* getBeforeSxReg(void);
whybiz_t* getWhybizFactor(void);

void sendWhybizFrame(void);
whybizFrame_t* getWhybizFrame(void);


void procChannel(void);
void procLora(void);
void procVersion(void);

void sendJsonForStatus(void);

void testPort(void);

void loop_uttec(void);
void signal(void);


void procBleRx(uint8_t device, uint8_t value);

#endif 

