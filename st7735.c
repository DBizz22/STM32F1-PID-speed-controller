#include "my_dma.h"
#include "my_gpio.h"
#include "my_spi.h"
#include "st7735.h"
#include "my_timer.h"
#include "stm32f10x.h"
#include <stdbool.h>
#include "fonts.h"
#include "my_systick.h"

#define SPI1_DATAFRAME8 SPI1->CR1&=~SPI_CR1_DFF
#define SPI1_DATAFRAME16 SPI1->CR1|=SPI_CR1_DFF 

void ST7735_select(void)
{
    pinOFF(CS);
}

void ST7735_deselect(void)
{
    pinON(CS);
}

void ST7735_CONFIG(const uint8_t clk)
{
	initSPI(SPI_1);
    outMODE(RST);
    outMODE(DC);
    outMODE(BLK);
    pinON(BLK);         //back light
    pinON(CS);         //chip select
    pinON(RST);        //reset is inactive

    //**/
    //initTDELAY(T3);       //initialize timer 3 for delay
    systickCONFIG(clk);
    startTICK();
}

void ST7735_WaitLastData()
{
  while((SPI1->SR & SPI_SR_TXE) == RESET);
  while((SPI1->SR & SPI_SR_BSY) != RESET);
}


void ST7735_SendCommand(uint8_t data)
{
    pinOFF(DC);  
    spi1_TX(data);
    //ST7735_WaitLastData();
}

void ST7735_SendData(uint8_t data)
{
    pinON(DC);
    spi1_TX(data);
    //ST7735_WaitLastData();
}

void ST7735_SendPixel(uint16_t data)
{
    pinON(DC);
    spi1_TX(data);
    //ST7735_WaitLastData();
}

/*void ST7735_SendDataMultiple(uint8_t *data, uint32_t num)
{
  for (uint32_t i = 0; i < num; i++)
  {
    ST7735_SendData(*data);
    data++;
  }
  
  ST7735_WaitLastData();
}*/

