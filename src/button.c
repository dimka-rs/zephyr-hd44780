#include <zephyr.h>
#include <inttypes.h>
#include <device.h>
#include <drivers/gpio.h>
#include "button.h"

static struct gpio_callback gpio_cb;
struct device *btn0;

void
button_pressed(struct device *btn, struct gpio_callback *cb,
            uint32_t pins)
{
    printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
}

void
button_init(struct device *btn)
{
    btn = device_get_binding(BTN_PORT);
    if (!btn) {
        printk("Get btn binding error\n");
        return;
    }

    gpio_pin_configure(btn, BTN_PIN,
               GPIO_DIR_IN | GPIO_INT | GPIO_PUD_PULL_UP | EDGE);

    gpio_init_callback(&gpio_cb, button_pressed, BIT(BTN_PIN));

    gpio_add_callback(btn, &gpio_cb);
    gpio_pin_enable_callback(btn, BTN_PIN);
    printk("BTN %d init done\n", BTN_PIN);
}