
#include "uttec.h"
#include "EEPROM.h"
#include "myJson.h"
#include "sx1509Lib.h"

#define SEL1            22 //SCL(32) 
#define SEL2            21 //SDA(33)
#define SX_RESET        17
#define RS485_EN        19
#define PWR_CTL         27
#define LORA_RST        18

#define SIGNAL_LED 2     //On board LED

#define PIN_A0 34     // some analog input sensor
#define PIN_A1 35     // some analog input sensor

factor_t myFactor = {0,};
device_t myDevice = {0,};

device_t* getMyDevice(void){
  return &myDevice;
}

void setMyDevice(device_t data){
  myDevice = data;
}

void loop_uttec(void){
  static uint32_t SensorUpdate = 0;
  uint32_t updateTime = 1000;
  if ((millis() - SensorUpdate) >= updateTime) {
    SensorUpdate = millis();
    myDevice.adc0 = analogRead(PIN_A0);
    myDevice.adc1 = analogRead(PIN_A1);
    for(int i = 0; i < 8; i++){
      myDevice.sw[i] = !myDevice.sw[i];
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
}

void dispFactor(void){
    EEPROM.readBytes(0, &myFactor, sizeof(factor_t));
    Serial.printf("flag: %x\r\n", myFactor.flag);
    Serial.printf("version: %d\r\n", myFactor.version);
    Serial.printf("mode: %0.2f\r\n", myFactor.mode);
    Serial.printf("channel: %d\r\n", myFactor.channel);
    Serial.printf("rssi: %d\r\n", myFactor.rssi);
    delay(2000);
}

void initEeprom(void){
  EEPROM.begin(EEPROM_MAX);
  Serial.printf("EEPROM Size: %d\r\n", EEPROM.length());
  EEPROM.readBytes(0, &myFactor, sizeof(factor_t));
  
  if(myFactor.flag != FLASH_FLAG){
    Serial.printf("New software version---> 2023.12.06\r\n");
    delay(2000);
    myFactor.flag = FLASH_FLAG;
    myFactor.version = VERSION;
    myFactor.mode = 0;
    myFactor.channel = 21; //lora channel
    myFactor.rssi = -102;
    myFactor.uChannel = 0; //uart channel
    for(int i = 0; i < sizeof(myFactor.relay); i++){
      myFactor.relay[i] = i;
    }
    myFactor.uiTest = 0x1234;
    myFactor.fTest = 0.001;
    EEPROM.writeBytes(0, &myFactor, sizeof(myFactor));
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


