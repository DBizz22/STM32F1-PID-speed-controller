#ifndef MY_CLOCK_H_
#define MY_CLOCK_H_
#include "stm32f10x.h"
#include <stdint.h>

/***************CLOCK configs********************/
#define S8MHZ 0
#define S16MHZ 1 
#define S24MHZ 2
#define S36MHZ 3
#define S40MHZ 4
#define S48MHZ 5 
#define S56MHZ 6
#define S64MHZ 7
#define S72MHZ 8

void setCLK(uint8_t clk);

#endif
