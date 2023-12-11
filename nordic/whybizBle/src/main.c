#include "uart_async_adapter.h"

#include <zephyr/types.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/usb/usb_device.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <soc.h>

#include <dk_buttons_and_leds.h>

#include <stdio.h>

#include <zephyr/logging/log.h>

#include "uttec.h"
#include "myBle.h"
#include "sx1509.h"
#include "adc.h"

#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>

#define RUN_LED_BLINK_INTERVAL 500
#define RUN_STATUS_LED DK_LED1

uint8_t bleTimeout = 0;
bool connectBleFlag = false;

bool* getConnectBleFlag(void){
	return &connectBleFlag;
}
// // GEN_ABSOLUTE_SYM_KCONFIG(CONFIG_BT_NUS_UART_BUFFER_SIZE, 40);
// GEN_ABSOLUTE_SYM_KCONFIG(CONFIG_BT_NUS_UART_BUFFER_SIZE, 200);
#include "nvm.h"

int main(void)
{
	int blink_status = 0;
	printf("nordic for whybiz.\r\n");
	
	initBle();
	initSx1509();
	initAdc();
	initPort();
	initNvm();

	for (;;) {
		static uint32_t count = 0;
		static bool toggle = false;

		dk_set_led(RUN_STATUS_LED, (++blink_status) % 2);
		k_sleep(K_MSEC(RUN_LED_BLINK_INTERVAL));
		// printf("----------> mainCount: %d\r\n", mainCount++);
		// testRelay();//relay, switch test
		if(toggle){
			procTxBle();// switch and adc tx test
		}
		else
			sendAdcValue();
		toggle = !toggle;
		count++;
		// testJsonOut();
		jsonFrame_t* pFrame = getJsonFrame();
		if(pFrame->flag){
			uttecJsonTest(pFrame->frame, pFrame->end+1);// json test
			dispJsonFrame();
			k_sleep(K_MSEC(20));
			clearJsonData();
		}
		readAdcValue();
		printk("relay status: %x, sw status: %x\r\n", 
			readSxRelay(), readSxSw());
		testNvm();	
	}
}







