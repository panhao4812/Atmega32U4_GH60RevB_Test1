#ifndef WS2812_h_
#define WS2812_h_

#include <avr/io.h>
#include <stdlib.h>
#include "avr/pgmspace.h"

#define WS2812_COUNT	6
#define WS2812_PORT		PORTF
#define WS2812_DDR		DDRF
#define WS2812_MASK		(1<<6)
#define WS2812_SAVE		1			/*Power saver, divide light level with this.*/
#define Maxdelay 0xFF


#define nop() __asm__ __volatile__ ("nop")
//��Ƕ���ָ��nop ��ʱ 0.25us
uint8_t WS2812Buffer[(WS2812_COUNT * 3)];


void WS2812Setup(void);
void WS2812Clear(void);
void WS2812SetRGB(uint16_t led, uint8_t red, uint8_t green, uint8_t blue);
void WS2812SetHSV(uint16_t led, uint16_t hue, uint16_t saturation, uint16_t value);
void WS2812Send(void);

extern const  uint8_t Rcolors[] PROGMEM;
extern const  uint8_t Gcolors[] PROGMEM;
extern const  uint8_t Bcolors[] PROGMEM;
#endif
