#ifndef WS2812B_H_
#define WS2812B_H_

#define WS2812B_PORT PORTD
#define WS2812B_PIN PD2
void WS2812B_sendone(void);
void WS2812B_sendzero(void);
void WS2812B_send(uint8_t r, uint8_t g, uint8_t b);
#endif