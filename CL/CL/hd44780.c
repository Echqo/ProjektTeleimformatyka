#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include "hd44780.h"

void rejestr(unsigned char data, unsigned char WriteOrErase)
{
if(WriteOrErase)
tempdata=(tempdata|data);
else
tempdata&=~(data);

ST_0;
#if MODE==1
	#if ENABLE_Q7_Q6==1
		unsigned char local = 0x80;
	#elif ENABLE_Q7_Q6==0
		unsigned char local = 0x20;
	#endif
	while(local)
	{
	SH_0;
	if(tempdata & local)DS_1; else DS_0;
	SH_1;
	local>>=1;
	}
#elif MODE==2
	#if ENABLE_Q7_Q6==1
	SH_0;
	if(tempdata & 0x80)DS_1; else DS_0;
	SH_1;
	SH_0;
	if(tempdata & 0x40)DS_1; else DS_0;
	SH_1;	
	# endif
	
	SH_0;
	if(tempdata & 0x20)DS_1; else DS_0;
	SH_1;
	SH_0;
	if(tempdata & 0x10)DS_1; else DS_0;
	SH_1;
	SH_0;
	if(tempdata & 0x08)DS_1; else DS_0;
	SH_1;
	SH_0;
	if(tempdata & 0x04)DS_1; else DS_0;
	SH_1;
	SH_0;
	if(tempdata & 0x02)DS_1; else DS_0;
	SH_1;
	SH_0;
	if(tempdata & 0x01)DS_1; else DS_0;
	SH_1;
	
#endif
ST_1;
}

void WriteToLCD (uint8_t v,unsigned char rs)
{
if(rs==1)RS_1; else RS_0;
	
	E_1;

    if(v&0x10) D4_1; else D4_0;
    if(v&0x20) D5_1; else D5_0;
    if(v&0x40) D6_1; else D6_0;
    if(v&0x80) D7_1; else D7_0;
    E_0;  
    _delay_ms(2);
	
	E_1;
	
    if(v&0x01) D4_1; else D4_0;
    if(v&0x02) D5_1; else D5_0;
    if(v&0x04) D6_1; else D6_0;
    if(v&0x08) D7_1; else D7_0;
    E_0;
	_delay_ms(2);
	
	}
void lcd_init(void)
{

DDRn|=(1<<DS_PIN)|(1<<ST_PIN)|(1<<SH_PIN);

//WriteToLCD(0x02,LCDCOMMAND);	//cursor home (pos 0,0)
#ifdef CHINA_SHIT
_delay_ms(20);
#endif
LCD_FUNCTION_SET(LCD_FUNCTION_SET_D);
LCD_DISPLAY(LCD_DISPLAY_D);
cls;
LCD_ENTRY_MODE_SET(LCD_ENTRY_MODE_SET_I);
}
void lcd_RAM(char *str)
{
    unsigned char i =0;

    while(str[i])
        LCD_WRITE_DATA(str[i++]) ;
}
void lcd_FLASH(const char *str)
{
unsigned char znak=0;
	while((znak=pgm_read_byte(str++)))
		LCD_WRITE_DATA(znak);
}
void lcd_int(uint8_t val,uint8_t system)
{
	char bufor[9];
	itoa(val,bufor,system);
	lcd_RAM(bufor);
}
void lcd_int16(uint16_t val,uint8_t system)
{
	char bufor[17];
	ltoa(val,bufor,system);
	lcd_RAM(bufor);
}

#ifdef USE_TEMP_CHAR
void definiuj_stopien_Celsjusza(void)
{
	unsigned char tab[8]={6,9,9,6,0,0,0,0};//°
	WriteToLCD(0x40,LCDCOMMAND);
	for(uint8_t i=0;i<8;i++)
	{
		WriteToLCD(tab[i],LCDDATA);
	}
	WriteToLCD(0x00,LCDDATA);
}
/*
	definiuj_stopien_Celsjusza();
	pos(0,0);
	WriteToLCD(0,LCDDATA);
*/

#endif
