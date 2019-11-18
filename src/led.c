#include <zephyr.h>
#include <inttypes.h>
#include <device.h>
#include <drivers/gpio.h>
#include <dt-bindings/gpio/gpio.h>
#include "led.h"

struct device *led0;

void
len_on()
{
    gpio_pin_write(led0, LED_PIN, 1);
}

void
len_off()
{
    gpio_pin_write(led0, LED_PIN, 0);
}

void
led_init()
{
    led0 = device_get_binding(LED_PORT);
    if (!led0) {
        printk("Get led binding error\n");
        return;
    }

    /* Set LED pin as output */
    gpio_pin_configure(led0, LED_PIN, GPIO_DIR_OUT);
    printk("LED %d init done\n", LED_PIN);
}