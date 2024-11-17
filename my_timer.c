#include "my_timer.h"
#include "my_clock.h"
#include "my_dma.h"
#include "my_gpio.h"
#include "stm32f10x.h"
#include <complex.h>
#include <stdlib.h>
#include <stdint.h>

extern uint16_t PCLK2;

TIM_TypeDef* getTIMER(const uint8_t timer)
{
    switch (timer) 
    {
        case T1 :    return TIM1;
        case T2 :    return TIM2;
        case T3 :    return TIM3;
        case T4 :    return TIM4;
    }
}

uint8_t getTIMER_CH_NO(const uint8_t timer_ch)
{
    switch (timer_ch) 
    {
        case TIM1_CH1 :
        case TIM2_CH1 :
        case TIM3_CH1 :
        case TIM4_CH1 :     return 1;
                            
        case TIM1_CH2 :
        case TIM2_CH2 :
        case TIM3_CH2 :
        case TIM4_CH2 :     return 2;
                            
        case TIM1_CH3 :
        case TIM2_CH3 :
        case TIM3_CH3 :
        case TIM4_CH3 :     return 3;
                            
        case TIM1_CH4 :
        case TIM2_CH4 :
        case TIM3_CH4 :
        case TIM4_CH4 :     return 4;    
    }
}

uint8_t getDMA_CH(const uint8_t timer_ch)
{
    switch (timer_ch) 
    {
        case TIM1_CH1 :     return DMA1_CH2;
        case TIM1_CH3 :     return DMA1_CH6;
        case TIM1_CH4 :     return DMA1_CH4;

        case TIM2_CH1 :     return DMA1_CH5;
        case TIM2_CH2 :     return DMA1_CH7;
        case TIM2_CH3 :     return DMA1_CH1;
        case TIM2_CH4 :     return DMA1_CH7;

        case TIM3_CH1 :     return DMA1_CH6;
        case TIM3_CH3 :     return DMA1_CH2;
        case TIM3_CH4 :     return DMA1_CH3;

        case TIM4_CH1 :     return DMA1_CH1;
        case TIM4_CH2 :     return DMA1_CH4;
        case TIM4_CH3 :     return DMA1_CH5; 
    }
}

void timerCONFIG(const uint8_t timer,const uint16_t psc,const uint16_t arr)
{
    if(timer==T1)
        RCC->APB2ENR|=(0x01<<11);
    else
        RCC->APB1ENR|=(0x01<<(timer-1));

    TIM_TypeDef* tim=getTIMER(timer);
    tim->PSC=psc-1;
    tim->ARR=arr;
}

void startTIMER(const uint8_t timer)
{
    TIM_TypeDef* tim=getTIMER(timer);
    tim->CNT=0;
    tim->CR1|=(0x01);
}

void stopTIMER(const uint8_t timer)
{
    TIM_TypeDef* tim=getTIMER(timer);
    tim->CR1&=~(0x01);
}

void initTDELAY(const uint8_t timer)
{
    timerCONFIG(timer,1,PCLK2);
    TIM_TypeDef* tim=getTIMER(timer);
    tim->CR1|=(0x01<<3);                    //set OPM
}

void timerDELAYUS(const uint8_t timer,uint16_t us)
{   
    TIM_TypeDef* tim=getTIMER(timer);
    tim->CNT=0;
    tim->CR1|=(0x01);                           //start counter
    while(us)          
    {
        while((tim->CR1&0x01)==0x01);           //wait for counter to stop
        tim->CR1|=(0x01);                       //restart counter
        us--;
    }
}

void timerDELAYMS(const uint8_t timer,uint16_t ms)
{
    while(ms)
    {
        timerDELAYUS(timer,1000);
        ms--;
    }
}

void timerINT_50MS(const uint8_t timer,uint8_t ms)
{
    /*if(PCLK2==S72MHZ)
        timerCONFIG(timer,2,ms*(PCLK2/2*1000));
    else
        timerCONFIG(timer,1,PCLK2*1000);*/

    timerCONFIG(timer,PCLK2,ms*1000);          //at 1Mhz

    TIM_TypeDef* tim=getTIMER(timer);
    tim->CR1|=(0x01<<2);                    //Set update request source
    tim->DIER|=(0x01);                      //Enable update interrupt
    __disable_irq();
    switch (timer)                          //NOTE: Can increase the timers later
    {
        case T1 :   NVIC_EnableIRQ(TIM1_UP_IRQn);
                    break;
        case T2 :   NVIC_EnableIRQ(TIM2_IRQn);
                    break;
        case T3 :   NVIC_EnableIRQ(TIM3_IRQn);
                    break;
        case T4 :   NVIC_EnableIRQ(TIM4_IRQn);
                    break;
    }
    __enable_irq();    
}


