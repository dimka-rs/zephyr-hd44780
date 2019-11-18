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
    gpio_pin_write(disp.port, disp.pin[EN], 1);
    // wait 50 ms
    // TODO: what timing shoud be?
    k_sleep(50);
    // dis
    gpio_pin_write(disp.port, disp.pin[EN], 0);
}

static void
hd44780_byte(u8_t b)
{
    // hogh nibble
    gpio_pin_write(disp.port, disp.pin[D7], (b & (1 << 7)) ? 1 : 0);
    gpio_pin_write(disp.port, disp.pin[D6], (b & (1 << 6)) ? 1 : 0);
    gpio_pin_write(disp.port, disp.pin[D5], (b & (1 << 5)) ? 1 : 0);
    gpio_pin_write(disp.port, disp.pin[D4], (b & (1 << 4)) ? 1 : 0);
    hd44780_pulse();
    // low nibble
    gpio_pin_write(disp.port, disp.pin[D7], (b & (1 << 3)) ? 1 : 0);
    gpio_pin_write(disp.port, disp.pin[D6], (b & (1 << 2)) ? 1 : 0);
    gpio_pin_write(disp.port, disp.pin[D5], (b & (1 << 1)) ? 1 : 0);
    gpio_pin_write(disp.port, disp.pin[D4], (b & (1 << 0)) ? 1 : 0);
    hd44780_pulse();
}

void
hd44780_data(u8_t val)
{
    // rs high - data
    gpio_pin_write(disp.port, disp.pin[RS], 1);
    hd44780_byte(val);
}

void
hd44780_cmd(u8_t cmd, u8_t flags)
{
    cmd |= flags;

    // rs low - command
    gpio_pin_write(disp.port, disp.pin[RS], 0);

    hd44780_byte(cmd);
}

void
hd44780_pos(u8_t row, u8_t col)
{
    u8_t addr;

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
    u32_t i;

    disp.port = device_get_binding(HD44780_PORT);
    for(i = 0; i < PINS_MAX; i++)
    {
        gpio_pin_configure(disp.port, disp.pin[i], GPIO_DIR_OUT);
    }

    hd44780_cmd(HD44780_CMD_CONFIG, HD44780_CONFIG_2LINES|HD44780_CONFIG_5X8|HD44780_CONFIG_DATA4);
    hd44780_cmd(HD44780_CMD_ONOFF, HD44780_ONOFF_DISP_ON);

    printk("HD44780 init done\n");
}