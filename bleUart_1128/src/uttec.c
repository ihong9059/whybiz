#include <zephyr/data/json.h>

#include "uttec.h"

// #include <zephyr/device.h>
// #include <zephyr/devicetree.h>
// #include <soc.h>

#include <zephyr/logging/log.h>


// #define LOG_MODULE_NAME peripheral_uart
// LOG_MODULE_REGISTER(LOG_MODULE_NAME);

#define MAX_END_POINT 	100
#define MAX_LINE		20

// #define printk LOG_INF 

char uartData[MAX_LINE][MAX_END_POINT] = {0,};
uint8_t positionFirst[MAX_LINE] = {0, };
uint8_t positionLast[MAX_LINE] = {0, };

uartBuf_t myUart = {0, }; 

struct temperature {
    int device;
    int value;
//   const char *unit;
//   int value;
};

static const struct json_obj_descr temperature_descr[] = {
//   JSON_OBJ_DESCR_PRIM(struct temperature, unit, JSON_TOK_STRING),
  JSON_OBJ_DESCR_PRIM(struct temperature, device, JSON_TOK_NUMBER),
  JSON_OBJ_DESCR_PRIM(struct temperature, value, JSON_TOK_NUMBER),
};

void uttecTest(void){
    static int device = 0; 
    static int value = 50; 
    // uint8_t json_msg[] = "{\"unit\":\"c\",\"value\":30}";
    uint8_t test_msg[100] = {0,};
    uint16_t length = sprintf(test_msg, 
        "{\"device\":%d, \"value\": %d}", device++, value++);

    struct temperature temp_results;
    
    // int64_t ret = json_obj_parse(json_msg, sizeof(json_msg),
    //                 temperature_descr,
    //                 ARRAY_SIZE(temperature_descr),
    //                 &temp_results);
    
    int16_t ret = json_obj_parse(test_msg, length,
                    temperature_descr,
                    ARRAY_SIZE(temperature_descr),
                    &temp_results);
    
    if (ret < 0)
    {
        // LOG_INF("JSON Parse Error: %d\r\n", ret);
        printk("JSON Parse Error");
    }
    else
    {
        printk("json_obj_parse return code: %d\r\n", ret);
        printk("Unit: %d\r\n", temp_results.device);
        printk("Value: %d\r\n", temp_results.value);
    }
}

void putBuff(uint8_t* pData, uint8_t len){
    for(int i = 0; i < len; i++){
        static bool newLineFlag = false;
        char temp = pData[i];
        putchar(temp);
        if((temp >= 32) && (temp <= 127)){
            uartData[myUart.lines][myUart.endPoint] = temp;
            if(temp == ':'){
                positionFirst[myUart.lines] = myUart.endPoint;
            }
            myUart.endPoint++;
            newLineFlag = false;
        }
        else if((temp == '\r') || (temp == '\n')){
            if(!newLineFlag){
                positionLast[myUart.lines] = myUart.endPoint;
                myUart.lines++;
                myUart.endPoint = 0;
            }
            newLineFlag = true;
            myUart.isReady = true;
        }
    }					 
}

void clearUart(void){
    myUart.isReady = false;
    myUart.endPoint = 0;
    myUart.lines = 0;
}

bool isRxReady(void){
    return myUart.isReady;
}

void clearUartBuf(void){
    for(int i = 0; i < MAX_LINE; i++){
        for(int k = 0; k < MAX_END_POINT; k++){
            uartData[i][k] = 0;
        }
    }
    clearUart();
}

#include <stdlib.h>
void checkFrame(void){
    uint8_t test[7] = "SRC PID";
    myUart.isValid = false;
    for(int i = 0; i < myUart.lines; i++){
        bool result = true;
        for(int k = 0; k < 6; k++){
            if(uartData[i][k] != test[k]){
                result = false;
            } 
        }
        
        if(result){
            printk("------> first SRC PID: %d\r\n", i);
            myUart.isValid = true;
            myUart.startLine = i - 1;
            printk("------> start line: %d\r\n", myUart.startLine);
        } 
    }
}

factor_t myFactor = {0,};

void callPrint(void){
    char temp[10] = {0,};
    if(isRxReady()){
        printk("---->callPrint, line: %d\r\n", myUart.lines);
        checkFrame();
        if(!myUart.isValid){
            printk("--------- not valid frame --------\r\n");
            return;
        }
        for(int i = 0; i < myUart.lines; i++){
            printk("line: %d, first: %d, last: %d\r\n"
                , i, positionFirst[i], positionLast[i]);
            printk("%s\r\n", uartData[i]);

            uint8_t tempPoint = 0;
            switch(i - myUart.startLine){
                case PID_line:
                    for(int k = PID_first; k < PID_last+1; k++){
                        temp[tempPoint++] = uartData[i][k];
                    }
                    myFactor.src_pid = (uint16_t)strtol(temp, NULL, 16);
                break;
                case ADR_line:
                    for(int k = ADR_first; k < ADR_last+1; k++){
                        temp[tempPoint++] = uartData[i][k];
                    }
                    myFactor.src_addr = (uint16_t)strtol(temp, NULL, 16);
                case RSSI_line:
                    for(int k = RSSI_fist; k < RSSI_last+1; k++){
                        temp[tempPoint++] = uartData[i][k];
                    }
                    myFactor.rssi = atoi(temp);
                break;
                case DATA_line:
                    for(int k = DATA_first; k < DATA_last+1; k++){
                        temp[tempPoint++] = uartData[i][k];
                    }
                    printk("Data: %d\r\n", atoi(temp));
                break;
            }
        }
        printk("pid: %x, addr: %x, rssi: %d\r\n",
            myFactor.src_pid, myFactor.src_addr, myFactor.rssi);
        clearUartBuf();
    }
}



