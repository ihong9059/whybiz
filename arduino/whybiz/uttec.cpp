#include "uttec.h"
#include "EEPROM.h"
#include "myJson.h"
#include "sx1509Lib.h"
#include "myBle.h"

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
    static uint32_t bleCount = 0;
    static bool toggle = false;
    bleFrame_t Frame = {0, };
    SensorUpdate = millis();
    uint8_t adc1 = analogRead(PIN_A0)/40;
    uint8_t adc2 = analogRead(PIN_A1)/40;

    switch(bleCount++ % MAX_CATEGORY){
      case ADC_DEVICE:
        // Serial.printf("adc1: %d, adc2: %d\r\n", adc1, adc2);
        Frame.category = ADC_DEVICE;
        Frame.sensor = adc1;
        Frame.value = adc2;
      break;
      case SWITCH_DEVICE:
        Frame.category = SWITCH_DEVICE;
        Frame.sensor = readSxSw();
        Frame.value = Frame.sensor;
      break;
      case RELAY_DEVICE:
        Frame.category = RELAY_DEVICE;
        Frame.sensor = readSxRelay();
        Frame.value = Frame.sensor;
      break;
      case LORA_DEVICE:
        Frame.category = LORA_DEVICE;
        Frame.sensor = myWhybiz.power;
        Frame.value = myWhybiz.rssi;
      break;
      case VERSION_DEVICE:
        Frame.category = VERSION_DEVICE;
        Frame.sensor = myWhybiz.version;
        Frame.value = myWhybiz.ble;
      break;
      case CHANNEL_DEVICE:
        Frame.category = CHANNEL_DEVICE;
        Frame.sensor = myWhybiz.channel;
        Frame.value = myWhybiz.lora_ch;
      break;
    }
    sendToBle(Frame);
    sendJsonForStatus();
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

  setUartChannel(ETHERNET_CHANNEL);
}

void dispFactor(void){
    EEPROM.readBytes(0, &myWhybiz, sizeof(whybiz_t));
    Serial.printf("flag: %x\r\n", myWhybiz.flashFlag);
    Serial.printf("version: %d\r\n", myWhybiz.version);
    Serial.printf("node: %d\r\n", myWhybiz.node);
    Serial.printf("channel: %d\r\n", myWhybiz.channel);
    Serial.printf("ble: %d\r\n", myWhybiz.ble);
    Serial.printf("adc1: %d\r\n", myWhybiz.adc1);
    Serial.printf("adc2: %d\r\n", myWhybiz.adc2);
    Serial.printf("sw: %d\r\n", myWhybiz.sw);
    Serial.printf("relay: %d\r\n", myWhybiz.relay);
    Serial.printf("lora_ch: %d\r\n", myWhybiz.lora_ch);
    Serial.printf("power: %d\r\n", myWhybiz.power);
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
    myWhybiz.ble = 12;
    myWhybiz.adc1 = 3;
    myWhybiz.adc2 = 4;
    myWhybiz.sw = 1;
    myWhybiz.relay = 2;
    myWhybiz.lora_ch = 0; //uart channel
    myWhybiz.power = 0;
    myWhybiz.rssi = 0;
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


void testEeprom(void){
  EEPROM.readBytes(0, &myWhybiz, sizeof(whybiz_t));
  Serial.printf("read rssi: %d\r\n", myWhybiz.rssi);
  myWhybiz.rssi++;
  EEPROM.writeBytes(0, &myWhybiz, sizeof(whybiz_t));
  EEPROM.commit();
}

void sendJsonForStatus(void){
    static uint32_t count = 0;
    whybiz_t* pFactor = getWhybizFactor();
    whybizFrame_t* pFrame = getWhybizFrame();

    connectFlag_t* pFlags = getConnectFlag();

    // if(!pFlags->first){
    //     sendFactorAtConnection();
    //     pFlags->first = true;
    //     return;
    // }
    // if(pFlags->ble) printf("Ble connected---->\r\n");
    // else return;

    if(!(count++ % 2)){
        static uint32_t sendCount = 0;
        uint8_t who = (sendCount++ % MAX_CATEGORY);
        pFrame->node = pFactor->node;
        // printf("who: %d\r\n", who);
        switch(who){
            case ADC_DEVICE: 
                printf("ADC_DEVICE: %d\r\n", who); 
                pFrame->category = ADC_DEVICE; 
                pFrame->sensor = pFactor->adc1; pFrame->value = pFactor->adc2; 
            break;
            case SWITCH_DEVICE: printf("SWITCH_DEVICE: %d\r\n", who); 
                pFrame->category = SWITCH_DEVICE; 
                pFrame->sensor = pFactor->sw; pFrame->value = pFactor->sw; 
            break;
            case RELAY_DEVICE: printf("RELAY_DEVICE: %d\r\n", who); 
                pFrame->category = RELAY_DEVICE; 
                pFrame->sensor = pFactor->relay; pFrame->value = pFactor->relay; 
            break;
            case LORA_DEVICE: printf("LORA_DEVICE: %d\r\n", who); 
                pFrame->category = LORA_DEVICE; 
                pFrame->sensor = pFactor->power; pFrame->value = pFactor->rssi++; 
            break;
            case VERSION_DEVICE: printf("VERSION_DEVICE: %d\r\n", who); 
                pFrame->category = VERSION_DEVICE; 
                pFrame->sensor = pFactor->version; pFrame->value = pFactor->ble; 
            break;
            case CHANNEL_DEVICE: printf("CHANNEL_DEVICE: %d\r\n", who); 
                pFrame->category = CHANNEL_DEVICE; 
                pFrame->sensor = pFactor->channel; pFrame->value = pFactor->lora_ch; 
            break;
        }
        pFrame->crc = pFrame->node + pFrame->category + pFrame->sensor + pFrame->value;  
        
//for server through selected uart channel. printf. 2023.12.15
        printf("{\"no\":%d,\"ca\":%d,\"se\":%d,\"va\":%d,\"crc\":%d}\r\n",
        pFrame->node, pFrame->category, pFrame->sensor, pFrame->value, pFrame->crc);
    }
}

connectFlag_t whybizConnect = {0, };
connectFlag_t* getConnectFlag(void){
	return &whybizConnect;
}

whybizFrame_t myWhybizFrame = {0, };

void sendWhybizFrame(void){
    whybizFrame_t* pJson = getWhybizFrame();
    pJson->crc = pJson->node + pJson->category + pJson->sensor + pJson->sensor;
    printf("{\"no\":%d,\"ca\":%d,\"se\":%d,\"va\":%d,\"crc\":%d}\r\n",
    pJson->node, pJson->category, pJson->sensor, pJson->value, pJson->crc);
}

whybizFrame_t* getWhybizFrame(void){
    return &myWhybizFrame;
}




