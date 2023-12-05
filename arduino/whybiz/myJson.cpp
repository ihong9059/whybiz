
#include <ArduinoJson.h>
#include "uttec.h"
#include "myJson.h"

void testSendJson(void){
  StaticJsonDocument<MAX_DOC> doc;
  uint8_t rand = random(1, 100);
  doc["no"] = rand;
  doc["ca"] = rand % 4;
  doc["de"] = rand;
  doc["va"] = rand;
  doc["crc"] = rand * 3 + (rand % 4);

  serializeJson(doc, Serial2);
}

void testReceiveJson(void){
  uttecJson_t myJason;
  char temp[MAX_DOC] = {0, };
  uint8_t count = 0;
  // send data only when you receive data:
  while(Serial2.available() > 0){
    // read the incoming byte:
    temp[count++] = Serial2.read();
  }
  if(count > 0){
    StaticJsonDocument<MAX_DOC> doc;
    DeserializationError error = deserializeJson(doc, temp);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }    

    myJason.node = doc["no"];
    myJason.category = doc["ca"];
    myJason.device = doc["de"];
    myJason.value = doc["va"];
    myJason.crc = doc["crc"];
    int16_t totalCrc = myJason.node + myJason.value + myJason.category + myJason.device;
    Serial.printf("node: %d, category: %d, device: %d, value: %d\r\n", 
      myJason.node, myJason.category, myJason.device, myJason.value);

    if(totalCrc == myJason.crc){
      Serial.printf("congratulation -------------> \r\n");
      switch(myJason.category){
        case 0:
        case 10:
          procCmdLine(myJason);
          Serial.printf("-------> Command line proc\r\n"); break;
        case 1:
        case 11:
          procSwitch(myJason);
          Serial.printf("-------> Switch proc\r\n"); break;
        case 2:
        case 12:
          procRelay(myJason);
          Serial.printf("-------> Relay proc\r\n"); break;
        case 3:
        case 13:
          procAdc(myJason);
          Serial.printf("-------> Adc proc\r\n"); break;
        default:
          Serial.printf("<-------- error category: %d\r\n", myJason.category); break;
      }
    } 
    else{
      Serial.printf("error in crc: %d, %d\r\n", totalCrc, myJason.crc);
      return;
    }

  }
}

void testParse(void){
  StaticJsonDocument<200> doc;
  char json[] =
      "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, json);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  const char* sensor = doc["sensor"];
  long time = doc["time"];
  double latitude = doc["data"][0];
  double longitude = doc["data"][1];

  // Print values.
  Serial.println(sensor);
  Serial.println(time);
  Serial.println(latitude, 6);
  Serial.println(longitude, 6);
}

void generateJsonSerial(void){
  // Allocate the JSON document
  //
  // Inside the brackets, 200 is the RAM allocated to this document.
  // Don't forget to change this value to match your requirement.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<200> doc;

  // StaticJsonObject allocates memory on the stack, it can be
  // replaced by DynamicJsonDocument which allocates in the heap.
  //
  // DynamicJsonDocument  doc(200);

  // Add values in the document
  //
  doc["sensor"] = "gps";
  doc["time"] = 1351824120;

  // Add an array.
  //
  JsonArray data = doc.createNestedArray("data");
  data.add(48.756080);
  data.add(2.302038);

  // Generate the minified JSON and send it to the Serial port.
  //
  serializeJson(doc, Serial);
  // The above line prints:
  // {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}

  // Start a new line
  Serial.println();

  // Generate the prettified JSON and send it to the Serial port.
  //
  // serializeJsonPretty(doc, Serial);
  // The above line prints:
  // {
  //   "sensor": "gps",
  //   "time": 1351824120,
  //   "data": [
  //     48.756080,
  //     2.302038
  //   ]
  // }  
}

