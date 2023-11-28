#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/devicetree.h> 
#include <zephyr/drivers/i2c.h>


/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   500

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_NODELABEL(led0)
#define TEST_NODE DT_NODELABEL(led1)
#define BUTTON_NODE DT_NODELABEL(button0)

static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec testLed = GPIO_DT_SPEC_GET(TEST_NODE, gpios);

static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(BUTTON_NODE, gpios);

static struct gpio_callback button_cb;

void button_pressed_callback(const struct device *gpiob, struct gpio_callback *cb, gpio_port_pins_t pins){
	gpio_pin_toggle_dt(&testLed);
}

#define AHT20_I2C_ADDRESS 0x38 
#define I2C_NODE DT_NODELABEL(arduino_i2c)
static const struct device *i2c0_dev = DEVICE_DT_GET(I2C_NODE);

#define MEASURE_CMD	0xAC 
#define INIT_CMD	0xBE 
#define SOFT_RESET_CMD	0xBA

uint8_t aht20Data[7] = {INIT_CMD, 8, 0, 0, 0, 0, 0};
// chapter3 11:00
void initAht20(void){
	int err;
	err = i2c_write(i2c0_dev, aht20Data, 3, AHT20_I2C_ADDRESS);
	if(err < 0){
		printk("aht20 write fail------------\r\n");
		return 0;
	}
	printf("end of initAht20\r\n");
	k_msleep(50);
}

uint8_t aht20_crc(uint8_t *data, uint8_t len)
{
    uint8_t i;
    uint8_t byte;
    uint8_t crc = 0xFF;
    
    for (byte = 0; byte < len; byte++)          /* len times */
    {
        crc ^= data[byte];                      /* xor byte */
        for (i = 8; i > 0; --i){                 /* one byte */
            if ((crc & 0x80) != 0){              /* if high*/
                crc = (crc << 1) ^ 0x31;        /* xor 0x31 */
            }
            else{
                crc = crc << 1;                 /* skip */
            }
        }
    }    
    return crc;                                 /* return crc */
}

void readAht20(void){
	int err;
	uint8_t buf[7] = {};
	uint8_t status = 0;

	buf[0] = 0xAC;
	buf[1] = 0x33;
	buf[2] = 0x00;
	err = i2c_write(i2c0_dev, buf, 3, AHT20_I2C_ADDRESS);
	k_msleep(50);
	err = i2c_read(i2c0_dev, buf, 1, AHT20_I2C_ADDRESS);
	if((status & 0x80) == 0){
		err = i2c_read(i2c0_dev, buf, 7, AHT20_I2C_ADDRESS);
		if(aht20_crc(buf, 6) != buf[6]){
			printf("aht20_crd error ---------->\r\n");
		}
		else{
			printf("<------------ ok end of aht20 test\r\n");
			
			uint32_t  humidity_raw = (((uint32_t)buf[1]) << 16) |
							(((uint32_t)buf[2]) << 8) |
							(((uint32_t)buf[3]) << 0);                    /* set the humidity */
			humidity_raw = (humidity_raw) >> 4;                         /* right shift 4 */
			float humidity = (uint8_t)((float)(humidity_raw)
									/ 1048576.0f * 100.0f);               /* convert the humidity */
									
			uint32_t temperature_raw = (((uint32_t)buf[3]) << 16) |
							(((uint32_t)buf[4]) << 8) |
							(((uint32_t)buf[5]) << 0);                 /* set the temperature */
			temperature_raw = (temperature_raw) & 0xFFFFF;              /* cut the temperature part */
			float temperature = (float)(temperature_raw) 
									/ 1048576.0f * 200.0f
									- 50.0f;                             /* right shift 4 */

			printf("temp: %0.2f, humidity: %0.2f\r\n", temperature, humidity);						
		}
	}
	else{
		printf("fail read. not ready -----------> \r\n");
	}
}

int main(void)
{
	uint32_t count = 0;
	int ret;
	int err;

	if(!device_is_ready(i2c0_dev)){
		printk("i2c_dev is not ready ----------->\r\n");
	}
	printk("<--------- pass i2c device\r\n");

	initAht20();
	
	gpio_pin_configure_dt(&testLed, GPIO_OUTPUT_ACTIVE);
	gpio_pin_configure_dt(&button, GPIO_INPUT);
	gpio_init_callback(&button_cb, button_pressed_callback, BIT(button.pin));
	gpio_add_callback(button.port, &button_cb);
	gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);

	while (1) {
		readAht20();
		gpio_pin_set_dt(&led0, 1);
		k_msleep(SLEEP_TIME_MS);
		printk("main count: %d\r\n", count++);
		gpio_pin_set_dt(&led0, 0);
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}
