#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>

#define AHT20   0x70

// #define I2C_NODE    DT_NODELABEL(arduino_i2c)

// static const struct device *i2c0_dev = DEVICE_DT_GET(I2C_NODE);

// static uint8_t aht20_data[2] = {};

int main(void)
{
    int err = 0;
    // if(!device_is_ready(i2c0_dev)){
    //     printf("Error i2c0_dev not ready\r\n");
    //     err = 0;
    // }

    while (1)
    {
        static uint32_t mainLoopCount = 0;
        printf("mainLoop: %d\r\n", mainLoopCount++);
        k_msleep(1000);
        /* code */
    }

    return 0;
}
