#ifndef MY_SYSTICK_H_
#define MY_SYSTICK_H_
#include <stdint.h>

void systickCONFIG(const uint8_t ahbclk);  
void systickINT(const uint8_t ahbclk);   //*Generates an interrupt every one millisecond 
void startTICK(void);
void stopTICK(void);
void delayUS(uint16_t us);
void delayMS(uint16_t ms);

#endif
