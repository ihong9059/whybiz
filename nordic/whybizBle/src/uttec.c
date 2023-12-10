#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#include <zephyr/data/json.h>

#include "uttec.h"
#include <zephyr/logging/log.h>

whybiz_t myWhybiz = {0, };

sx1509_t beforeSx1509 = {0, };

struct whybiz{
    uint8_t no;
    uint8_t ca;
    uint8_t se;
    uint8_t va;
    uint8_t crc;
};

static const struct json_obj_descr whybiz_descr[] = {
  JSON_OBJ_DESCR_PRIM(struct whybiz, no, JSON_TOK_NUMBER),
  JSON_OBJ_DESCR_PRIM(struct whybiz, ca, JSON_TOK_NUMBER),
  JSON_OBJ_DESCR_PRIM(struct whybiz, se, JSON_TOK_NUMBER),
  JSON_OBJ_DESCR_PRIM(struct whybiz, va, JSON_TOK_NUMBER),
  JSON_OBJ_DESCR_PRIM(struct whybiz, crc, JSON_TOK_NUMBER),
};

#define SELECT1_NODE DT_NODELABEL(select1)
#define SELECT2_NODE DT_NODELABEL(select2)
#define LORARST_NODE DT_NODELABEL(lorarst)
#define PWR_CRL_NODE DT_NODELABEL(pwrctl)

static const struct gpio_dt_spec select1 = GPIO_DT_SPEC_GET(SELECT1_NODE, gpios);
static const struct gpio_dt_spec select2 = GPIO_DT_SPEC_GET(SELECT2_NODE, gpios);
static const struct gpio_dt_spec loraRst = GPIO_DT_SPEC_GET(LORARST_NODE, gpios);
static const struct gpio_dt_spec pwrCtl = GPIO_DT_SPEC_GET(PWR_CRL_NODE, gpios);

whybiz_t* getWhybizFactor(void){
    return &myWhybiz;    
}

void initPort(void){
	gpio_pin_configure_dt(&select1, GPIO_OUTPUT_ACTIVE);
	gpio_pin_configure_dt(&select2, GPIO_OUTPUT_ACTIVE);
	gpio_pin_configure_dt(&loraRst, GPIO_OUTPUT_ACTIVE);
	gpio_pin_configure_dt(&pwrCtl, GPIO_OUTPUT_ACTIVE);

    gpio_pin_set_dt(&select1, 1);//set low
    gpio_pin_set_dt(&select2, 1);//set low
    gpio_pin_set_dt(&loraRst, 0);//set high
    gpio_pin_set_dt(&pwrCtl, 1);//set low
    // setUartChannel(0);
}

void setUartChannel(uint8_t channel){
    switch(channel){
        case 0: 
            gpio_pin_set_dt(&select2, 1); gpio_pin_set_dt(&select1, 1); 
            printf("Ethernet channel\r\n");
        break;
        case 1:
            gpio_pin_set_dt(&select2, 1); gpio_pin_set_dt(&select1, 0); 
            printf("rs485 channel\r\n");
        break;
        case 2:
            gpio_pin_set_dt(&select2, 0); gpio_pin_set_dt(&select1, 1); 
            printf("lora channel\r\n");
        break;
        case 3:
            gpio_pin_set_dt(&select2, 0); gpio_pin_set_dt(&select1, 0); 
            printf("tbd channel\r\n");
        break;
        default: printf("error channel: %d\r\n", channel);
    }
}

sx1509_t* getBeforeSxReg(void){
    return &beforeSx1509;
}

#include "sx1509.h"
void uttecJsonTest(uint8_t* pData, uint8_t len){
    uint8_t test_msg[100] = {0,};
    for(int i = 0; i < len; i++) test_msg[i] = *pData++;

    uint16_t length = len;
    struct whybiz ctr;
    
    int16_t ret = json_obj_parse(test_msg, length,
                    whybiz_descr,
                    ARRAY_SIZE(whybiz_descr),
                    &ctr);
    
    if (ret < 0)
    {
        // LOG_INF("JSON Parse Error: %d\r\n", ret);
        printk("JSON Parse Error");
    }
    else
    {
        printk("no:%d, ca:%d, se:%d, va:%d, crc:%d\r\n",
        ctr.no, ctr.ca, ctr.se, ctr.va, ctr.crc);
    	writeOutSx(ctr.se, ctr.va);
    }
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

void testJsonOut(void){
    static uint8_t count = 0;
    myWhybizFrame.node = 1;
    myWhybizFrame.category = 2;
    myWhybizFrame.sensor = count % 8;
    myWhybizFrame.value = count;
    count++;
    getWhybizFrame();
}



