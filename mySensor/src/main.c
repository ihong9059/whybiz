#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/services/bas.h>

#include <zephyr/sys/printk.h>
// #include <zephyr/types.h>
// #include <stddef.h>
// #include <string.h>
// #include <errno.h>
// #include <zephyr/sys/byteorder.h>
// #include <zephyr/kernel.h>
// #include <zephyr/drivers/gpio.h>
// #include <soc.h>

// #include <zephyr/bluetooth/hci.h>
// #include <zephyr/bluetooth/conn.h>
// #include <zephyr/bluetooth/uuid.h>
// #include <zephyr/bluetooth/gatt.h>


LOG_MODULE_REGISTER(ble_test);

volatile bool ble_ready = false;
uint8_t battery_level = 100;

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_UUID16_ALL, BT_UUID_16_ENCODE(BT_UUID_BAS_VAL)),
};

void bt_ready(int err){
    if(err){
            LOG_ERR("bt enable return %d", err);
    }
    printk("bt_ready\n");
    ble_ready = true;
}

int init_ble(void){
    LOG_INF("Init BLE");
    int err;

    // bt_conn_cb_register(&conn_callback);

    err = bt_enable(bt_ready);
    if (err) {
            printk("Bluetooth init failed (err %d)\n", err);
            return 0;
    }
    return 0;
}

int main(void)
{
    init_ble();
    while (!ble_ready)
    {
        LOG_INF("BLE stack not ready yet");
        k_msleep(100);
    }
    LOG_INF("BLE stack ready");

    int err;
    err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ARRAY_SIZE(ad), NULL, 0);
	if (err) {
		printk("Advertising failed to start (err %d)\n", err);
		return 0;
	}
    while (true)
    {
        k_msleep(2000);
        if(battery_level < 25){
            battery_level = 100;
        }
        else{
            battery_level--;
        }
        bt_bas_set_battery_level(battery_level);
        /* code */
    }
    

    printk("Hello World! %s\n", CONFIG_BOARD);

    return 0;
}
