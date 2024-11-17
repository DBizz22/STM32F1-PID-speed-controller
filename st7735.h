#ifndef ST7735_H_
#define ST7735_H_
#include "stdint.h"
#include "fonts.h"
#include "stdbool.h"

/********st7735 portpins setup**********/
#define SCL PA5
#define SDA PA7
#define RST PA13
#define DC PC12
#define CS PA4
#define BLK PD2

/* Declarations and definitions ----------------------------------------------*/
#define ST7735_COLOR_RED                                                0xF800
#define ST7735_COLOR_GREEN                                              0x07E0
#define ST7735_COLOR_BLUE                                               0x001F
#define ST7735_COLOR_YELLOW                                             0xFFE0
#define ST7735_COLOR_WHITE                                              0xFFFF
#define ST7735_COLOR_BLACK                                              0x0000
#define ST7735_COLOR_ORANGE                                             0xFA20
#define ST7735_COLOR_CYAN                                               0x07FF
#define ST7735_COLOR_MAGENTA                                            0xF81F
#define ST7735_RESET_PORT                                               (GPIOB)
#define ST7735_RESET_PIN                                                (GPIO_PIN_7)
#define ST7735_CS_PORT                                                  (GPIOC)
#define ST7735_CS_PIN                                                   (GPIO_PIN_11)
#define ST7735_DC_PORT                                                  (GPIOB)
#define ST7735_DC_PIN                                                   (GPIO_PIN_6)
#define ST7735_X_SIZE                                                   128
#define ST7735_Y_SIZE                                                   160
#define ST7735_NOP                                                      0x00
#define ST7735_SWRESET                                                  0x01
#define ST7735_RDDID                                                    0x04
#define ST7735_RDDST                                                    0x09
#define ST7735_SLPIN                                                    0x10
#define ST7735_SLPOUT                                                   0x11
#define ST7735_PTLON                                                    0x12
#define ST7735_NORON                                                    0x13
#define ST7735_INVOFF                                                   0x20
#define ST7735_INVON                                                    0x21
#define ST7735_DISPOFF                                                  0x28
#define ST7735_DISPON                                                   0x29
#define ST7735_CASET                                                    0x2A
#define ST7735_RASET                                                    0x2B
#define ST7735_RAMWR                                                    0x2C
#define ST7735_RAMRD                                                    0x2E
#define ST7735_PTLAR                                                    0x30
#define ST7735_COLMOD                                                   0x3A
#define ST7735_MADCTL                                                   0x36
#define ST7735_FRMCTR1                                                  0xB1
#define ST7735_FRMCTR2                                                  0xB2
#define ST7735_FRMCTR3                                                  0xB3
#define ST7735_INVCTR                                                   0xB4
#define ST7735_DISSET5                                                  0xB6
#define ST7735_PWCTR1                                                   0xC0
#define ST7735_PWCTR2                                                   0xC1
#define ST7735_PWCTR3                                                   0xC2
#define ST7735_PWCTR4                                                   0xC3
#define ST7735_PWCTR5                                                   0xC4
#define ST7735_VMCTR1                                                   0xC5
#define ST7735_RDID1                                                    0xDA
#define ST7735_RDID2                                                    0xDB
#define ST7735_RDID3                                                    0xDC
#define ST7735_RDID4                                                    0xDD
#define ST7735_PWCTR6                                                   0xFC
#define ST7735_GMCTRP1                                                  0xE0
#define ST7735_GMCTRN1                                                  0xE1
#define ST7735_SPI_TIMEOUT                                              100

//display dimensions
#define ST7735_WIDTH  128
#define ST7735_HEIGHT 160
#define ST7735_XSTART 2
#define ST7735_YSTART 3

//functions
void ST7735_Init(const uint8_t clk);
void ST7735_Init2(const uint8_t clk);
void ST7735_DrawRect(uint8_t cStart, uint8_t rStart, uint8_t cStop, uint8_t rStop, uint16_t color);
void ST7735_SetBackgroundImage(uint16_t cStart, uint16_t rStart, uint16_t cStop, uint16_t rStop, const uint16_t *image);
void ST7735_WriteString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor);
void ST7735_InvertColors(bool invert);
void ST7735_DrawRect(uint8_t cStart, uint8_t rStart, uint8_t cStop, uint8_t rStop, uint16_t color);
void ST7735_FillScreen(uint16_t color);
void ST7735_DrawPixel(uint8_t x, uint8_t y, uint16_t color);

#endif
