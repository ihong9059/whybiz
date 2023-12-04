/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/** @file
 *  @brief Nordic UART Bridge Service (NUS) sample
#define BT_UUID_NUS_SERVICE   BT_UUID_DECLARE_128(BT_UUID_NUS_VAL)
#define BT_UUID_NUS_RX        BT_UUID_DECLARE_128(BT_UUID_NUS_RX_VAL)
#define BT_UUID_NUS_TX        BT_UUID_DECLARE_128(BT_UUID_NUS_TX_VAL)

 * 
 */
// test0
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

// test1
void testSendBle(void);

int main(void)
{
	int blink_status = 0;
// 	BT_UUID_128_ENCODE(0x6e400001, 0xb5a3, 0xf393, 0xe0a9, 0xe50e24dcca9e)
// 	Service UUID: 			6e400001-b5a3-f393-e0a9-e50e24dcca9e
// 	Tx Characteristic UUID: 6e400003-b5a3-f393-e0a9-e50e24dcca9e
// 	Rx Characteristic UUID: 6e400002-b5a3-f393-e0a9-e50e24dcca9e
// test2
	// uttecTest();
	initBle();
	// initSx1509();

	for (;;) {
		dk_set_led(RUN_STATUS_LED, (++blink_status) % 2);
		k_sleep(K_MSEC(RUN_LED_BLINK_INTERVAL));
		callPrint();
		uttecTest();
		// testI2c();
		procTxBle();
	}
}


