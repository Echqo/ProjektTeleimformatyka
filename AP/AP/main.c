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

#define indeks_poczatkowy_ramki 36

volatile uint8_t indeks_uart=0,flaga_uart=0,cnt=0;
volatile char bufor_uart[64];
volatile char znak;

uint8_t server_ready=0;
#define debug
inline uint8_t* znajdz(const char *pattern);
ISR(USART_RX_vect)
{
	znak=UDR0;
	cnt++;
	if(znak==10 || znak ==13)
	{
		if(indeks_uart)
		{
			indeks_uart=0;
			flaga_uart=1;
		}
	}
	if(znak!=13 && znak !=10)
	{
		bufor_uart[indeks_uart++]=znak;
		indeks_uart &= 0x3f;		//utnij gdyby indeks > 63
	}
}
void esp_startup(void)
{
	enum esp_enum {ESP_ATE=1,ESP_CWMODE,ESP_CIPAP,ESP_CWSAP,ESP_CIPMUX,ESP_CIPSERVER,ESP_CISPSTO,ESP_SUCCESS};
	static uint8_t esp_status=0;
	uint8_t new_cmd=0;
	if(znajdz("ready"))
	{
		#ifdef debug
			pos(0,0);
			lcd("ready");
		#endif
		esp_status = ESP_ATE;
		new_cmd=1;
	}
	else if(znajdz("OK"))
	{
		if(esp_status && esp_status < ESP_SUCCESS)
		{	esp_status++;}
		new_cmd=1;
		memset(bufor_uart,0,64);
	}
	if(new_cmd)
	{
		new_cmd=0;
		switch(esp_status)
		{
			case ESP_ATE:
			{
				#ifdef debug
					pos(0,1);
					lcd("ATE0");
				#endif
				new_cmd=0;
				_delay_ms(10);
				UART_Write("ATE0");UART_crlf();
				break;
			}
			case ESP_CWMODE:
			{
				#ifdef debug
					pos(20,0);
					lcd("CWMODE");
				#endif
				_delay_ms(10);
				UART_Write("AT+CWMODE=2");UART_crlf();
				break;
			}
			case ESP_CIPAP:
			{
				#ifdef debug
					pos(20,1);
					lcd("CIPAP");
				#endif
				_delay_ms(10);
				UART_Write("AT+CIPAP_DEF=\"192.168.4.0\",\"192.168.4.0\",\"255.255.255.0\"");UART_crlf();
				break;
			}
			case ESP_CWSAP:
			{
				#ifdef debug
					pos(10,0);
					lcd("CWSAP");
				#endif	
				_delay_ms(10);
				UART_Write("AT+CWSAP_DEF=\"PTPT\",\"12341234\",6,4");UART_crlf();
				break;
			}
			case ESP_CIPMUX:
			{
				#ifdef debug
					pos(10,1);
					lcd("CPMU");
				#endif
				_delay_ms(10);
				UART_Write("AT+CIPMUX=1");UART_crlf();
				break;
			}
			case ESP_CIPSERVER:
			{
				#ifdef debug
					pos(30,0);
					lcd("CPSV");
				#endif
				_delay_ms(10);
				UART_Write("AT+CIPSERVER=1,81");UART_crlf();
				break;
			}
			case ESP_CISPSTO:
			{
				#ifdef debug
					pos(30,1);
					lcd("CPST");
				#endif
				_delay_ms(10);
				UART_Write("AT+CIPSTO=100");UART_crlf();
				break;
			}
			case ESP_SUCCESS:
			{
				lcd("START");
				server_ready=1;
				break;
			}
		}//switch		
	}//new cmd
}
int main(void)
{
	UART_Init();
	lcd_init();
	sei();
  
	lcd("WiFi signal");
	
	uint8_t *ptr;
	char wifi_strength[4];
//	UART_Write("AT");UART_crlf();
    while (1) 
    {
		if(flaga_uart)
		{
			flaga_uart=0;
			esp_startup();
			
		}
		if(server_ready==1)
		{
			server_ready=0;
			cls;
			lcd("START");
		}
    }
}

inline uint8_t* znajdz(const char *pattern)
{
	return(strstr(bufor_uart,pattern));	
}

//strcpy(wifi_strength,&bufor_uart[indeks_poczatkowy_ramki]);
//lcd_RAM(wifi_strength);
//
//_delay_ms(100);
//memset(bufor_uart,0,sizeof(bufor_uart));
//
//UART1_Write("AT+CWJAP?");UART1_crlf();