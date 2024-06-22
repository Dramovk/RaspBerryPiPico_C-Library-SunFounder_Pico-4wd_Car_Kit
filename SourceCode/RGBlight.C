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
        gpio_init(25);    
        gpio_set_dir(25, GPIO_OUT);
        gpio_put(25, 1);
	
        adc_init();
        adc_gpio_init(26);
        adc_select_input(0);
       
	gpio_init(15);
        gpio_pull_up(15);	        

        gpio_init(16);
        gpio_set_function(16, GPIO_FUNC_PWM);
        uint slice_num1 = pwm_gpio_to_slice_num(16);
        uint chan16 = pwm_gpio_to_channel(16);

	gpio_init(17);
	gpio_set_function(17, GPIO_FUNC_PWM);
	uint slice_num2= pwm_gpio_to_slice_num(17);
	uint chan17 = pwm_gpio_to_channel(17);

	gpio_init(18);
        gpio_set_function(18, GPIO_FUNC_PWM);
        uint slice_num3= pwm_gpio_to_slice_num(18);
        uint chan18 = pwm_gpio_to_channel(18);
 
        //Setting the frequency to be 1000 with a duty cycle of 10000 
        //pwm_set_freq_duty(slice_num1, chan16, 10000, 10000);
	//pwm_set_freq_duty(slice_num2, chan17, 10000, 10000);       
	//pwm_set_freq_duty(slice_num3, chan18, 10000, 10000);

        //Enabling the pin for PWM
        pwm_set_enabled(slice_num1, true);
	pwm_set_enabled(slice_num2, true);
	pwm_set_enabled(slice_num3, true);        

	bool flag=true, buttonp=true, R=true, G, B;

        while(true)
        {
                uint16_t result =adc_read();
		if(gpio_get(13)==1 && flag)
		{
			flag=false;
			if(R)
			{
				G=true;
				R=false;
				B=false;
			}
			else if(G)
                        {
                                B=true;
                                R=false;
                                G=false;
                        }
			else if(B)
                        {
                                R=true;
                                B=false;
                                G=false;
                        }
		} 
		flag=true;

		if(R){		
                pwm_set_chan_level(slice_num1, PWM_CHAN_A, result);
		}
		if(G){
		pwm_set_chan_level(slice_num2, PWM_CHAN_B, result);
		//R=false;
		//B=false;
		}
		if(B){
                pwm_set_chan_level(slice_num3, PWM_CHAN_A, result);
                //R=false;
                //G=false;
                }	  
	}	
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
