#include <zephyr.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/gpio.h>
#include <inttypes.h> 
#include "button.h"
#include "led.h"
#include "hd44780.h"
#include "adc.h"

const u8_t str1[] = "@ZephyrIoT";
const u8_t str2[] = "@NordicTweets";

void main(void)
{
    u32_t i;
    u8_t col = 0, btn = 0;
    printk("start app\n");

    button_init();
    hd44780_init();
    adc_init();

    while (1)
    {
        hd44780_cmd(HD44780_CMD_CLEAR, 0);

        hd44780_pos(0, col);
        for(i = 0; i < sizeof(str1) - 1; i++)
        {
            hd44780_data(str1[i]);
        }

        hd44780_pos(1, col);
        for(i = 0; i < sizeof(str2) - 1; i++)
        {
            hd44780_data(str2[i]);
        }
        col++;
        if (col > 3)
            col = 0;

        hd44780_pos(0, 15);
        btn = adc_sample();
        switch (btn)
        {
            case btn_none:
                hd44780_data(0xBC); // sort of smile, ROM Code: A00
                break;

            case btn_select:
                hd44780_data(0xF4); // omega, ROM Code: A00
                break;

            case btn_left:
                hd44780_data(0x7F); // left arrow, ROM Code: A00
                break;

            case btn_down:
                hd44780_data('v');
                break;

            case btn_up:
                hd44780_data('^');
                break;

            case btn_right:
                hd44780_data(0x7E); // right arrow, ROM Code: A00
                break;

            default:
                hd44780_data('E');
                break;
        }

        k_sleep(1000);
    }
}

