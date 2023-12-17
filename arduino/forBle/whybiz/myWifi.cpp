#include <WiFi.h>       // standard library
#include <WebServer.h>  // standard library
#include "SuperMon.h"   // .h file that stores your html page code

#include "uttec.h"
#include "myWifi.h"
#include "sx1509Lib.h"

// #define USE_INTRANET

// #define MYHOME 1

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

WebServer server(80);
WiFiServer uttecServer(20000);

void initWifi(void){
  disableCore0WDT();
  Serial.println("starting server");
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
  server.on("/", SendWebsite);
  server.on("/xml", SendXML);

  server.on("/BUTTON_0", ProcessButton_0);
  server.on("/BUTTON_1", ProcessButton_1);
  server.on("/BUTTON_2", ProcessButton_2);
  server.on("/BUTTON_3", ProcessButton_3);
  server.on("/BUTTON_4", ProcessButton_4);
  server.on("/BUTTON_5", ProcessButton_5);
  server.on("/BUTTON_6", ProcessButton_6);
  server.on("/BUTTON_7", ProcessButton_7);

  server.on("/UPDATE_SELECT", ProcessSelect);
  server.on("/UPDATE_TOGGLE", ProcessToggle);

  server.begin();
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
  WiFiClient uttec_client = uttecServer.available();
  if(!uttec_client){
    return;
  }
  Serial.printf("new client");
  while(uttec_client.connected()){
    if(uttec_client.available()){
      String data = uttec_client.readStringUntil('%');
      Serial.println(data);
    }
  }
  Serial.printf("connection end\r\n");
}

void loop_wifi(void){
  char temp[100];
  server.handleClient();
  serverForGeneral(); //for uttec_client, port: 2000
  connectToServer(); //for myPort: 8091, 192.168.1.4
}


void ProcessToggle() {
  String str_select = server.arg("VALUE");
  // uint8_t select_num = str_select.toInt();
  // mySelect = select_num;
  Serial.printf("\r\nSelect: %s\r\n", str_select);
  server.send(200, "text/plain", ""); //Send web page
}

void ProcessSelect() {
  String str_select = server.arg("VALUE");
  uint8_t select_num = str_select.toInt();
  mySelect = select_num;
  Serial.printf("\r\nSelect: %d\r\n", select_num);
  server.send(200, "text/plain", ""); //Send web page
}

void ProcessButton_0() {
  whybiz_t* pData = getWhybizFactor();
  if(pData->relay & 0x01) setRelay(0, 1);
  else setRelay(0, 0);
  Serial.println("Button 0 press");
  server.send(200, "text/plain", ""); //Send web page
}

void ProcessButton_1() {
  whybiz_t* pData = getWhybizFactor();
  if(pData->relay & 0x02) setRelay(1, 1);
  else setRelay(1, 0);
  Serial.println("Button 1 press");
  server.send(200, "text/plain", ""); //Send web page
}

void ProcessButton_2() {
  whybiz_t* pData = getWhybizFactor();
  if(pData->relay & 0x04) setRelay(2, 1);
  else setRelay(2, 0);
  Serial.println("Button 2 press");
  server.send(200, "text/plain", ""); //Send web page
}

void ProcessButton_3() {
  whybiz_t* pData = getWhybizFactor();
  if(pData->relay & 0x08) setRelay(3, 1);
  else setRelay(3, 0);
  Serial.println("Button 3 press");
  server.send(200, "text/plain", ""); //Send web page
}

void ProcessButton_4() {
  whybiz_t* pData = getWhybizFactor();
  if(pData->relay & 0x10) setRelay(4, 1);
  else setRelay(4, 0);
  Serial.println("Button 4 press");
  server.send(200, "text/plain", ""); //Send web page
}

void ProcessButton_5() {
  whybiz_t* pData = getWhybizFactor();
  if(pData->relay & 0x20) setRelay(5, 1);
  else setRelay(5, 0);
  Serial.println("Button 5 press");
  server.send(200, "text/plain", ""); //Send web page
}

void ProcessButton_6() {
  whybiz_t* pData = getWhybizFactor();
  if(pData->relay & 0x40) setRelay(6, 1);
  else setRelay(6, 0);
  Serial.println("Button 6 press");
  server.send(200, "text/plain", ""); //Send web page
}

void ProcessButton_7() {
  whybiz_t* pData = getWhybizFactor();
  if(pData->relay & 0x80) setRelay(7, 1);
  else setRelay(7, 0);
  Serial.println("Button 7 press");
  server.send(200, "text/plain", ""); //Send web page
}

