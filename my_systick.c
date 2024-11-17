#include "my_systick.h"
#include "stm32f10x.h"
#include <stdint.h>

uint32_t loader;

void systickCONFIG(const uint8_t ahbclk)             //ahbclk in Mhz
{
    SysTick->CTRL&=(0x0);
    //loader=(ahbclk*1000)-1;                 //Default counter for 1 millisecond
    loader=ahbclk-1; 
    SysTick->LOAD=loader;
    SysTick->VAL=(0x0);
    SysTick->CTRL|=(0x4);                   //Set processor clock(AHB)
}

void systickINT(const uint8_t ahbclk)
{
    systickCONFIG(ahbclk);
    __disable_irq();
    SysTick->CTRL|=(0x2);
    NVIC_EnableIRQ(SysTick_IRQn);
    __enable_irq();
}

void startTICK(void)
{
    SysTick->CTRL|=(0x01);              //Enable counter
}

void stopTICK(void)
{
    SysTick->CTRL&=~(0x01);             //Disable counter
}

void delayUS(uint16_t us)
{
    while(us)
    {
        //SysTick->LOAD=loader;
        SysTick->VAL=(0x0);
        while((SysTick->CTRL&(0x01<<16))==0);
        us--;
    }   
}

void delayMS(uint16_t ms)
{
    /*while(ms)
    {
        //SysTick->LOAD=loader;
        SysTick->VAL=(0x0);
        while((SysTick->CTRL&(0x01<<16))==0);
        ms--;
    }*/
    while(ms)
    {
        delayUS(1000);
        ms--;
    }
}
