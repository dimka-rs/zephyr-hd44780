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
len_on(u8_t l)
{
    gpio_pin_write(leds.port, leds.led[l], 1);
}

void
len_off(u8_t l)
{
    gpio_pin_write(leds.port, leds.led[l], 0);
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
    for(u8_t i = 0; i < LEDS_MAX; i++)
    {
        gpio_pin_configure(leds.port, leds.led[i], GPIO_DIR_OUT);
    }
    printk("LEDs init done\n");
}