void ST7735_Init(const uint8_t clk)
{ 
    ST7735_CONFIG(clk);
    startSPI(SPI_1);

    ST7735_select();
    pinON(RST);
    //timerDELAYMS(T3,5);
    delayMS(5);
    pinOFF(RST);
    //timerDELAYUS(T3,10);
    delayMS(10);
    pinON(RST);
    //timerDELAYMS(T3,120);
    delayMS(120);


    ST7735_SendCommand(ST7735_SWRESET);  
    //timerDELAYMS(T3,120);
    delayMS(120);
    ST7735_SendCommand(ST7735_SLPOUT);  
    //timerDELAYMS(T3,120);
    delayMS(120);
    ST7735_SendCommand(ST7735_FRMCTR1);
    ST7735_SendData(0x05);
    ST7735_SendData(0x3C);
    ST7735_SendData(0x3C);
    ST7735_SendCommand(ST7735_FRMCTR2);
    ST7735_SendData(0x05);
    ST7735_SendData(0x3C);
    ST7735_SendData(0x3C);
    ST7735_SendCommand(ST7735_FRMCTR3);
    ST7735_SendData(0x05);
    ST7735_SendData(0x3C);
    ST7735_SendData(0x3C);
    ST7735_SendData(0x05);
    ST7735_SendData(0x3C);
    ST7735_SendData(0x3C);
    ST7735_SendCommand(ST7735_INVCTR);
    ST7735_SendData(0x03);
    ST7735_SendCommand(ST7735_PWCTR1);
    ST7735_SendData(0x28);
    ST7735_SendData(0x08);
    ST7735_SendData(0x04);
    ST7735_SendCommand(ST7735_PWCTR2);
    ST7735_SendData(0xC0);
    ST7735_SendCommand(ST7735_PWCTR3);
    ST7735_SendData(0x0D);
    ST7735_SendData(0x00);
    ST7735_SendCommand(ST7735_PWCTR4);
    ST7735_SendData(0x8D);
    ST7735_SendData(0x2A);
    ST7735_SendCommand(ST7735_PWCTR5);
    ST7735_SendData(0x8D);
    ST7735_SendData(0xEE);
    ST7735_SendCommand(ST7735_VMCTR1);
    ST7735_SendData(0x1A);
    ST7735_SendCommand(ST7735_INVOFF);
    ST7735_SendCommand(ST7735_MADCTL);
    ST7735_SendData(0xC0);
    ST7735_SendCommand(ST7735_COLMOD);
    ST7735_SendData(0x05);
    ST7735_SendCommand(ST7735_GMCTRP1);
    ST7735_SendData(0x04);
    ST7735_SendData(0x22);
    ST7735_SendData(0x07);
    ST7735_SendData(0x0A);
    ST7735_SendData(0x2E);
    ST7735_SendData(0x30);
    ST7735_SendData(0x25);
    ST7735_SendData(0x2A);
    ST7735_SendData(0x28);
    ST7735_SendData(0x26);
    ST7735_SendData(0x2E);
    ST7735_SendData(0x3A);    
    ST7735_SendData(0x00);
    ST7735_SendData(0x01);
    ST7735_SendData(0x03);
    ST7735_SendData(0x13);
    ST7735_SendCommand(ST7735_GMCTRN1);
    ST7735_SendData(0x04);
    ST7735_SendData(0x16);
    ST7735_SendData(0x06);
    ST7735_SendData(0x0D);
    ST7735_SendData(0x2D);
    ST7735_SendData(0x26);
    ST7735_SendData(0x23);
    ST7735_SendData(0x27);
    ST7735_SendData(0x27);
    ST7735_SendData(0x25);
    ST7735_SendData(0x2D);
    ST7735_SendData(0x3B);    
    ST7735_SendData(0x00);
    ST7735_SendData(0x01);
    ST7735_SendData(0x04);
    ST7735_SendData(0x13);
    ST7735_SendCommand(ST7735_NORON);
    ST7735_SendCommand(ST7735_DISPON);
    //timerDELAYMS(T3,500);
    delayMS(500);
    ST7735_deselect();
}

