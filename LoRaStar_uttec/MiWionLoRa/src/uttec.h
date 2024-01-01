#ifndef UTTEC_H
#define UTTEC_H

// for lora category
#define LORA_CHANNEL_INFO   30
#define LORA_POWER_INFO     31
#define LORA_RSSI_INFO      32
#define LORA_TEST_UART      33
#define LORA_TEST_INFO      34


typedef struct{
    uint8_t channel;
    uint8_t power;
    uint8_t rssi;
} uttec_t;

uttec_t* getloraFactor(void);


#endif
