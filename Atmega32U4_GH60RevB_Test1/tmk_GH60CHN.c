/*
* tmk_GH60CHN.c
*
*  Created on: 2014Äê7ÔÂ25ÈÕ
*      Author: zian1
*/

#include "Functions.h"
#include "Matrix_XD60.h"

uint8_t r, c,i;
//lrow B6 F5 F6 F7 F4 B2
//row D0 D1 D2 D3 D5
//col F0 F1 E6 C7 C6 B7 D4 B0 B1 B5 B4 D7 D6 B3

void init_cols(){
	for ( i=0; i<COLS; i++){
		pinMode(colPins[i],INPUT);
		digitalWrite(colPins[i],HIGH);
	}
}
void init_rows(){
	for ( i=0; i<ROWS; i++){
		pinMode(rowPins[i],INPUT);
		digitalWrite(rowPins[i],HIGH);
	}
}
uint8_t FN=0;
uint8_t send_required=0;
uint8_t change=0;
//keytype ==> 0,mask_FN,mask_ModifierKeys,mask_Codekeys,0,0,pressFN,press_keys
void init_LED(){
	#if(defined MATRIX_XD60_H_)
	for ( i=0; i<3; i++){
		pinMode(ledPins[i],OUTPUT);
		digitalWrite(ledPins[i],LOW);
	}
	#endif
}
void LED(){
	#if(defined MATRIX_XD60_H_)	
	for ( i=0; i<3; i++){
		if((keyboard_leds&(1<<i))==(1<<i)){ digitalWrite(ledPins[i],LOW);break;}
		else{ digitalWrite(ledPins[i],HIGH);}
	}	
	 #endif
}
void pokerMode(){
	while (1) {
		FN=0;
		for (r = 0; r < ROWS; r++) {
			pinMode(rowPins[r],OUTPUT);
			digitalWrite(rowPins[r],LOW);
			for (c = 0; c < COLS; c++) {
				if (digitalRead(colPins[c])) {keytype[r][c]&= ~0x01;} else {keytype[r][c]|= 0x01;send_required=0X20;}
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
		if(send_required==0X20 && change==1)usb_keyboard_send();
		if(send_required==1)usb_keyboard_send();
		if(send_required>0)send_required-=1;
		LED();
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
	init_LED();
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

