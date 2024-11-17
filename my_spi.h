#ifndef MY_SPI_H_
#define MY_SPI_H_
#include "stm32f10x.h"
#include "my_gpio.h"

//SPI 
#define SPI_1 0
#define SPI_2 1
#define SPI_3 2

//spi portpins
//spi1
#define SPI1_NSS PA4
#define SPI1_SCK PA5
#define SPI1_MISO PA6
#define SPI1_MOSI PA7
//spi2
#define SPI2_NSS PB12
#define SPI2_SCK PB13
#define SPI2_MISO PB14
#define SPI2_MOSI PB15

//baud rates
#define SPEED1 0
#define SPEED2 1
#define SPEED3 2
#define SPEED4 3
#define SPEED5 4
#define SPEED6 5
#define SPEED7 6
#define SPEED8 7  

//spi functions
void spiCONFIG(uint8_t spi_x,uint8_t baud,bool data_frame16);
void initSPI(uint8_t spi_x);
void spiINT(uint8_t spi_x);
void startSPI(uint8_t spi_x);
void stopSPI(uint8_t spi_x);
void spi1_TX(uint16_t data);
void spi2_Tx(uint16_t data);

#endif
