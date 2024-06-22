#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"

uint32_t pwm_set_freq_duty(uint slice_num, uint chan, uint32_t f, int d);
uint32_t pwm_get_wrap(uint silce_num);
void pwm_set_duty(uint slice_num, uint chan, int d);
long map(long x, long in_min, long in_max, long out_min, long out_max);

int main()
{
//	gpio_init_all();
	adc_init();
	adc_gpio_init(26);
	adc_select_input(0);

	gpio_init(17);
	gpio_set_dir(17, GPIO_OUT);
	gpio_put(17,1);
	
	gpio_init(25);
	gpio_set_dir(25, GPIO_OUT);
	gpio_put(25, 1);

	gpio_init(16);
	//Making pin 16  be the PWM pin
	gpio_set_function(16, GPIO_FUNC_PWM);
	//Setting the slices to pull from pin 16
	uint slice_num = pwm_gpio_to_slice_num(16);

	//Making the PWM channel be pin 16
	uint chan16 = pwm_gpio_to_channel(16);
	
	//Setting the frequency to be 1000 with a duty cycle of 10000	
	//EITHER WAY WORKS HIGHER FREQUENCY BRIGHTER LIGHT (MORE POWER)
	pwm_set_freq_duty(slice_num, chan16, 10000, 10000);
	//pwm_set_duty(slice_num, PWM_CHAN_A, 10000);
	
	//Enabling the pin for PWM
	pwm_set_enabled(slice_num, true);

	while(true)
        {
                uint16_t result =adc_read();
		//long pwm_value = map(result, 0, 10000, 0, 255);
		
                pwm_set_chan_level(slice_num, PWM_CHAN_A, result);
        }

	//Making the pin 16 be the PWM pin of slices
	//Setting the pwm cycle to have 100 cycles
	//pwm_hw->slice[0].top = 9999;
	//dividing the frequency by two
	//pwm_hw->slice[0].div = 4 << PWM_CH0_DIV_INT_LSB;C
	//Setting the pin 14 to a duty cycle of 25% at high
	//pwm_hw->slice[0].cc = 0 << PWM_CH0_CC_A_LSB;
	//Set the PWM running	
	//pwm_hw->slice[0].csr= PWM_CH0_CSR_EN_BITS;
}

uint32_t pwm_set_freq_duty(uint slice_num, uint chan, uint32_t f, int d)
{
	uint32_t clock= 125000000;
	uint32_t divider16= clock /  f / 4096 + (clock % (f * 4096)!=0);
	if (divider16 / 16 ==0)
		divider16 = 16; 
	uint32_t wrap = clock * 16 / divider16 / f - 1;
	pwm_set_clkdiv_int_frac(slice_num, divider16/16, divider16 & 0xF);
	pwm_set_wrap(slice_num, wrap);
	pwm_set_chan_level(slice_num, chan, wrap * d / 100);
	return wrap;
}

uint32_t pwm_get_wrap(uint slice_num)
{
	valid_params_if(PWM, slice_num >=0 && slice_num <NUM_PWM_SLICES);
	return pwm_hw->slice[slice_num].top;
}

void pwm_set_duty(uint slice_num, uint chan, int d)
{
	pwm_set_chan_level(slice_num, chan, pwm_get_wrap(slice_num)*d/100);
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
	return(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
