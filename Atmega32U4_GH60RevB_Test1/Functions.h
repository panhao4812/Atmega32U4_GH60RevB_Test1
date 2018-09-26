#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include "usb_keyboard.h"

//////////////////////////////////////////////////////
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

void pinMode(uint8_t IO,uint8_t value);
void digitalWrite(uint8_t IO,uint8_t value);
uint8_t digitalRead(uint8_t IO);
void closeJtag();
//////////////////////override//////////////////////////
uint8_t FN;
uint8_t RGB_Type;// bit1-> 0 off 1 on ;bit0-> 0 fix£¬1 Rainbow

extern  uint8_t hexaKeys0[ROWS][COLS] ;
extern  uint8_t hexaKeys1[ROWS][COLS];
extern  uint8_t keymask[ROWS][COLS];
extern  uint8_t rowPins[ROWS];
extern  uint8_t colPins[COLS];


#define _delay_after 0x08
#define _delay_before 0x03
#define add1 10
#define add2 add1+ROWS //15 15
#define add3 add2+COLS //30 29
#define add4 add3+(ROWS*COLS) //30+15*5=105 99
#define add5 add4+(ROWS*COLS) //105+75=180 169
#define addRGB add5+(ROWS*COLS) //180+75=255 239
#define addPrint addRGB+(WS2812_COUNT*3) //255+6*3=273 275
int init_main(void);
void init_cols();
void init_rows();

void pokerMode();
void XD60Mode();
void XD75Mode();
void eepwrite();
void ResetMatrixFormEEP();
uint8_t IsBufferClear();
void init_LED();
void Open_LED();
void Close_LED();
void LED();

////////////////////////////////////////////////////////
#endif /* */