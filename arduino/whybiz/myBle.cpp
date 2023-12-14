#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include "uttec.h"
#include "myBle.h"
#include "sx1509Lib.h"

BLEServer *pServer = NULL;
BLECharacteristic * pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};
// void setRelay(uint8_t pin, uint8_t set);
	// uint8_t readByte(uint8_t registerAddress);


class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();
      Serial.printf("length: %d\r\n", rxValue.length());
      Serial.printf("ca: %d, se: %d, va: %d\r\n", rxValue[0], rxValue[1], rxValue[2]);
      uint8_t ca = rxValue[0];
      uint8_t se = rxValue[1];
      uint8_t va = rxValue[2];
      switch(ca){
        case CTR_RELAY:
          setRelay(se, va);
      }
      // procRx(rxValue[0], rxValue[1]);
      // if (rxValue.length() > 0) {
      //   Serial.println("*********");
      //   Serial.print("Received Value: ");
      //   for (int i = 0; i < rxValue.length(); i++)
      //     Serial.print(rxValue[i]);
      //   Serial.println("*********");
      // }
    }
};

void initBle(void){
  // Create the BLE Device
  BLEDevice::init("whybiz for esp32");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
										CHARACTERISTIC_UUID_TX,
										BLECharacteristic::PROPERTY_NOTIFY
									);
                      
  pTxCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
											 CHARACTERISTIC_UUID_RX,
											BLECharacteristic::PROPERTY_WRITE
										);

  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");  
}

void loop_ble(void){

    if (deviceConnected) {
      static uint8_t buf[3] = {0, };
      static uint16_t count = 0;
      static uint8_t sw = 0;
      uint8_t test = 1;
      buf[0] = 2;
      buf[1] = test << (count % 8);
      buf[2] = sw;
      if(!(count % 8)){
        if(sw) sw = 0;
        else sw = 1;
      } 
      count++;
      pTxCharacteristic->setValue(buf, sizeof(buf));
      pTxCharacteristic->notify();

		  delay(10); // bluetooth stack will go into congestion, if too many packets are sent
	  }

    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
		// do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }

}

