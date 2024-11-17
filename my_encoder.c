#include "my_encoder.h"
#include "my_gpio.h"
#include "my_pid.h"
#include "my_timer.h"
#include "stm32f10x.h"
#include <stdbool.h>
#include <stdint.h>

void init_encoder(Encoder *enc, uint8_t encoder_timer, uint8_t pwm_ch)
{
    timer_encoder_init(encoder_timer, ENCODER_MODE_3, enc->sensorB_pin, enc->sensorA_pin, CAPTURE_RISING);
    enc->prev_pulse_count = enc->pulse_count = 0;
    enc->RPM = enc->RPS = enc->prev_RPM = enc->filtered_RPM = 0;

    // enc->prev_RPS=enc->filtered_RPS=0;
    enc->dir = true;

    // Initialize timer for continuous PWM conv36000

    timer_ch_PWMH(pwm_ch, 0, 100, false); //* 2000 for 200ms timeout
    TIM4->PSC = 7200 - 1;                 //*100 us
}

float get_pulse_per_sec(Encoder *enc, float ms)
{
    uint16_t count;
    float pulse_per_sec;

    if (enc->dir && enc->prev_pulse_count > enc->pulse_count)
        count = TIM_ARR_ARR + enc->pulse_count - enc->prev_pulse_count;
    else if (!enc->dir && enc->prev_pulse_count < enc->pulse_count)
        count = TIM_ARR_ARR + enc->prev_pulse_count - enc->pulse_count;
    else if (enc->dir)
        count = enc->pulse_count - enc->prev_pulse_count;
    else
        count = enc->prev_pulse_count - enc->pulse_count;

    count = count / 4;
    pulse_per_sec = count / (ms * 0.001);
    return pulse_per_sec;
}

void filter_RPM(Encoder *enc)
{
    //*LOW pass filter for RPM, fc=5HZ, sampling rate=20ms
    // enc->filtered_RPM=0.8627*enc->RPM+0.8627*enc->prev_RPM-0.7254*enc->filtered_RPM; 0.2
    // enc->filtered_RPM=0.7585*enc->RPM+0.7585*enc->prev_RPM-0.5171*enc->filtered_RPM; //0.1
    // enc->filtered_RPM = 0.5569 * enc->RPM + 0.5569 * enc->prev_RPM - 0.1137 * enc->filtered_RPM; 0.02
    enc->filtered_RPM = 0.3859 * enc->RPM + 0.3859 * enc->prev_RPM + 0.2283 * enc->filtered_RPM;
}

void update_RPM(Encoder *enc, float ms)
{
    float pulse_per_sec = get_pulse_per_sec(enc, ms);
    // enc->prev_RPS=enc->RPS;//*
    enc->RPS = pulse_per_sec / enc->PPR;
    // filter_RPS(enc);//*
    enc->prev_RPM = enc->RPM;
    enc->RPM = enc->RPS * 60; //* RPS
    enc->RPM = enc->dir ? enc->RPM : -1 * enc->RPM;
    filter_RPM(enc);
    // enc->filtered_RPM = enc->dir ? enc->filtered_RPM : -1 * enc->filtered_RPM;
}

void controller_dir(PID_Controller *pid, Encoder *enc, uint8_t IN1, uint8_t IN2)
{
    if (pid->output > 0)
    {
        // TODO: forward direction, CW direction
        enc->dir = true;
        pinOFF(IN1);
        pinON(IN2);
    }
    else if (pid->output < 0)
    {
        // TODO: backward direction, CCW direction
        enc->dir = false;
        pinON(IN1);
        pinOFF(IN2);
    }
    else if (pid->output == 0)
    {
        // TODO: stall the motor
        pinON(IN1);
        pinON(IN2);
    }
}

/*void controller_output(PID_Controller* pid,Encoder* enc)
{
    enc->torque=fabs(round(pid->output/enc->max_RPM*36000));
}*/
