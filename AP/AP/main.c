/*
 * AP.c
 *
 * Created: 2017-03-30 01:15:17
 * Author : Kamil
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "UART.h"
#include "hd44780.h"

volatile char znak;
volatile uint8_t indeks_uart=0,flaga_uart=0;
 volatile char bufor_uart[64];

static inline uint8_t znajdz(const char *pattern);
ISR(USART_RX_vect)
{
	znak=UDR0;
	if(znak==10 || znak ==13)
	{
		if(indeks_uart)
		{
			flaga_uart=1;
			indeks_uart=0;
		}
	}
	if(znak!=13 && znak !=10)
	{
		bufor_uart[indeks_uart++]=znak;
		indeks_uart &= 0x3f;		//utnij gdyby indeks > 63
	}
}
int main(void)
{
	UART_Init();
	lcd_init();
	sei();
  
//	UART_Write("AT+CIPSTART=\"TCP\",\"192.168.4.0\",81");UART_crlf();
//	_delay_ms(1000);
	lcd("start");
	UART_Write("AT+CWJAP?");UART_crlf();
	DDRC|=1;
	uint8_t *ptr;
	
    while (1) 
    {
		if(flaga_uart)
		{
			flaga_uart=0;
			if((ptr=znajdz("CWJAP")))
			{
				//pos(0,0);
				PORTC^=1;
				//ptr+=12;
				lcd_RAM(ptr);
				
				//UART_Write(*ptr);
				_delay_ms(1000);
				UART_Write("AT+CWJAP?");UART_crlf();	
				
			}
		//	
		}
		
		
    }
}

static inline uint8_t znajdz(const char *pattern)
{
	if(strstr(bufor_uart,pattern))
	return 1;
	return 0;
}