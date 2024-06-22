#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
void turnLightOn(int state);
void turnRight();
void turnLeft();
void turnLeft1(int speed);
void turnRight1(int speed);
void reverse(int speed);
void goForward(int speed);
void stop();
void followLine();
void mazeSolve();
void follow();

void servoInit();
void set_angle(uint angle);
void scan();
void pwm_set_dutyS(uint slice_num, uint chan, int d);

uint32_t measure_distance();
#define TRIGGER_PIN 6
#define ECHO_PIN 7


uint32_t pwm_set_freq_duty(uint slice_num, uint chan, uint32_t f, int d);
uint32_t pwm_get_wrap(uint silce_num);
void pwm_set_duty(uint slice_num, uint chan, int d);
long map(long x, long in_min, long in_max, long out_min, long out_max);
void gpio_init_all();

int main()
{
	gpio_init_all();
	
	turnLightOn(1);
	
	//Motors
	gpio_set_function(17, GPIO_FUNC_PWM);
	gpio_set_function(16, GPIO_FUNC_PWM);
	
	gpio_set_function(15, GPIO_FUNC_PWM);
        gpio_set_function(14, GPIO_FUNC_PWM);
	
	gpio_set_function(13, GPIO_FUNC_PWM);
        gpio_set_function(12, GPIO_FUNC_PWM);
	
	gpio_set_function(11, GPIO_FUNC_PWM);
        gpio_set_function(10, GPIO_FUNC_PWM);

	//Servo 
	gpio_set_function(18, GPIO_FUNC_PWM);
	servoInit();
	
	//Ultrasonic Sensor
	gpio_init(TRIGGER_PIN);     
    gpio_set_dir(TRIGGER_PIN, GPIO_OUT);
        
	gpio_init(ECHO_PIN);
	gpio_set_dir(ECHO_PIN, GPIO_IN);
	
    //Gray Scale Module
    adc_init();
    adc_gpio_init(26); //ADC READ 0 Left
    adc_gpio_init(27); //ADC READ 1 Middle
    adc_gpio_init(28); //ADC READ 2 Right
    
    bool close=false, startTime=false, startEnd=false;
    uint32_t Grey[3];
    uint32_t start_clock=0, end_clock=0, time_clock=0;
	while(true)
    {
        uint32_t distance=measure_distance();
        set_angle(50);
        do{
            
            for(int i=0; i<3; i++)
            {
                adc_select_input(i);
                Grey[i]= adc_read();
            }
            //If straddling the line go straight
            if(Grey[0] > 1350 && Grey[2] > 1350)//&& 1500< Grey[1] <1600 && 1500< Grey[2] <1600)
            {
                goForward(50);
            }
            //If left sensor touches black turn left
            else if(Grey[0] <1350)
            {
                turnLeft1(90);
            }
            //If right sensor touches black turn right
            else if (Grey[2] <1350)
            {
                turnRight1(90);
            }
            //Remeasure distance
            distance=measure_distance();
            //Do it until close to the front
            if(distance<10)
            {
                close=true;
                stop();
            }
        }while(!close);
        turnRight1(80);
        sleep_ms(700);
        close=false;
        while(Grey[2]>1350)
        {
            for(int i=0; i<3; i++)
            {
                adc_select_input(i);
                Grey[i]= adc_read();
            }
        }
        turnLeft();
        sleep_ms(200);
        set_angle(100);
            do{
                for(int i=0; i<3; i++)
                {
                    adc_select_input(i);
                    Grey[i]= adc_read();
                }
                //If straddling the line go straight
                if(Grey[0] > 1350 && Grey[2] > 1350)//&& 1500< Grey[1] <1600 && 1500< Grey[2] <1600)
                {
                    goForward(70);
                }
                //If left sensor touches black turn left
                else if(Grey[0] <1350)
                {
                    turnLeft1(90);
                }
                //If right sensor touches black turn right
                else if (Grey[2] <1350)
                {
                    turnRight1(90);
                }
                //Remeasure distance
                distance=measure_distance();
                //Do it until close to the front
                if(distance<25 && !close)
                {
                    close=true;
                    startTime=true;
                }
                if(close && startTime )
                {
                    startTime=false;
                    startEnd=true;
                    start_clock= time_us_32();
                    end_clock = time_us_32();
                }
                if(close && !startTime && startEnd)
                {
                    end_clock = time_us_32();
                }
                time_clock=(end_clock - start_clock);
            }while(time_clock < 5000000);
        //stop();
        //return 0;
            turnLeft1(80);
            sleep_ms(700);
            close=false;
            startTime=false;
            startEnd=false;
            start_clock=0;
            end_clock=0;
            while(Grey[2]>1350)
            {
                for(int i=0; i<3; i++)
                {
                    adc_select_input(i);
                    Grey[i]= adc_read();
                }
            }
            
            turnRight();
            sleep_ms(200);
        
    }
	return 0;
}

