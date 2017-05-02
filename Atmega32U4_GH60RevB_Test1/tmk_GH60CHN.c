/*
* tmk_GH60CHN.c
*
*  Created on: 2014Äê7ÔÂ25ÈÕ
*      Author: Administrator
*/

#include "Functions.h"
#include "Matrix_GH60CHN.h"

uint8_t r, c;
//lrow B6 F5 F6 F7 F4 B2
//row D0 D1 D2 D3 D5
//col F0 F1 E6 C7 C6 B7 D4 B0 B1 B5 B4 D7 D6 B3

void read_cols(uint8_t row) {
	if (PINF & (1 << 0)) {keytype[row][0]&= ~0x01;} else {keytype[row][0]|= 0x01;}
	if (PINF & (1 << 1)) {keytype[row][1]&= ~0x01;} else {keytype[row][1]|= 0x01;}
	if (PINE & (1 << 6)) {keytype[row][2]&= ~0x01;} else {keytype[row][2]|= 0x01;}
	if (PINC & (1 << 7)) {keytype[row][3]&= ~0x01;} else {keytype[row][3]|= 0x01;}
	if (PINC & (1 << 6)) {keytype[row][4]&= ~0x01;} else {keytype[row][4]|= 0x01;}
	if (PINB & (1 << 7)) {keytype[row][5]&= ~0x01;} else {keytype[row][5]|= 0x01;}
	if (PIND & (1 << 4)) {keytype[row][6]&= ~0x01;} else {keytype[row][6]|= 0x01;}
	if (PINB & (1 << 0)) {keytype[row][7]&= ~0x01;} else {keytype[row][7]|= 0x01;}
	if (PINB & (1 << 1)) {keytype[row][8]&= ~0x01;} else {keytype[row][8]|= 0x01;}
	if (PINB & (1 << 5)) {keytype[row][9]&= ~0x01;} else {keytype[row][9]|= 0x01;}
	if (PINB & (1 << 4)) {keytype[row][10]&= ~0x01;} else {keytype[row][10]|= 0x01;}
	if (PIND & (1 << 7)) {keytype[row][11]&= ~0x01;} else {keytype[row][11]|= 0x01;}
	if (PIND & (1 << 6)) {keytype[row][12]&= ~0x01;} else {keytype[row][12]|= 0x01;}
	if (PINB & (1 << 3)) {keytype[row][13]&= ~0x01;} else {keytype[row][13]|= 0x01;}
}
void pullDownRows(uint8_t row) {
	switch (row) {
		case 0:
		DDRD |= (1 << 0);
		PORTD &= ~(1 << 0);
		break;
		case 1:
		DDRD |= (1 << 1);
		PORTD &= ~(1 << 1);
		break;
		case 2:
		DDRD |= (1 << 2);
		PORTD &= ~(1 << 2);
		break;
		case 3:
		DDRD |= (1 << 3);
		PORTD &= ~(1 << 3);
		break;
		case 4:
		DDRD |= (1 << 5);
		PORTD &= ~(1 << 5);
		break;
	}
}
void unselect_rows(void) {
	DDRD &= ~0b00101111;
	PORTD|=0b00101111;
}
void init_cols(void) {
	DDRF &= ~(1 << 0 | 1 << 1);
	PORTF |= (1 << 0 | 1 << 1);
	DDRE &= ~(1 << 6);
	PORTE |= (1 << 6);
	DDRD &= ~(1 << 7 | 1 << 6 | 1 << 4);
	PORTD |= (1 << 7 | 1 << 6 | 1 << 4);
	DDRC &= ~(1 << 7 | 1 << 6);
	PORTC |= (1 << 7 | 1 << 6);
	DDRB &= ~(1 << 7 | 1 << 5 | 1 << 4 | 1 << 3 | 1 << 1 | 1 << 0);
	PORTB |= (1 << 7 | 1 << 5 | 1 << 4 | 1 << 3 | 1 << 1 | 1 << 0);
}
uint8_t FN=0;
//keytype ==> 0,mask_FN,mask_ModifierKeys,mask_Codekeys,0,0,pressFN,press_keys
void pokerMode(){
	while (1) {
	FN=0;
		for (r = 0; r < ROWS; r++) {
			pullDownRows(r);
			for (c = 0; c < COLS; c++) {
				if (digitalRead(colPins[c])) {keytype[r][c]&= ~0x01;} else {keytype[r][c]|= 0x01;}
				if(keytype[r][c]==0x41)FN=0x02;
			}
			unselect_rows();
		}
		releaseAll();
			for (r = 0; r < ROWS; r++) {
				for (c = 0; c < COLS; c++) {
					switch(keytype[r][c]|FN){
						case 0x21 :pressModifierKeys(hexaKeys[r][c]);
						case 0x23 :pressModifierKeys(hexaKeys[r][c]);
						case 0x11 :presskey(hexaKeys[r][c]);
						case 0x13 :presskey(hexaKeys2[r][c]);
					}
				}
			}
		usb_keyboard_send();
		///////////////////////////////////
	}
}
int main(void) {
	CPU_PRESCALE(CPU_16MHz);
	usb_init();
	while (!usb_configured()){_delay_ms(300);}
	
	//  TCCR0A = 0x00;
	//	TCCR0B =(1<<CS00);
	//	TIMSK0 = (1<<TOIE0);
	////////////////////////////////////////////////
	init_cols();
	unselect_rows();
	_delay_ms(500);
	pokerMode();
	
	return 0;
}
/*
ISR(TIMER0_OVF_vect)
{

}
*/

