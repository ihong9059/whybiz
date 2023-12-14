#ifndef UTTEC_H
#define UTTEC_H

#include <Arduino.h>
#include "myBle.h"

#define FLASH_FLAG  0xaaaa

#define VERSION     1
#define EEPROM_MAX  1024

// for status
#define ADC_DEVICE		    1
#define SWITCH_DEVICE 	    2
#define RELAY_DEVICE	    3
#define LORA_DEVICE	        4
#define VERSION_DEVICE	    5
#define CHANNEL_DEVICE		6
// #define BLE_NUM_DEVICE	    6
#define MAX_CATEGORY        CHANNEL_DEVICE + 1
#define MAX_BLE_CATEGORY    4
// end

// for control
#define CTR_RELAY           13
#define CTR_LORA            14
#define CTR_CHANNEL         16
// end

// for control
#define SET_LORA            24
#define SET_VERSION         25
#define SET_CHANNEL         26
// end

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
bleFrame_t* getBleFrame(void);

void testEeprom(void);
#endif 

