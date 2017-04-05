#include <avr/io.h>
#include <stdlib.h>
#include "UART.h"

void UART_Init(void)
{
	UBRR0H=0;
	UBRR0L=8;		//kolego dla 115200 8 przy @16Mhz ;)
	UCSR0B=(1<<TXEN0)|(1<<RXEN0)|(1<<RXCIE0);
	UCSR0C=(1<<UCSZ00)|(1<<UCSZ01);		
}

void UART_Char(char data)
{
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0=data;
}

void UART_Write(const char *string)
{
	while(*string)
	{
		UART_Char(*string++);
	}	
}
char UART_Read(void)
{
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

void UART_crlf()
{
	UART_Char(13);
	UART_Char(10);
}

void UART_int(uint8_t val, uint8_t system)
{
	char bufor[9];
	itoa(val,bufor,system);
	UART_Write(bufor);
}

void UART_int16(uint16_t val, uint8_t system)
{
	char bufor[17];
	ltoa(val,bufor,system);
	UART_Write(bufor);
}