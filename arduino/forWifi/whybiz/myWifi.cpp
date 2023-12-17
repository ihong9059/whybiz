#include <WiFi.h>       // standard library
#include <WebServer.h>  // standard library
#include "SuperMon.h"   // .h file that stores your html page code

#include "uttec.h"
#include "myWifi.h"
#include "sx1509Lib.h"

#define USE_INTRANET
#define MYHOME 1

#ifdef MYHOME
#define LOCAL_SSID "ihong"
#define LOCAL_PASS "hongks@6063"
IPAddress local_IP(192, 168, 0, 111);
IPAddress local_gw(192, 168, 0, 1);
#else
#define LOCAL_SSID "uttec8"
#define LOCAL_PASS "a123456789"
IPAddress local_IP(192, 168, 8, 111);
IPAddress local_gw(192, 168, 8, 1);
#endif

#define AP_SSID "uttec_whybiz"
#define AP_PASS "uttec_whybiz"

char XML[2048];
static uint8_t mySelect = 0;

IPAddress Actual_IP;

IPAddress PageIP(192, 168, 1, 3);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress ip;

IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

WiFiServer uttecServer(20000);

void initWifi(void){
  disableCore0WDT();
#ifdef USE_INTRANET
  if (!WiFi.config(local_IP, local_gw, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  Serial.print("Connecting to ");
  Serial.println(LOCAL_SSID);
  WiFi.begin(LOCAL_SSID, LOCAL_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("IP address: "); Serial.println(WiFi.localIP());
  Actual_IP = WiFi.localIP();
#endif

#ifndef USE_INTRANET
  WiFi.softAP(AP_SSID, AP_PASS);
  delay(100);
  WiFi.softAPConfig(PageIP, gateway, subnet);
  delay(100);
  Actual_IP = WiFi.softAPIP();
  Serial.print("IP address: "); Serial.println(Actual_IP);
#endif

  printWifiStatus();
  uttecServer.begin();
  whybiz_t* pData = getWhybizFactor();
  pData->relay = 0xff;
  // testEeprom();
  // initSx1509();
}

// #include <WiFi.h>

const uint16_t myPort = 8091;
const char* myHost = "192.168.1.4";

void connectToServer(void){
  static uint32_t count = 0;
  WiFiClient client;
  if(!client.connect(myHost, myPort)){
    Serial.printf("Connection fail\r\n");
    return;
  }
  client.printf("Hello from esp32: %d", count++);
  client.stop();
}

void serverForGeneral(void){
  static bool connectFlag = false;
  static uint32_t count = 0;
  WiFiClient uttec_client = uttecServer.available();
  if(!uttec_client){
    return;
  }
  Serial.printf("new client");
  while(uttec_client.connected()){
    if(!connectFlag){
      Serial.print("remoteIP: ");
      Serial.println(uttec_client.remoteIP());
      printf("\r\nremotePort: %d\r\n", uttec_client.remotePort());
      connectFlag = true;
    }
    if(uttec_client.available()){
      String receiveData = uttec_client.readStringUntil('\n');
      Serial.println(receiveData);
      
      char temp [100] = {0, };
      uint8_t ca = count % 7;
      uint8_t se;
      uint8_t va;

      switch(ca){
        case ADC_DEVICE: se = random(0, 100); va = random(0, 100); break;
        case SWITCH_DEVICE: se = count; va = count; break;
        case RELAY_DEVICE: se = count; va = count; break;
        case LORA_DEVICE: se = count % 20; va = count; break;
        case VERSION_DEVICE: se = count; va = count; break;
        // case CHANNEL_DEVICE: se = 1; va = count % 23; break;
        case CHANNEL_DEVICE: se = count % 4; va = count % 23; break;
      }
      
      uint8_t len = sprintf(temp, "{\"ca\":%d,\"se\":%d,\"va\":%d}\n", 
        ca, se, va);
      uttec_client.write_P(temp, len);
      count++;
    }
  }
  connectFlag = false;
  Serial.printf("connection end\r\n");
}

void loop_wifi(void){
  char temp[100];
  serverForGeneral(); //for uttec_client, port: 2000
  // connectToServer(); //for myPort: 8091, 192.168.1.4
}


void SendWebsite() {
  Serial.println("sending web page");
}

void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  Serial.print("Open http://");
  Serial.println(ip);
}
