#ifndef UTTEC_BLE_H
#define UTTEC_BLE_H

typedef struct{
  uint8_t category;
  uint8_t sensor;
  uint8_t value;
} bleFrame_t;

void initBle(void);
void loop_ble(void);

void sendToBle(bleFrame_t);

#endif

