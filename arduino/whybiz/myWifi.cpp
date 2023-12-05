#include <WiFi.h>       // standard library
#include <WebServer.h>  // standard library
#include "SuperMon.h"   // .h file that stores your html page code

#include "uttec.h"
#include "myWifi.h"
#include "sx1509Lib.h"

#define USE_INTRANET

// #define LOCAL_SSID "ihong"
// #define LOCAL_PASS "hongks@6063"

#define LOCAL_SSID "uttec8"
#define LOCAL_PASS "a123456789"

#define AP_SSID "TestWebSite"
#define AP_PASS "023456789"

#define PIN_LED 2     //On board LED
#define PIN_A0 34     // some analog input sensor
#define PIN_A1 35     // some analog input sensor

int BitsA0 = 0, BitsA1 = 0;
bool LED0 = false;

char XML[2048];


bool relay[8] = {0,};
bool sw[8] = {0,};

IPAddress Actual_IP;

IPAddress PageIP(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress ip;

IPAddress local_IP(192, 168, 8, 111);
IPAddress local_gw(192, 168, 8, 1);
// IPAddress local_IP(192, 168, 0, 111);
// IPAddress local_gw(192, 168, 0, 1);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

WebServer server(80);

void initWifi(void){

  pinMode(PIN_LED, OUTPUT);

  LED0 = false;
  digitalWrite(PIN_LED, LED0);

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
  setPort();
  testEeprom();
  // initSx1509();
}

void loop_wifi(void){
  static uint32_t SensorUpdate = 0;
  uint32_t updateTime = 1000;
  if ((millis() - SensorUpdate) >= updateTime) {
    SensorUpdate = millis();
    BitsA0 = analogRead(PIN_A0);
    BitsA1 = analogRead(PIN_A1);
    for(int i = 0; i < 8; i++){
      sw[i] = !sw[i];
    }
    testPort();
  }
  server.handleClient();
}

static uint8_t mySelect = 0;

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
  Serial.println("Button 0 press");
  LED0 = !LED0;
  relay[0] = !relay[0];
  digitalWrite(PIN_LED, LED0);
  Serial.print("Button 0 "); Serial.println(LED0);
  server.send(200, "text/plain", ""); //Send web page
}

void ProcessButton_1() {
  Serial.println("Button 1 press");
  relay[1] = !relay[1];
  server.send(200, "text/plain", ""); //Send web page
}

void ProcessButton_2() {
  Serial.println("Button 2 press");
  relay[2] = !relay[2];
  server.send(200, "text/plain", ""); //Send web page
}

void ProcessButton_3() {
  Serial.println("Button 3 press");
  relay[3] = !relay[3];
  server.send(200, "text/plain", ""); //Send web page
}

void ProcessButton_4() {
  Serial.println("Button 4 press");
  relay[4] = !relay[4];
  server.send(200, "text/plain", ""); //Send web page
}

void ProcessButton_5() {
  Serial.println("Button 5 press");
  relay[5] = !relay[5];
  server.send(200, "text/plain", ""); //Send web page
}

void ProcessButton_6() {
  Serial.println("Button 6 press");
  relay[6] = !relay[6];
  server.send(200, "text/plain", ""); //Send web page
}

void ProcessButton_7() {
  String str_value = server.arg("VALUE");
  uint8_t value = str_value.toInt();
  Serial.print("LED7 value: "); Serial.println(value);  // LED0 = !LED0;

  Serial.println("Button 7 press");
  relay[7] = !relay[7];
  server.send(200, "text/plain", ""); //Send web page
}

void SendWebsite() {
  Serial.println("sending web page");
  server.send(200, "text/html", PAGE_MAIN);
}

void SendXML() {
  static uint32_t count = 0;
  char buf[32];

  int16_t lora_rssi = -110;
  if(count++ % 20 == 0)
    Serial.println("sending xml");
  else Serial.print(count % 20);

  strcpy(XML, "<?xml version = '1.0'?>\n<Data>\n");

  sprintf(buf, "<B0>%d</B0>\n", BitsA0);
  strcat(XML, buf);

  sprintf(buf, "<B1>%d</B1>\n", BitsA1);
  strcat(XML, buf);

  if (relay[0]) {strcat(XML, "<LED>1</LED>\n");}
  else {strcat(XML, "<LED>0</LED>\n");}

  if (relay[1]) {strcat(XML, "<LED1>1</LED1>\n");}
  else {strcat(XML, "<LED1>0</LED1>\n");}

  if (relay[2]) {strcat(XML, "<LED2>1</LED2>\n");}
  else {strcat(XML, "<LED2>0</LED2>\n");}

  if (relay[3]) {strcat(XML, "<LED3>1</LED3>\n");}
  else {strcat(XML, "<LED3>0</LED3>\n");}

  if (relay[4]) {strcat(XML, "<LED4>1</LED4>\n");}
  else {strcat(XML, "<LED4>0</LED4>\n");}

  if (relay[5]) {strcat(XML, "<LED5>1</LED5>\n");}
  else {strcat(XML, "<LED5>0</LED5>\n");}

  if (relay[6]) {strcat(XML, "<LED6>1</LED6>\n");}
  else {strcat(XML, "<LED6>0</LED6>\n");}

  if (relay[7]) {strcat(XML, "<LED7>1</LED7>\n");}
  else {strcat(XML, "<LED7>0</LED7>\n");}

// --------------------------------------------------------
  if (sw[0]) {strcat(XML, "<sw1>1</sw1>\n");}
  else {strcat(XML, "<sw1>0</sw1>\n");}

  if (sw[1]) {strcat(XML, "<sw2>1</sw2>\n");}
  else {strcat(XML, "<sw2>0</sw2>\n");}

  if (sw[2]) {strcat(XML, "<sw3>1</sw3>\n");}
  else {strcat(XML, "<sw3>0</sw3>\n");}

  if (sw[3]) {strcat(XML, "<sw4>1</sw4>\n");}
  else {strcat(XML, "<sw4>0</sw4>\n");}

  if (sw[4]) {strcat(XML, "<sw5>1</sw5>\n");}
  else {strcat(XML, "<sw5>0</sw5>\n");}

  if (sw[5]) {strcat(XML, "<sw6>1</sw6>\n");}
  else {strcat(XML, "<sw6>0</sw6>\n");}

  if (sw[6]) {strcat(XML, "<sw7>1</sw7>\n");}
  else {strcat(XML, "<sw7>0</sw7>\n");}

  if (sw[7]) {strcat(XML, "<sw8>1</sw8>\n");}
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
