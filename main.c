#include "fonts.h"
#include "math.h"
#include "my_clock.h"
#include "my_encoder.h"
#include "my_gpio.h"
#include "my_pid.h"
#include "my_systick.h"
#include "my_timer.h"
#include "remote.h"
#include "st7735.h"
#include "stm32f10x.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/*PIN CONFIGURATION
 A->PA9
 B->PA8
 PWM PA7

 IN1 PC0
 IN2 PC1
*/

#define IN1 PC0
#define IN2 PC1
// #define REMOTE_PIN PA10

uint16_t delay_calc;
uint16_t display_counter;
int16_t set_point = -1500;

char string[100];
PID_Controller pid;
Encoder enc;

remote_controller remote;
int16_t input_num;
bool key_pressed = false;
void remote_cmd(const remote_controller *remote);

int main()
{
    SystemInit();
    setCLK(S72MHZ);
    // setCLK(S8MHZ);

    //@param format for pid_controller, add output limits as no load speed limit
		pid.kp =0.08;//0.00038;//0.001; //0.2;
    pid.ki =0.05;//0.005;//0.000001;//0.00001;//0.000001; //0.02;
    pid.kd =0;//0.000001;//0.0002;

    enc.sensorA_pin = PA8;
    enc.sensorB_pin = PA9;
    enc.PPR = 100; // 100;
    enc.max_RPM = 6000;

    pid_init(&pid, 0.01, 0.0079577, 100, 8); //* 10-5 ms
    init_encoder(&enc, T1, TIM4_CH4);         //@param PWM at 2KHz/0.5 miliseconds

    outMODE(IN1);
    outMODE(IN2);
    pinOFF(IN1);
    pinOFF(IN2);

    ST7735_Init(72); // 8
    ST7735_FillScreen(ST7735_COLOR_WHITE);
    sprintf(string, "PID control            Set point: %d RPM                Cur point: %d RPM", set_point,
            (int)enc.RPM);
    ST7735_WriteString(0, 0, string, Font_11x18, ST7735_COLOR_BLACK, ST7735_COLOR_WHITE);

    delay_calc = 0;
    display_counter = 0;

    remote.pin = PA10;
    remote_init(&remote);

    startTIMER(T1);

    // startTIMER(T3);
    startTIMER(T4);
    systickCONFIG(72); // 8
    startTICK();
    timerINT_50MS(T2, 10);
    startTIMER(T2);
    while (true)
    {
        if (delay_calc == 10) //*every 20 ms
        {
            enc.prev_pulse_count = enc.pulse_count;
            enc.pulse_count = TIM1->CNT;
            update_RPM(&enc, 10);
            pid_update(&pid, set_point, enc.filtered_RPM);
            controller_dir(&pid, &enc, IN1, IN2);
            TIM4->CCR4 = fabs(round(pid.output));
            //  TIM4->CCR4=20;//5ms

            delay_calc = 0;
        }

        if (display_counter >= 15000) //*every 500 msec
        {
					set_point+=200;
					if(set_point>=1500)
						set_point=-1500;
            /*sprintf(string, "PID control            Set point: %d RPM                Cur point: %d RPM", set_point,
                    (int)(enc.filtered_RPM));
            ST7735_WriteString(0, 0, string, Font_11x18, ST7735_COLOR_BLACK, ST7735_COLOR_WHITE);*/
            display_counter = 0;
        }

        if (key_pressed)
        {
            remote_cmd(&remote);
            key_pressed = false;
        }
    }
    return 0;
}

void TIM2_IRQHandler(void)
{
    TIM2->SR &= ~TIM_SR_UIF;
    delay_calc = 10;
    display_counter += 10;
}

void EXTI15_10_IRQHandler(void)
{
    update_remote_key(&remote, NEC);
    key_pressed = true;
    // while(!pinR(remote.pin));
    EXTI->PR |= EXTI_PR_PR10;
}

void remote_cmd(const remote_controller *remote)
{
    if (remote->key == '\0')
        return;
    switch (remote->key)
    {
    case '*':
    case '#':
        input_num = 0;
        break;
    case '>':
        input_num *= input_num < 0 ? -1 : 1;
        break;
    case '<':
        input_num *= input_num > 0 ? -1 : 1;
        break;
    case '^':
        input_num++;
        break;
    case 'D':
        input_num--;
        break;
    case 'k':
        set_point = input_num;
        break;
    default:
        input_num *= 10;
        input_num += (remote->key - '0');
        break;
    }
}