#ifndef LED_H
#define LED_H

#define LED_PORT    ("GPIO_0")
#define LED1_PIN     (17)
#define LED2_PIN     (18)
#define LED3_PIN     (19)
#define LED4_PIN     (20)

enum leds_e
{
    LED1,
    LED2,
    LED3,
    LED4,
    LEDS_MAX
};

struct leds_s
{
    const struct device *port;
    uint32_t led[LEDS_MAX];
};


void led_init();
void len_on(uint8_t l);
void len_off(uint8_t l);


#endif // LED_H
