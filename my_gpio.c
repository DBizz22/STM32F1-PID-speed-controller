#include "my_gpio.h"
#include <stm32f10x.h>

void enPORT(char port)
{
    //enable port
    RCC->APB2ENR|=(0x01<<(2+port));
}


GPIO_TypeDef* getGPIO(char port)
{
    if(port==0)
        return GPIOA;
    else if(port==1)
        return GPIOB;
    else if(port==2)
        return GPIOC;
    else if(port==3)
        return GPIOD;
    else if(port==4)
        return GPIOE;
    else if(port==5)
        return GPIOF;
    else 
        return GPIOG;
}

void inCONFIG(char portpin,char mode)       //portpin input config
{      
    char port=portpin/16;
    enPORT(port);
    GPIO_TypeDef* gpio=getGPIO(port);

    char pin=portpin%16;
    char shift=(pin%8)*4;
    if(pin<8)                       //first 8 pins
    {
        gpio->CRL&=~(0x0f<<shift);
        gpio->CRL|=(mode<<shift);

        /*if(mode==ANA)
        {
            //already reset which is analog mode   
        }
        else if(mode==FLT)
        {
            gpio->CRL|=(0x04<<shift);
        }
        else if(mode==PUD)
        {
            gpio->CRL|=(0x08<<shift);
        }*/
    }
    else {                      //last 8 pins
        gpio->CRH&=~(0x0f<<shift);
        gpio->CRH|=(mode<<shift);

        /*if(mode==ANA)
        {
            //already reset which is analog mode   
        }
        else if(mode==FLT)
        {
            gpio->CRH|=(FLT<<shift);
        }
        else if(mode==PUD)
        {
            gpio->CRH|=(PUD<<shift);
        }*/
    }
}

void inMODE(char portpin)
{
    inCONFIG(portpin,PUD);
}

void inANA(char portpin)
{
    inCONFIG(portpin,ANA);
}

void inFLT(char portpin)
{
    inCONFIG(portpin,FLT);
}

void outCONFIG(char portpin, char mode)         //portpin output config
{
    char port=portpin/16;
    enPORT(port);
    GPIO_TypeDef* gpio=getGPIO(port);

    char pin=portpin%16;
    char shift=(pin%8)*4;
    if(pin<8)                       //first 8 pins
    {
        gpio->CRL&=~(0x0f<<shift);
        gpio->CRL|=(mode<<shift);

        /*if(mode==PP)
        {
            gpio->CRL|=(PP<<shift);
        }
        else if(mode==OD)
        {
            gpio->CRL|=(OD<<shift);
        }
        else if(mode==APP)
        {
            gpio->CRL|=(APP<<shift);
        }
        else if(mode==AOD)
        {
            gpio->CRL|=(AOD<<shift);
        }*/
    }
    else { 
        //last 8 pins
        gpio->CRH&=~(0x0f<<shift);
        gpio->CRH|=(mode<<shift);

        /*if(mode==PP)
        {
            gpio->CRH|=(PP<<shift);
        }
        else if(mode==OD)
        {
            gpio->CRH|=(OD<<shift);
        }
        else if(mode==APP)
        {
            gpio->CRH|=(APP<<shift);
        }
        else if(mode==AOD)
        {
            gpio->CRH|=(AOD<<shift);
        }*/
    }

    if(mode==APP||mode==AOD) 
    {
        RCC->APB2ENR|=0x1;      //enable Af clock
    }
      
}

void outMODE(char portpin)
{
    outCONFIG(portpin,PP);
}

void outMODE2(char portpin)
{
    outCONFIG(portpin,OD);
}

bool pinR(char portpin)
{
    char port=portpin/16;
    char pin=portpin%16;
    GPIO_TypeDef* gpio=getGPIO(port);
    return (gpio->IDR&(0x01<<pin));
}

void pinW(char portpin,char mode)       //portpin write func
{
    char port=portpin/16;
    char pin=portpin%16;
    GPIO_TypeDef* gpio=getGPIO(port);

    if(mode==OFF)
    {
        gpio->ODR&=~(ON<<pin);
    }
    else if(mode==ON)
    {
        gpio->ODR|=(ON<<pin);
    }
}

void pinON(char portpin)
{
    pinW(portpin,ON);
}

void pinOFF(char portpin)
{
    pinW(portpin,OFF);
}

void pinTOGGLE(char portpin)
{
    if(pinR(portpin))
    {
        pinOFF(portpin);
    }
    else {
        pinON(portpin);
    }
}

void intCONFIG(char portpin,char trig)       //portpin interrupt config
{
    inMODE(portpin);
    RCC->APB2ENR|=(0x01);
    uint16_t port=portpin/16;
    uint16_t pin=portpin%16;

    __disable_irq();
    AFIO->EXTICR[pin/4]&=~( 0x0f<<((pin%4)*4) );
    AFIO->EXTICR[pin/4]|=( port<<((pin%4)*4) );          //Enable select source input for EXTI interupt
    EXTI->IMR|=(0x01<<pin);

    if(trig==RIS)
    {
        EXTI->RTSR|=(0x01<<pin);
    }
    else if(trig==FAL)
    {
        EXTI->FTSR|=(0x01<<pin);
    }

    if(pin==0)
    {
        NVIC_EnableIRQ(EXTI0_IRQn);
    }
    else if (pin==1) {
        NVIC_EnableIRQ(EXTI1_IRQn);
    }
    else if(pin==2)
    {
        NVIC_EnableIRQ(EXTI2_IRQn);
    }
    else if (pin==3) {
        NVIC_EnableIRQ(EXTI3_IRQn);
    }
    else if (pin==4) {
        NVIC_EnableIRQ(EXTI4_IRQn);
    }
    else if ((pin>=5) && (pin<=9)) {
        NVIC_EnableIRQ(EXTI9_5_IRQn);
    }
    else if ((pin>=10)&&(pin<=15)) {
        NVIC_EnableIRQ(EXTI15_10_IRQn);
    }

    __enable_irq();

}

void risINT(char portpin)
{
    intCONFIG(portpin,RIS);
}

void falINT(char portpin)
{
    intCONFIG(portpin,FAL);
}
                    
void outAF(char portpin)    //alternate func output push pull mode
{
    outCONFIG(portpin,APP);
}

void outOAF(char portpin)
{
    outCONFIG(portpin,AOD);
}
