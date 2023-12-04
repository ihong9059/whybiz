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
	}
	printk("<--------- pass i2c device\r\n");

	// buf[0] = 0x00; buf[1] = 0xff; //port b, input disable	
	buf[0] = 0x00; buf[1] = 0x00; //port b, input enable	
	err = i2c_write(i2c0_dev, buf, 2, SX1509_ADDRESS);
	if(err < 0){
		printk("i2c write fail------------\r\n");
		// return 0;
	}
	buf[0] = 0x01; buf[1] = 0x00; //port a, input enable
	// buf[0] = 0x01; buf[1] = 0xff; //port a, input disable
	err = i2c_write(i2c0_dev, buf, 2, SX1509_ADDRESS);
	if(err < 0){
		printk("i2c write fail------------\r\n");
		// return 0;
	}

	buf[0] = 0x06; buf[1] = 0xff; //port b, pull up
	err = i2c_write(i2c0_dev, buf, 2, SX1509_ADDRESS);
	if(err < 0){
		printk("i2c write fail------------\r\n");
		// return 0;
	}
	buf[0] = 0x07; buf[1] = 0xff; //port a, pull up
	err = i2c_write(i2c0_dev, buf, 2, SX1509_ADDRESS);
	if(err < 0){
		printk("i2c write fail------------\r\n");
		// return 0;
	}

	buf[0] = 0x0e; buf[1] = 0x00; //port b, output
	err = i2c_write(i2c0_dev, buf, 2, SX1509_ADDRESS);
	if(err < 0){
		printk("i2c write fail------------\r\n");
		// return 0;
	}
	buf[0] = 0x0f; buf[1] = 0xff; //port a, input
	err = i2c_write(i2c0_dev, buf, 2, SX1509_ADDRESS);
	if(err < 0){
		printk("i2c write fail------------\r\n");
		// return 0;
	}
	printk("end of initSx1509\r\n");

}

static uint8_t sx1509_in = 0;
// 0x10(regB), 0x11(regA)
#define BIT_0 0B11111110
#define BIT_1 0B11111101
#define BIT_2 0B11111011
#define BIT_3 0B11110111
#define BIT_4 0B11101111
#define BIT_5 0B11011111
#define BIT_6 0B10111111
#define BIT_7 0B01111111

bool isSwitchOn_0(void){
	uint8_t sw = sx1509_in | BIT_0;
	if(sw == BIT_0) return true;
	return false;
}

bool isSwitchOn_1(void){
	uint8_t sw = sx1509_in | BIT_1;
	if(sw == BIT_1) return true;
	return false;
}

bool isSwitchOn_2(void){
	uint8_t sw = sx1509_in | BIT_2;
	if(sw == BIT_2) return true;
	return false;
}

bool isSwitchOn_3(void){
	uint8_t sw = sx1509_in | BIT_3;
	if(sw == BIT_3) return true;
	return false;
}

bool isSwitchOn_4(void){
	uint8_t sw = sx1509_in | BIT_4;
	if(sw == BIT_4) return true;
	return false;
}

bool isSwitchOn_5(void){
	uint8_t sw = sx1509_in | BIT_5;
	if(sw == BIT_5) return true;
	return false;
}

bool isSwitchOn_6(void){
	uint8_t sw = sx1509_in | BIT_6;
	if(sw == BIT_6) return true;
	return false;
}

bool isSwitchOn_7(void){
	uint8_t sw = sx1509_in | BIT_7;
	if(sw == BIT_7) return true;
	return false;
}

void readIn(void){
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

void writeOut(uint8_t bitNum, bool output){
	uint8_t regB = readOut();
	// uint8_t regB = 0;
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

void testI2c(void){
	static bool toggle = true;
	static uint16_t count = 0;
	uint8_t buf[2];
	int err;
	switch(count%16){
		case 0: writeOut(count%8, true); break;
		case 1: writeOut(count%8, true); break;
		case 2: writeOut(count%8, true); break;
		case 3: writeOut(count%8, true); break;
		case 4: writeOut(count%8, true); break;
		case 5: writeOut(count%8, true); break;
		case 6: writeOut(count%8, true); break;
		case 7: writeOut(count%8, true); break;
		case 8: writeOut(count%8, false); break;
		case 9: writeOut(count%8, false); break;
		case 10: writeOut(count%8, false); break;
		case 11: writeOut(count%8, false); break;
		case 12: writeOut(count%8, false); break;
		case 13: writeOut(count%8, false); break;
		case 14: writeOut(count%8, false); break;
		case 15: writeOut(count%8, false); break;
	}
	count++;
	toggle = !toggle;
	printk("12345----------------\r\n");
	readIn();
	if(isSwitchOn_0()){
		printk("Switch1_On\r\n");
	}
	else{
		printk("Switch1_Off\r\n");
	}
}