#include "uart_async_adapter.h"

#include <zephyr/types.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/usb/usb_device.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <soc.h>

// #include <zephyr/bluetooth/bluetooth.h>
// #include <zephyr/bluetooth/uuid.h>
// #include <zephyr/bluetooth/gatt.h>
// #include <zephyr/bluetooth/hci.h>

// #include <bluetooth/services/nus.h>

#include <dk_buttons_and_leds.h>

// #include <zephyr/settings/settings.h>

#include <stdio.h>

#include <zephyr/logging/log.h>

#include "uttec.h"
#include "myBle.h"
#include "sx1509.h"

#define RUN_LED_BLINK_INTERVAL 500
#define RUN_STATUS_LED DK_LED1

int main(void)
{
	int blink_status = 0;
	uint32_t mainCount = 0;
	// uttecTest();
	initBle();
	initSx1509();

	for (;;) {
		dk_set_led(RUN_STATUS_LED, (++blink_status) % 2);
		k_sleep(K_MSEC(RUN_LED_BLINK_INTERVAL));
		printk("----------> mainCount: %d\r\n", mainCount++);
		callPrint();
		uttecTest();
		testI2c();
		procTxBle();
	}
}


