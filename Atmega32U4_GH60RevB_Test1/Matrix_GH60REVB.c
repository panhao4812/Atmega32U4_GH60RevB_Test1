
#include "Functions.h"
#ifdef revB
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

uint8_t rowPins[ROWS]={5,6,7,8,23};
uint8_t colPins[COLS]={21,20,24,10,9,4,22,0,1,14,13,12,11,3};
uint8_t RowLED[ROWS+1]={15,18,17,16,19,2};
uint8_t ColLED[COLS]={21,20,24,10,9,4,22,0,1,14,13,12,11,3};

uint8_t hexaKeys[ROWS][COLS] = {
	{KEY_ESC,       KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_0,        KEY_MINUS,     KEY_EQUAL,      KEY_BACKSPACE},
	{KEY_TAB  ,     KEY_Q,KEY_W,KEY_E,KEY_R,KEY_T,KEY_Y,KEY_U,KEY_I,KEY_O,KEY_P,        KEY_LEFT_BRACE,KEY_RIGHT_BRACE,KEY_BACKSLASH},
	{KEY_CAPS_LOCK, KEY_A,KEY_S,KEY_D,KEY_F,KEY_G,KEY_H,KEY_J,KEY_K,KEY_L,KEY_SEMICOLON,KEY_QUOTE,     0x00,           KEY_ENTER},
	{KEY_LEFT_SHIFT,0x00 ,KEY_Z,KEY_X,KEY_C,KEY_V,KEY_B,KEY_N,KEY_M,KEY_COMMA,KEY_PERIOD,KEY_SLASH,    0x00,           KEY_RIGHT_SHIFT},
	{KEY_LEFT_CTRL,KEY_LEFT_GUI,KEY_LEFT_ALT ,0x00,0x00,KEY_SPACE,0x00,0x00,0x00,0x00,KEY_RIGHT_ALT,KEY_RIGHT_GUI,0x00, KEY_RIGHT_CTRL  }
};
uint8_t hexaKeys2[ROWS][COLS] = {
	{KEY_TILDE,     KEY_F1,KEY_F2,KEY_F3,KEY_F4,KEY_F5,KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,KEY_F11,KEY_F12,      KEY_DELETE},
	{KEY_TAB  ,     0x00,KEY_UP,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,KEY_BACKSLASH},
	{KEY_CAPS_LOCK, KEY_LEFT,KEY_DOWN,KEY_RIGHT,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,KEY_ENTER},
	{KEY_LEFT_SHIFT,0x00 ,KEY_NUM_LOCK,KEY_SCROLL_LOCK,KEY_INSERT,KEY_PRINTSCREEN,0x00,0x00,0x00,0x00,0x00,0x00,0x00,KEY_RIGHT_SHIFT},
	{KEY_LEFT_CTRL,KEY_LEFT_GUI,KEY_LEFT_ALT ,0x00,0x00,KEY_SPACE,0x00,0x00,0x00,0x00,KEY_RIGHT_ALT,KEY_RIGHT_GUI,0x00, KEY_RIGHT_CTRL  }
};
uint8_t keymask[ROWS][COLS] = {
	{0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10},
	{0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10},
	{0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x10},
	{0x20,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x20},
	{0x20,0x20,0x20,0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x20,0x20,0x40,0x20}
};
uint8_t r,c,i;
uint8_t FN=0;
uint8_t delay_after=0;
uint8_t delay_before=0;
uint8_t delayval;
void pokerMode(){
	while (1) {
		FN=0;
		for (r = 0; r < ROWS; r++) {
			pinMode(rowPins[r],OUTPUT);
			digitalWrite(rowPins[r],LOW);
			for (c = 0; c < COLS; c++) {
				if (digitalRead(colPins[c])) {keymask[r][c]&= ~0x01;} else {keymask[r][c]|= 0x01;delay_after=0X20;}
				if(keymask[r][c]==0x41)FN=0x02;
			}
			init_rows();
		}
		releaseAll();
		for (r = 0; r < ROWS; r++) {
			for (c = 0; c < COLS; c++) {
				if((keymask[r][c] | FN)== 0x21) pressModifierKeys(hexaKeys[r][c]);
				else if((keymask[r][c] | FN)== 0x23) pressModifierKeys(hexaKeys[r][c]);
				else if((keymask[r][c] | FN)== 0x11) presskey(hexaKeys[r][c]);
				else if((keymask[r][c] | FN)== 0x13) presskey(hexaKeys2[r][c]);
			}
		}

		if(usb_keyboard_send_required())delay_before=4
		if(delay_after==0X20 && delay_before==1)usb_keyboard_send();
		if(delay_after==1)usb_keyboard_send();
		if(delay_after>0)delay_after-=1;
		if(delay_before>0)delay_before-=1;
}
}
int init_main(void) {
	CPU_PRESCALE(CPU_16MHz);//16M�����Ƶ����
	MCUCR = (1<<JTD);
	MCUCR = (1<<JTD);
	//�ر�jtag
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
#endif

