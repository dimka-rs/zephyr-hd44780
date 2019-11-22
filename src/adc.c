#include <zephyr.h>
#include <drivers/adc.h>
#include "adc.h"
#include <string.h>

// based on https://github.com/zephyrproject-rtos/zephyr/blob/master/tests/drivers/adc/adc_api/src/test_adc.c

// NRF52 specific
#include <hal/nrf_saadc.h>
#define ADC_DEVICE_NAME		DT_ADC_0_NAME
#define ADC_RESOLUTION		10
#define ADC_GAIN		ADC_GAIN_1_6
#define ADC_REFERENCE		ADC_REF_INTERNAL
#define ADC_ACQUISITION_TIME	ADC_ACQ_TIME(ADC_ACQ_TIME_MICROSECONDS, 10)
#define ADC_1ST_CHANNEL_ID	0
#define ADC_1ST_CHANNEL_INPUT	NRF_SAADC_INPUT_AIN1 // Arduino A0, P0.03
//#define ADC_2ND_CHANNEL_ID	2
//#define ADC_2ND_CHANNEL_INPUT	NRF_SAADC_INPUT_AIN2

static s16_t sample;
static struct device *adc_dev;

static const struct adc_channel_cfg m_1st_channel_cfg = {
	.gain             = ADC_GAIN,
	.reference        = ADC_REFERENCE,
	.acquisition_time = ADC_ACQUISITION_TIME,
	.channel_id       = ADC_1ST_CHANNEL_ID,
#if defined(CONFIG_ADC_CONFIGURABLE_INPUTS)
	.input_positive   = ADC_1ST_CHANNEL_INPUT,
#endif
};


void
adc_init()
{
    adc_dev = device_get_binding(ADC_DEVICE_NAME);
    if (!adc_dev)
        printk("Failed to get ADC device\n");

    if (adc_channel_setup(adc_dev, &m_1st_channel_cfg))
        printk("Failed to set up ADC channel 1\n");

    sample = 0;
    printk("ADC init done\n");
}

u8_t
adc_sample()
{
    u8_t btn = 0;
    int ret;
    const struct adc_sequence sequence = {
        .channels    = BIT(ADC_1ST_CHANNEL_ID),
        .buffer      = &sample,
        .buffer_size = sizeof(sample),
        .resolution  = ADC_RESOLUTION,
    };

    ret = adc_read(adc_dev, &sequence);
    if (ret)
    {
        printk("ADC channel 1 read error: %d\n", ret);
        return btn_error;
    }


    printk("ADC read: %d\t", sample);
    // open     - 3.4 V - 480..500
    // select   - 3.1 V - 860..890
    // left     - 2.0 V - 550..560
    // down     - 1.2 V - 345..355
    // up       - 0.5 V - 130..140
    // right    - 0 V   - -5..0

    printk("Button: ");
    if(sample > 480 && sample < 500)
    {
        printk("none");
        btn = btn_none;
    }
    else if (sample > 860 && sample < 890 )
    {
        printk("select");
        btn = btn_select;
    }
    else if (sample > 545 && sample < 600)
    {
        printk("left");
        btn = btn_left;
    }
    else if (sample > 340 && sample < 360)
    {
        printk("down");
        btn = btn_down;
    }
    else if (sample > 125 && sample < 145)
    {
        printk("up");
        btn = btn_up;
    }
    else if (sample > -10 && sample < 10)
    {
        printk("right");
        btn = btn_right;
    } else {
        printk("wrong reading");
        btn = btn_error;
    }
    printk("\n");
    return btn;
}