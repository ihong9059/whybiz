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

#define UTTEC_ID 1 

static uttec_t myUttec = {0, };

void initNvm(void){
    int rc = 0;
	struct flash_pages_info info;

	/* define the nvs file system by settings with:
	 *	sector_size equal to the pagesize,
	 *	3 sectors
	 *	starting at NVS_PARTITION_OFFSET
	 */
	fs.flash_device = NVS_PARTITION_DEVICE;
	if (!device_is_ready(fs.flash_device)) {
		printf("Flash device %s is not ready\n", fs.flash_device->name);
	}
	fs.offset = NVS_PARTITION_OFFSET;
	rc = flash_get_page_info_by_offs(fs.flash_device, fs.offset, &info);
	if (rc) {
		printf("Unable to get page info\n");
	}
	fs.sector_size = info.size;
	fs.sector_count = 3U;

	rc = nvs_mount(&fs);
	if (rc) {
		printf("Flash Init failed\n");
	}

	rc = nvs_read(&fs, UTTEC_ID, &myUttec, sizeof(myUttec));
	if (rc > 0) { /* item was found, show it */
		printf("version: %d, temp0: %d\n", myUttec.version, myUttec.temp0);
        printf("data: %s\r\n", myUttec.data);
	} else   {/* item was not found, add it */
        myUttec.version = 1234;
        myUttec.temp0 = 4321;
        for(int i = 0; i < 10; i++){
            myUttec.data[i] = 'a' + i;
        }
		(void)nvs_write(&fs, UTTEC_ID, &myUttec, sizeof(myUttec)+1);
	}
}

void checkFactor(void){
    int rc = 0;

	rc = nvs_read(&fs, UTTEC_ID, &myUttec, sizeof(myUttec));
	if (rc > 0) { /* item was found, show it */
		printf("version: %d, temp0: %d\n", myUttec.version, myUttec.temp0);
        printf("data: %s\r\n", myUttec.data);
	} else   {/* item was not found, add it */
        myUttec.version = 1;
        myUttec.temp0 = 1;
        for(int i = 0; i < 10; i++){
            myUttec.data[i] = 'a' + i;
        }
		(void)nvs_write(&fs, UTTEC_ID, &myUttec, sizeof(myUttec)+1);
	}
}

uttec_t* getFactor(void){
	return &myUttec;
}

void writeFactor(void){
	(void)nvs_write(&fs, UTTEC_ID, &myUttec, sizeof(myUttec)+1);
}

void nvmTest(void){
	uttec_t* pFactor = getFactor();
	printf("version: %d\r\n", pFactor->version);
	pFactor->version++;
	writeFactor();
}