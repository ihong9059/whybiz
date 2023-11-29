#include <zephyr/kernel.h>
#include <zephyr/sys/reboot.h>
#include <zephyr/device.h>
#include <string.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>

static struct nvs_fs fs;

#define NVS_PARTITION		storage_partition
#define NVS_PARTITION_DEVICE	FIXED_PARTITION_DEVICE(NVS_PARTITION)
#define NVS_PARTITION_OFFSET	FIXED_PARTITION_OFFSET(NVS_PARTITION)

#define MAX_REBOOT 400

#define ADDRESS_ID 1
#define KEY_ID 2
#define RBT_CNT_ID 3
#define STRING_ID 4
#define LONG_ID 5
#define UTTEC_ID 6 


#define SLEEP_TIME      100

#include "nvm.h"

uttec_t myUttec = {0, };

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
		return 0;
	}
	fs.offset = NVS_PARTITION_OFFSET;
	rc = flash_get_page_info_by_offs(fs.flash_device, fs.offset, &info);
	if (rc) {
		printf("Unable to get page info\n");
		return 0;
	}
	fs.sector_size = info.size;
	fs.sector_count = 3U;

	rc = nvs_mount(&fs);
	if (rc) {
		printf("Flash Init failed\n");
		return 0;
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


void nvmTest(void){
	int rc = 0, cnt = 0, cnt_his = 0;
	char buf[16];
	uint8_t key[8], longarray[128];
	uint32_t reboot_counter = 0U, reboot_counter_his;
	struct flash_pages_info info;

	/* define the nvs file system by settings with:
	 *	sector_size equal to the pagesize,
	 *	3 sectors
	 *	starting at NVS_PARTITION_OFFSET
	 */
	// fs.flash_device = NVS_PARTITION_DEVICE;
	// if (!device_is_ready(fs.flash_device)) {
	// 	printf("Flash device %s is not ready\n", fs.flash_device->name);
	// 	return 0;
	// }
	// fs.offset = NVS_PARTITION_OFFSET;
	// rc = flash_get_page_info_by_offs(fs.flash_device, fs.offset, &info);
	// if (rc) {
	// 	printf("Unable to get page info\n");
	// 	return 0;
	// }
	// fs.sector_size = info.size;
	// fs.sector_count = 3U;

	// rc = nvs_mount(&fs);
	// if (rc) {
	// 	printf("Flash Init failed\n");
	// 	return 0;
	// }

	/* ADDRESS_ID is used to store an address, lets see if we can
	 * read it from flash, since we don't know the size read the
	 * maximum possible
	 */
	rc = nvs_read(&fs, ADDRESS_ID, &buf, sizeof(buf));
	if (rc > 0) { /* item was found, show it */
		printf("Id: %d, Address: %s\n", ADDRESS_ID, buf);
	} else   {/* item was not found, add it */
		strcpy(buf, "192.168.1.1");
		printf("No address found, adding %s at id %d\n", buf,
		       ADDRESS_ID);
		(void)nvs_write(&fs, ADDRESS_ID, &buf, strlen(buf)+1);
	}
	/* KEY_ID is used to store a key, lets see if we can read it from flash
	 */
	rc = nvs_read(&fs, KEY_ID, &key, sizeof(key));
	if (rc > 0) { /* item was found, show it */
		printf("Id: %d, Key: ", KEY_ID);
		for (int n = 0; n < 8; n++) {
			printf("%x ", key[n]);
		}
		printf("\n");
	} else   {/* item was not found, add it */
		printf("No key found, adding it at id %d\n", KEY_ID);
		key[0] = 0xFF;
		key[1] = 0xFE;
		key[2] = 0xFD;
		key[3] = 0xFC;
		key[4] = 0xFB;
		key[5] = 0xFA;
		key[6] = 0xF9;
		key[7] = 0xF8;
		(void)nvs_write(&fs, KEY_ID, &key, sizeof(key));
	}
	/* RBT_CNT_ID is used to store the reboot counter, lets see
	 * if we can read it from flash
	 */
	rc = nvs_read(&fs, RBT_CNT_ID, &reboot_counter, sizeof(reboot_counter));
	if (rc > 0) { /* item was found, show it */
		printf("Id: %d, Reboot_counter: %d\n",
			RBT_CNT_ID, reboot_counter);
	} else   {/* item was not found, add it */
		printf("No Reboot counter found, adding it at id %d\n",
		       RBT_CNT_ID);
		(void)nvs_write(&fs, RBT_CNT_ID, &reboot_counter,
			  sizeof(reboot_counter));
	}
	/* STRING_ID is used to store data that will be deleted,lets see
	 * if we can read it from flash, since we don't know the size read the
	 * maximum possible
	 */
	rc = nvs_read(&fs, STRING_ID, &buf, sizeof(buf));
	if (rc > 0) {
		/* item was found, show it */
		printf("Id: %d, Data: %s\n",
			STRING_ID, buf);
		/* remove the item if reboot_counter = 10 */
		if (reboot_counter == 10U) {
			(void)nvs_delete(&fs, STRING_ID);
		}
	} else   {
		/* entry was not found, add it if reboot_counter = 0*/
		if (reboot_counter == 0U) {
			printf("Id: %d not found, adding it\n",
			STRING_ID);
			strcpy(buf, "DATA");
			(void)nvs_write(&fs, STRING_ID, &buf, strlen(buf) + 1);
		}
	}

	/* LONG_ID is used to store a larger dataset ,lets see if we can read
	 * it from flash
	 */
	rc = nvs_read(&fs, LONG_ID, &longarray, sizeof(longarray));
	if (rc > 0) {
		/* item was found, show it */
		printf("Id: %d, Longarray: ", LONG_ID);
		for (int n = 0; n < sizeof(longarray); n++) {
			printf("%x ", longarray[n]);
		}
		printf("\n");
	} else   {
		/* entry was not found, add it if reboot_counter = 0*/
		if (reboot_counter == 0U) {
			printf("Longarray not found, adding it as id %d\n",
			       LONG_ID);
			for (int n = 0; n < sizeof(longarray); n++) {
				longarray[n] = n;
			}
			(void)nvs_write(
				&fs, LONG_ID, &longarray, sizeof(longarray));
		}
	}
    printf("end of nvm ----------------\r\n");

	// cnt = 5;
	// while (1) {
	// 	k_msleep(SLEEP_TIME);
	// 	if (reboot_counter < MAX_REBOOT) {
	// 		if (cnt == 5) {
	// 			/* print some history information about
	// 			 * the reboot counter
	// 			 * Check the counter history in flash
	// 			 */
	// 			printf("Reboot counter history: ");
	// 			while (1) {
	// 				rc = nvs_read_hist(
	// 					&fs, RBT_CNT_ID,
	// 					&reboot_counter_his,
	// 					sizeof(reboot_counter_his),
	// 					cnt_his);
	// 				if (rc < 0) {
	// 					break;
	// 				}
	// 				printf("...%d", reboot_counter_his);
	// 				cnt_his++;
	// 			}
	// 			if (cnt_his == 0) {
	// 				printf("\n Error, no Reboot counter");
	// 			} else {
	// 				printf("\nOldest reboot counter: %d",
	// 				       reboot_counter_his);
	// 			}
	// 			printf("\nRebooting in ");
	// 		}
	// 		printf("...%d", cnt);
	// 		cnt--;
	// 		if (cnt == 0) {
	// 			printf("\n");
	// 			reboot_counter++;
	// 			(void)nvs_write(
	// 				&fs, RBT_CNT_ID, &reboot_counter,
	// 				sizeof(reboot_counter));
	// 			if (reboot_counter == MAX_REBOOT) {
	// 				printf("Doing last reboot...\n");
	// 			}
	// 			sys_reboot(0);
	// 		}
	// 	} else {
	// 		printf("Reboot counter reached max value.\n");
	// 		printf("Reset to 0 and exit test.\n");
	// 		reboot_counter = 0U;
	// 		(void)nvs_write(&fs, RBT_CNT_ID, &reboot_counter,
	// 		  sizeof(reboot_counter));
	// 		break;
	// 	}
	// }

}