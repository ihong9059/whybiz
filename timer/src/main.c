#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   100

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_NODELABEL(led0)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

static void mytimer_cb(struct k_timer *dummy){
	gpio_pin_toggle_dt(&led);
};

struct k_timer mytimer;


int main(void)
{
	int ret;
	uint32_t count = 0;

	k_timer_init(&mytimer, mytimer_cb, NULL); //exfire fun, stop fun
	k_timer_start(&mytimer, K_SECONDS(2), K_SECONDS(0.05));

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT);
	if (ret < 0) {
		return 0;
	}

	while (1) {
		k_msleep(SLEEP_TIME_MS);
		printk("nrf52dk main: %d\r\n", count++);
	}
	return 0;
}
