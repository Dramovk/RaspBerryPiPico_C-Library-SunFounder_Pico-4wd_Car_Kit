#include "pico/stdlib.h"

/*int main (void)
{
	//gpio_init(25);
	//gpio_set_dir(25, GPIO_OUT);
	//gpio_put(25, 1);
	
	gpio_init(22);

	gpio_set_dir(22, GPIO_OUT);
	while(1)
	{	
	gpio_put(22, 1);
	sleep_us(25);
	gpio_put(22, 0);
	sleep_us(25);
	for(int i=0; i<100; i++)
        {

        }
	}
	gpio_init(25);  
        gpio_set_dir(25, GPIO_OUT);
        gpio_put(25, 1);
}*/

int main (void)
{       
        gpio_init(22);
        
        gpio_set_dir(22, GPIO_OUT);
	gpio_put(22, 1);
	
	gpio_init(14);
	gpio_set_dir(14, GPIO_OUT);
	gpio_put(14, 1);
}
