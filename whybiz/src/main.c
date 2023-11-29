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

#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/adc.h>


#define RUN_LED_BLINK_INTERVAL 1000
#define RUN_STATUS_LED DK_LED1

#define RED_LED_NODE DT_NODELABEL(led1)
static const struct gpio_dt_spec redLed = GPIO_DT_SPEC_GET(RED_LED_NODE, gpios);

#define RS485_EN_NODE DT_NODELABEL(led3)
static const struct gpio_dt_spec rs485En = GPIO_DT_SPEC_GET(RS485_EN_NODE, gpios);

#define BLUE_LED_NODE DT_NODELABEL(led4)
static const struct gpio_dt_spec greenLed = GPIO_DT_SPEC_GET(BLUE_LED_NODE, gpios);

#define SELECT1_NODE DT_NODELABEL(select1)
static const struct gpio_dt_spec select1 = GPIO_DT_SPEC_GET(SELECT1_NODE, gpios);

#define SELECT2_NODE DT_NODELABEL(select2)
static const struct gpio_dt_spec select2 = GPIO_DT_SPEC_GET(SELECT2_NODE, gpios);

#define PWR_CTL_NODE DT_NODELABEL(pwrctl)
static const struct gpio_dt_spec pwrCtl = GPIO_DT_SPEC_GET(PWR_CTL_NODE, gpios);

#define LORA_RESET_NODE DT_NODELABEL(lorarst)
static const struct gpio_dt_spec loraRst = GPIO_DT_SPEC_GET(LORA_RESET_NODE, gpios);

#define SX1509_RESET_NODE DT_NODELABEL(sx1509rst)
static const struct gpio_dt_spec sx1509Rst = GPIO_DT_SPEC_GET(SX1509_RESET_NODE, gpios);

#define ADC DT_NODELABEL(adc)
static const struct device *adc_dev = DEVICE_DT_GET(ADC);

#define ADC_RESOLUTION 	10
#define ADC0_CHANNEL	0
#define ADC1_CHANNEL	2
#define ADC0_PORT		SAADC_CH_PSELN_PSELN_AnalogInput0
#define ADC1_PORT		SAADC_CH_PSELN_PSELN_AnalogInput2
#define ADC_REFERENCE	ADC_REF_INTERNAL
#define ADC_GAIN		ADC_GAIN_1_6 

struct adc_channel_cfg ch0_cfg = {
	.gain = ADC_GAIN,
	.reference = ADC_REFERENCE,
	.acquisition_time = ADC_ACQ_TIME_DEFAULT,
	.channel_id = ADC0_CHANNEL,
#ifdef CONFIG_ADC_NRFX_SAADC
	.input_positive = ADC0_PORT
#endif 	
}; 
struct adc_channel_cfg ch1_cfg = {
	.gain = ADC_GAIN,
	.reference = ADC_REFERENCE,
	.acquisition_time = ADC_ACQ_TIME_DEFAULT,
	.channel_id = ADC1_CHANNEL,
#ifdef CONFIG_ADC_NRFX_SAADC
	.input_positive = ADC1_PORT
#endif 	
}; 

int16_t sample_buffer[2];
struct adc_sequence sequence = {
	.channels = BIT(ADC0_CHANNEL) | BIT(ADC1_CHANNEL),
	// .channels = BIT(ADC0_CHANNEL),
	.buffer = sample_buffer,
	.buffer_size = sizeof(sample_buffer),
	.resolution = ADC_RESOLUTION
};

#include "nvm.h"

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
	if(!device_is_ready(adc_dev)){
		printf("adc_dev is not ready.");
	}
	int err = adc_channel_setup(adc_dev, &ch0_cfg);
	err = adc_channel_setup(adc_dev, &ch1_cfg);
	
	initNvm();
	nvmTest();

	for (;;) {
		static bool toggle = false;
		dk_set_led(RUN_STATUS_LED, (++blink_status) % 2);
		k_sleep(K_MSEC(RUN_LED_BLINK_INTERVAL));
		callPrint();
		uttecTest();
		printf("pin num: %d\r\n", greenLed.pin);
		if(toggle){
			gpio_pin_set_dt(&greenLed, 0);
			gpio_pin_set_dt(&redLed, 0);
			gpio_pin_set_dt(&select1, 0);
			gpio_pin_set_dt(&select2, 0);

			gpio_pin_set_dt(&rs485En, 0);
			gpio_pin_set_dt(&pwrCtl, 0);
			gpio_pin_set_dt(&loraRst, 0);
			gpio_pin_set_dt(&sx1509Rst, 0);
		}
		else{
			gpio_pin_set_dt(&greenLed, 1);
			gpio_pin_set_dt(&redLed, 1);
			gpio_pin_set_dt(&select1, 1);
			gpio_pin_set_dt(&select2, 1);

			gpio_pin_set_dt(&rs485En, 1);
			gpio_pin_set_dt(&pwrCtl, 1);
			gpio_pin_set_dt(&loraRst, 1);
			gpio_pin_set_dt(&sx1509Rst, 1);
		}
		toggle = !toggle;
		err = adc_read(adc_dev, &sequence);
		if(err != 0){
			printf("ADC reading failed with error %d\r\n", err);
			return 0;
		}
		int32_t mv_value = sample_buffer[0];
		int32_t adc_vref = adc_ref_internal(adc_dev);
		adc_raw_to_millivolts(adc_vref, ADC_GAIN, ADC_RESOLUTION, &mv_value);
		printf("ADC-spanning: %d mV\r\n", mv_value);

		int32_t mv_value1 = sample_buffer[1];
		adc_raw_to_millivolts(adc_vref, ADC_GAIN, ADC_RESOLUTION, &mv_value1);
		printf("ADC-spanning: %d mV\r\n", mv_value1);
		// testI2c();
	}
}