void timerINT_US(const uint8_t timer,uint8_t us)
{
    //timerCONFIG(timer,1,PCLK2);
    timerCONFIG(timer,PCLK2,us*PCLK2);

    TIM_TypeDef* tim=getTIMER(timer);
    tim->CR1|=(0x01<<2);                    //Set update request source
    tim->DIER|=(0x01);                      //Enable update interrupt
    __disable_irq();
    switch (timer)                          //NOTE: Can increase the timers later
    {
        case T1 :   NVIC_EnableIRQ(TIM1_UP_IRQn);
                    break;
        case T2 :   NVIC_EnableIRQ(TIM2_IRQn);
                    break;
        case T3 :   NVIC_EnableIRQ(TIM3_IRQn);
                    break;
        case T4 :   NVIC_EnableIRQ(TIM4_IRQn);
                    break;
    }
    __enable_irq();    
}


void ccmCONFIG(const uint8_t timer_ch,uint8_t mode,uint16_t ccr,uint16_t arr,bool dma_burst)
{
    uint8_t timer;
    switch (timer_ch) 
    {
        case TIM1_CH1 :
        case TIM1_CH2 :
        case TIM1_CH3 :
        case TIM1_CH4 :     timer=T1;
                            break;
        case TIM2_CH1 :
        case TIM2_CH2 :
        case TIM2_CH3 :
        case TIM2_CH4 :     timer=T2;
                            break;
        case TIM3_CH1 :
        case TIM3_CH2 :
        case TIM3_CH3 :
        case TIM3_CH4 :     timer=T3;
                            break;
        case TIM4_CH1 :
        case TIM4_CH2 :
        case TIM4_CH3 :
        case TIM4_CH4 :     timer=T4;    
    }
    timerCONFIG(timer,1,arr);

    TIM_TypeDef* tim;
    if (timer_ch>=TIM1_CH1&&timer_ch<=TIM1_CH4) 
        tim=getTIMER(T1);
    else if (timer_ch>=TIM2_CH1&&timer_ch<=TIM2_CH4) 
        tim=getTIMER(T2);
    else if ( timer_ch>=TIM3_CH1&&timer_ch<=TIM3_CH2) 
        tim=getTIMER(T3);
    else if (timer_ch>=TIM3_CH3&&timer_ch<=TIM3_CH4)
        tim=getTIMER(T3);
    else if (timer_ch>=TIM4_CH1&&timer_ch<=TIM4_CH4) 
        tim=getTIMER(T4);

    uint8_t ch=getTIMER_CH_NO(timer_ch);

/***************Config of capture/compare register******************/
//Preload enabled
    if(ch==1)
    {
        tim->CCMR1&=~TIM_CCMR1_CC1S;
        tim->CCMR1&=~(0x07<<4);
        tim->CCMR1|=(mode<<4);
        tim->CCMR1|=(0x01<<3);
        tim->CCR1&=~(0xffff);
        tim->CCR1|=ccr;
    }
    else if (ch==2)
    {
        tim->CCMR1&=~TIM_CCMR1_CC2S;
        tim->CCMR1&=~(0x07<<12);
        tim->CCMR1|=(mode<<12);
        tim->CCMR1|=(0x01<<11);
        tim->CCR2&=~(0xffff);
        tim->CCR2|=ccr;
    }
    else if (ch==3)
    {
        tim->CCMR2&=~TIM_CCMR2_CC3S;
        tim->CCMR2&=~(0x07<<4);
        tim->CCMR2|=(mode<<4);
        tim->CCMR2|=(0x01<<3);               
        tim->CCR3&=~(0xffff);
        tim->CCR3|=ccr;
    }
    else if(ch==4)
    {
        tim->CCMR2&=~TIM_CCMR2_CC4S;
        tim->CCMR2&=~(0x07<<12);
        tim->CCMR2|=(mode<<12);
        tim->CCMR2|=(0x01<<11);             
        tim->CCR4&=~(0xffff);
        tim->CCR4|=ccr;
    }

    tim->CCER|=(0x01<<4*(ch-1));          //enable channel as output on pin

    tim->CR1|=(0x01<<7);                //auto reload preload enabled

    
    /*************DMA config*****************/
    if(dma_burst)
    {
        //tim->CR1|=(0x01<<3);                    //set OPM
        tim->CR1|=(0x01<<2);                    //Set update request source
        tim->DIER|=(0x01<<8);                       //Update DMA request enabled
        tim->DIER|=(0x01<<(9+(ch-1)) );         //Enable channel DMA request
        tim->DCR&=~(0x01F);
        tim->DCR|=(0x0D+(ch-1));                    //set DMA base address
        //init2DMA(getDMA_CH(timer_ch),28,M_addr,tim->DMAR);
        tim->CR2|=(0x01<<3);                    //DMA requests when update occurs
    }

    //pin config
    outAF(timer_ch);
}

void timer_ch_PWMH(const uint8_t timer_ch,uint16_t ccr,uint16_t arr,bool dma_burst)
{
    ccmCONFIG(timer_ch,PWM1,ccr,arr,dma_burst);
}

