#include "Functions.h"

void ClearMouse(){
	memset(&mouse_report, 0, sizeof(mouse_report));
	memset(&mouse_buffer,0,sizeof(mouse_buffer));
	mouse_report.mouse.report_id= REPORT_ID_MOUSE;
	mouse_report.system_keys.report_id= REPORT_ID_SYSTEM;
	mouse_report.consumer_keys.report_id= REPORT_ID_CONSUMER;
	mouse_buffer.mouse_protocol=1;
}
void ClearKeyboard(){
	memset( &keyboard_report, 0,sizeof(keyboard_report));
	memset( &keyboard_buffer, 0,sizeof(keyboard_buffer));
	keyboard_buffer.enable_pressing=1;
	// protocol setting from the host.  We use exactly the same reportMOUSE_ENABLE
	// either way, so this variable only stores the setting since we
	// are required to be able to report which setting is in use.
	keyboard_buffer.keyboard_protocol=1;
	// the idle configuration, how often we send the report to the
	// host (ms * 4) even when it hasn't changed
	keyboard_buffer.keyboard_idle_config=125;
	// count until idle timeout
	keyboard_buffer.keyboard_idle_count=0;
}
void ClearRaw(){
	memset( &raw_report_in, 0,sizeof(raw_report_in));
	memset(&raw_report_out, 0,sizeof(raw_report_out));
}

