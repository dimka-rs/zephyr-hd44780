#ifndef BUTTON_H
#define BUTTON_H

#define BTN_PORT     "GPIO_0"
#define BTN1_PIN     (13)
#define BTN2_PIN     (14)
/* Pins 15 and 16 used for LCD */

#define EDGE    (GPIO_INT_EDGE | GPIO_INT_ACTIVE_LOW)

enum btns_e
{
    BTN1,
    BTN2,
    BTNS_MAX
};

struct btns_s
{
    const struct device *port;
    uint32_t btn[BTNS_MAX];
};

void button_init();

#endif //BUTTON_H
