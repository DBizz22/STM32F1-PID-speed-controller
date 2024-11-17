#ifndef MY_TIMER_H_
#define MY_TIMER_H_
#include "stm32f10x.h"
#include "my_gpio.h"
#include "my_clock.h"
#include "my_dma.h"

//Timer name constants
#define T1 0
#define T2 1
#define T3 2
#define T4 3
#ifndef TIM5
#define T5 4
#endif
#ifndef TIM6
#define T6 5
#endif
#ifndef TIM7
#define T7 6
#endif

//Timer channels
#define TIM1_CH1 PA8
#define TIM1_CH2 PA9
#define TIM1_CH3 PA10
#define TIM1_CH4 PA11

#define TIM2_CH1 PA0
#define TIM2_CH2 PA1
#define TIM2_CH3 PA2
#define TIM2_CH4 PA3

#define TIM3_CH1 PA6
#define TIM3_CH2 PA7
#define TIM3_CH3 PB0
#define TIM3_CH4 PB1

#define TIM4_CH1 PB6
#define TIM4_CH2 PB7
#define TIM4_CH3 PB8
#define TIM4_CH4 PB9

//Timer Encoder MODE
#define ENCODER_MODE_1 1
#define ENCODER_MODE_2 2
#define ENCODER_MODE_3 3

//Timer input capture trigger
#define CAPTURE_RISING 1
#define CAPTURE_FALLING 2

//capture/compare modes
//output modes
#define FROZEN 0            //No effect
#define ACTIVE 1            //active when cnt==ccr
#define INACTIVE 2         //inactive when cnt==ccr
#define TOGGLE 3           //toggles when cnt==ccr
#define FORCE_INACTIVE 4    //force low
#define FORCE_ACTIVE 5      //force high
#define PWM1 6              //pwm mode 1, active when cnt<ccr else inactive
#define PWM2 7              //pwm mode 2, inactive when cnt<ccr else active

void initTDELAY(const uint8_t timer);
void timerDELAYUS(const uint8_t timer,uint16_t us);
void timerDELAYMS(const uint8_t timer,uint16_t ms);
void timerINT_US(const uint8_t timer,uint8_t us);
void timerINT_50MS(const uint8_t timer,uint8_t ms);
void timer_ch_PWMH(const uint8_t timer_ch,uint16_t ccr,uint16_t arr,bool dma_burst);
void timer_ch_PWML(const uint8_t timer_ch,uint16_t ccr,uint16_t arr,bool dma_burst);
void timer_ch_HIGH(const uint8_t timer_ch,uint16_t ccr,uint16_t arr,bool dma_burst);
void timer_ch_LOW(const uint8_t timer_ch,uint16_t ccr,uint16_t arr,bool dma_burst);
void timer_ch_TOGGLE(const uint8_t timer_ch,uint16_t ccr,uint16_t arr,bool dma_burst);
void timer_ch_FORCEHIGH(const uint8_t timer_ch,uint16_t ccr,uint16_t arr,bool dma_burst);
void timer_ch_FORCELOW(const uint8_t timer_ch,uint16_t ccr,uint16_t arr,bool dma_burst);
void timer_encoder_init(const uint8_t timer,uint8_t mode,uint8_t TI1_CH, uint8_t TI2_CH,uint8_t trigger);
void startTIMER(const uint8_t timer);
void stopTIMER(const uint8_t timer);

#endif
