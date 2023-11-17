#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

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

// chapter3 11:00
int main(void)
{
	uint32_t count = 0;
	int ret;

	if (!gpio_is_ready_dt(&led0)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	gpio_pin_configure_dt(&testLed, GPIO_OUTPUT_ACTIVE);

	gpio_pin_configure_dt(&button, GPIO_INPUT);
	gpio_init_callback(&button_cb, button_pressed_callback, BIT(button.pin));
	gpio_add_callback(button.port, &button_cb);
	gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);

	while (1) {
		// ret = gpio_pin_toggle_dt(&led0);
		// if (ret < 0) {
		// 	return 0;
		// }
		gpio_pin_set_dt(&led0, 1);
		k_msleep(SLEEP_TIME_MS);
		printk("main count: %d\r\n", count++);
		gpio_pin_set_dt(&led0, 0);
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}
