#include <avr/io.h>
#include "WS2812B.h"
void WS2812B_sendone(void)
{
	WS2812B_PORT|=(1<<WS2812B_PIN);
	asm ("nop"); asm ("nop");asm ("nop");asm ("nop");asm("nop");asm("nop");asm("nop");asm("nop");//asm("nop");
	WS2812B_PORT&=~(1<<WS2812B_PIN);
}
void WS2812B_sendzero(void)
{
	WS2812B_PORT|=(1<<WS2812B_PIN);
	asm ("nop");asm ("nop");asm("nop");asm("nop");asm("nop");asm("nop");//asm("nop");
	WS2812B_PORT&=~(1<<WS2812B_PIN);
}
void WS2812B_send(uint8_t r, uint8_t g, uint8_t b)
{
		if(g&0x80)WS2812B_sendone();else WS2812B_sendzero();		if(g&0x40)WS2812B_sendone();else WS2812B_sendzero();		if(g&0x20)WS2812B_sendone();else WS2812B_sendzero();
		if(g&0x10)WS2812B_sendone();else WS2812B_sendzero();		if(g&0x08)WS2812B_sendone();else WS2812B_sendzero();		if(g&0x04)WS2812B_sendone();else WS2812B_sendzero();
		if(g&0x02)WS2812B_sendone();else WS2812B_sendzero();		if(g&0x01)WS2812B_sendone();else WS2812B_sendzero();
	
		if(r&0x80)WS2812B_sendone();else WS2812B_sendzero();		if(r&0x40)WS2812B_sendone();else WS2812B_sendzero();		if(r&0x20)WS2812B_sendone();else WS2812B_sendzero();
		if(r&0x10)WS2812B_sendone();else WS2812B_sendzero();		if(r&0x08)WS2812B_sendone();else WS2812B_sendzero();		if(r&0x04)WS2812B_sendone();else WS2812B_sendzero();
		if(r&0x02)WS2812B_sendone();else WS2812B_sendzero();		if(r&0x01)WS2812B_sendone();else WS2812B_sendzero();
	
		if(b&0x80)WS2812B_sendone();else WS2812B_sendzero();		if(b&0x40)WS2812B_sendone();else WS2812B_sendzero();		if(b&0x20)WS2812B_sendone();else WS2812B_sendzero();
		if(b&0x10)WS2812B_sendone();else WS2812B_sendzero();		if(b&0x08)WS2812B_sendone();else WS2812B_sendzero();		if(b&0x04)WS2812B_sendone();else WS2812B_sendzero();
		if(b&0x02)WS2812B_sendone();else WS2812B_sendzero();		if(b&0x01)WS2812B_sendone();else WS2812B_sendzero();
}