void follow()
{
    goForward(25);
    uint32_t distance;
    for (int i=10; i<90; i+=10)
    {
        set_angle(i);
        distance= measure_distance();
        sleep_ms(200);
        if(distance < 20)
        {
            while(distance<20 || distance==100)
            {
                if(i<40)
                {
                    turnRight1(25);
                }
                else if(i>60)
                {
                    turnLeft1(25);
                }
                distance= measure_distance();
            }
        }
    }
    goForward(25);
    for (int i=90; i>10; i-=10)
    {
        set_angle(i);
        distance= measure_distance();
        sleep_ms(200);
        if(distance < 20)
        {
            while(distance<20 || distance ==100)
            {
                if(i<40)
                {
                    turnRight1(25);
                }
                else if(i>60)
                {
                    turnLeft1(25);
                }
                distance= measure_distance();
            }
        }
    }
    
}

/*if(i<40)
 {
     turnRight1(25);
     sleep_ms(100);
 }
 else if(i>60)
 {
     turnLeft1(25);
     sleep_ms(100);
 }*/


void followLine(int speed)
{
    uint32_t Grey[3];
    //Measuring each sensor's input
    do{
        for(int i=0; i<3; i++)
        {
            adc_select_input(i);
            Grey[i]= adc_read();
        }
        //If straddling the line go straight
        if(Grey[0] > 1350 && Grey[2] > 1350)//&& 1500< Grey[1] <1600 && 1500< Grey[2] <1600)
        {
            goForward(speed);
        }
        //If left sensor touches black turn left
        else if(Grey[0] <1350)
        {
            turnRight1(90);
        }
        //If right sensor touches black turn right
        else if (Grey[2] <1350)
        {
            turnRight1(90);
        }
        
        //Do it until all three touch white
    }while(Grey[0]<1500 || Grey[1]<1500 || Grey[2]<1500);
    return;
}

void mazeSolve()
{
    bool turn=false;
    do
    {
        uint32_t distance= measure_distance();
        if(distance < 15 && !turn)
        {
            stop();
            sleep_us(300);
            turnRight();
            sleep_ms(500);
            turn=true;
            stop();
            sleep_ms(200);
        }
        else if(distance < 15 && turn)
        {
            stop();
            sleep_us(300);
            turnLeft();
            sleep_ms(950);
            stop();
            sleep_ms(200);
        }
        else if(distance <20)
        {
            goForward(20);
            turn=false;
        }
        else if(distance <30)
        {
            goForward(40);
            turn=false;
        }
        else if(distance <45)
        {
            goForward(60);
            turn=false;
        }
        else
        {
            goForward(75);
            turn=false;
        }
        sleep_ms(100);
        
    }while(true);
    return;
}

uint32_t measure_distance() 
{
    bool timeOut=false;
    uint32_t start_time, end_time, time_out, time_out1, count;
    gpio_put(TRIGGER_PIN,0);
    sleep_us(5);

    // Send a 10us pulse to trigger the ultrasonic sensor
    gpio_put(TRIGGER_PIN, 1);
    busy_wait_us_32(10);
    gpio_put(TRIGGER_PIN, 0);
        
    time_out=time_us_32();
    while (!gpio_get(ECHO_PIN) && !timeOut)
    {
        start_time = time_us_32();
        time_out1=time_us_32();
        if((time_out1-time_out)>3000)
        {
            return 100;
        }
	}
            
    while (gpio_get(ECHO_PIN)) {
        //count++;
        end_time = time_us_32();
	}

    // Calculate the distance in cm
    return (end_time - start_time) / 58;
}

void turnLightOn(int state)
{
	gpio_init(19);
	gpio_set_dir(19, GPIO_OUT);
	if(state==0)
	{
		gpio_put(19, 0);
{
                set_angle(60);  
        }
        sleep_ms(500);
        }
        
}
                