void ST7735_Init2(const uint8_t clk)
{
    ST7735_CONFIG(clk);
    startSPI(SPI_1);

    ST7735_select();
    pinON(RST);
    timerDELAYMS(T3,5);
    pinOFF(RST);
    timerDELAYUS(T3,10);
    pinON(RST);
    timerDELAYMS(T3,120);

    ST7735_SendCommand(ST7735_SWRESET);  
    timerDELAYMS(T3,120);
    ST7735_SendCommand(ST7735_SLPOUT);  
    timerDELAYMS(T3,120);
    ST7735_SendCommand(ST7735_FRMCTR1);
    ST7735_SendData(0x01);
    ST7735_SendData(0x2C);
    ST7735_SendData(0x2D);
    ST7735_SendCommand(ST7735_FRMCTR2);
    ST7735_SendData(0x01);
    ST7735_SendData(0x2C);
    ST7735_SendData(0x2D);
    ST7735_SendCommand(ST7735_FRMCTR3);
    ST7735_SendData(0x01);
    ST7735_SendData(0x2C);
    ST7735_SendData(0x2D);
    ST7735_SendData(0x01);
    ST7735_SendData(0x2C);
    ST7735_SendData(0x2D);
    ST7735_SendCommand(ST7735_INVCTR);
    ST7735_SendData(0x07);
    ST7735_SendCommand(ST7735_PWCTR1);
    ST7735_SendData(0xA2);
    ST7735_SendData(0x02);
    ST7735_SendData(0x84);
    ST7735_SendCommand(ST7735_PWCTR2);
    ST7735_SendData(0xC5);
    ST7735_SendCommand(ST7735_PWCTR3);
    ST7735_SendData(0x0A);
    ST7735_SendData(0x00);
    ST7735_SendCommand(ST7735_PWCTR4);
    ST7735_SendData(0x8A);
    ST7735_SendData(0x2A);
    ST7735_SendCommand(ST7735_PWCTR5);
    ST7735_SendData(0x8A);
    ST7735_SendData(0xEE);
    ST7735_SendCommand(ST7735_VMCTR1);
    ST7735_SendData(0x0E);
    ST7735_SendCommand(ST7735_INVOFF);
    ST7735_SendCommand(ST7735_MADCTL);
    ST7735_SendData(0xC0);
    ST7735_SendCommand(ST7735_COLMOD);
    ST7735_SendData(0x05);
    ST7735_SendCommand(ST7735_GMCTRP1);
    ST7735_SendData(0x02);
    ST7735_SendData(0x1c);
    ST7735_SendData(0x07);
    ST7735_SendData(0x12);
    ST7735_SendData(0x37);
    ST7735_SendData(0x32);
    ST7735_SendData(0x29);
    ST7735_SendData(0x2d);
    ST7735_SendData(0x29);
    ST7735_SendData(0x25);
    ST7735_SendData(0x2B);
    ST7735_SendData(0x39);    
    ST7735_SendData(0x00);
    ST7735_SendData(0x01);
    ST7735_SendData(0x03);
    ST7735_SendData(0x10);
    ST7735_SendCommand(ST7735_GMCTRN1);
    ST7735_SendData(0x03);
    ST7735_SendData(0x1d);
    ST7735_SendData(0x07);
    ST7735_SendData(0x06);
    ST7735_SendData(0x2E);
    ST7735_SendData(0x2C);
    ST7735_SendData(0x29);
    ST7735_SendData(0x2D);
    ST7735_SendData(0x2E);
    ST7735_SendData(0x2E);
    ST7735_SendData(0x37);
    ST7735_SendData(0x3F);    
    ST7735_SendData(0x00);
    ST7735_SendData(0x00);
    ST7735_SendData(0x02);
    ST7735_SendData(0x10);
    ST7735_SendCommand(ST7735_NORON);
    timerDELAYMS(T3,10);
    ST7735_SendCommand(ST7735_DISPON);
    timerDELAYMS(T3,150);
    ST7735_deselect();
}

/*void ST7735_SetColAddr(uint16_t cStart, uint16_t cStop)
{
    uint8_t data[4];
    
    data[0] = (cStart & 0xFF00) >> 8;
    data[1] = cStart & 0x00FF;
    data[2] = (cStop & 0xFF00) >> 8;
    data[3] = cStop & 0x00FF;
    
    ST7735_select();
    ST7735_SendCommand(ST7735_CASET);
    ST7735_SendDataMultiple(data, 4);
    ST7735_deselect();
}*/

/*void ST7735_SetRowAddr(uint16_t rStart, uint16_t rStop)
{
    uint8_t data[4];
    
    data[0] = (rStart & 0xFF00) >> 8;
    data[1] = rStart & 0x00FF;
    data[2] = (rStop & 0xFF00) >> 8;
    data[3] = rStop & 0x00FF;

    ST7735_select();
    ST7735_SendCommand(ST7735_RASET);
    ST7735_SendDataMultiple(data, 4);
    ST7735_deselect();
}*/

void ST7735_SetAddressWindow(uint8_t cStart,uint8_t rStart, uint8_t cStop,uint8_t rStop) 
{
    //ST7735_SetColAddr(cStart, cStop - 1);
    //ST7735_SetRowAddr(rStart, rStop - 1); 
    ST7735_select();
    ST7735_SendCommand(ST7735_CASET);
    ST7735_SendData(0x00);
    ST7735_SendData(cStart);
    ST7735_SendData(0x00);
    ST7735_SendData(cStop-1);
    ST7735_SendCommand(ST7735_RASET);
    ST7735_SendData(0x00);
    ST7735_SendData(rStart);
    ST7735_SendData(0x00);
    ST7735_SendData(rStop-1);
    ST7735_SendCommand(ST7735_RAMWR);
    ST7735_WaitLastData();
    ST7735_deselect();  
}

