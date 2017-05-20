/*
 * CL.c
 *
 * Created: 2017-05-03 18:12:20
 * Author : Admin
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include "hd44780.h"
#include "UART.h"

volatile uint8_t indeks_uart=0,flaga_uart=0;
volatile char bufor_uart[64];
volatile char znak;
uint8_t EEMEM ee_nr_urzadzenia=2;
#define debug
ISR(USART_RX_vect)
{
	znak=UDR0;
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

uint8_t client_connected, nr_urzadzenia,cwjap;
inline uint8_t znajdz(const char *pattern);
void esp_startup(void)
{
	enum esp_enum {ESP_ATE=1,ESP_CWMODE,ESP_CIPSTA,ESP_CWJAP,ESP_CIPMODE,ESP_CPSTART,ESP_SUCCESS};
	static uint8_t esp_status=0,connected=0;
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
		if(esp_status && esp_status <= ESP_SUCCESS)
		{	
			esp_status++;
		}
		new_cmd=1;
		memset(bufor_uart,0,64);
	}
	if(znajdz("WIFI GOT"))
	{
		connected=1;
	}
	if(znajdz("+CWJAP"))
	{
		cwjap=1;
		//memset(bufor_uart,0,64);
		return;
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
				UART_Write("AT+CWMODE=1");UART_crlf();
				break;
			}
			case ESP_CIPSTA:
			{
				#ifdef debug
				pos(20,1);
				lcd("CIPSTA");
				#endif
				_delay_ms(10);
				UART_Write("AT+CIPSTA=\"192.168.4.");
				UART_Char(nr_urzadzenia+'0');
				UART_Write("\",\"192.168.4.0\",\"255.255.255.0\"");UART_crlf();
				break;
			}
			case ESP_CWJAP:
			{
				#ifdef debug
				pos(10,0);
				lcd("CWJAP");
				#endif
				_delay_ms(10);
				UART_Write("AT+CWJAP=\"PTPT\",\"12341234\"");UART_crlf();
				break;
			}
			case ESP_CIPMODE:
			{
				if(connected)
				{
					connected=0;
					#ifdef debug
					pos(10,1);
					lcd("CIPMODE");
					#endif
					_delay_ms(10);
					UART_Write("AT+CIPMODE=1");UART_crlf();
				}
				break;
			}
			case ESP_CPSTART:
			{
				#ifdef debug
				pos(30,0);
				lcd("CPST");
				#endif
				_delay_ms(10);
				UART_Write("AT+CIPSTART=\"TCP\",\"192.168.4.0\",81");UART_crlf();
				break;
			}
			
			case ESP_SUCCESS:
			{
				client_connected=1;
				break;
			}
		}//switch
	}//new cmd
}
/*
case ESP_CIPSEND:
{
	#ifdef debug
	pos(30,1);
	lcd("CIPSEND");
	#endif
	_delay_ms(10);
	UART_Write("AT+CIPSEND");UART_crlf();
	break;
}
*/

int main(void)
{
	UART_Init();
	lcd_init();

	nr_urzadzenia = eeprom_read_byte(&ee_nr_urzadzenia);
	lcd("WiFi Client");
	char wifi_strength[4];
	sei();
    /* Replace with your application code */
    while (1) 
    {
		if(flaga_uart)
		{
			flaga_uart=0;
			esp_startup();
			if(cwjap)
			{
				pos(0,1);
				strcpy(wifi_strength,&bufor_uart[37]);
				lcd_RAM(wifi_strength);
				lcd("  ");
				UART_Write("AT+CIPSEND");UART_crlf();
				_delay_ms(20);
				UART_int(nr_urzadzenia,10);
				UART_Char(',');
				UART_Write(wifi_strength);
				_delay_ms(40);
				UART_Write("+++");
				_delay_ms(40);
				UART_Write("AT+CWJAP?");UART_crlf();
			}
		}
		if(client_connected)
		{
			client_connected=0;
			cls;
			lcd("Connected");
			UART_Write("AT+CWJAP?");UART_crlf();
		}
    }
}

inline uint8_t znajdz(const char *pattern)
{
	if(strstr(bufor_uart,pattern))
	return 1;
	return 0;
}
