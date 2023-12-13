#include <zephyr/kernel.h>
#include <zephyr/sys/reboot.h>
#include <zephyr/device.h>
#include <string.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>

#include "nvm.h"

static struct nvs_fs fs;

#define NVS_PARTITION		storage_partition
#define NVS_PARTITION_DEVICE	FIXED_PARTITION_DEVICE(NVS_PARTITION)
#define NVS_PARTITION_OFFSET	FIXED_PARTITION_OFFSET(NVS_PARTITION)

#define UTTEC_ID 2 

// static uttec_t myUttec = {0, };
#include "uttec.h"

void dispFactors(void){
	whybiz_t* pWhybiz = getWhybizFactor();
	printf("============= whybiz factors ==========\r\n");
	printf("version: %d\r\n", pWhybiz->version);
	printf("node: %d\r\n", pWhybiz->node);
	printf("uart: %d\r\n", pWhybiz->channel);
	printf("ble: %d\r\n", pWhybiz->ble);
	printf("adc1: %d\r\n", pWhybiz->adc1);
	printf("adc2: %d\r\n", pWhybiz->adc2);
	printf("sw: %d\r\n", pWhybiz->sw);
	printf("relay: %d\r\n", pWhybiz->relay);
	printf("lora_ch: %d\r\n", pWhybiz->lora_ch);
	printf("power: %d\r\n", pWhybiz->power);
	printf("rssi: %d\r\n", pWhybiz->rssi);
	printf("============= whybiz factors ==========\r\n");
	k_sleep(K_MSEC(1000));
}

void initNvm(void){
    int rc = 0;
	struct flash_pages_info info;
	whybiz_t* pWhybiz = getWhybizFactor();

	fs.flash_device = NVS_PARTITION_DEVICE;
	if (!device_is_ready(fs.flash_device)) {
		printk("Flash device %s is not ready\n", fs.flash_device->name);
	}
	fs.offset = NVS_PARTITION_OFFSET;
	rc = flash_get_page_info_by_offs(fs.flash_device, fs.offset, &info);
	if (rc) {
		printk("Unable to get page info\n");
	}
	fs.sector_size = info.size;
	fs.sector_count = 3U;

	rc = nvs_mount(&fs);
	if (rc) {
		printk("Flash Init failed\n");
	}                    

	rc = nvs_read(&fs, UTTEC_ID, pWhybiz, sizeof(whybiz_t));

	if(pWhybiz->flashFlag == FLASH_FLAG){
		printf("Already written to flash factors\r\n");
		printf("version: %d\r\n", pWhybiz->version);			
	}
	else{
		printf("New Program. now set first factors\r\n");
		pWhybiz->flashFlag = FLASH_FLAG;
		pWhybiz->version = 1;
		pWhybiz->node = 111;
		pWhybiz->channel = 0;
		pWhybiz->ble = 123;
		pWhybiz->adc1 = 0;
		pWhybiz->adc2 = 0;
		pWhybiz->sw = 0;
		pWhybiz->relay = 0xff;
		pWhybiz->lora_ch = 20;
		pWhybiz->power = 16;
		pWhybiz->rssi = 0;
		(void)nvs_write(&fs, UTTEC_ID, pWhybiz, sizeof(whybiz_t)+1);
	}
	dispFactors();
}

void testNvm(void){
    int rc = 0;
	whybiz_t* pWhybiz = getWhybizFactor();

	rc = nvs_read(&fs, UTTEC_ID, pWhybiz, sizeof(whybiz_t));
	if (rc > 0) { /* item was found, show it */
		printk("version: %d, node: %d\n", pWhybiz->version, pWhybiz->node);
		pWhybiz->version++;
		(void)nvs_write(&fs, UTTEC_ID, pWhybiz, sizeof(whybiz_t)+1);
	} else   {/* item was not found, add it */
		printk("flash write read error\r\n");
	}
}

void saveFactorToFlash(void){
	whybiz_t* pWhybiz = getWhybizFactor();
	(void)nvs_write(&fs, UTTEC_ID, pWhybiz, sizeof(whybiz_t)+1);
}
