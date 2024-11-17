#include"my_clock.h"
#include <stdint.h>

uint16_t SYSCLK,HCLK,PCLK1,PCLK2;

void setSYSCLK(uint8_t clk)
{
    RCC->CR|=(0x01<<16);                //Enable HSE
    while( (RCC->CR&(0x01<<17))==0 );   //Wait for HSE ON
    /*if(clk==S8MHZ)
    {
        RCC->CR&=~(0x01<<24);           //Disable PLL
        while( (RCC->CR&(0x01<<25)) );   //wait to disable PLL
    }*/
    if(clk==S16MHZ||clk==S24MHZ||clk==S36MHZ||clk==S40MHZ||clk==S48MHZ||clk==S56MHZ||clk==S64MHZ||clk==S72MHZ) 
    {
        RCC->CFGR&=~(0x0f<<18);
        RCC->CFGR|=(clk-1);                       //Set PLLMUL 
        RCC->CFGR|=(0x01<<16);                 //Set PLL to HSE
        RCC->CR|=(0x01<<24);                  //Enable PLL
        while( (RCC->CR&(0x01<<25))==0 );    //wait to enable PLL    
    }
}

void setHCLK(void)               //uint8_t clk
{
    //if(clk==S16MHZ||clk==S24MHZ||clk==S36MHZ||clk==S40MHZ||clk==S48MHZ||clk==S56MHZ||clk==S64MHZ||clk==S72MHZ)
    //{
        RCC->CFGR&=~(0x0F<<4);
        //RCC->CFGR|=(0x0);                   //set AHB prescaler to 1
    //}
}

void setPCLK1(uint8_t clk)
{
    RCC->CFGR&=~(0x0F<<8);
    /*if(clk==S16MHZ||clk==S24MHZ||clk==S36MHZ)
    {
        RCC->CFGR|=(0x0);               //set APB1 prescaler to 1
    }*/
    PCLK1=8*(clk+1);
    if(clk==S40MHZ||clk==S48MHZ||clk==S56MHZ||clk==S64MHZ||clk==S72MHZ) 
    {
        RCC->CFGR|=(0x04<<8);               //set APB1 prescaler to 2    
        PCLK1=PCLK1/2;    
    }
}

void setPCLK2(uint8_t clk)
{
    //if(clk==S16MHZ||clk==S24MHZ||clk==S36MHZ||clk==S40MHZ||clk==S48MHZ||clk==S56MHZ||clk==S64MHZ||clk==S72MHZ)
    //{
        RCC->CFGR&=~(0x0F<<11);
        //RCC->CFGR|=(0x0);                   //set APB2 prescaler to 1
    //}
}

void setCLK(uint8_t clk)
{
    setPCLK1(clk);
    setPCLK2(clk);
    setHCLK();
    setSYSCLK(clk);
    RCC->CFGR&=~(0x03);
    if(clk==S8MHZ)
    {
        RCC->CFGR|=(0x01);          //set system clock mux to HSE
        while( (RCC->CFGR&(0x03<<2))!=0x04 );   //wait for system clock to set HSE
    }
    else 
    {
        RCC->CFGR|=(0x02);         //set system clock mux to PLLCLK
        while( (RCC->CFGR&(0x03<<2))!=0x08 );     //wait for system clock to set PLLCLK
    }

    PCLK2=HCLK=SYSCLK=8*(clk+1);
}
