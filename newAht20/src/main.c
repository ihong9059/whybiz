#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/sensor.h>

const struct device *aht20_dev = DEVICE_DT_GET_ANY(asair_aht20__decoder_api);
static struct sensor_value temp;

int main(void)
{
    printk("now start aht20 i2c\r\n");
    if(!(device_is_ready(aht20_dev))){
        printk("aht20_dev is not ready\r\n");
        return;
    }

    while(1){
        static uint32_t mainCount = 0;
        printf("main: %d\r\n", mainCount++);
        k_msleep(1000);
    }
    return 0;
}
