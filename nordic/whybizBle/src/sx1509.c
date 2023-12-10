#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/devicetree.h> 
#include <zephyr/drivers/i2c.h>

#define SX1509_ADDRESS		0x3e
#define I2C_NODE DT_NODELABEL(arduino_i2c)
static const struct device *i2c0_dev = DEVICE_DT_GET(I2C_NODE);

void initSx1509(void){
	int err;
	uint8_t buf[2];

	if(!device_is_ready(i2c0_dev)){
		printk("i2c_dev is not ready ----------->\r\n");
		while(1);
	}

	buf[0] = 0x00; buf[1] = 0x00; //port b, input enable	
	err = i2c_write(i2c0_dev, buf, 2, SX1509_ADDRESS);
	buf[0] = 0x01; buf[1] = 0x00; //port a, input enable
	err = i2c_write(i2c0_dev, buf, 2, SX1509_ADDRESS);

	buf[0] = 0x06; buf[1] = 0xff; //port b, pull up
	err = i2c_write(i2c0_dev, buf, 2, SX1509_ADDRESS);
	buf[0] = 0x07; buf[1] = 0xff; //port a, pull up
	err = i2c_write(i2c0_dev, buf, 2, SX1509_ADDRESS);
	buf[0] = 0x0e; buf[1] = 0x00; //port b, output
	err = i2c_write(i2c0_dev, buf, 2, SX1509_ADDRESS);
	buf[0] = 0x0f; buf[1] = 0xff; //port a, input
	err = i2c_write(i2c0_dev, buf, 2, SX1509_ADDRESS);
	if(err < 0){
		printk("i2c write fail------------\r\n");		// return 0;
		while(1);
	}
	printk("------> end of initSx1509\r\n");
}

static uint8_t sx1509_in = 0;
#define BIT_0 0B11111110
#define BIT_1 0B11111101
#define BIT_2 0B11111011
#define BIT_3 0B11110111
#define BIT_4 0B11101111
#define BIT_5 0B11011111
#define BIT_6 0B10111111
#define BIT_7 0B01111111


uint8_t getSx1509(void){
	int err;
	uint8_t buf = 0x11;

	err = i2c_write(i2c0_dev, &buf, 1, SX1509_ADDRESS);
	if(err < 0){
		printk("i2c write fail------------\r\n");
	}
	err = i2c_read(i2c0_dev, &buf, 1, SX1509_ADDRESS);
	if(err < 0){
		printk("i2c write fail------------\r\n");
	}
	sx1509_in = buf;
	return sx1509_in;
}

#include "uttec.h"
#include "myBle.h"

bool checkSx1509In(void){
	static uint16_t testCount = 0;
	uint8_t swNow = getSx1509();
	sx1509_t* pBefore = getBeforeSxReg();

	if(pBefore->sw == swNow){
		printk("now and before same: %d\r\n", testCount++);
		return 0;	
	} 		
	printk("now: %x, reg: %x\r\n", swNow, pBefore->sw);
	whybizFrame_t* pJson = getWhybizFrame();
	pJson->node = 1;
	pJson->category = 2;

	for(int i = 0; i < 8; i++){
		uint8_t test = 0x01;
		test = test << i;
		uint8_t tempNow = swNow & test;
		uint8_t tempBefore = pBefore->sw & test; 
		if(tempBefore != tempNow){
			pJson->sensor = i;
			pJson->value = tempNow;

			printk("send Change report: %d, %x\r\n", i, tempNow);
			k_sleep(K_MSEC(100));
			procSwitchTxBle(i, tempNow);
			sendWhybizFrame();
		}
	}
	pBefore->sw = swNow;
	return true;
}

uint8_t readOut(void){
	int err;
	uint8_t buf = 0x10;

	err = i2c_write(i2c0_dev, &buf, 1, SX1509_ADDRESS);
	if(err < 0){
		printk("i2c write fail------------\r\n");
	}
	err = i2c_read(i2c0_dev, &buf, 1, SX1509_ADDRESS);
	if(err < 0){
		printk("i2c write fail------------\r\n");
	}
	printk("read buf: %x\r\n", buf);
	return buf;
}

void writeOutSx(uint8_t bitNum, bool output){
	uint8_t regB = readOut();
	int err;
	printk("~BIT_0: %x\r\n", (uint8_t)~BIT_0);

	switch(bitNum){
		case 0:
			if(output) regB = regB | (uint8_t)~BIT_0;
			else regB = regB & BIT_0;
		break;
		case 1:
			if(output) regB = regB | (uint8_t)~BIT_1;
			else regB = regB & BIT_1;
		break;
		case 2:
			if(output) regB = regB | (uint8_t)~BIT_2;
			else regB = regB & BIT_2;
		break;
		case 3:
			if(output) regB = regB | (uint8_t)~BIT_3;
			else regB = regB & BIT_3;
		break;
		case 4:
			if(output) regB = regB | (uint8_t)~BIT_4;
			else regB = regB & BIT_4;
		break;
		case 5:
			if(output) regB = regB | (uint8_t)~BIT_5;
			else regB = regB & BIT_5;
		break;
		case 6:
			if(output) regB = regB | (uint8_t)~BIT_6;
			else regB = regB & BIT_6;
		break;
		case 7:
			if(output) regB = regB | (uint8_t)~BIT_7;
			else regB = regB & BIT_7;
		break;
	}
	uint8_t buf[2];
	buf[0] = 0x10; buf[1] = regB; //regB, all high
	err = i2c_write(i2c0_dev, buf, 2, SX1509_ADDRESS);
	if(err < 0){
		printk("i2c write fail------------\r\n");
	}
	printk("regB: %x\r\n", regB);
}
// void setRelay()
void testRelay(void){
	static uint16_t count = 0;
	switch(count%16){
		case 0: writeOutSx(count%8, true); break;
		case 1: writeOutSx(count%8, true); break;
		case 2: writeOutSx(count%8, true); break;
		case 3: writeOutSx(count%8, true); break;
		case 4: writeOutSx(count%8, true); break;
		case 5: writeOutSx(count%8, true); break;
		case 6: writeOutSx(count%8, true); break;
		case 7: writeOutSx(count%8, true); break;
		case 8: writeOutSx(count%8, false); break;
		case 9: writeOutSx(count%8, false); break;
		case 10: writeOutSx(count%8, false); break;
		case 11: writeOutSx(count%8, false); break;
		case 12: writeOutSx(count%8, false); break;
		case 13: writeOutSx(count%8, false); break;
		case 14: writeOutSx(count%8, false); break;
		case 15: writeOutSx(count%8, false); break;
	}
	count++;
}

void testSw(void){
	// static uint16_t count = 0;
	checkSx1509In();
}
