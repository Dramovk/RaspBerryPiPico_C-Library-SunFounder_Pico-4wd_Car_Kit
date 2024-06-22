#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "unistd.h"

void stop() {
    for (int i = 10; i <= 17; i++) {
        gpio_set_function(i, GPIO_FUNC_PWM);
        uint slice_num = pwm_gpio_to_slice_num(i);
        pwm_set_clkdiv(slice_num, 16.0f);
        pwm_set_wrap(slice_num, 65535);
        pwm_set_chan_level(slice_num, pwm_gpio_to_channel(i), 65535);
        pwm_set_enabled(slice_num, true);
    }
}

int main() {
    stdio_init_all();

    gpio_init(13);
    gpio_init(12);

    uint slice_num_A = pwm_gpio_to_slice_num(13);
    uint slice_num_B = pwm_gpio_to_slice_num(12);
    pwm_set_clkdiv(slice_num_A, 16.0f);
    pwm_set_clkdiv(slice_num_B, 16.0f);
    pwm_set_wrap(slice_num_A, 65535);
    pwm_set_wrap(slice_num_B, 65535);
    pwm_set_chan_level(slice_num_A, pwm_gpio_to_channel(13), 65535);
    pwm_set_chan_level(slice_num_B, pwm_gpio_to_channel(12), 43690);
    pwm_set_enabled(slice_num_A, true);
    pwm_set_enabled(slice_num_B, true);

    gpio_init(14);
    gpio_init(15);

    gpio_set_dir(14, GPIO_OUT);
    gpio_set_dir(15, GPIO_OUT);
    
    gpio_put(14,0);
    gpio_put(15,1);
    
   
    sleep_ms(5000);

    stop();

    return 0;
}
