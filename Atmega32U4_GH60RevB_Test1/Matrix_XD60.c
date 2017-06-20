#include "Functions.h"
#include "ws2812.h"
#ifdef xd60
//lrow B6 F5 F6 F7 F4 B2
//row D0 D1 D2 D3 D5
//col F0, F1, E6, C7, C6, B6, D4, B1, B0, B5, B4, D7, D6, B3
uint8_t r,c,i,delayval;
uint8_t rowPins[ROWS]={5,6,7,8,23};
uint8_t colPins[COLS]={21,20,24,10,9,15,22,1,4,14,13,12,11,3};
//                        1  2  3  4  5  6  7 8 9 10 11 12 13 14
#define ledcount 2
uint8_t ledPins[ledcount]={18,2};
/*
uint8_t LED_caps=2;
uint8_t LED_GPIO1=17;
uint8_t LED_GPIO2=18;
*/
uint8_t hexaKeys[ROWS][COLS] = {
	{KEY_ESC,       KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_0,        KEY_MINUS,     KEY_EQUAL,      KEY_BACKSPACE},
	{KEY_TAB  ,     KEY_Q,KEY_W,KEY_E,KEY_R,KEY_T,KEY_Y,KEY_U,KEY_I,KEY_O,KEY_P,        KEY_LEFT_BRACE,KEY_RIGHT_BRACE,KEY_BACKSLASH},
	{KEY_CAPS_LOCK, KEY_A,KEY_S,KEY_D,KEY_F,KEY_G,KEY_H,KEY_J,KEY_K,KEY_L,KEY_SEMICOLON,KEY_QUOTE,     0x00,           KEY_ENTER},
	{KEY_LEFT_SHIFT,0x00 ,KEY_Z,KEY_X,KEY_C,KEY_V,KEY_B,KEY_N,KEY_M,KEY_COMMA,KEY_PERIOD,0x00, KEY_SLASH,   KEY_UP       },
	{KEY_LEFT_CTRL,0x00,KEY_LEFT_ALT ,0x00,0x00,KEY_SPACE,0x00,KEY_SLASH,KEY_LEFT,0x00,0x00,KEY_RIGHT_CTRL,KEY_DOWN, KEY_RIGHT  }
};
uint8_t hexaKeys2[ROWS][COLS] = {
	{KEY_TILDE,KEY_F1,KEY_F2,KEY_F3,KEY_F4,KEY_F5,KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,KEY_F11,KEY_F12, KEY_DELETE},
	{KEY_TAB,KEYPAD_1,KEYPAD_2,KEYPAD_3,KEYPAD_4,KEYPAD_5,KEYPAD_6,KEYPAD_7,KEYPAD_8,KEYPAD_9,KEYPAD_0,KEYPAD_MINUS,KEYPAD_PLUS,KEY_BACKSLASH},
	{KEY_CAPS_LOCK, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  KEY_ENTER},
	{KEY_LEFT_SHIFT,0x00,KEY_NUM_LOCK ,KEY_SCROLL_LOCK,KEY_INSERT,KEY_PRINTSCREEN,0x00,0x00,0x00,0x00,0x00,0x00,0x00, KEY_UP       },
	{KEY_LEFT_CTRL,0x00,KEY_LEFT_ALT ,0x00,0x00,KEY_SPACE,0x00,0x00,KEY_LEFT,0x00,0x00,KEY_RIGHT_CTRL,KEY_DOWN, KEY_RIGHT  }
};
uint8_t keymask[ROWS][COLS] = {
	{0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10},
	{0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10},
	{0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x10},
	{0x20,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x10,0x10},
	{0x20,0x40,0x20,0x00,0x00,0x10,0x00,0x10,0x10,0x00,0x40,0x20,0x10,0x10}
};

