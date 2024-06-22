#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

#define TRIGGER_PIN 6
#define ECHO_PIN 7
#define LED_PIN 16

void setup_pins() {
    gpio_init(TRIGGER_PIN);
    gpio_set_dir(TRIGGER_PIN, GPIO_OUT);

    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
}

uint32_t measure_distance() {
    uint32_t start_time, end_time;

    // Send a 10us pulse to trigger the ultrasonic sensor
    gpio_put(TRIGGER_PIN, 1);
    busy_wait_us_32(10);
    gpio_put(TRIGGER_PIN, 0);

    while (!gpio_get(ECHO_PIN)) {
        start_time = time_us_32();
    }

    while (gpio_get(ECHO_PIN)) {
        end_time = time_us_32();
    }

    // Calculate the distance in cm
    return (end_time - start_time) / 58;
}

int main() {
    stdio_init_all();
    setup_pins();

    printf("Ultrasonic sensor and LED control\n");

    while (true) {
        uint32_t distance = measure_distance();
        printf("Distance: %u cm\n", distance);

        if (distance < 20) {
            gpio_put(LED_PIN, 1); // Turn on LED
        } else {
            gpio_put(LED_PIN, 0); // Turn off LED
        }

        sleep_ms(500); // Wait for 500ms before taking the next measurement
    }

    return 0;
}

