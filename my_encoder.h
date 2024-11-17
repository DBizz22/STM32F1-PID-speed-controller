#ifndef MY_ENCODER_H_
#define MY_ENCODER_H_
#include "my_pid.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    uint8_t sensorA_pin;
    uint8_t sensorB_pin;
    volatile uint16_t prev_pulse_count;
    volatile uint16_t pulse_count;
    bool dir; // true for cw, false for ccw
    float RPS;
    float prev_RPM;
    float RPM;

    float filtered_RPM;
    int8_t PPR;
    int16_t max_RPM; //@param for no load speed at given voltage
    // uint16_t torque;
} Encoder;

void init_encoder(Encoder *enc, uint8_t encoder_timer, uint8_t pwm_ch);
void update_RPM(Encoder *enc, float ms);
// void controller_output(PID_Controller* pid,Encoder* enc);
void controller_dir(PID_Controller *pid, Encoder *enc, uint8_t IN1, uint8_t IN2);

#endif