#ifndef LED_H
#define LED_H

#define LED_PORT    ("GPIO_0")
#define LED_PIN     (17)

void led_init();
void len_on();
void len_off();


#endif // LED_H