void timer_ch_PWML(const uint8_t timer_ch,uint16_t ccr,uint16_t arr,bool dma_burst)
{
    ccmCONFIG(timer_ch,PWM2,ccr,arr,dma_burst);    
}

void timer_ch_HIGH(const uint8_t timer_ch,uint16_t ccr,uint16_t arr,bool dma_burst)
{
    ccmCONFIG(timer_ch,ACTIVE,ccr,arr,dma_burst);    
}

void timer_ch_LOW(const uint8_t timer_ch,uint16_t ccr,uint16_t arr,bool dma_burst)
{
    ccmCONFIG(timer_ch,INACTIVE,ccr,arr,dma_burst);    
}

void timer_ch_TOGGLE(const uint8_t timer_ch,uint16_t ccr,uint16_t arr,bool dma_burst)
{
    ccmCONFIG(timer_ch,TOGGLE,ccr,arr,dma_burst);    
}

void timer_ch_FORCEHIGH(const uint8_t timer_ch,uint16_t ccr,uint16_t arr,bool dma_burst)
{
    ccmCONFIG(timer_ch,FORCE_ACTIVE,ccr,arr,dma_burst);       
}

void timer_ch_FORCELOW(const uint8_t timer_ch,uint16_t ccr,uint16_t arr,bool dma_burst)
{
    ccmCONFIG(timer_ch,FORCE_INACTIVE,ccr,arr,dma_burst);       
}

//******************Encoder************************* */
void set_encoder_mode(TIM_TypeDef* tim,uint8_t mode)
{
    tim->SMCR&=~(TIM_SMCR_SMS);
    tim->SMCR|=mode;
}

void set_encoder_TI(TIM_TypeDef* tim,uint8_t channel,uint8_t TI)
{
    if(channel==1)
    {
        tim->CCMR1&=~TIM_CCMR1_CC1S;
        tim->CCMR1|=TI;
    }
    else if (channel==2)
    {
        tim->CCMR1&=~TIM_CCMR1_CC2S;
        tim->CCMR1|=( (TI==1)?2:1)<<8;
    }
    else
        exit(EXIT_FAILURE);
}

void set_encoder_rising_trigger(TIM_TypeDef* tim,uint8_t channel)
{
    if(channel==1)
        tim->CCER&=~TIM_CCER_CC1P;
    else if (channel==2)
        tim->CCER&=~TIM_CCER_CC2P;
    else
        exit(EXIT_FAILURE);
}

void set_encoder_falling_trigger(TIM_TypeDef* tim,uint8_t channel)
{
    if(channel==1)
    {
        tim->CCER&=~TIM_CCER_CC1P;
        tim->CCER|=TIM_CCER_CC1P;
    }
    else if (channel==2)
    {
        tim->CCER&=~TIM_CCER_CC2P;
        tim->CCER|=TIM_CCER_CC2P;
    }
    else
        exit(EXIT_FAILURE);
}

void enable_capture(TIM_TypeDef* tim,uint8_t channel)
{
    if(channel==1)
        tim->CCER|=TIM_CCER_CC1E;
    else if (channel==2)
        tim->CCER|=TIM_CCER_CC2E;
    else
        exit(EXIT_FAILURE);
}

void set_encoder_filter(TIM_TypeDef* tim,uint8_t channel)
{
    if(channel==1)
    {
        tim->CCMR1&=~TIM_CCMR1_IC1F;
    }
    else if (channel==2)
    {
        tim->CCMR1&=~TIM_CCMR1_IC2F;
    }
    else
        exit(EXIT_FAILURE);
}

void encoderCONFIG(uint8_t timer, uint8_t mode, uint8_t channel1,uint8_t channel2,uint8_t trigger)
{
    TIM_TypeDef* tim=getTIMER(timer);
    timerCONFIG(timer,1,TIM_ARR_ARR);

    set_encoder_mode(tim,mode);

    set_encoder_TI(tim,channel1,1);
    set_encoder_TI(tim,channel2,2);

    if(trigger==CAPTURE_RISING)
    {
        set_encoder_rising_trigger(tim,channel1);
        set_encoder_rising_trigger(tim,channel2);
    }
    else if(trigger==CAPTURE_FALLING)
    {
        set_encoder_falling_trigger(tim,channel1);
        set_encoder_falling_trigger(tim,channel2);
    }

    set_encoder_filter(tim,channel1);
    set_encoder_filter(tim,channel2);

    enable_capture(tim,channel1);
    enable_capture(tim,channel2);
}

void timer_encoder_init(uint8_t timer,uint8_t mode,uint8_t TI1_CH, uint8_t TI2_CH,uint8_t trigger)
{
    uint8_t ch1_no=getTIMER_CH_NO(TI1_CH);
    uint8_t ch2_no=getTIMER_CH_NO(TI2_CH);
    encoderCONFIG(timer,mode,ch1_no,ch2_no,trigger);
    inMODE(TI1_CH);
    inMODE(TI2_CH);
}
