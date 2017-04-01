#ifndef UART_H
#define UART_H


void UART_Init(void);

void UART_Char(char data);

void UART_Write(const char *string);

char UART_Read(void);

void UART_crlf(void);

void UART_int(uint8_t val, uint8_t system);

void UART_int16(uint16_t val, uint8_t system);

#endif