#ifndef HD44780_H
#define HD44780_H

#include <zephyr.h>

/* LCD shield pinout
 *  Ard nRF     Usage
 *  A0  0.03    Buttons
 *  D4  0.15    DB4
 *  D5  0.16    DB5
 *  D6  0.17    DB6
 *  D7  0.18    DB7
 *  D8  0.19    RS
 *  D9  0.20    EN
 */

#define HD44780_PORT   "GPIO_0"
#define HD44780_PIN_D4 (15)
#define HD44780_PIN_D5 (16)
#define HD44780_PIN_D6 (17)
#define HD44780_PIN_D7 (18)
#define HD44780_PIN_RS (19)
#define HD44780_PIN_EN (20)

enum hd44780_pins
{
    D4,
    D5,
    D6,
    D7,
    RS,
    EN,
    PINS_MAX
};

enum hd44780_cmds
{
    HD44780_CMD_CLEAR = 1,
    HD44780_CMD_HOME = 2,
    HD44780_CMD_MODE = 4,
        HD44780_MODE_INC = 2,
        HD44780_MODE_DEC = 0,
        HD44780_MODE_SHIFT = 1,
    HD44780_CMD_ONOFF = 8,
        HD44780_ONOFF_DISP_ON = 4,
        HD44780_ONOFF_DISP_OFF = 0,
        HD44780_ONOFF_CURS_ON = 2,
        HD44780_ONOFF_CURS_OFF = 0,
        HD44780_ONOFF_BLINK_ON = 1,
        HD44780_ONOFF_BLINK_OFF = 0,
    HD44780_CMD_SHIFT = 16,
        HD44780_SHIFT_DISP = 8,
        HD44780_SHIFT_CURS = 0,
        HD44780_SHIFT_RIGHT = 4,
        HD44780_SHIFT_LEFT = 0,
    HD44780_CMD_CONFIG = 32,
        HD44780_CONFIG_DATA8 = 16,
        HD44780_CONFIG_DATA4 = 0,
        HD44780_CONFIG_2LINES = 8,
        HD44780_CONFIG_1LINE0 = 0,
        HD44780_CONFIG_5X11 = 4,
        HD44780_CONFIG_5X8 = 0,
    HD44780_CMD_CGRAM = 64,
    HD44780_CMD_DDRAM = 128,
};

struct hd44780_display
{
    const struct device *port;
    uint32_t pin[PINS_MAX];
};

void hd44780_init();
void hd44780_cmd(uint8_t cmd, uint8_t flags);
void hd44780_data(uint8_t val);
void hd44780_pos(uint8_t row, uint8_t col);


#endif // HD44780_H
