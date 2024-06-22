#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
uint32_t getDistance();

int main()
{
	gpio_init(6);
	gpio_init(7);

	gpio_set_dir(6, GPIO_OUT);
	gpio_set_dir(7, GPIO_IN);

	gpio_init(16);
	gpio_set_dir(16, GPIO_OUT);
	gpio_put(16,0);	
	while(true)
	{
	uint32_t distance= getDistance();
	if(distance <10)
	{
		gpio_put(16,1);
	}
	else
	{
		gpio_put(16,0);
	}
	sleep_ms(500);
	}
}

uint32_t getDistance()
{
	uint32_t start_time, end_time;
 
        gpio_put(6, 1);
        sleep_us(10);
        gpio_put(6, 0);
        
        //uint width = 0;
        //int timeout= 26100;

        while(!gpio_get(7))
        {
                
        }
	start_time= time_us_32();
        while(gpio_get(7)==1)
        {       
                /*width++;
                sleep_us(1);
                if(width > timeout)
                {
                        return 0;
                }*/
        }
	end_time= time_us_32();
 	
	return (end_time - start_time) /58;
	
}
