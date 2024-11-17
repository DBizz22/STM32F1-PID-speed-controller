#include "my_pid.h"
#include <math.h>

void pid_init(PID_Controller *pid, float sampling_time, float time_constant, float max_output, float min_output)
{
    pid->sampling_time = sampling_time;
    pid->time_constant = time_constant;
    pid->proportional = pid->integrator = pid->derivative = 0.0f;
    pid->previous_error = 0.0f;
    pid->max_output = max_output;
    pid->min_output = min_output;
    pid->output = 0.0f;
}

float compute_integration(PID_Controller *pid, float error)
{
    float term1 = 0.5f * pid->ki * pid->sampling_time * (error); //+ pid->previous_error); //*
    return term1;
    // return term1 + pid->integrator;
}

void dynamic_integrator_clamp(PID_Controller *pid, float proportional)
{
    float max_integrator_limit, min_integrator_limit;

    max_integrator_limit = (pid->max_output > proportional) ? pid->max_output - proportional : 0.0f;

    min_integrator_limit = (pid->min_output < proportional) ? pid->min_output - proportional : 0.0f;

    if (pid->integrator > max_integrator_limit)
        pid->integrator = max_integrator_limit;
    else if (pid->integrator < min_integrator_limit)
        pid->integrator = min_integrator_limit;
}

float compute_derivative(PID_Controller *pid, float error)
{
    float term1 = 2 * pid->kd * (error - pid->previous_error);
    float term2 = (2 * pid->time_constant - pid->sampling_time) * pid->derivative;
    return (term1 + term2) / (2 * pid->time_constant + pid->sampling_time);
}

void pid_update(PID_Controller *pid, float setpoint, float measured_value)
{

    float error = setpoint - measured_value;

    pid->proportional = pid->kp * (error - pid->previous_error); //*

    pid->integrator = compute_integration(pid, error);

    dynamic_integrator_clamp(pid, pid->proportional);

    pid->derivative = compute_derivative(pid, error);

    //*****************added****************
    // Limit the output to the from bouncing between sign
    // values depending on the set point
    // float new_output = (pid->proportional + pid->integrator + pid->derivative);
    // if ((setpoint && new_output < 0) || (setpoint < 0 && new_output > 0))
    //   pid->output += new_output;
    // else
    pid->output += (pid->proportional + pid->integrator + pid->derivative);

    if (fabs(pid->output) > pid->max_output)
        pid->output = pid->max_output * (pid->output > 0 ? 1 : -1);
    else if (fabs(pid->output) < pid->min_output)
        pid->output = pid->min_output * (pid->output > 0 ? 1 : -1);

    pid->previous_error = error;
    return;
}