void ResetMatrix(uint8_t mask,uint16_t address){
	uint8_t j=0;
	for (r = 0; r < ROWS; r++) {
		for (c = 0; c < ROWS; c++) {
			switch (mask){
				case 0:
				hexaKeys[r][c]=eeprom_read_byte((uint8_t *)((uint16_t)j+address));
				break;
				case 1:
				hexaKeys2[r][c]=eeprom_read_byte((uint8_t *)((uint16_t)j+address));
				break;
				case 2:
				keymask[r][c]=eeprom_read_byte((uint8_t *)((uint16_t)j+address));
				break;
			}
			j++;
		}
	}
}
void ResetMatrixFormEEP(){
	//////////////////////////////////menu///////////////////////
	//(u8)address_row,(u8)address_col,(u16)address_hexakeys,(u16)address_hexakeys2,(u16)address_keymask
	//0,                1,               2 ,         3,         4,         5,         6,          7
	//   8            8+5=13               8+5+14=27               8+5+14+70=97       8+5+14+140=167
	uint8_t address_row=eeprom_read_byte((uint8_t *)0);
	uint8_t address_col=eeprom_read_byte((uint8_t *)1);
	uint16_t address_hexakeys=eeprom_read_word((uint16_t *)2);
	uint16_t address_hexakeys2=eeprom_read_word((uint16_t *)4);
	uint16_t address_keymask=eeprom_read_word((uint16_t *)6);
	uint8_t j;
	///////////////////////////////////
	if(address_row==8){for( j=0;j<ROWS;j++){rowPins[j]=eeprom_read_byte((uint8_t *)((uint16_t)j+address_row));}}
	if(address_col==13){for( j=0;j<COLS;j++){colPins[j]=eeprom_read_byte((uint8_t *)((uint16_t)j+address_col));}}
	if(address_hexakeys==27){ResetMatrix(0,address_hexakeys);}
	if(address_hexakeys2==97){ResetMatrix(1,address_hexakeys2);}
	if(address_keymask==167){ResetMatrix(2,address_keymask);}
}
uint8_t FN=0;
uint8_t delay_after=0;
uint8_t delay_before=0;
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
void init_LED(){
	for ( i=0; i<ledcount; i++){
		pinMode(ledPins[i],OUTPUT);
		digitalWrite(ledPins[i],LOW);
	}
	WS2812Setup();delayval=Maxdelay;
	WS2812Clear();
	WS2812Send();
}
void LED(){
	for ( i=0; i<ledcount; i++){
		if((keyboard_buffer.keyboard_leds&(1<<i))==(1<<i)){ digitalWrite(ledPins[i],LOW);}
		else{ digitalWrite(ledPins[i],HIGH);}
	}
	if(delayval>=Maxdelay){
		if((keyboard_buffer.keyboard_leds&(1<<2))==(1<<2)){
			for(char i = 0; i<WS2812_COUNT; i++)
			{
				WS2812SetRGB(i, 50*i, 255-50*i, 50*i);
			}
			}else{
			WS2812Clear();
		}
		WS2812Send();
		delayval--;
	}
	else{
		if(delayval>0 ){delayval--;}
		else {delayval=Maxdelay;}
	}
}
void pokerMode(){
	FN=0;
	for (r = 0; r < ROWS; r++) {
		pinMode(rowPins[r],OUTPUT);
		digitalWrite(rowPins[r],LOW);
		for (c = 0; c < COLS; c++) {
			if (digitalRead(colPins[c])) {keymask[r][c]&= ~0x01;} else {keymask[r][c]|= 0x01;delay_after=_delay_after;}
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
	if(usb_keyboard_send_required())delay_before=_delay_before	;
	if(delay_after==_delay_after && delay_before==1)usb_keyboard_send();
	if(delay_after==1)usb_keyboard_send();
	if(delay_after>0)delay_after-=1;
	if(delay_before>0)delay_before-=1;
}
void eepwrite(){
	//	address,word1,word2,word3
	if (EnableRecv==0){
		uint16_t address=raw_report_out.word[0];
		if(address==0xF1FF && keyboard_buffer.enable_pressing==1 ){
			keyboard_buffer.enable_pressing=0;
		}
		else if(address==0xF2FF && keyboard_buffer.enable_pressing==0 ){
			keyboard_buffer.enable_pressing=2;
		}
		else{
			if(keyboard_buffer.enable_pressing==0){
				for ( i=0; i<ledcount; i++){
					digitalWrite(ledPins[i],LOW);
				}
				if(address<(maxEEP-1)){
					eeprom_busy_wait();
					eeprom_write_word ((uint16_t *)address,raw_report_out.word[1]);
				}
				if((address+2)<(maxEEP-1)){
					eeprom_busy_wait();
					eeprom_write_word ((uint16_t *)(address+2),raw_report_out.word[2]);
				}
				if((address+4)<(maxEEP-1)){
					eeprom_busy_wait();
					eeprom_write_word ((uint16_t *)(address+4),raw_report_out.word[3]);
				}
				for ( i=0; i<ledcount; i++){
					digitalWrite(ledPins[i],HIGH);
				}
			}
		}
		memset(&raw_report_out, 0,sizeof(raw_report_out));
		EnableRecv=1;
	}
}
int init_main(void) {
	CPU_PRESCALE(CPU_16MHz);//16M¾§Õñ·ÖÆµÉèÖÃ
	closeJtag();
	usb_init();
	while (!usb_configured()){_delay_ms(300);}
	//  TCCR0A = 0x00;
	//	TCCR0B =(1<<CS00);
	//	TIMSK0 = (1<<TOIE0);
	////////////////////////////////////////////////
	init_LED();
	init_cols();
	init_rows();
	while (1) {//ÖØÆô
		EnableRecv=1;
		keyboard_buffer.enable_pressing=1;
		ResetMatrixFormEEP();
		_delay_ms(500);
		releaseAll();
		usb_keyboard_send();
		while (1) {
			eepwrite();
			if(keyboard_buffer.enable_pressing==2){
				break;
			}
			else if(keyboard_buffer.enable_pressing==1){
				pokerMode();
				LED();
			}
		}
	}
	return 0;
}
#endif
/*
ISR(TIMER0_OVF_vect)
{

}
*/