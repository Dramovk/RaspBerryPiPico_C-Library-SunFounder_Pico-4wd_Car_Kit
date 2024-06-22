#include "pico/stdlib.h"
#include <time.h>

int main(void){
	
	uint64_t start1, end1;
	uint64_t start2, end2;	
	bool flag1=false, flag2=false, blinking=false, blinkFlag=false, 
	redLight=false;
	double bt=0, lt=0;

	gpio_init(25);
	gpio_set_dir(25,GPIO_OUT);
	gpio_put(25,1);

	gpio_init(21);
	gpio_pull_up(21);

	gpio_init(20);
	gpio_set_dir(20, GPIO_OUT);	

	gpio_init(22);
	gpio_set_dir(22, GPIO_OUT);
	while(true){
		if(gpio_get(21) && blinkFlag && !redLight)
                {
			redLight=true;   
		}
		else if((gpio_get(21)==0) && blinkFlag && redLight && lt<5000000)
                {
                        gpio_put(20,1);
                        if(flag2)
                        {
                                start2=time_us_64();
                                flag2=false;
                        }
                        end2=time_us_64();
                        lt=(uint64_t)(end2-start2);
				
		}
		else if((gpio_get(21)==0) && blinkFlag && redLight)
		{
			gpio_put(20,0);
			gpio_put(22,0);
			blinkFlag=false;
			redLight=false;
			lt=0;
			bt=0;
			sleep_ms(1000);
		}
		else if(gpio_get(21) && blinkFlag)
		{
			gpio_put(20,0);
			gpio_put(22,1);
			sleep_ms(200);
			gpio_put(22, 0);
			sleep_ms(200);
			lt=0;
			flag2=true;
		}
		else if(gpio_get(21) && !blinkFlag)
		{
			gpio_put(22,0);
			flag1=true;
			bt=0;
		}
		else if((gpio_get(21)==0) &&  bt<2000000)
                {       
                        gpio_put(22,1);                        
			if(flag1)  
                        {
                        	start1=time_us_64();
                        	flag1=false;
                        }
                        end1=time_us_64();
			bt=(uint64_t)(end1-start1);			
                }
		else
		{
			gpio_put(22,0);
			sleep_ms(500);
			gpio_put(22,1);
                        sleep_ms(500);
			blinkFlag=true;
		}
	}
}
