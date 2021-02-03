#include <stdint.h>
#include <drivers/gpio.h>
#include "hd44780.h"

static struct hd44780_display disp = {
    .port = NULL,
    .pin[D4] = HD44780_PIN_D4,
    .pin[D5] = HD44780_PIN_D5,
    .pin[D6] = HD44780_PIN_D6,
    .pin[D7] = HD44780_PIN_D7,
    .pin[RS] = HD44780_PIN_RS,
    .pin[EN] = HD44780_PIN_EN,
};

static inline void
hd44780_pulse()
{
    // en
    gpio_pin_set(disp.port, disp.pin[EN], 1);
    // ds says min 1 us, but only 1000 works reliably
    k_usleep(1000);
    // dis
    gpio_pin_set(disp.port, disp.pin[EN], 0);
}

inline static void
hd44780_nibble(uint8_t n)
{
    gpio_pin_set(disp.port, disp.pin[RS], 0); //cmd
    gpio_pin_set(disp.port, disp.pin[D7], (n & (1 << 7)) ? 1 : 0);
    gpio_pin_set(disp.port, disp.pin[D6], (n & (1 << 6)) ? 1 : 0);
    gpio_pin_set(disp.port, disp.pin[D5], (n & (1 << 5)) ? 1 : 0);
    gpio_pin_set(disp.port, disp.pin[D4], (n & (1 << 4)) ? 1 : 0);
    hd44780_pulse();
}

static void
hd44780_byte(uint8_t b)
{
    // high nibble
    gpio_pin_set(disp.port, disp.pin[D7], (b & (1 << 7)) ? 1 : 0);
    gpio_pin_set(disp.port, disp.pin[D6], (b & (1 << 6)) ? 1 : 0);
    gpio_pin_set(disp.port, disp.pin[D5], (b & (1 << 5)) ? 1 : 0);
    gpio_pin_set(disp.port, disp.pin[D4], (b & (1 << 4)) ? 1 : 0);
    hd44780_pulse();
    // low nibble
    gpio_pin_set(disp.port, disp.pin[D7], (b & (1 << 3)) ? 1 : 0);
    gpio_pin_set(disp.port, disp.pin[D6], (b & (1 << 2)) ? 1 : 0);
    gpio_pin_set(disp.port, disp.pin[D5], (b & (1 << 1)) ? 1 : 0);
    gpio_pin_set(disp.port, disp.pin[D4], (b & (1 << 0)) ? 1 : 0);
    hd44780_pulse();

    // most commands take about 37 us, 1000 to be safe
    k_usleep(1000);
}

void
hd44780_data(uint8_t val)
{
    // rs high - data
    gpio_pin_set(disp.port, disp.pin[RS], 1);
    hd44780_byte(val);
}

void
hd44780_cmd(uint8_t cmd, uint8_t flags)
{
    cmd |= flags;
    // rs low - command
    gpio_pin_set(disp.port, disp.pin[RS], 0);
    hd44780_byte(cmd);
}

void
hd44780_pos(uint8_t row, uint8_t col)
{
    uint8_t addr;

    switch (row)
    {
        case 0:
            addr = 0x00;
            break;
        case 1:
            addr = 0x40;
            break;
        default:
            return;
    }

    if (col < 16)
        addr += col;
    else
        return;

    hd44780_cmd(HD44780_CMD_DDRAM, addr);
}

void
hd44780_init()
{
    uint32_t i;

    disp.port = device_get_binding(HD44780_PORT);
    for(i = 0; i < PINS_MAX; i++)
    {
        gpio_pin_configure(disp.port, disp.pin[i], GPIO_OUTPUT);
    }

    // display starts in 8 bit mode, so first command must pulse enable only once
    // _nibble will do this using only high nibble, though only bus width can be set
    hd44780_nibble(0x20);
    // this command is long
    k_sleep(K_MSEC(10));
    hd44780_cmd(HD44780_CMD_CONFIG, HD44780_CONFIG_2LINES|HD44780_CONFIG_5X8|HD44780_CONFIG_DATA4);
    k_sleep(K_MSEC(10));
    hd44780_cmd(HD44780_CMD_ONOFF, HD44780_ONOFF_DISP_ON);
    k_sleep(K_MSEC(10));

    printk("HD44780 init done\n");
}