void pressmousekey(uint8_t key){
	mouse_buffer.mouse_keys|=key;
}
void releasemousekey(uint8_t key){
	mouse_buffer.mouse_keys&=~key;
}
void releaseAllmousekey(){
	mouse_buffer.mouse_keys=0x00;
}
uint8_t releasekey(uint8_t key)
{
	uint8_t i;
	uint8_t send_required=0;
	for ( i=0; i < 6; i++) {
		if (keyboard_buffer.keyboard_keys[i] == key) {
			keyboard_buffer.keyboard_keys[i] = 0;
			send_required=1;
			break;
		}
	}
	return send_required;
}
void releaseAll()
{
	uint8_t i;
	for ( i=0; i < 6; i++) {
		keyboard_buffer.keyboard_keys[i] = 0;
	}
	keyboard_buffer.keyboard_modifier_keys=0;
}
uint8_t presskey(uint8_t key)
{
	uint8_t i;
	for ( i=0; i < 6; i++) {
		if (keyboard_buffer.keyboard_keys[i] == key) {
			return 1;
		}
	}
	for ( i=0; i < 6; i++) {
		if (keyboard_buffer.keyboard_keys[i] == 0) {
			keyboard_buffer.keyboard_keys[i] = key;
			return 1;
		}
	}
	return 0;
}
void pressModifierKeys(uint8_t key)
{
	keyboard_buffer.keyboard_modifier_keys|=key;
}
void releaseModifierKeys(uint8_t key)
{
	keyboard_buffer.keyboard_modifier_keys&=~key;
}
uint8_t usb_keyboard_send_required(){
uint8_t send_required_t=0;
	if(keyboard_report.modifier!=keyboard_buffer.keyboard_modifier_keys)
	{keyboard_report.modifier = keyboard_buffer.keyboard_modifier_keys;send_required_t=1;}
	if(keyboard_report.keycode[0]!=keyboard_buffer.keyboard_keys[0])
	{keyboard_report.keycode[0]=keyboard_buffer.keyboard_keys[0];send_required_t=1;}
	if(keyboard_report.keycode[1]!=keyboard_buffer.keyboard_keys[1])
	{keyboard_report.keycode[1]=keyboard_buffer.keyboard_keys[1];send_required_t=1;}
	if(keyboard_report.keycode[2]!=keyboard_buffer.keyboard_keys[2])
	{keyboard_report.keycode[2]=keyboard_buffer.keyboard_keys[2];send_required_t=1;}
	if(keyboard_report.keycode[3]!=keyboard_buffer.keyboard_keys[3])
	{keyboard_report.keycode[3]=keyboard_buffer.keyboard_keys[3];send_required_t=1;}
	if(keyboard_report.keycode[4]!=keyboard_buffer.keyboard_keys[4])
	{keyboard_report.keycode[4]=keyboard_buffer.keyboard_keys[4];send_required_t=1;}
	if(keyboard_report.keycode[5]!=keyboard_buffer.keyboard_keys[5])
	{keyboard_report.keycode[5]=keyboard_buffer.keyboard_keys[5];send_required_t=1;}
	if(send_required_t)keyboard_buffer.Send_Required=send_required_t;
	return send_required_t;	
}
uint8_t usb_keyboard_send()
{
	if(keyboard_buffer.Send_Required){
	keyboard_buffer.Send_Required=0;
		uint8_t send_required_t=usb_send(KEYBOARD_ENDPOINT,(uint8_t *)&keyboard_report,8,50);
		return send_required_t;
	}return 1;
}
uint8_t usb_mouse_send_required(){
	uint8_t send_required_t=0;
	if(mouse_report.mouse.buttons!=mouse_buffer.mouse_keys)
	{
		mouse_report.mouse.buttons=mouse_buffer.mouse_keys;
		send_required_t=REPORT_ID_MOUSE;
	}
	if(mouse_report.system_keys.usage!=mouse_buffer.system_keys)
	{
		mouse_report.system_keys.usage=mouse_buffer.system_keys;
		send_required_t=REPORT_ID_SYSTEM;
	}
	if(mouse_report.consumer_keys.usage!=mouse_buffer.consumer_keys)
	{
		mouse_report.consumer_keys.usage=mouse_buffer.consumer_keys;
		send_required_t=REPORT_ID_CONSUMER;
	}
	if(send_required_t)mouse_buffer.Send_Required=send_required_t;
	return send_required_t;
}
uint8_t usb_mouse_send(){
	uint8_t intr_state,timeout;
	if(mouse_buffer.Send_Required==0)return 1;
	if (!usb_configured()) return 1;
	intr_state = SREG;
	cli();
	timeout = FrameNumber() + 50;
	SetEP(MOUSE_ENDPOINT);
	while (1) {
		if (ReadWriteAllowed()) break;
		SREG = intr_state;
		if (!usb_configured()) return 1;
		if (FrameNumber() == timeout) return 1;
		intr_state = SREG;
		cli();
		SetEP(MOUSE_ENDPOINT);
	}
	switch(mouse_buffer.Send_Required){
		case REPORT_ID_MOUSE:
		mouse_buffer.Send_Required=0;
		UEDATX=mouse_report.mouse.report_id;
		UEDATX=mouse_report.mouse.buttons;
		UEDATX=mouse_report.mouse.x;
		UEDATX=mouse_report.mouse.y;
		UEDATX=mouse_report.mouse.v;
		UEDATX=mouse_report.mouse.h;
		break;
		case REPORT_ID_SYSTEM:
		mouse_buffer.Send_Required=0;
		UEDATX=mouse_report.system_keys.report_id;
		UEDATX=LSB(mouse_report.system_keys.usage);
		UEDATX=MSB(mouse_report.system_keys.usage);
		break;
		case REPORT_ID_CONSUMER:
		mouse_buffer.Send_Required=0;
		UEDATX=mouse_report.consumer_keys.report_id;
		UEDATX=LSB(mouse_report.consumer_keys.usage);
		UEDATX=MSB(mouse_report.consumer_keys.usage);
		break;
	}
	ReleaseTX();
	SREG = intr_state;
	return 0;
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
/*
0		PIN_B0
1		PIN_B1
2		PIN_B2
3		PIN_B3
4		PIN_B7
5		PIN_D0
6		PIN_D1
7		PIN_D2
8		PIN_D3
9		PIN_C6
10		PIN_C7
11		PIN_D6
12		PIN_D7
13		PIN_B4
14		PIN_B5
15		PIN_B6
16		PIN_F7
17		PIN_F6
18		PIN_F5
19		PIN_F4
20		PIN_F1
21		PIN_F0
22		PIN_D4
23		PIN_D5
24		PIN_E6
*/
#endif