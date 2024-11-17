#include "my_dma.h"
#include "stm32f10x.h"
#include <stdint.h>

DMA_Channel_TypeDef* getDMA(uint8_t ch)
{
    switch (ch) 
    {
        case 1 :    return DMA1_Channel1;
        case 2 :    return DMA1_Channel2;
        case 3 :    return DMA1_Channel3;
        case 4 :    return DMA1_Channel4;
        case 5 :    return DMA1_Channel5;
        case 6 :    return DMA1_Channel6;
        case 7 :    return DMA1_Channel7;
        case 8 :    return DMA2_Channel1;
        case 9 :    return DMA2_Channel2;
        case 10 :   return DMA2_Channel3;
        case 11 :   return DMA2_Channel4;
        case 12 :   return DMA2_Channel5;
    }
}

void initDMA(uint8_t ch,uint16_t num,uint32_t P_addr,uint32_t M_addr)
{
    DMA_Channel_TypeDef* dma_ch=getDMA(ch);
    if(ch<=DMA1_CH7)
    {
        RCC->AHBENR|=(0x01);        //Enable DMA1 clock
    }
    else 
    {
        RCC->AHBENR|=(0x02);        //Enable DMA2 clock
    }
    dma_ch->CPAR=P_addr;
    dma_ch->CMAR=M_addr;
    dma_ch->CNDTR=num;
    dma_ch->CCR=0;                  //Read from peripheral
    //dma_ch->CCR|=(0x01<<5);         //Enable Circular mode
    dma_ch->CCR|=(0x01<<7);         //Enable Memory increment mode
    dma_ch->CCR|=(0x01<<8);         //set peripheral size to 16 bits
    dma_ch->CCR|=(0x01<<10);        //set memory size to 16 bits
}

void init2DMA(uint8_t ch,uint16_t num,uint32_t M_addr,uint32_t P_addr)
{
    DMA_Channel_TypeDef* dma_ch=getDMA(ch);
    if(ch<=DMA1_CH7)
    {
        RCC->AHBENR|=(0x01);        //Enable DMA1 clock
    }
    else 
    {
        RCC->AHBENR|=(0x02);        //Enable DMA2 clock
    }
    dma_ch->CMAR=M_addr;
    dma_ch->CPAR=P_addr;   
    dma_ch->CNDTR=num; 
    dma_ch->CCR=0;                  
    dma_ch->CCR|=(0x01<<4);         //Read from memory
    //dma_ch->CCR|=(0x01<<5);         //Enable Circular mode
    dma_ch->CCR|=(0x01<<7);         //Enable Memory increment mode  
    dma_ch->CCR|=(0x01<<8);         //set peripheral size to 16 bits
    dma_ch->CCR|=(0x01<<10);        //set memory size to 16 bits    
}

void dmaINT(uint8_t ch)
{
    DMA_Channel_TypeDef* dma_ch=getDMA(ch);

    /********DMA interrupt enable***********/
    __disable_irq();
    dma_ch->CCR|=(0x01<<1);         //Enable Transfer complete interrupt
    switch(ch) 
    {
        case 1 :    NVIC_EnableIRQ(DMA1_Channel1_IRQn);
                    break;
        case 2 :    NVIC_EnableIRQ(DMA1_Channel2_IRQn);
                    break;
        case 3 :    NVIC_EnableIRQ(DMA1_Channel3_IRQn);
                    break;
        case 4 :    NVIC_EnableIRQ(DMA1_Channel4_IRQn);
                    break;
        case 5 :    NVIC_EnableIRQ(DMA1_Channel5_IRQn);
                    break;
        case 6 :    NVIC_EnableIRQ(DMA1_Channel6_IRQn);
                    break;
        case 7 :    NVIC_EnableIRQ(DMA1_Channel7_IRQn);
                    break;
        /*
        case 8 :    NVIC_EnableIRQ(DMA);
                    break;
        case 9 :        NVIC_EnableIRQ(DMA1_Channel1_IRQn);
                    break;
        case 10 :       NVIC_EnableIRQ(DMA1_Channel1_IRQn);
                    break;
        case 11 :       NVIC_EnableIRQ(DMA1_Channel1_IRQn);
                    break;
        case 12 :       NVIC_EnableIRQ(DMA1_Channel1_IRQn);*/
    }

    __enable_irq();   
}

void startDMA(uint8_t ch,bool circ_mode)
{
    DMA_Channel_TypeDef* dma_ch=getDMA(ch);
    if(circ_mode)
        dma_ch->CCR|=(0x01<<5);         //Enable Circular mode
    else
        dma_ch->CCR&=~(0x01<<5);        //Disable Circular mode
    dma_ch->CCR|=(0x01);                //Enable DMA Channel
}

void stopDMA(uint8_t ch)
{
    DMA_Channel_TypeDef* dma_ch=getDMA(ch);
    dma_ch->CCR&=~(0x01);       //Disable DMA Channel    
}

void mem2mem(uint8_t ch,uint16_t num,uint32_t source,uint32_t dest)
{
    DMA_Channel_TypeDef* dma_ch=getDMA(ch);
    dma_ch->CCR|=DMA_CCR1_MEM2MEM;
    dma_ch->CCR|=DMA_CCR1_DIR;

    dma_ch->CNDTR=num;
    dma_ch->CMAR=source;
    dma_ch->CPAR=dest;
}