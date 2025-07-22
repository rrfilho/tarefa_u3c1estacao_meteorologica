#include "leds.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define RED_PWM_LED 13
#define CLOCK_DIVIDER 2.0
#define WRAP 22
#define OFF 0
#define ON 11

void leds_init() {
    gpio_set_function(RED_PWM_LED, GPIO_FUNC_PWM);
    unsigned int slice = pwm_gpio_to_slice_num(RED_PWM_LED);
    pwm_set_clkdiv(slice, CLOCK_DIVIDER);
    pwm_set_wrap(slice, WRAP);
    pwm_set_gpio_level(RED_PWM_LED, OFF);
    pwm_set_enabled(slice, true);
}

void leds_set_red(bool status) {
    pwm_set_gpio_level(RED_PWM_LED, status ? ON : OFF);
}