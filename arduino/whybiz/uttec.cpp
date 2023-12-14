
#include "uttec.h"
#include "EEPROM.h"
#include "myJson.h"
#include "sx1509Lib.h"


#define SIGNAL_LED 2     //On board LED

#define PIN_A0 34     // some analog input sensor
#define PIN_A1 35     // some analog input sensor

#define ETHERNET_CHANNEL  0
#define RS485_CHANNEL     1
#define LORA_CHANNEL      2
#define BLANK_CHANNEL     3    

whybiz_t myWhybiz = {0, };
sx1509_t beforeSx1509 = {0, };

whybiz_t* getWhybizFactor(void){
    return &myWhybiz;    
}

void loop_uttec(void){
  static uint32_t SensorUpdate = 0;
  uint32_t updateTime = 1000;
  if ((millis() - SensorUpdate) >= updateTime) {
    SensorUpdate = millis();
    myWhybiz.adc1 = analogRead(PIN_A0);
    myWhybiz.adc2 = analogRead(PIN_A1);
    for(int i = 0; i < 8; i++){
      // myDevice.sw[i] = getSwitch(i);
    }
    // testPort();
  }
}

void initPort(void){
  pinMode(SEL1, OUTPUT);
  pinMode(SEL2, OUTPUT);
  pinMode(SX_RESET, OUTPUT);
  pinMode(RS485_EN, OUTPUT);
  pinMode(PWR_CTL, OUTPUT);
  pinMode(LORA_RST, OUTPUT);
  pinMode(SIGNAL_LED, OUTPUT);
  digitalWrite(LORA_RST, 1);

  setUartChannel(RS485_CHANNEL);
}

void dispFactor(void){
    EEPROM.readBytes(0, &myWhybiz, sizeof(whybiz_t));
    Serial.printf("flag: %x\r\n", myWhybiz.flashFlag);
    Serial.printf("version: %d\r\n", myWhybiz.version);
    Serial.printf("mode: %0.2f\r\n", myWhybiz.node);
    Serial.printf("channel: %d\r\n", myWhybiz.channel);
    Serial.printf("rssi: %d\r\n", myWhybiz.rssi);
    delay(2000);
}

void initEeprom(void){
  EEPROM.begin(EEPROM_MAX);
  Serial.printf("EEPROM Size: %d\r\n", EEPROM.length());
  EEPROM.readBytes(0, &myWhybiz, sizeof(whybiz_t));
  
  if(myWhybiz.flashFlag != FLASH_FLAG){
    Serial.printf("New software version---> 2023.12.06\r\n");
    delay(2000);
    myWhybiz.flashFlag = FLASH_FLAG;
    myWhybiz.version = VERSION;
    myWhybiz.node = 0;
    myWhybiz.channel = 21; //lora channel
    myWhybiz.rssi = -102;
    myWhybiz.channel = 0; //uart channel
    myWhybiz.relay = 0;
    EEPROM.writeBytes(0, &myWhybiz, sizeof(whybiz_t));
    EEPROM.commit();
  }
  else{
    Serial.printf("Already factor Ok.\r\n");
    dispFactor();
  }
  // while(1);
}

void initUttec(void){
  initEeprom();
  initPort();
  initSx1509();
}

void procCmdLine(uttecJson_t data){
  Serial.printf("procCmdLine in uttec.c\r\n");
}

void procAdc(uttecJson_t data){
  Serial.printf("procAdc in uttec.c\r\n");
}

void testPort(void){
  static bool toggle = false;

  // digitalWrite(SEL1, toggle);
  // digitalWrite(SEL2, toggle);
  // digitalWrite(SX_RESET, toggle);
  // digitalWrite(RS485_EN, toggle);
  // digitalWrite(PWR_CTL, toggle);
  digitalWrite(LORA_RST, toggle);
  toggle = !toggle;
}

void signal(void){
  static bool toggle = false;
  digitalWrite(SIGNAL_LED, toggle);
  toggle = !toggle;
}

void setUartChannel(uint8_t channel){
  switch(channel){
    case 0:
      digitalWrite(SEL2, 0);  digitalWrite(SEL1, 0);
    break;
    case 1:
      digitalWrite(SEL2, 0);  digitalWrite(SEL1, 1);
    break;
    case 2:
      digitalWrite(SEL2, 1);  digitalWrite(SEL1, 0);
    break;
    case 3:
      digitalWrite(SEL2, 1);  digitalWrite(SEL1, 1);
    break;
    default:
    break;
  }
  Serial.printf("uart channel: %d\r\n", channel);
}

void procBleRx(uint8_t device, uint8_t value){
  uint8_t pin = device % 10;
  device = device / 10;
  Serial.printf("device: %d\r\n", device);
  switch(device){
    case 4:
      Serial.printf("setRelay\r\n");
      // setRelay(pin, value);
    break;
    default: Serial.printf("Error-------- %d\r\n", device); break;
  }
}

