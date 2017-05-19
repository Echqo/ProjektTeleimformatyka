#include <avr/io.h>
#include <stdlib.h>
#include "UART.h"
//
//void UART2_Init(void)
//{
	//UBRR2H=0;
	//UBRR2L=8;		//kolego dla 115200 8 przy @16Mhz ;)
	//UCSR2B=(1<<TXEN2)|(1<<RXEN2)|(1<<RXCIE2);
	//UCSR2C=(1<<UCSZ20)|(1<<UCSZ21);		
//}
//
//void UART2_Char(char data)
//{
	//while(!(UCSR2A & (1<<UDRE2)));
	//UDR2=data;
//}
//
//void UART2_Write(const char *string)
//{
	//while(*string)
	//{
		//UART2_Char(*string++);
	//}	
//}
//char UART2_Read(void)
//{
	//while(!(UCSR2A & (1<<RXC2)));
	//return UDR2;
//}
//
//void UART2_crlf()
//{
	//UART2_Char(13);
	//UART2_Char(10);
//}
//
//void UART2_int(uint8_t val, uint8_t system)
//{
	//char bufor[9];
	//itoa(val,bufor,system);
	//UART2_Write(bufor);
//}
//
//void UART2_int16(uint16_t val, uint8_t system)
//{
	//char bufor[17];
	//ltoa(val,bufor,system);
	//UART2_Write(bufor);
//}

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


void UART_crlf(void)
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