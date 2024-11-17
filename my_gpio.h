#ifndef MY_GPIO_H_
#define MY_GPIO_H_
#include <stm32f10x.h>
#include<stdbool.h>
#include<stdint.h>

//portpin constants
enum {PA0=0,PA1,PA2,PA3,PA4,PA5,PA6,PA7,PA8,PA9,PA10,PA11,PA12,PA13,PA14,PA15};
enum {PB0=16,PB1,PB2,PB3,PB4,PB5,PB6,PB7,PB8,PB9,PB10,PB11,PB12,PB13,PB14,PB15};
enum {PC0=32,PC1,PC2,PC3,PC4,PC5,PC6,PC7,PC8,PC9,PC10,PC11,PC12,PC13,PC14,PC15};
#ifdef GPIOD
enum {PD0=48,PD1,PD2,PD3,PD4,PD5,PD6,PD7,PD8,PD9,PD10,PD11,PD12,PD13,PD14,PD15};
#endif
#ifdef GPIOE
enum {PE0=64,PE1,PE2,PE3,PE4,PE5,PE6,PE7,PE8,PE9,PE10,PE11,PE12,PE13,PE14,PE15};
#endif
#ifdef GPIOF 
enum {PF0=80,PF1,PF2,PF3,PF4,PF5,PF6,PF7,PF8,PF9,PF10,PF11,PF12,PF13,PF14,PF15};
#endif
#ifdef GPIOG
enum {PG0=96,PG1,PG2,PG3,PG4,PG5,PG6,PG7,PG8,PG9,PG10,PG11,PG12,PG13,PG14,PG15};
#endif

//input config modes
#define ANA 0x0     //analog mode
#define FLT 0x4     //floating input mode
#define PUD 0x8     //input with push up/pull down

//output config modes   /*****using max speed****/
#define PP 0x3      //push pull
#define OD 0x7      //Open drain
#define APP 0xB     //Alternate func push pull
#define AOD 0xF     //Alternate func Open drain

//output data modes
#define OFF 0x0
#define ON 0x1

//gpio interrupt trigger
#define RIS 0x0
#define FAL 0x1

//pin config
void inMODE(char portpin);                  //portpin pulldown/push pull input mode
void inANA(char portpin);                   //portpin input analog mode
void inFLT(char portpin);                   //portpin input float mode
void outMODE(char portpin);                 //output mode push pull
void outMODE2(char portpin);                //output mode open drain
void outAF(char portpin);                  //alternate func output push pull mode
void outOAF(char portpin);                  //alternate func output open drain

//pin data
bool pinR(char portpin);                    //portpin read
void pinON(char portpin);                   //portpin ON
void pinOFF(char portpin);                  //portpin OFF
void pinTOGGLE(char portpin);               //portpin Toggle

//portpin interrupt
void falINT(char portpin);                  //portpin rising interrupt
void risINT(char portpin);                  //portpin falling interrupt

#endif
