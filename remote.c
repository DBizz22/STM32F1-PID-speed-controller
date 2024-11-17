#include"remote.h"
#include"my_gpio.h"
#include "my_systick.h"
#include "stm32f10x.h"
#include <stdint.h>

const uint32_t NEC_reference[]={0xFF9867,0xFFA25D,0xFF629D,0xFFE21D,0xFF22DD,0xFF02FD,0xFFC23D,0xFFE01F,
0xFFA857,0xFF906F,0xFFB04F,0XFF6897,0xFF38C7,0xFF18E7,0xFF10EF,0xFF5AA5,0xFF4AB5};
const uint8_t remote_key[]={'0','1','2','3','4','5','6','7','8','9','#','*','k','^','<','>','D'};

void remote_init(const remote_controller* remote)
{
	falINT(remote->pin);
}

uint32_t NEC_decoder(const uint8_t pin)
{
		int count;
	uint32_t NEC_protocol=0;
	
    while(!pinR(pin));
	while(pinR(pin));
	
	for (int i=0;i<32;i++) 
	{
		count=0;
		while(!pinR(pin));
		while(pinR(pin))
		{
			count++;
			delayUS(100);
		}

		if(count>=5)
		{
			NEC_protocol|=(0x01<<(31-i));
		}
		
	}
    return NEC_protocol;
}

uint8_t NEC(const uint8_t pin)
{
    uint32_t protocol=NEC_decoder(pin);
	for (int i=0; i<17; i++) 
	{
		if(protocol==NEC_reference[i])
		{
			return remote_key[i];
		}
	}
    return '\0';
}

void update_remote_key(remote_controller* remote,uint8_t (*protocol)(const uint8_t pin))
{
	//EXTI->IMR&=~EXTI_IMR_MR10;
    remote->key=protocol(remote->pin);
	//EXTI->IMR|=EXTI_IMR_MR10;
}