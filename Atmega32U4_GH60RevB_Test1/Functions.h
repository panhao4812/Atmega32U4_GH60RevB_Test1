#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

//#define  xd60
#define revB
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "usb_keyboard.h"

#define LED_CONFIG	(DDRE |= (1<<6))
#define LED_OFF		(PORTE &= ~(1<<6))
#define LED_ON	(PORTE |= (1<<6))

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
#define CPU_16MHz       0x00
#define CPU_8MHz        0x01
#define CPU_4MHz        0x02
#define CPU_2MHz        0x03
#define CPU_1MHz        0x04
#define CPU_500kHz      0x05
#define CPU_250kHz      0x06
#define CPU_125kHz      0x07
#define CPU_62kHz       0x08

#define LOW 0
#define HIGH 1
#define INPUT 0
#define OUTPUT 1
#define ROWS  5
#define COLS  14
extern  uint8_t hexaKeys[ROWS][COLS] ;
extern  uint8_t hexaKeys2[ROWS][COLS];
extern  uint8_t keymask[ROWS][COLS];
extern  uint8_t rowPins[ROWS];
extern  uint8_t colPins[COLS];
int init_main(void);
void init_cols();
void init_rows();
void pinMode(uint8_t IO,uint8_t value);
void digitalWrite(uint8_t IO,uint8_t value);
uint8_t digitalRead(uint8_t IO);
uint8_t releasekey(uint8_t key);
void releaseAll();
uint8_t presskey(uint8_t key);
void pressModifierKeys(uint8_t key);
void releaseModifierKeys(uint8_t key);
void closeJtag();
#endif /* */