#include "my_spi.h"
#include "my_gpio.h"
#include "stm32f10x.h"
#include <stdbool.h>
#include <stdint.h>

SPI_TypeDef* getSPI(uint8_t spi_x)
{
    switch(spi_x) 
    {
        case SPI_1: return SPI1;
        case SPI_2: return SPI2;
        case SPI_3: return SPI3;
    }
}

void spiCONFIG(uint8_t spi_x,uint8_t baud,bool data_frame16)
{
    RCC->APB2ENR|=0x01;     //Enable AF

    switch(spi_x)
    {
        case SPI_1: RCC->APB2ENR|=(0x01<<12);       //Enable spi1 clk
                    break;
        case SPI_2: RCC->APB1ENR|=(0x01<<14);       //Enable spi2 clk
                    break; 
        case SPI_3: RCC->APB1ENR|=(0x01<<15);       //Enable spi3 clk
                    break;
    }
        
    //Setting SPI portpins configs
    if(spi_x==SPI_1)
    {
        outMODE(SPI1_NSS);
        outAF(SPI1_SCK);
        outAF(SPI1_MOSI);
        inMODE(SPI1_MISO);
    }
    else if(spi_x==SPI_2) 
    {
        outMODE(SPI2_NSS);
        outAF(SPI2_SCK);
        outAF(SPI2_MOSI);
        inMODE(SPI2_MISO);    
    }

    SPI_TypeDef* spi=getSPI(spi_x);
    spi->CR1|=(baud<<3);                       //set baud rate=pclk1 or 2 / 2-256

    if(data_frame16)                            
        spi->CR1|=(0x01<<11);                 //set data frame to i6 bits, otherwise 8-bits
    
    spi->CR1|=(0x01<<2);                    //Enable master config
    spi->CR1|=(0x01<<15);                   //enable 1 line bidirectional mode
    spi->CR1|=(0x01<<14);                   //enable transmit only mode
    //Clock polarity=0
    //Clock phase=0
    //MSB trnasmitted first

    spi->CR2|=(0x01<<2);                    //SS output enable
    //spi->CR2|=(0x01);                      //Enable Rx buffer DMA 
    spi->CR2|=(0x01<<1);                   //Enable Tx buffer DMA
}

void initSPI(uint8_t spi_x)
{
    spiCONFIG(spi_x,SPEED1,false);  
}

void spiINT(uint8_t spi_x)
{
    initSPI(spi_x);  
    SPI_TypeDef* spi=getSPI(spi_x);
    spi->CR2|=(0x01<<7);                    //Enable TXE interrupt
    spi->CR2|=(0x01<<6);                    //Enable RXE interrupt  
    __disable_irq();
    switch (spi_x) 
    {
        case SPI_1: NVIC_EnableIRQ(SPI1_IRQn);
                    break;
        case SPI_2: NVIC_EnableIRQ(SPI2_IRQn);
    }
    __enable_irq();
}


void startSPI(uint8_t spi_x)
{
    SPI_TypeDef* spi=getSPI(spi_x);
    spi->CR1|=(0x01<<6);                //Enable spi peripheral
}

void stopSPI(uint8_t spi_x)
{
    SPI_TypeDef* spi=getSPI(spi_x);
    while((spi->SR&0x01)==0x01);        //Wait for recieve buffer to be empty ***my own preference**
    while((spi->SR&0x02)==0x00);        //wait for transmit buffer to be empty
    while((spi->SR&(0x01<<7))==0x80);   //Wait for spi not busy
    spi->CR1&=~(0x01<<6);               //Disable spi peripheral
}

void sendDATA( SPI_TypeDef* spi,uint16_t data)
{
    spi->DR=data;
    while ((spi->SR&0x02)==0x00);       //wait for transmit buffer to be empty
    //while((spi->SR&0x01)==0x01);        //Wait for recieve buffer to not be empty
    //temp=spi->DR;
}

void spi1_TX(uint16_t data)
{
    sendDATA(SPI1,data);
}

void spi2_Tx(uint16_t data)
{
    sendDATA(SPI2,data);
}