void ST7735_DrawPixel(uint8_t x, uint8_t y, uint16_t color) 
{
    if((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT))
        return;

    ST7735_select();

    ST7735_SetAddressWindow(x, y, x+1, y+1);
    //uint8_t data[] = { color >> 8, color & 0xFF };
    SPI1_DATAFRAME16;
    ST7735_SendPixel(color);

    ST7735_deselect();
    SPI1_DATAFRAME8;
}

void ST7735_DrawRect(uint8_t cStart, uint8_t rStart, uint8_t cStop, uint8_t rStop, uint16_t color)
{
    //ST7735_SetColAddr(cStart, cStop - 1);
    //ST7735_SetRowAddr(rStart, rStop - 1);
    uint32_t size = (cStop - cStart) * (rStop - rStart); 
    ST7735_SetAddressWindow(cStart,rStart,cStop,rStop);

    SPI1_DATAFRAME16;

    ST7735_select();
    for (uint32_t i = 0; i < size; i++)
    {
        ST7735_SendPixel(color);
    }
    ST7735_WaitLastData();
    ST7735_deselect();
    SPI1_DATAFRAME8;
}

void ST7735_SetBackgroundImage(uint16_t cStart, uint16_t rStart, uint16_t cStop, uint16_t rStop, const uint16_t *image) 
{
    uint32_t size = (cStop - cStart) * (rStop - rStart);

    ST7735_SetAddressWindow(cStart,rStart,cStop,rStop);    


    init2DMA(DMA1_CH3, size, (uint32_t)image, (uint32_t)&SPI1->DR);

    /*********Set to use 16 bits for data transfer***************/
    DMA1_Channel3->CCR|=(0x01<<8);
    DMA1_Channel3->CCR|=(0x01<<10);
		
    //spiCONFIG(SPI_1,SPEED1,true);
    SPI1_DATAFRAME16;
    pinON(DC);
    ST7735_select();
    startDMA(DMA1_CH3,false);


    while (DMA1_Channel3->CNDTR!=0);
    ST7735_WaitLastData();
    ST7735_deselect();
    stopDMA(DMA1_CH3);
    SPI1_DATAFRAME8;
}

static void ST7735_WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor) 
{
    uint32_t i, b, j;

    ST7735_SetAddressWindow(x, y, x+font.width, y+font.height); //maybe swap parameters
    SPI1_DATAFRAME16;
    ST7735_select();
    for(i = 0; i < font.height; i++) 
    {
        b = font.data[(ch - 32) * font.height + i];
        for(j = 0; j < font.width; j++) 
        {
            if((b << j) & 0x8000)  
            {
                //uint8_t data[] = { color >> 8, color & 0xFF };
                ST7735_SendPixel(color);
            } 
            else 
            {
                //uint8_t data[] = { bgcolor >> 8, bgcolor & 0xFF };
                ST7735_SendPixel(bgcolor);
            }
        }
    }
    ST7735_deselect();
    SPI1_DATAFRAME8;
}

void ST7735_WriteString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor) 
{
    while(*str) {
        if(x + font.width >= ST7735_WIDTH) {
            x = 0;
            y += font.height;
            if(y + font.height >= ST7735_HEIGHT) {
                break;
            }

            if(*str == ' ') {
                // skip spaces in the beginning of the new line
                str++;
                continue;
            }
        }

        ST7735_WriteChar(x, y, *str, font, color, bgcolor);
        x += font.width;
        str++;
    }
}

void ST7735_InvertColors(bool invert) 
{
    ST7735_select();
    ST7735_SendCommand(invert ? ST7735_INVON : ST7735_INVOFF);
    ST7735_deselect();
}

void ST7735_FillScreen(uint16_t color) {
    ST7735_DrawRect(0, 0, ST7735_WIDTH, ST7735_HEIGHT, color);
}