void reverse(int speed)
{
	uint slice_numL, chanL;
	for(int i=10; i<18; i++)
        {
                slice_numL = pwm_gpio_to_slice_num(i);
                chanL= pwm_gpio_to_channel(i);
                pwm_set_enabled(slice_numL, true); 
                pwm_set_freq_duty(slice_numL, chanL, 20000, 0xFFFF);
                pwm_set_duty(slice_numL, chanL, (100-speed));
		i+=3;
                slice_numL = pwm_gpio_to_slice_num(i);
                chanL= pwm_gpio_to_channel(i);
                pwm_set_enabled(slice_numL, true);
                pwm_set_freq_duty(slice_numL, chanL, 20000, 0xFFFF);
                pwm_set_duty(slice_numL, chanL, (100-speed));
        }
        for(int i=11; i<18; i+=3)
        {
                slice_numL = pwm_gpio_to_slice_num(i);
                chanL= pwm_gpio_to_channel(i);
                pwm_set_enabled(slice_numL, true);
                pwm_set_freq_duty(slice_numL, chanL, 20000, 0xFFFF);
		i++;
                slice_numL = pwm_gpio_to_slice_num(i);
                chanL= pwm_gpio_to_channel(i);
                pwm_set_enabled(slice_numL, true);
                pwm_set_freq_duty(slice_numL, chanL, 20000, 0xFFFF);
        }
}

void stop()
{
	uint slice_numL, chanL;
	for(int i=10; i<18; i++)
        {
                slice_numL = pwm_gpio_to_slice_num(i);
                chanL= pwm_gpio_to_channel(i);
                pwm_set_enabled(slice_numL, true);
                pwm_set_freq_duty(slice_numL, chanL, 20000, 0xFFFF);
        }
}

void goForward(int speed)
{
	for(int i=10; i<18; i++)
        {
                uint slice_numL = pwm_gpio_to_slice_num(i);
                uint chanL= pwm_gpio_to_channel(i);
                pwm_set_enabled(slice_numL, true);
                pwm_set_freq_duty(slice_numL, chanL, 20000, 0xFFFF);
		i+=3;
        	slice_numL = pwm_gpio_to_slice_num(i);
                chanL= pwm_gpio_to_channel(i);
                pwm_set_enabled(slice_numL, true);
                pwm_set_freq_duty(slice_numL, chanL, 20000, 0xFFFF);
	}
	for(int i=11; i<18; i+=3)
        {
                uint slice_numL = pwm_gpio_to_slice_num(i);
                uint chanL= pwm_gpio_to_channel(i);
                pwm_set_enabled(slice_numL, true);
                pwm_set_freq_duty(slice_numL, chanL, 20000, 0xFFFF);
                pwm_set_duty(slice_numL, chanL, (100-speed));
		i++;
        	slice_numL = pwm_gpio_to_slice_num(i);
                chanL= pwm_gpio_to_channel(i);
                pwm_set_enabled(slice_numL, true);
                pwm_set_freq_duty(slice_numL, chanL, 20000, 0xFFFF);
                pwm_set_duty(slice_numL, chanL, (100-speed));
	}
}


void turnLeft()
{
        for(int i=10; i<18; i+=2)
        {
                uint slice_numL = pwm_gpio_to_slice_num(i);
                uint chanL= pwm_gpio_to_channel(i);
                pwm_set_enabled(slice_numL, true);
                pwm_set_freq_duty(slice_numL, chanL, 20000, 0xFFFF);
        }
        for(int i=11; i<19; i+=2)
        {
                uint slice_numL = pwm_gpio_to_slice_num(i);
                uint chanL= pwm_gpio_to_channel(i);
                pwm_set_enabled(slice_numL, true);
                pwm_set_freq_duty(slice_numL, chanL, 20000, 0x0000);
        }
}

