#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/services/bas.h>

#include <zephyr/sys/printk.h>

LOG_MODULE_REGISTER(ble_test);


// 058596d8-c8df-4ad6-842a-4d73d45b4ec6
// UUID of the custom Service
#define BT_UUID_MY_CUSTOM_SERV_VAL BT_UUID_128_ENCODE(0x058596d8, 0xc8df, 0x4ad6, 0x842a, 0x4d73d45b4ec6)
#define BT_UUID_MY_CUSTOM_SERVICE BT_UUID_DECLARE_128(BT_UUID_MY_CUSTOM_SERV_VAL)

// UUID of the custom Characteristic
#define BT_UUID_MY_TEMP_CHRC_VAL BT_UUID_128_ENCODE(0x058596d8, 0xc8df, 0x4ad6, 0x842a, 0x4d73d45b4ec7)
#define BT_UUID_MY_TEMP_CHRC BT_UUID_DECLARE_128(BT_UUID_MY_TEMP_CHRC_VAL)



volatile bool ble_ready = false;

int16_t temperature = 2412;
uint16_t humidity = 5012;
uint32_t pressure = 9999;
uint8_t battery_level = 50;

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_MY_CUSTOM_SERV_VAL),
};

ssize_t read_temperature(struct bt_conn *conn,
			       const struct bt_gatt_attr *attr, void *buf,
			       uint16_t len, uint16_t offset)
{
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &temperature,
				 sizeof(temperature));
}
ssize_t read_humidity(struct bt_conn *conn,
			       const struct bt_gatt_attr *attr, void *buf,
			       uint16_t len, uint16_t offset)
{
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &humidity,
				 sizeof(humidity));
}
ssize_t read_pressure(struct bt_conn *conn,
			       const struct bt_gatt_attr *attr, void *buf,
			       uint16_t len, uint16_t offset)
{
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &pressure,
				 sizeof(pressure));
}

static void custom_ccc_cfg_changed(const struct bt_gatt_attr *attr,
				       uint16_t value){
	bool notif_enabled = (value == BT_GATT_CCC_NOTIFY);

	LOG_INF("Custom Notifications %s", notif_enabled ? "enabled" : "disabled");
}

BT_GATT_SERVICE_DEFINE(custom_svc,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_MY_CUSTOM_SERVICE),
	BT_GATT_CHARACTERISTIC(BT_UUID_MY_TEMP_CHRC, BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ, read_temperature, NULL,NULL),
	BT_GATT_CCC(custom_ccc_cfg_changed,
		    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);


int bt_custom_set_level(int16_t level)
{
	int rc;

	temperature = level;

	rc = bt_gatt_notify(NULL, &custom_svc.attrs[1], &level, sizeof(level));

	return rc == -ENOTCONN ? 0 : rc;
}


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

    printf("---------> now start sensor ble. 2023.11.17. 14:05 \r\n");

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
        static uint16_t count = 0;
        k_msleep(1000);
        if(battery_level < 25){
            battery_level = 100;
            temperature = 3000;
        }
        else{
            battery_level--;
            temperature--;
        }

        bt_bas_set_battery_level(battery_level);
        bt_custom_set_level(temperature);
        printk("count: %d, bat: %d, temp: %d\r\n", count++, battery_level, temperature);
        /* code */
    }
    

    printk("Hello World! mySensor BLE: %s\n", CONFIG_BOARD);

    return 0;
}
