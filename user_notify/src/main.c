#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/services/bas.h>

#include <zephyr/sys/printk.h>

LOG_MODULE_REGISTER(uttec);

volatile bool ble_ready = false;
uint16_t battery_level = 75;
int16_t temperature = 3232;

char userData[50] = {};
int16_t userDataLength = 0;

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_UUID16_ALL, BT_UUID_16_ENCODE(BT_UUID_BAS_VAL), BT_UUID_16_ENCODE(BT_UUID_ESS_VAL)),
};

ssize_t readTemperature(struct bt_conn *conn,
			       const struct bt_gatt_attr *attr, void *buf,
			       uint16_t len, uint16_t offset)
{
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &temperature,
				 sizeof(temperature));
}

ssize_t readData(struct bt_conn *conn,
			       const struct bt_gatt_attr *attr, void *buf,
			       uint16_t len, uint16_t offset)
{

	return bt_gatt_attr_read(conn, attr, buf, len, offset, userData, userDataLength);
	// return bt_gatt_attr_read(conn, attr, buf, len, offset, userData, userDataLength);
	// return bt_gatt_attr_read(conn, attr, buf, len, offset, userData, userDataLength);
	// return bt_gatt_attr_read(conn, attr, buf, len, offset, userData,
	// 			 sizeof(myData));
}

static ssize_t writeData(struct bt_conn *conn,
             const struct bt_gatt_attr *attr,
             const void *buf,
             uint16_t len, uint16_t offset, uint8_t flags)
{

    printk("---------------------- start writeData ------------");
    LOG_INF("Attribute write, handle: %u, conn: %p", attr->handle,
        (void *)conn);

    LOG_INF("buf = %s", buf);
    LOG_INF("len = %d", len);
    LOG_INF("offset = %d", offset);
    LOG_INF("flags = %d", flags);
    LOG_INF("------------------- end writeData ---------->");
    return len;
}

static void custom_ccc_cfg_changed(const struct bt_gatt_attr *attr,
				       uint16_t value){
	bool notif_enabled = (value == BT_GATT_CCC_NOTIFY);

	LOG_INF("Custom Notifications %s", notif_enabled ? "enabled" : "disabled");
}

BT_GATT_SERVICE_DEFINE(whybiz_srv,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_UDS),
    BT_GATT_CHARACTERISTIC(BT_UUID_GATT_USRIDX, BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ, readData, NULL, NULL),
	BT_GATT_CCC(custom_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),    
    BT_GATT_CHARACTERISTIC(BT_UUID_GATT_USRIDX, BT_GATT_CHRC_WRITE, BT_GATT_PERM_WRITE, NULL, writeData, NULL),
);
	// BT_GATT_CCC(custom_ccc_cfg_changed,
	// 	    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),    

int bt_userData_set(void)
{
	int rc;

	// temperature = level;

	rc = bt_gatt_notify(NULL, &whybiz_srv.attrs[1], userData, userDataLength);

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
        static uint32_t count = 0;
        k_msleep(2000);
        if(battery_level < 25){
            battery_level = 100;
        }
        else{
            battery_level--;
        }
        bt_bas_set_battery_level(battery_level);
        LOG_INF("whybiz count: %d, bat: %d, temp: %d", count++, battery_level, temperature);
        userDataLength = sprintf(userData, "send: %d", count);
        bt_userData_set();
    }
    

    printk("Hello World! %s\n", CONFIG_BOARD);

    return 0;
}
