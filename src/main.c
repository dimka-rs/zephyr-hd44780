#include <zephyr.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/gpio.h>
#include <inttypes.h> 
#include "button.h"
#include "led.h"
#include "hd44780.h"

/* sleep time in msec */
#define SLEEPTIME  1000
const u8_t str1[] = "@ZephyrIoT";
const u8_t str2[] = "@NordicTweets";

void main(void)
{
    u32_t val = 0, i;
    printk("start app\n");

    led_init();
    button_init();
    hd44780_init();

    while (1) {
        printk("LED %d\n", val);
        if (val > 0)
        {
            len_off();
            val = 0;
        }
        else
        {
            len_on();
            val = 1;
        }

    hd44780_cmd(HD44780_CMD_CLEAR, 0);
    hd44780_pos(0, 0);
    for(i = 0; i < sizeof(str1) - 1; i++)
    {
        hd44780_data(str1[i]);
    }

    hd44780_pos(1, 0);
    for(i = 0; i < sizeof(str2) - 1; i++)
    {
        hd44780_data(str2[i]);
    }
        k_sleep(SLEEPTIME);
    }
}

