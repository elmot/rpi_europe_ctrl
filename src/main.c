/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// Output PWM signals on pins 0 and 1

#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define PWMA_GPIO 16
#define PWMB_GPIO 17

int main() {

    // Tell GPIO 0 and 1 they are allocated to the PWM
    gpio_set_function(PWMA_GPIO, GPIO_FUNC_PWM);
    gpio_set_function(PWMB_GPIO, GPIO_FUNC_PWM);

    // Find out which PWM slice is connected to GPIO 0 (it's slice 0)
    uint slice_num = pwm_gpio_to_slice_num(PWMA_GPIO);

    // Set period of 4 cycles (0 to 3 inclusive)
    pwm_set_clkdiv(slice_num, 125);
    pwm_set_wrap(slice_num, 10000);
    // Set channel A output high for one cycle before dropping
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 1500);
    // Set initial B output high for three cycles before dropping
    pwm_set_chan_level(slice_num, PWM_CHAN_B, 2000);
    // Set the PWM running
    pwm_set_enabled(slice_num, true);

    // Note we could also use pwm_set_gpio_level(gpio, x) which looks up the
    // correct slice and channel for a given GPIO.
    while(1) {
        sleep_ms(700);
        pwm_set_chan_level(slice_num, PWM_CHAN_A, 1200);
        sleep_ms(700);
        pwm_set_chan_level(slice_num, PWM_CHAN_A, 1500);
    }
}
