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

void initNvm(void){
    int rc = 0;
	struct flash_pages_info info;
	whybiz_t* pWhybiz = getWhybizFactor();

	/* define the nvs file system by settings with:
	 *	sector_size equal to the pagesize,
	 *	3 sectors
	 *	starting at NVS_PARTITION_OFFSET
	 */
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
		pWhybiz->version = 123;
		pWhybiz->node = 111;
		pWhybiz->channel = 2;
		pWhybiz->ble = 44;
		pWhybiz->adc1 = 55;
		pWhybiz->adc2 = 66;
		pWhybiz->sw = 77;
		pWhybiz->relay = 88;
		pWhybiz->lora_ch = 20;
		pWhybiz->power = 12;
		pWhybiz->relay = 123;
		pWhybiz->rssi = 123;
	}
	else{
		printf("New Program. now set first factors\r\n");
		pWhybiz->flashFlag = FLASH_FLAG;
		pWhybiz->version = 123;
		pWhybiz->node = 111;
		pWhybiz->channel = 2;
		pWhybiz->ble = 44;
		pWhybiz->adc1 = 55;
		pWhybiz->adc2 = 66;
		pWhybiz->sw = 77;
		pWhybiz->relay = 88;
		pWhybiz->lora_ch = 20;
		pWhybiz->power = 12;
		pWhybiz->relay = 123;
		pWhybiz->rssi = 123;
		(void)nvs_write(&fs, UTTEC_ID, pWhybiz, sizeof(whybiz_t)+1);
	}
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

void writeFactor(void){
	whybiz_t* pWhybiz = getWhybizFactor();
	(void)nvs_write(&fs, UTTEC_ID, pWhybiz, sizeof(whybiz_t)+1);
}
