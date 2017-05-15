#include "Functions.h"
#include <string.h>

uint8_t releasekey(uint8_t key)
{uint8_t i=0;
	uint8_t send_required=0;
	for ( i=0; i < 6; i++) {
		if (keyboard_keys[i] == key) {
			keyboard_keys[i] = 0;
			send_required=1;
			break;
		}
	}
	return send_required;
}
void releaseAll()
{
	memset(keyboard_keys,0,6);
	keyboard_modifier_keys=0;
}
uint8_t presskey(uint8_t key)
{uint8_t i=0;
	uint8_t send_required=0;
	for ( i=0; i < 6; i++) {
		if (keyboard_keys[i] == key) {
			send_required = 1;
			return send_required;
		}
	}
	for ( i=0; i < 6; i++) {
		if (keyboard_keys[i] == 0) {
			keyboard_keys[i] = key;
			send_required = 1;
			return send_required;
		}
	}
	return send_required;
}
void pressModifierKeys(uint8_t key)
{
	keyboard_modifier_keys|=key;
}
void releaseModifierKeys(uint8_t key)
{
	keyboard_modifier_keys&=~key;
}
#if defined(__AVR_ATmega32U4__)
void closeJtag(){
MCUCR = (1<<JTD);
MCUCR = (1<<JTD);

}

