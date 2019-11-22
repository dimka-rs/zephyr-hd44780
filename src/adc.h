#ifndef ADC_C
#define ADC_C

void adc_init();
u8_t adc_sample();

enum adc_button
{
    btn_error,
    btn_none,
    btn_select,
    btn_left,
    btn_down,
    btn_up,
    btn_right
};

#endif //ADC_C