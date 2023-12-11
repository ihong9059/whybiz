#include "uart_async_adapter.h"

#include <zephyr/types.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/uart.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <soc.h>

#include <stdio.h>
#include <zephyr/logging/log.h>

#include "uttec.h"
#include "myBle.h"
#include "sx1509.h"
#include "adc.h"
#include "nvm.h"

#define RUN_LED_BLINK_INTERVAL 500
#define RUN_STATUS_LED DK_LED1

connectFlag_t whybizConnect = {0, };
connectFlag_t* getConnectFlag(void){
	return &whybizConnect;
}

int main(void)
{
	printf("nordic for whybiz.\r\n");
	initBle();
	initSx1509();
	initAdc();
	initPort();
	initNvm();
	// connectFlag_t* pFlags = getConnectFlag();
	whybizConnect.first = true;
    whybiz_t* pFactor = getWhybizFactor();

	for (;;) {
		static uint32_t count = 0;
		static bool toggle = false;

		k_sleep(K_MSEC(RUN_LED_BLINK_INTERVAL));
		// if(toggle){
		// 	procTxBle();// switch and adc tx test
		// }
		// else
		// 	sendAdcValue();
		// testJsonOut();
		jsonFrame_t* pFrame = getJsonFrame();
		if(pFrame->flag){
			uttecJsonTest(pFrame->frame, pFrame->end+1);// json test
			dispJsonFrame();
			k_sleep(K_MSEC(20));
			clearJsonData();
		}
		// readAdcValue();
		// printk("relay status: %x, sw status: %x\r\n", 
		// 	readSxRelay(), readSxSw());

		sendFactorRepeat();	
		printk("main: %d\r\n", count);
		// procSwitchTxBle();
		// procRelayTxBle();
		// procAdcTxBle();
		// testNvm();	
		toggle = !toggle;
		count++;
	}
}







