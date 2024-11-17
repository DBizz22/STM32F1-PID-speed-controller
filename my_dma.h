#ifndef MY_DMA_H_
#define MY_DMA_H_
#include "stm32f10x.h"
#include "stdbool.h"

//DMA channels
#define DMA1_CH1 1
#define DMA1_CH2 2
#define DMA1_CH3 3
#define DMA1_CH4 4
#define DMA1_CH5 5
#define DMA1_CH6 6
#define DMA1_CH7 7
#define DMA2_CH1 8
#define DMA2_CH2 9
#define DMA2_CH3 10
#define DMA2_CH4 11
#define DMA2_CH5 12

//DMA functions
void initDMA(uint8_t ch,uint16_t num,uint32_t P_addr,uint32_t M_addr);
void init2DMA(uint8_t ch,uint16_t num,uint32_t M_addr,uint32_t P_addr);
void mem2mem(uint8_t ch,uint16_t num,uint32_t source,uint32_t dest);
void dmaINT(uint8_t ch);
void startDMA(uint8_t ch,bool circ_mode);
void stopDMA(uint8_t ch);

#endif
