#ifndef UTTEC__H__
#define UTTEC__H__

#include <stdio.h>
#include <stdbool.h>

typedef struct{
    bool isReady;
    bool isValid;
    uint8_t endPoint;
    uint8_t lines;
    uint8_t startLine;
} uartBuf_t;

typedef enum{
    PID_line = 1,
    PID_first = 9,
    PID_last = 13,

    ADR_line = 2,
    ADR_first = 9,
    ADR_last = 13,

    RSSI_line = 4,
    RSSI_fist = 6,
    RSSI_last = 9,

    DATA_line = 7,
    DATA_first = 0,
    DATA_last = 3,
} parsePoint_t;

typedef struct{
    uint16_t src_pid;
    uint16_t src_addr;
    int rssi;
} factor_t;

void putBuff(uint8_t*, uint8_t);
void uttecTest(void);
void callPrint(void);
void clearUart(void);

#endif 

