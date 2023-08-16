#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "pindefs.h"
#include "ppm.pio.h"

static void setupBtn(uint pin) {
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin);
}

static void setupLed(uint pin) {
    gpio_init(pin);
    gpio_set_drive_strength(pin, GPIO_DRIVE_STRENGTH_4MA);
    gpio_set_dir(pin, GPIO_OUT);
}

uint slice_num;

void output_mode_ch(uint value) {
    pwm_set_chan_level(slice_num, PWM_MODE_CHAN, value);
    ppm_set_value(PPM_CH_NAV_MODE, value);
}


int main() {
    setupLed(LED_MODE_1_GPIO);
    setupLed(LED_MODE_2_GPIO);
    setupLed(LED_MODE_3_GPIO);
    setupLed(LED_MODE_4_GPIO);

    // Tell GPIO 0 and 1 they are allocated to the PWM
    gpio_set_function(PWM_MODE_GPIO, GPIO_FUNC_PWM);
    gpio_set_function(PWM_STEER_GPIO, GPIO_FUNC_PWM);

    setupBtn(BTN_MODE1_GPIO);
    setupBtn(BTN_MODE2_GPIO);
    setupBtn(BTN_MODE3_GPIO);
    setupBtn(BTN_MODE4_GPIO);

    setupBtn(BTN_L_GPIO);
    setupBtn(BTN_L_FINE_GPIO);
    setupBtn(BTN_R_GPIO);
    setupBtn(BTN_R_FINE_GPIO);


    // Find out which PWM slice is connected to mode GPIO (it's slice 0)
    slice_num = pwm_gpio_to_slice_num(PWM_MODE_GPIO);

    // Set PWM freq 100 Hz
    pwm_set_clkdiv(slice_num, 125);
    pwm_set_wrap(slice_num, 10000);

    // Set the PWM running
    pwm_set_enabled(slice_num, true);
    uint mode = 0;
    uint steerPwm = 0;
    pwm_set_chan_level(slice_num, PWM_MODE_CHAN, 0);

    ppm_program_init(0, PPM_OUT_GPIO);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (1) {
        uint newMode = 0;
        if (gpio_get(BTN_MODE4_GPIO) == 0) {
            newMode = 4;
        }
        if (gpio_get(BTN_MODE3_GPIO) == 0) {
            newMode = 3;
        } else if (gpio_get(BTN_MODE2_GPIO) == 0) {
            newMode = 2;
        } else if (gpio_get(BTN_MODE1_GPIO) == 0) {
            newMode = 1;
        }
        if (newMode == 0 && mode == 0) {
            newMode = 1;
        }
        if (newMode != 0 && newMode != mode) {
            mode = newMode;
            switch (mode) {
                case 1:
                    gpio_put(LED_MODE_1_GPIO, true);
                    gpio_put(LED_MODE_2_GPIO, false);
                    gpio_put(LED_MODE_3_GPIO, false);
                    gpio_put(LED_MODE_4_GPIO, false);
                    output_mode_ch(1300);
                    break;
                case 2:
                    gpio_put(LED_MODE_1_GPIO, true);
                    gpio_put(LED_MODE_2_GPIO, true);
                    gpio_put(LED_MODE_3_GPIO, false);
                    gpio_put(LED_MODE_4_GPIO, false);
                    output_mode_ch(1400);
                    break;
                case 3:
                    gpio_put(LED_MODE_1_GPIO, true);
                    gpio_put(LED_MODE_2_GPIO, true);
                    gpio_put(LED_MODE_3_GPIO, true);
                    gpio_put(LED_MODE_4_GPIO, false);
                    output_mode_ch(1550);
                    break;
                case 4:
                    gpio_put(LED_MODE_1_GPIO, true);
                    gpio_put(LED_MODE_2_GPIO, true);
                    gpio_put(LED_MODE_3_GPIO, true);
                    gpio_put(LED_MODE_4_GPIO, true);
                    output_mode_ch(1680);
                    break;
                default:
                    gpio_put(LED_MODE_1_GPIO, false);
                    gpio_put(LED_MODE_2_GPIO, false);
                    gpio_put(LED_MODE_3_GPIO, false);
                    gpio_put(LED_MODE_4_GPIO, false);
                    output_mode_ch(0);
                    break;
            }
        }
        uint newSteer;
        if (gpio_get(BTN_L_GPIO) == 0) {
            newSteer = 1300;
        } else if (gpio_get(BTN_L_FINE_GPIO) == 0) {
            newSteer = 1450;
        } else if (gpio_get(BTN_R_FINE_GPIO) == 0) {
            newSteer = 1550;
        } else if (gpio_get(BTN_R_GPIO) == 0) {
            newSteer = 1700;
        } else {
            newSteer = 1497;
        }
        if (newSteer != steerPwm) {
            steerPwm = newSteer;
            pwm_set_chan_level(slice_num, PWM_STEER_CHAN, steerPwm);
            ppm_set_value(PPM_CH_STEER, steerPwm);
        }
    }
#pragma clang diagnostic pop
}
