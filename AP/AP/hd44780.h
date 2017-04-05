#ifndef LCD_HD44780_H
#define LCD_HD44780_H

#include <avr/pgmspace.h> //PSTR

#define DDRn DDRD
#define PORTn PORTD

#define DS_PIN PD5
#define ST_PIN PD6
#define SH_PIN PD7

#define DS_0 PORTn&=~(1<<DS_PIN) 
#define DS_1 PORTn|=(1<<DS_PIN)
#define ST_0 PORTn&=~(1<<ST_PIN) 
#define ST_1 PORTn|=(1<<ST_PIN)
#define SH_0 PORTn&=~(1<<SH_PIN)
#define SH_1 PORTn|=(1<<SH_PIN) 


/*		rejestr(0xaa);
*	MODE				6 bit		8 bit
*	1 SLOW while >>=1	17 us		12.87 us
*	2 FAST 8x if		11.75 us	8.875 us
SPI 8MHz / 4						6.375 us
SPI 8MHz / 2						4.375 us
*/

/*		lcd("A");
*	MODE				6 bit		8 bit
*	1 SLOW while >>=1	3.213 ms	3.265 ms 
*	2 FAST 8x if		3.161 ms	3.196 ms 
SPI 8MHz / 4						3.265 ms
SPI 8MHz / 2						3.265 ms
*/
#define MODE 2

/*
*	ENABLE_Q7_Q6
*	0 Q7 Q6 DISABLED hi-Z
*	1 Q7 Q6 ENABLED
*/

#define ENABLE_Q7_Q6 0


/* RS */

#define RS_1      rejestr(0x01,LCDWRITE)
#define RS_0      rejestr(0x01,LCDERASE)


/* RW */
//RW CONNECT TO GND
//#define RW1      rejestr(0x02,LCDWRITE)
//#define RW0      rejestr(0x02,LCDERASE)


/* E */

#define E_1       rejestr(0x02,LCDWRITE)
#define E_0       rejestr(0x02,LCDERASE)


/* D4 */

#define D4_1      rejestr(0x04,LCDWRITE)
#define D4_0      rejestr(0x04,LCDERASE)


/* D5 */

#define D5_1      rejestr(0x08,LCDWRITE)
#define D5_0      rejestr(0x08,LCDERASE)


/* D6 */

#define D6_1      rejestr(0x10,LCDWRITE)
#define D6_0      rejestr(0x10,LCDERASE)


/* D7 */

#define D7_1      rejestr(0x20,LCDWRITE)
#define D7_0      rejestr(0x20,LCDERASE)

#define LCDWRITE 1
#define LCDERASE 0

#define LCDCOMMAND 0
#define LCDDATA    1

//#define USE_TEMP_CHAR
#ifdef USE_TEMP_CHAR

	#define lcd_stopien_celsjusza WriteToLCD(0,LCDDATA);lcd("C")

	void definiuj_stopien_Celsjusza(void);

#endif

#define pos(x,y)  WriteToLCD(0x80|((x)+((y)*0x40)), LCDCOMMAND)

#define cls             WriteToLCD(0x01, LCDCOMMAND)

//LCD ENTRY MODE SET RS=0 R/W=0 data= 000001IS
#define LCD_ENTRY_MODE_SET_S 0x01 //po zapisaniu znaku kursor nie zmienia polozenia, natomiast przesuwa sie cala zawartosc wyswietlacza,
								  //dla 0 po zapisaniu znaku kursor zmienia polozenie, a przesuwanie okna jest wylaczone
#define LCD_ENTRY_MODE_SET_I 0x02 //kursor lub okno wyswietlacza przesuwa sie w prawo (inkrementacja adresu znaku)
								  //kursor lub okno wysiwetlacza przesuwa sie w lewo (dekrementacja adresu znaku)
#define LCD_ENTRY_MODE_SET(SI) WriteToLCD(0x04|(SI),LCDCOMMAND)

//LCD DISPLAY RS=0 R/W=0 data= 00001DCB
#define LCD_DISPLAY_D 0x04	//wlacz LCD
#define LCD_DISPLAY_C 0x02	//wlacz kursor
#define LCD_DISPLAY_B 0x01	//wlacz miganie kursora
#define LCD_DISPLAY(DCB) WriteToLCD(0x08|(DCB),LCDCOMMAND)

//LCD DISPLAY CURSOR SHIFT RS=0 R/W=0 data= 0001SRxx
#define LCD_CURSOR_S 0x08 //przesuwana jest zawartosc okna, dla 0 przesuwany jest kursor
#define LCD_CURSOR_R 0x04 //kierunek przesuwania w prawo, dla 0 kierunek przesuwania w lewo
#define LCD_CURSOR_SHIFT(SR) WriteToLCD(0x01|(SR),LCDCOMMAND)

//LCD FUNCTION SET RS=0 R/W=0 data= 001DNFxx
#define LCD_FUNCTION_SET_D 0x10 //interfejs 8 bitowy, dla 0 interfejs 4 bitowy
#define LCD_FUNCTION_SET_N 0x08 //wyswietlacz dwuwierszowy, dla 0 jednowierszowy
#define LCD_FUNCTION_SET_F 0x04 //matryca 5*10, dla 0 5*7
#define LCD_FUNCTION_SET(DNF) WriteToLCD(0x02|(DNF),LCDCOMMAND);

#define LCD_WRITE_DATA(D)      WriteToLCD((D),LCDDATA)

void rejestr(unsigned char data,unsigned char WriteOrErase);
volatile static uint8_t tempdata;
void lcd_RAM(char *str);
void lcd_init(void);
void WriteToLCD(uint8_t v,unsigned char rs);
void lcd_FLASH(const char *str);
void lcd_int(uint8_t val, uint8_t system);
void lcd_int16(uint16_t val, uint8_t system);
#define lcd(tekst) lcd_FLASH(PSTR(tekst))

#endif