void SendWebsite() {
  Serial.println("sending web page");
  server.send(200, "text/html", PAGE_MAIN);
}

void SendXML() {
  static uint32_t count = 0;
  char buf[32];
  whybiz_t* pData = getWhybizFactor();

  int16_t lora_rssi = -110;
  if(count++ % 20 == 0)
    Serial.println("sending xml");
  else Serial.print(count % 20);

  strcpy(XML, "<?xml version = '1.0'?>\n<Data>\n");

  sprintf(buf, "<B0>%d</B0>\n", pData->adc1);
  strcat(XML, buf);

  sprintf(buf, "<B1>%d</B1>\n", pData->adc2);
  strcat(XML, buf);

  if (pData->relay & 0x01) {strcat(XML, "<LED>1</LED>\n");}
  else {strcat(XML, "<LED>0</LED>\n");}

  if (pData->relay & 0x02) {strcat(XML, "<LED1>1</LED1>\n");}
  else {strcat(XML, "<LED1>0</LED1>\n");}

  if (pData->relay & 0x04) {strcat(XML, "<LED2>1</LED2>\n");}
  else {strcat(XML, "<LED2>0</LED2>\n");}

  if (pData->relay & 0x08) {strcat(XML, "<LED3>1</LED3>\n");}
  else {strcat(XML, "<LED3>0</LED3>\n");}

  if (pData->relay & 0x10) {strcat(XML, "<LED4>1</LED4>\n");}
  else {strcat(XML, "<LED4>0</LED4>\n");}

  if (pData->relay & 0x20) {strcat(XML, "<LED5>1</LED5>\n");}
  else {strcat(XML, "<LED5>0</LED5>\n");}

  if (pData->relay & 0x40) {strcat(XML, "<LED6>1</LED6>\n");}
  else {strcat(XML, "<LED6>0</LED6>\n");}

  if (pData->relay & 0x80) {strcat(XML, "<LED7>1</LED7>\n");}
  else {strcat(XML, "<LED7>0</LED7>\n");}

// --------------------------------------------------------
  if (pData->sw & 0x01) {strcat(XML, "<sw1>1</sw1>\n");}
  else {strcat(XML, "<sw1>0</sw1>\n");}

  if (pData->sw & 0x02) {strcat(XML, "<sw2>1</sw2>\n");}
  else {strcat(XML, "<sw2>0</sw2>\n");}

  if (pData->sw & 0x04) {strcat(XML, "<sw3>1</sw3>\n");}
  else {strcat(XML, "<sw3>0</sw3>\n");}

  if (pData->sw & 0x08) {strcat(XML, "<sw4>1</sw4>\n");}
  else {strcat(XML, "<sw4>0</sw4>\n");}

  if (pData->sw & 0x10) {strcat(XML, "<sw5>1</sw5>\n");}
  else {strcat(XML, "<sw5>0</sw5>\n");}

  if (pData->sw & 0x20) {strcat(XML, "<sw6>1</sw6>\n");}
  else {strcat(XML, "<sw6>0</sw6>\n");}

  if (pData->sw & 0x40) {strcat(XML, "<sw7>1</sw7>\n");}
  else {strcat(XML, "<sw7>0</sw7>\n");}

  if (pData->sw & 0x80) {strcat(XML, "<sw8>1</sw8>\n");}
  else {strcat(XML, "<sw8>0</sw8>\n");}

  // switch(mySelect){
  //   case 0: strcat(XML, "<select>0</select>\n"); break; 
  //   case 1: strcat(XML, "<select>1</select>\n"); break; 
  //   case 2: strcat(XML, "<select>2</select>\n"); break; 
  //   case 3: strcat(XML, "<select>3</select>\n"); break; 
  // }
  // if(count % 2) {strcat(XML, "<act1>1</act1>\n");}
  // else {strcat(XML, "<act1>0</act1>\n");}

  // if(count % 2) {strcat(XML, "<act2>1</act2>\n");}
  // else {strcat(XML, "<act2>0</act2>\n");}

  sprintf(buf, "<VERSION>%02f</VERSION>\n", 1.1);
  strcat(XML, buf);

  sprintf(buf, "<LORA_RSSI>%d</LORA_RSSI>\n", lora_rssi);
  strcat(XML, buf);


  strcat(XML, "</Data>\n");
  // Serial.println(XML);
  server.send(200, "text/xml", XML);
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
