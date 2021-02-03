#include <zephyr.h>
#include <inttypes.h>
#include <device.h>
#include <drivers/gpio.h>
#include "button.h"


static struct btns_s btns = {
    .port = NULL,
    .btn[BTN1] = BTN1_PIN,
    .btn[BTN2] = BTN2_PIN,
    .btn[BTN3] = BTN3_PIN,
    .btn[BTN4] = BTN4_PIN,
};

static struct gpio_callback gpio_cb[BTNS_MAX];

void

button_pressed(const struct device *dev, struct gpio_callback *cb,
                uint32_t pins)
{
    printk("Button %d pressed at %u\n", pins,  k_cycle_get_32());
}

void
button_init()
{
    btns.port = device_get_binding(BTN_PORT);
    if (!btns.port) {
        printk("Get btn binding error\n");
        return;
    }

    for (uint8_t i = 0; i < BTNS_MAX; i++)
    {
        gpio_pin_configure(btns.port, btns.btn[i],
               GPIO_INPUT | GPIO_INT_EDGE_BOTH | GPIO_PULL_UP );
        gpio_init_callback(&gpio_cb[i], button_pressed, BIT(btns.btn[i]));
        gpio_add_callback(btns.port, &gpio_cb[i]);
        //gpio_pin_enable_callback(btns.port, btns.btn[i]);
    }

    printk("BTNs init done\n");
}
