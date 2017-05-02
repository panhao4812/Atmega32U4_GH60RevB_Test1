/*
* tmk_GH60CHN.c
*
*  Created on: 2014Äê7ÔÂ25ÈÕ
*      Author: zian1
*/

#include "Functions.h"
#include "Matrix_GH60CHN.h"

uint8_t r, c,i;
//lrow B6 F5 F6 F7 F4 B2
//row D0 D1 D2 D3 D5
//col F0 F1 E6 C7 C6 B7 D4 B0 B1 B5 B4 D7 D6 B3

void init_rows(void) {
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
uint8_t send_required=0;
	uint8_t change=0;
//keytype ==> 0,mask_FN,mask_ModifierKeys,mask_Codekeys,0,0,pressFN,press_keys
void pokerMode(){
	while (1) {
	FN=0;
		for (r = 0; r < ROWS; r++) {
		    pinMode(rowPins[r],OUTPUT);
			digitalWrite(rowPins[r],LOW);
			for (c = 0; c < COLS; c++) {
				if (digitalRead(colPins[c])) {keytype[r][c]&= ~0x01;} else {keytype[r][c]|= 0x01;send_required=0X40;}
				if(keytype[r][c]==0x41)FN=0x02;
			}
			init_rows();
		}
		releaseAll();
			for (r = 0; r < ROWS; r++) {
				for (c = 0; c < COLS; c++) {
						if((keytype[r][c] | FN)== 0x21) pressModifierKeys(hexaKeys[r][c]);
			       else if((keytype[r][c] | FN)== 0x23) pressModifierKeys(hexaKeys[r][c]);
				   else if((keytype[r][c] | FN)== 0x11) presskey(hexaKeys[r][c]);
				   else if((keytype[r][c] | FN)== 0x13) presskey(hexaKeys2[r][c]);					
				}
			}
			 change=0;
			if(keyboard_modifier_keys2!=keyboard_modifier_keys){
				keyboard_modifier_keys2=keyboard_modifier_keys;
				change=1;
			}
			for (i = 0; i < 6; i++) {
			if(keyboard_keys2[i]!=keyboard_keys[i]){
				keyboard_keys2[i]=keyboard_keys[i];
				change=1;
			}
			}
	if(send_required==0X40 && change==1)usb_keyboard_send();
	if(send_required==1)usb_keyboard_send();
	if(send_required>0)send_required-=1;
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
	init_rows();
	_delay_ms(500);
	releaseAll();
	usb_keyboard_send();
	pokerMode();	
	return 0;
}
/*
ISR(TIMER0_OVF_vect)
{

}
*/