void turnRight1(int speed)
{
    for(int i=10; i<18; i++)
        {
                uint slice_numL = pwm_gpio_to_slice_num(i);
                uint chanL= pwm_gpio_to_channel(i);
                pwm_set_enabled(slice_numL, true);
                pwm_set_freq_duty(slice_numL, chanL, 20000, 0xFFFF);
        i+=3;
            slice_numL = pwm_gpio_to_slice_num(i);
                chanL= pwm_gpio_to_channel(i);
                pwm_set_enabled(slice_numL, true);
                pwm_set_freq_duty(slice_numL, chanL, 20000, 0xFFFF);
    }
    for(int i=11; i<18; i+=3)
        {
                uint slice_numL = pwm_gpio_to_slice_num(i);
                uint chanL= pwm_gpio_to_channel(i);
                pwm_set_enabled(slice_numL, true);
                pwm_set_freq_duty(slice_numL, chanL, 20000, 0xFFFF);
                pwm_set_duty(slice_numL, chanL, (speed));
        i++;
            slice_numL = pwm_gpio_to_slice_num(i);
                chanL= pwm_gpio_to_channel(i);
                pwm_set_enabled(slice_numL, true);
                pwm_set_freq_duty(slice_numL, chanL, 20000, 0xFFFF);
                pwm_set_duty(slice_numL, chanL, (100-speed));
    }
}


void turnRight() 
{
        for(int i=11; i<19; i+=2)
        {
                uint slice_numL = pwm_gpio_to_slice_num(i);
                uint chanL= pwm_gpio_to_channel(i);
                pwm_set_enabled(slice_numL, true);
                pwm_set_freq_duty(slice_numL, chanL, 20000, 0xFFFF);
        }                                                
        for(int i=10; i<18; i+=2)                 
        {
                uint slice_numL = pwm_gpio_to_slice_num(i);
                uint chanL= pwm_gpio_to_channel(i);
                pwm_set_enabled(slice_numL, true); 
                pwm_set_freq_duty(slice_numL, chanL, 20000, 0x0000);
        }                                               
}

void turnLeft1(int speed)
{
    for(int i=10; i<18; i++)
        {
                uint slice_numL = pwm_gpio_to_slice_num(i);
                uint chanL= pwm_gpio_to_channel(i);
                pwm_set_enabled(slice_numL, true);
                pwm_set_freq_duty(slice_numL, chanL, 20000, 0xFFFF);
                
        i+=3;
            slice_numL = pwm_gpio_to_slice_num(i);
                chanL= pwm_gpio_to_channel(i);
                pwm_set_enabled(slice_numL, true);
                pwm_set_freq_duty(slice_numL, chanL, 20000, 0xFFFF);
                
    }
    for(int i=11; i<18; i+=3)
        {
                uint slice_numL = pwm_gpio_to_slice_num(i);
                uint chanL= pwm_gpio_to_channel(i);
                pwm_set_enabled(slice_numL, true);
                pwm_set_freq_duty(slice_numL, chanL, 20000, 0xFFFF);
                pwm_set_duty(slice_numL, chanL, (100-speed));
               
        i++;
            slice_numL = pwm_gpio_to_slice_num(i);
                chanL= pwm_gpio_to_channel(i);
                pwm_set_enabled(slice_numL, true);
                pwm_set_freq_duty(slice_numL, chanL, 20000, 0xFFFF);
            pwm_set_duty(slice_numL, chanL, (speed));
                
    }
}

void servoInit()
{
	uint slice_num= pwm_gpio_to_slice_num(18);
	uint chan18= pwm_gpio_to_channel(18);
	pwm_set_enabled(slice_num, true);
        pwm_set_freq_duty(slice_num, chan18, 50, 0);	
}

void set_angle(uint percent)
{
	uint slice_num= pwm_gpio_to_slice_num(18);
        uint chan18= pwm_gpio_to_channel(18);
	pwm_set_dutyS(slice_num, chan18, percent*10+250);
}

void pwm_set_dutyS(uint slice_num, uint chan, int d)
{
	pwm_set_chan_level(slice_num, chan, pwm_get_wrap(slice_num) * d / 
10000);
}

int getDistance()
{
	//uint pulse_send, pulse_received, pulse_duration;
	
	gpio_put(6, 1);
	sleep_us(10);	
	gpio_put(6, 0);
	
	uint width = 0;
	int timeout= 26100;
	
	while(gpio_get(7)==0) 
	{
		tight_loop_contents();
	}
	while(gpio_get(7)==1)
	{
		width++;
		sleep_us(1);
		if(width > timeout)
		{
			return 0;
		}
	}
	
	uint64_t distance = (width / 29 / 2);
	
	return distance;	
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

void gpio_init_all()
{
	for(int i=0; i<21; i++)
	{
		 gpio_init(i);
	}
}

