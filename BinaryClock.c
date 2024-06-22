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
        adc_init();
        adc_gpio_init(26);
        adc_select_input(0);
       	
	//USE PIN 36 to be power to Potentiometer
 
        gpio_init(25);
        gpio_set_dir(25, GPIO_OUT);
        gpio_put(25, 1);
      
	
        gpio_init(16);
        //Making pin 16  be the PWM pin
        gpio_set_function(16, GPIO_FUNC_PWM);
        //Setting the slices to pull from pin 16
        uint slice_num0 = pwm_gpio_to_slice_num(16);
        
        //Making the PWM channel be pin 16
        uint chan16 = pwm_gpio_to_channel(16);
 
        //Setting the frequency to be 1000 with a duty cycle of 10000
        //EITHER WAY WORKS HIGHER FREQUENCY BRIGHTER LIGHT (MORE POWER) 
        pwm_set_freq_duty(slice_num0, chan16, 10000, 10000);
        
        //Enabling the pin for PWM
        pwm_set_enabled(slice_num0, true);
        
	gpio_init(17);
        //Making pin 17  be the PWM pin
        gpio_set_function(17, GPIO_FUNC_PWM);
        //Setting the slices to pull from pin 17
        uint slice_num1 = pwm_gpio_to_slice_num(17);
                
        //Making the PWM channel be pin 17
        uint chan17 = pwm_gpio_to_channel(17);
                
        //Setting the frequency to be 1000 with a duty cycle of 10000
        //EITHER WAY WORKS HIGHER FREQUENCY BRIGHTER LIGHT (MORE POWER)
        pwm_set_freq_duty(slice_num1, chan17, 10000, 10000);
                
        //Enabling the pin for PWM
        pwm_set_enabled(slice_num1, true);


	gpio_init(18); 
        //Making pin 18  be the PWM pin
        gpio_set_function(18, GPIO_FUNC_PWM);
        //Setting the slices to pull from pin 18
        uint slice_num2 = pwm_gpio_to_slice_num(18);
        
        //Making the PWM channel be pin 18
        uint chan18 = pwm_gpio_to_channel(18);
        
        //Setting the frequency to be 1000 with a duty cycle of 10000
        //EITHER WAY WORKS HIGHER FREQUENCY BRIGHTER LIGHT (MORE POWER)
        pwm_set_freq_duty(slice_num2, chan18, 10000, 10000);
        
        //Enabling the pin for PWM
        pwm_set_enabled(slice_num2, true); 	
		
	
	bool buttonP= false;
	bool changeState=true, switch1=true;
	
	//Pin 15 is the button pin
	gpio_init(15);
        gpio_pull_up(15);
	
	//timer variables	
	uint start=0, end=0, t, end1=0, start1=0, t1=0;
	bool flag=true, flag1=true;

        while(true)
        {
		//ALLOWING FOR BRIGHTNESS COINTROL FROM POTENTIOMETER
	        uint16_t result =adc_read();
 	
		//Making the button press switch between countiung up and down by make a 
		//boolean true or false
		if(gpio_get(15))
		{
			start=0;
			end=0;
			t=0;
			if(flag1)
                        {
                                start1=time_us_64();
                                flag1=false;
                        }
                        end1=time_us_64();
                        t1=(uint64_t)(end1-start1);		
			if(t1<1000000)
			{
				pwm_set_chan_level(slice_num0, PWM_CHAN_A, 0);
                                pwm_set_chan_level(slice_num1, PWM_CHAN_B, 0);
                                pwm_set_chan_level(slice_num2, PWM_CHAN_A, 0);
			}
			else if(t1>1000000 && t1<2000000)
			{
				pwm_set_chan_level(slice_num0, PWM_CHAN_A, result);
                		pwm_set_chan_level(slice_num1, PWM_CHAN_B, 0);
                		pwm_set_chan_level(slice_num2, PWM_CHAN_A, 0);
			}
			else if(t1>2000000 && t1<3000000)
                        {
                                pwm_set_chan_level(slice_num0, PWM_CHAN_A, 0);
                                pwm_set_chan_level(slice_num1, PWM_CHAN_B, result);     
                                pwm_set_chan_level(slice_num2, PWM_CHAN_A, 0);     
                        }
			else if(t1>3000000 && t1<4000000)
                        {
                                pwm_set_chan_level(slice_num0, PWM_CHAN_A, result);
                                pwm_set_chan_level(slice_num1, PWM_CHAN_B, result);
                                pwm_set_chan_level(slice_num2, PWM_CHAN_A, 0);
                        }
			else if(t1>4000000 && t1<5000000)
                        {
                                pwm_set_chan_level(slice_num0, PWM_CHAN_A, 0);
                                pwm_set_chan_level(slice_num1, PWM_CHAN_B, 0);
                                pwm_set_chan_level(slice_num2, PWM_CHAN_A, result);
                        }
			else if(t1>5000000 && t1<6000000)
                        {
                                pwm_set_chan_level(slice_num0, PWM_CHAN_A, result);
                                pwm_set_chan_level(slice_num1, PWM_CHAN_B, 0);
                                pwm_set_chan_level(slice_num2, PWM_CHAN_A, result);
                        }
			else if(t1>6000000 && t1<7000000)
                        {
                                pwm_set_chan_level(slice_num0, PWM_CHAN_A, 0);
                                pwm_set_chan_level(slice_num1, PWM_CHAN_B, result);
                                pwm_set_chan_level(slice_num2, PWM_CHAN_A, result);
                        }
			else if(t1>7000000 && t1<8000000)
                        {
                                pwm_set_chan_level(slice_num0, PWM_CHAN_A, result);
                                pwm_set_chan_level(slice_num1, PWM_CHAN_B, result);
                                pwm_set_chan_level(slice_num2, PWM_CHAN_A, result);
			}	
			else if(t1>8000000)
			{
				flag1=true;
			}
		}
		else
		{
			start1=0;
			end1=0;
			t1=0;
			if(flag)
                        {
                                start=time_us_64();
                                flag=false;
                        }
                        end=time_us_64();
                        t=(uint64_t)(end-start);
                        if(t>7000000 && t<8000000)
                        {
                                pwm_set_chan_level(slice_num0, PWM_CHAN_A, 0);
                                pwm_set_chan_level(slice_num1, PWM_CHAN_B, 0);
                                pwm_set_chan_level(slice_num2, PWM_CHAN_A, 0);
                        }
                        else if(t>6000000 && t<7000000)
                        {
                                pwm_set_chan_level(slice_num0, PWM_CHAN_A, result);
                                pwm_set_chan_level(slice_num1, PWM_CHAN_B, 0);
                                pwm_set_chan_level(slice_num2, PWM_CHAN_A, 0);
                        }
                        else if(t>5000000 && t<6000000)
                        {
                                pwm_set_chan_level(slice_num0, PWM_CHAN_A, 0);
                                pwm_set_chan_level(slice_num1, PWM_CHAN_B, result);
                                pwm_set_chan_level(slice_num2, PWM_CHAN_A, 0);
                        }
                        else if(t>4000000 && t<5000000)
                        {
                                pwm_set_chan_level(slice_num0, PWM_CHAN_A, result);
                                pwm_set_chan_level(slice_num1, PWM_CHAN_B, result);
                                pwm_set_chan_level(slice_num2, PWM_CHAN_A, 0);
                        }
			else if(t>3000000 && t<4000000)
                        {
                                pwm_set_chan_level(slice_num0, PWM_CHAN_A, 0);
                                pwm_set_chan_level(slice_num1, PWM_CHAN_B, 0);
                                pwm_set_chan_level(slice_num2, PWM_CHAN_A, result);
                        }
                        else if(t>2000000 && t<3000000)
                        {
                                pwm_set_chan_level(slice_num0, PWM_CHAN_A, result);
                                pwm_set_chan_level(slice_num1, PWM_CHAN_B, 0);
                                pwm_set_chan_level(slice_num2, PWM_CHAN_A, result);
                        }
                        else if(t>1000000 && t<2000000)
                        {
                                pwm_set_chan_level(slice_num0, PWM_CHAN_A, 0);
                                pwm_set_chan_level(slice_num1, PWM_CHAN_B, result);
                                pwm_set_chan_level(slice_num2, PWM_CHAN_A, result);
                        }
                        else if(t<1000000)
                        {
                                pwm_set_chan_level(slice_num0, PWM_CHAN_A, result);
                                pwm_set_chan_level(slice_num1, PWM_CHAN_B, result);
                                pwm_set_chan_level(slice_num2, PWM_CHAN_A, result);
                        }
			else if(t>8000000)
			{
				flag=true;
			}
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
        pwm_set_chan_level(slice_num, chan, 
pwm_get_wrap(slice_num)*d/100);
}
        
long map(long x, long in_min, long in_max, long out_min, long out_max)
{       
        return(x - in_min) * (out_max - out_min) / (in_max - in_min) + 
out_min;
}