void pinMode(uint8_t IO,uint8_t value){
	switch(IO){
		case 0: if(value){DDRB|= (1<<0);}else{DDRB &= ~(1<<0);}break;
		case 1: if(value){DDRB|= (1<<1);}else{DDRB &= ~(1<<1);}break;
		case 2: if(value){DDRB|= (1<<2);}else{DDRB &= ~(1<<2);}break;
		case 3: if(value){DDRB|= (1<<3);}else{DDRB &= ~(1<<3);}break;
		case 4: if(value){DDRB|= (1<<7);}else{DDRB &= ~(1<<7);}break;
		case 5: if(value){DDRD|= (1<<0);}else{DDRD &= ~(1<<0);}break;
		case 6: if(value){DDRD|= (1<<1);}else{DDRD &= ~(1<<1);}break;
		case 7: if(value){DDRD|= (1<<2);}else{DDRD &= ~(1<<2);}break;
		case 8: if(value){DDRD|= (1<<3);}else{DDRD &= ~(1<<3);}break;
		case 9: if(value){DDRC|= (1<<6);}else{DDRC &= ~(1<<6);}break;
		case 10: if(value){DDRC|= (1<<7);}else{DDRC &= ~(1<<7);}break;
		case 11: if(value){DDRD|= (1<<6);}else{DDRD &= ~(1<<6);}break;
		case 12: if(value){DDRD|= (1<<7);}else{DDRD &= ~(1<<7);}break;
		case 13: if(value){DDRB|= (1<<4);}else{DDRB &= ~(1<<4);}break;
		case 14: if(value){DDRB|= (1<<5);}else{DDRB &= ~(1<<5);}break;
		case 15: if(value){DDRB|= (1<<6);}else{DDRB &= ~(1<<6);}break;
		case 16: if(value){DDRF|= (1<<7);}else{DDRF &= ~(1<<7);}break;
		case 17: if(value){DDRF|= (1<<6);}else{DDRF &= ~(1<<6);}break;
		case 18: if(value){DDRF|= (1<<5);}else{DDRF &= ~(1<<5);}break;
		case 19: if(value){DDRF|= (1<<4);}else{DDRF &= ~(1<<4);}break;
		case 20: if(value){DDRF|= (1<<1);}else{DDRF &= ~(1<<1);}break;
		case 21: if(value){DDRF|= (1<<0);}else{DDRF &= ~(1<<0);}break;
		case 22: if(value){DDRD|= (1<<4);}else{DDRD &= ~(1<<4);}break;
		case 23: if(value){DDRD|= (1<<5);}else{DDRD &= ~(1<<5);}break;
		case 24: if(value){DDRE|= (1<<6);}else{DDRE &= ~(1<<6);}break;
	}
}
void digitalWrite(uint8_t IO,uint8_t value){
	switch(IO){
		case 0: if(value){PORTB |= (1<<0);}else{PORTB &= ~(1<<0);}break;
		case 1: if(value){PORTB|= (1<<1);}else{PORTB &= ~(1<<1);}break;
		case 2: if(value){PORTB|= (1<<2);}else{PORTB &= ~(1<<2);}break;
		case 3: if(value){PORTB|= (1<<3);}else{PORTB &= ~(1<<3);}break;
		case 4: if(value){PORTB|= (1<<7);}else{PORTB &= ~(1<<7);}break;
		case 5: if(value){PORTD|= (1<<0);}else{PORTD &= ~(1<<0);}break;
		case 6: if(value){PORTD|= (1<<1);}else{PORTD &= ~(1<<1);}break;
		case 7: if(value){PORTD|= (1<<2);}else{PORTD &= ~(1<<2);}break;
		case 8: if(value){PORTD|= (1<<3);}else{PORTD &= ~(1<<3);}break;
		case 9: if(value){PORTC|= (1<<6);}else{PORTC &= ~(1<<6);}break;
		case 10: if(value){PORTC|= (1<<7);}else{PORTC &= ~(1<<7);}break;
		case 11: if(value){PORTD|= (1<<6);}else{PORTD &= ~(1<<6);}break;
		case 12: if(value){PORTD|= (1<<7);}else{PORTD &= ~(1<<7);}break;
		case 13: if(value){PORTB|= (1<<4);}else{PORTB &= ~(1<<4);}break;
		case 14: if(value){PORTB|= (1<<5);}else{PORTB &= ~(1<<5);}break;
		case 15: if(value){PORTB|= (1<<6);}else{PORTB &= ~(1<<6);}break;
		case 16: if(value){PORTF|= (1<<7);}else{PORTF &= ~(1<<7);}break;
		case 17: if(value){PORTF|= (1<<6);}else{PORTF &= ~(1<<6);}break;
		case 18: if(value){PORTF|= (1<<5);}else{PORTF &= ~(1<<5);}break;
		case 19: if(value){PORTF|= (1<<4);}else{PORTF &= ~(1<<4);}break;
		case 20: if(value){PORTF|= (1<<1);}else{PORTF &= ~(1<<1);}break;
		case 21: if(value){PORTF|= (1<<0);}else{PORTF &= ~(1<<0);}break;
		case 22: if(value){PORTD|= (1<<4);}else{PORTD &= ~(1<<4);}break;
		case 23: if(value){PORTD|= (1<<5);}else{PORTD &= ~(1<<5);}break;
		case 24: if(value){PORTE|= (1<<6);}else{PORTE &= ~(1<<6);}break;
	}
}
uint8_t digitalRead(uint8_t IO){
	uint8_t value=0;
	switch(IO){
		case 0:  value= PINB & (1<<0);break;
		case 1:  value= PINB& (1<<1) ;break;
		case 2:  value= PINB& (1<<2) ;break;
		case 3:  value= PINB& (1<<3) ;break;
		case 4:  value= PINB& (1<<7) ;break;
		case 5:  value= PIND& (1<<0) ;break;
		case 6:  value= PIND& (1<<1) ;break;
		case 7:  value= PIND& (1<<2) ;break;
		case 8:  value= PIND& (1<<3) ;break;
		case 9:  value= PINC& (1<<6) ;break;
		case 10:  value= PINC& (1<<7);break;
		case 11:  value= PIND& (1<<6);break;
		case 12:  value= PIND& (1<<7);break;
		case 13:  value= PINB& (1<<4);break;
		case 14:  value= PINB& (1<<5);break;
		case 15:  value= PINB& (1<<6);break;
		case 16:  value= PINF& (1<<7);break;
		case 17:  value= PINF& (1<<6);break;
		case 18:  value= PINF& (1<<5);break;
		case 19:  value= PINF& (1<<4);break;
		case 20:  value= PINF& (1<<1);break;
		case 21:  value= PINF& (1<<0);break;
		case 22:  value= PIND& (1<<4);break;
		case 23:  value= PIND& (1<<5);break;
		case 24:  value= PINE& (1<<6);break;
	}
	return value;
}
#endif