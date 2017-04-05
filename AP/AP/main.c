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
#include "WS2812B.h"

#define indeks_poczatkowy_ramki 36
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
  
	lcd("WiFi signal ");
	UART_Write("AT+CWJAP?");UART_crlf();
	DDRC|=(1<<PC0); //normalnie pisz ustawienia rejestrow
	DDRD|=(1<<PD2);
	WS2812B_send(255,255,255);
	_delay_ms(100);
	WS2812B_send(0,0,0);
	uint8_t *ptr;
	char wifi_strength[4];
    while (1) 
    {
		if(flaga_uart)
		{
			flaga_uart=0;
			if((ptr=znajdz("CWJAP:")))
			{
				pos(12,0);
				PORTC^=1;

				strcpy(wifi_strength,&bufor_uart[indeks_poczatkowy_ramki]);
				lcd_RAM(wifi_strength);
				
				_delay_ms(100);
				memset(bufor_uart,0,sizeof(bufor_uart));
				
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