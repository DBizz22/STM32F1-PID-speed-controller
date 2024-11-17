#ifndef MY_PID_H_
#define MY_PID_H_

typedef struct
{
    float sampling_time, time_constant;
    float kp, ki, kd;
    float proportional, integrator, derivative;
    float previous_error;
    float max_output, min_output;
    float output;
} PID_Controller;

void pid_init(PID_Controller *pid, float sampling_time, float time_constant, float max_output, float min_output);
void pid_update(PID_Controller *pid, float setpoint, float measured_value);

#endif