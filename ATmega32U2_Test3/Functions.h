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

extern  uint8_t hexaKeys0[ROWS][COLS] ;
extern  uint8_t hexaKeys1[ROWS][COLS];
extern  uint8_t keymask[ROWS][COLS];
extern  uint8_t rowPins[ROWS];
extern  uint8_t colPins[COLS];


#define _delay_after 0x09
#define _delay_before 0x03
int init_main(void);
void init_cols();
void init_rows();

void pokerMode();
void XDMode();
void BfaceMode();
void eepwrite();
void ResetMatrixFormEEP();

void init_LED();
void Open_LED();
void Close_LED();
void LED();

////////////////////////////////////////////////////////
#endif /* */