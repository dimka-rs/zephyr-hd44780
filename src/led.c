#include <zephyr.h>
#include <inttypes.h>
#include <device.h>
#include <drivers/gpio.h>
#include "led.h"

static struct leds_s leds = {
    .port = NULL,
    .led[LED1] = LED1_PIN,
    .led[LED2] = LED2_PIN,
    .led[LED3] = LED3_PIN,
    .led[LED4] = LED4_PIN,
};

void
len_on(uint8_t l)
{
    gpio_pin_set(leds.port, leds.led[l], 1);
}

void
len_off(uint8_t l)
{
    gpio_pin_set(leds.port, leds.led[l], 0);
}

void
led_init()
{
    leds.port = device_get_binding(LED_PORT);
    if (!leds.port) {
        printk("Get led binding error\n");
        return;
    }

    /* Set LEDs pins as output */
    for(uint8_t i = 0; i < LEDS_MAX; i++)
    {
        gpio_pin_configure(leds.port, leds.led[i], GPIO_OUTPUT);
    }
    printk("LEDs init done\n");
}
