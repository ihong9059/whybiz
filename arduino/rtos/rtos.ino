#include <Arduino.h>
#include <stdio.h>

#include <ArduinoJson.h>

typedef struct{
  uint16_t no;
  uint8_t ca;
  uint8_t se;
  int16_t va;
  uint16_t crc;
} uttecJson_t;


#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// LED rates
static const int rate_1 = 500;  // ms
static const int rate_2 = 323;  // ms

// Pins
static const int led_pin = 2;

uint32_t total = 0;
// Our task: blink an LED at one rate
void toggleLED_1(void *parameter) {
  static uint32_t count = 0;
  while(1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(500);
    digitalWrite(led_pin, LOW);
    vTaskDelay(500);
    Serial.print("task1");
    Serial.println(count);
    // Serial.print("portTICK_PERIOD_MS: ");
    // Serial.println(portTICK_PERIOD_MS);
    count++;
    total++;
  }
}


#define MAX_DOC 100

void parseJson(char* pJson){
  uttecJson_t ctr;
  StaticJsonDocument<MAX_DOC> doc;
  DeserializationError error = deserializeJson(doc, pJson);
  if (error) {
    // Serial.print(F("deserializeJson() failed: "));
    // Serial.println(error.f_str());
    Serial.printf("json error ---------\r\n");
  }    

  ctr.no = doc["no"];
  ctr.ca = doc["ca"];
  ctr.se = doc["se"];
  ctr.va = doc["va"];
  ctr.crc = doc["crc"];
  int16_t totalCrc = ctr.no + ctr.va + ctr.ca + ctr.se;
  Serial.printf("no: %d, ca: %d, se: %d, va: %d, crc: %d\r\n",
    ctr.no, ctr.ca, ctr.se, ctr.va, ctr.crc);
}

// Our task: blink an LED at another rate
void parseData(void *parameter) {
  char temp[100] = {0, };
  bool startFlag = false;

  uint16_t position = 0;

  while (1)
  {
    while(Serial2.available() > 0){
      char test = Serial2.read();
      if(test == '{'){
        startFlag = true;
        position = 0;
        temp[position++] = test;
      }
      else if(startFlag){
        if(test == '}'){
          temp[position++] = test;
          temp[position++] = '\r';    
          temp[position] = '\n';    

          if(position > 10){
            Serial.println("test-------->");
            Serial.print("position: ");
            Serial.println(position);
            Serial.print(temp);   
            parseJson(temp); 
            // uttecJson_t ctr;
            // StaticJsonDocument<MAX_DOC> doc;
            // DeserializationError error = deserializeJson(doc, temp);
            // if (error) {
            //   // Serial.print(F("deserializeJson() failed: "));
            //   // Serial.println(error.f_str());
            //   Serial.printf("json error ---------\r\n");
            // }    

            // ctr.no = doc["no"];
            // ctr.ca = doc["ca"];
            // ctr.se = doc["se"];
            // ctr.va = doc["va"];
            // // ctr.crc = doc["crc"];
            // int16_t totalCrc = ctr.no + ctr.va + ctr.ca + ctr.se;
            // // Serial.printf("no: %d, ca: %d, se: %d, va: %d\r\n", 
            // //   ctr.no, ctr.ca, ctr.se, ctr.va);
            // // sprintf(temp, "ca: %d, se: %d, va: %d\r\n", 
            // //   ctr.ca, ctr.se, ctr.va);
            // // Serial.print(temp);
            // uint8_t test = 100;
            // sprintf(temp, "ca: %d, se: %d, va: %d", ctr.ca, ctr.se, ctr.va);
            // Serial.print(temp);
            // // Serial.print(ctr.ca); 
            // // Serial.print(", se:"); Serial.println(ctr.se);
            // // Serial.print(", va:"); Serial.println(ctr.va);
            // // Serial.print(", va:"); 
            // // Serial.println(temp);
          }
          startFlag = false;
          position = 0;
        }
        else{
          if(position > MAX_DOC){
            startFlag = false;
            position = 0;
          }
          else{
            temp[position++] = test;
          } 
        }       
      } 
    }
    delay(100);
  }  
}

#define RXD2 4  //ok
#define TXD2 5  //ok

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  // Configure pin
  pinMode(led_pin, OUTPUT);

  // Task to run forever
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
              toggleLED_1,  // Function to be called
              "Toggle 1",   // Name of task
              1024,         // Stack size (bytes in ESP32, words in FreeRTOS)
              NULL,         // Parameter to pass to function
              1,            // Task priority (0 to configMAX_PRIORITIES - 1)
              NULL,         // Task handle
              app_cpu);     // Run on one core for demo purposes (ESP32 only)

  // Task to run forever
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
              parseData,  // Function to be called
              "Json parsing",   // Name of task
              2048,         // Stack size (bytes in ESP32, words in FreeRTOS)
              NULL,         // Parameter to pass to function
              1,            // Task priority (0 to configMAX_PRIORITIES - 1)
              NULL,         // Task handle
              app_cpu);     // Run on one core for demo purposes (ESP32 only)

  // If this was vanilla FreeRTOS, you'd want to call vTaskStartScheduler() in
  // main after setting up your tasks.
}

void loop() {
  Serial.print("--> total: ");
  Serial.println(total);
  delay(500);
  // Do nothing
  // setup() and loop() run in their own task with priority 1 in core 1
  // on ESP32
}