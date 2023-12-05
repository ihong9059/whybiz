
#include "uttec.h"
#include "EEPROM.h"
#include "myJson.h"

#define SEL1            22 //SCL(32) 
#define SEL2            21 //SDA(33)
#define SX_RESET        17
#define RS485_EN        19
#define PWR_CTL         27
#define LORA_RST        18

#define PIN_LED 2     //On board LED

#define PIN_A0 34     // some analog input sensor
#define PIN_A1 35     // some analog input sensor

factor_t myFactor = {0,};
device_t myDevice = {0,};

#define EEPROM_MAX  1024

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

void initUttec(void){
  pinMode(SEL1, OUTPUT);
  pinMode(SEL2, OUTPUT);
  pinMode(SX_RESET, OUTPUT);
  pinMode(RS485_EN, OUTPUT);
  pinMode(PWR_CTL, OUTPUT);
  pinMode(LORA_RST, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, 1);
  // digitalWrite(PIN_LED, myDevice.LED0);
}

void setPort(void){
}

void initEeprom(void){
  EEPROM.begin(EEPROM_MAX);
  Serial.printf("EEPROM Size: %d\r\n", EEPROM.length());

}

void testEeprom(void){
  initEeprom();
  myFactor.version = 1;
  myFactor.rssi = -108;
  myFactor.temp = 1234;
  myFactor.test = 43.21;

  EEPROM.writeBytes(0, &myFactor, sizeof(myFactor));
  factor_t newFactor = {0, };
  EEPROM.readBytes(0, &newFactor, sizeof(factor_t));
  Serial.printf("version: %d\r\n", newFactor.version);
  Serial.printf("rssi: %d\r\n", newFactor.rssi);
  Serial.printf("temp: %d\r\n", newFactor.temp);
  Serial.printf("test: %0.2f\r\n", newFactor.test);
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




