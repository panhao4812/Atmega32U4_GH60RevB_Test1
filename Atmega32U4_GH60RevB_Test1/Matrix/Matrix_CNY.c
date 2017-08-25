
#include "../Functions.h"
#ifdef CNY
//lrow B6 F5 F6 F7 B2 F4 
//row D0 D1 D2 D3 D5
//col F0 F1 E6 C7 C6 B7 D4 B0 B1 B5 B4 D7 D6 B3

void init_ledrows(void) {
	DDRB |= (1 << 2 | 1 << 6);
	PORTB&=~(1 << 2 | 1 << 6);
	DDRF |= (1 << 4 | 1 << 5 | 1 << 6 | 1 << 7);
	PORTF&=~( 1 << 5 | 1 << 6 | 1 << 7);
}
void init_rows(void) {
	DDRD&=~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3 | 1 << 5);
	PORTD|=(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3 | 1 << 5);
}
void init_cols(void) {
	DDRF&=~(1 << 0 | 1 << 1);
	PORTF |= (1 << 0 | 1 << 1);
	DDRE&=~(1 << 6);
	PORTE |= (1 << 6);
	DDRD&=~(1 << 7 | 1 << 6 | 1 << 4);
	PORTD |= (1 << 7 | 1 << 6 | 1 << 4);
	DDRC&=~(1 << 7 | 1 << 6);
	PORTC|=(1 << 7 | 1 << 6);
	DDRB&=~(1 << 7 | 1 << 5 | 1 << 4 | 1 << 3 | 1 << 1 | 1 << 0);
	PORTB |= (1 << 7 | 1 << 5 | 1 << 4 | 1 << 3 | 1 << 1 | 1 << 0);
}
void init_ledcols(){
	DDRF |= (1 << 0 | 1 << 1);
	PORTF&=~(1 << 0 | 1 << 1);
	DDRE |= (1 << 6);
	PORTE&=~(1 << 6);
	DDRD |= (1 << 7 | 1 << 6 | 1 << 4);
	PORTD&=~(1 << 7 | 1 << 6 | 1 << 4);
	DDRC |= (1 << 7 | 1 << 6);
	PORTC&=~(1 << 7 | 1 << 6);
	DDRB |= (1 << 7 | 1 << 5 | 1 << 4 | 1 << 3 | 1 << 1 | 1 << 0);
	PORTB&=~(1 << 7 | 1 << 5 | 1 << 4 | 1 << 3 | 1 << 1 | 1 << 0);
}
uint8_t rowPins[ROWS]={5,6,7,8,23};
uint8_t colPins[COLS]={21,20,24,10,9,4,22,0,1,14,13,12,11,3};
uint8_t RowLED[ROWS+1]={15,18,17,16,2,19};
void Open_LED(){}
void Close_LED(){}
void init_LED(){}
uint8_t hexaKeys0[ROWS][COLS] = {
	{KEY_ESC,       KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_0,        KEY_MINUS,     KEY_EQUAL,      KEY_BACKSPACE},
	{KEY_TAB  ,     KEY_Q,KEY_W,KEY_E,KEY_R,KEY_T,KEY_Y,KEY_U,KEY_I,KEY_O,KEY_P,        KEY_LEFT_BRACE,KEY_RIGHT_BRACE,KEY_BACKSLASH},
	{KEY_CAPS_LOCK, KEY_A,KEY_S,KEY_D,KEY_F,KEY_G,KEY_H,KEY_J,KEY_K,KEY_L,KEY_SEMICOLON,KEY_QUOTE,     0x00,           KEY_ENTER},
	{KEY_LEFT_SHIFT,0x00 ,KEY_Z,KEY_X,KEY_C,KEY_V,KEY_B,KEY_N,KEY_M,KEY_COMMA,KEY_PERIOD,KEY_SLASH,    0x00,           KEY_RIGHT_SHIFT},
	{KEY_LEFT_CTRL,KEY_LEFT_GUI,KEY_LEFT_ALT ,0x00,0x00,KEY_SPACE,0x00,0x00,0x00,0x00,KEY_RIGHT_ALT,KEY_RIGHT_GUI,KEY_FN, KEY_RIGHT_CTRL  }
};
uint8_t hexaKeys1[ROWS][COLS] = {
	{KEY_TILDE,     KEY_F1,KEY_F2,KEY_F3,KEY_F4,KEY_F5,KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,KEY_F11,KEY_F12,      KEY_DELETE},
	{KEY_TAB  ,     0x00,KEY_UP,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,KEY_BACKSLASH},
	{KEY_CAPS_LOCK, KEY_LEFT,KEY_DOWN,KEY_RIGHT,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,KEY_ENTER},
	{KEY_LEFT_SHIFT,0x00 ,KEY_NUM_LOCK,KEY_SCROLL_LOCK,KEY_INSERT,KEY_PRINTSCREEN,0x00,0x00,0x00,0x00,0x00,0x00,0x00,KEY_RIGHT_SHIFT},
	{KEY_LEFT_CTRL,KEY_LEFT_GUI,KEY_LEFT_ALT ,0x00,0x00,KEY_SPACE,0x00,0x00,0x00,0x00,KEY_RIGHT_ALT,KEY_RIGHT_GUI,KEY_FN, KEY_RIGHT_CTRL  }
};
uint8_t keymask[ROWS][COLS] = {
	{0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11},
	{0x11,0x10,0x11,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x11},
	{0x11,0x11,0x11,0x11,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x11},
	{0x22,0x00,0x11,0x11,0x11,0x11,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x22},
	{0x22,0x22,0x22,0x00,0x00,0x11,0x00,0x00,0x00,0x00,0x22,0x22,0x66,0x22}
};
uint8_t keymaskled[ROWS][COLS] = {
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
};
uint8_t r,c;
uint8_t stepLED=0;
uint8_t delay_after=0;
uint8_t delay_before=0;
void LED(){
init_ledcols();
digitalWrite(RowLED[stepLED],HIGH);
for (c = 0; c < COLS; c++) {
	if(keymaskled[stepLED][c]){digitalWrite(colPins[c],HIGH);}else{digitalWrite(colPins[c],LOW);}
	if(keymaskled[stepLED][c])keymaskled[stepLED][c]--;
}
if((keyboard_buffer.keyboard_leds&(1<<0))==(1<<0) && stepLED==0 )
{ digitalWrite(colPins[13],HIGH);}
if((keyboard_buffer.keyboard_leds&(1<<1))==(1<<1) && stepLED==2 )
{ digitalWrite(colPins[0],HIGH);}
if((keyboard_buffer.keyboard_leds&(1<<2))==(1<<2))
{PORTF|= (1 << 4);}else{PORTF&=~(1 << 4);}
stepLED++;
if(stepLED>=ROWS)stepLED=0;
}
uint8_t usb_macro_send(){
	return 0;
}
void PokerMode(){
///////////////////////////////////////////////
	init_ledrows();init_rows();init_cols();
	FN=0xF0;
	for (r = 0; r < ROWS; r++) {
		pinMode(rowPins[r],OUTPUT);
		digitalWrite(rowPins[r],LOW);
		for (c = 0; c < COLS; c++) {
			if (digitalRead(colPins[c])) {keymask[r][c]&= ~0x88;}
			else {keymask[r][c]|= 0x88;delay_after=_delay_after;keymaskled[r][c]=0x6C;}
			if(keymask[r][c]==0xEE )FN=0x0F;
		}
		init_rows();
	}
//////////////////////////////////////////////	
	LED();
//////////////////////////////////////////////
	releaseAllkeyboardkeys();
	releaseAllmousekeys();
	macrobuffer=0;
	for (r = 0; r < ROWS; r++) {
		for (c = 0; c < COLS; c++) {
			switch(keymask[r][c]&FN){
				case 0x90:
				presskey(hexaKeys0[r][c]);
				break;
				case 0xA0:
				pressModifierKeys(hexaKeys0[r][c]);
				break;
				case 0xB0:
				pressmousekey(hexaKeys0[r][c]);
				break;
				case 0xC0:
				presssystemkey(hexaKeys0[r][c]);
				break;
				case 0xD0:
				pressconsumerkey(hexaKeys0[r][c]);
				break;
				case 0xF0:
				pressmacrokey(hexaKeys0[r][c]);
				break;
				case 0x09:
				presskey(hexaKeys1[r][c]);
				break;
				case 0x0A:
				pressModifierKeys(hexaKeys1[r][c]);
				break;
				case 0x0B:
				pressmousekey(hexaKeys1[r][c]);
				break;
				case 0x0C:
				presssystemkey(hexaKeys1[r][c]);
				break;
				case 0x0D:
				pressconsumerkey(hexaKeys1[r][c]);
				break;
				case 0x0F:
				pressmacrokey(hexaKeys1[r][c]);
				break;
			}
		}
	}
	if(usb_macro_send_required())delay_before=_delay_before;
	if(usb_keyboard_send_required())delay_before=_delay_before;
	if(usb_mouse_send_required())delay_before=_delay_before;
	if(delay_after==_delay_after && delay_before==1)
	{usb_macro_send();usb_keyboard_send();usb_mouse_send();}
	if(delay_after==1)
	{usb_macro_send();usb_keyboard_send();usb_mouse_send();}
	if(delay_after>0)delay_after-=1;
	if(delay_before>0)delay_before-=1;
}
int init_main(void) {
	CPU_PRESCALE(CPU_16MHz);//16Mæß’Ò∑÷∆µ…Ë÷√
	closeJtag();
	usb_init();
	while (!usb_configured()){_delay_ms(300);}
	//  TCCR0A = 0x00;
	//	TCCR0B =(1<<CS00);
	//	TIMSK0 = (1<<TOIE0);
	////////////////////////////////////////////////
	init_cols();
	init_rows();
	init_ledrows();
	while (1) {//÷ÿ∆Ù
		EnableRecv=1;
		keyboard_buffer.enable_pressing=1;
		ResetMatrixFormEEP();
		_delay_ms(500);
		releaseAllkeyboardkeys();
		releaseAllmousekeys();
		usb_send(KEYBOARD_ENDPOINT,(uint8_t *)&keyboard_report,8,50);
		while (1) {
			eepwrite();
			if(keyboard_buffer.enable_pressing==2){
				break;
			}
			else if(keyboard_buffer.enable_pressing==1){
				PokerMode();
			}
		}
	}
	return 0;
}
#endif

