#include "../Functions.h"
#include "../ws2812.h"
#ifdef satan
//row D0, D1, D2, D3, D5
//backlight B6
//rgb E2

uint8_t rowPins[ROWS]={5,6,7,8,23};
uint8_t colPins[COLS]={21,20,24,10, 9, 4,22, 1, 0,14,13,12,11, 3};
//                 col F0,F1,E6,C7,C6,B7,D4,B1,B0,B5,B4,D7,D6,B3
#define ledcaps 2
#define fullled 15

uint16_t cindex[WS2812_COUNT]={0,34,68,102,136,170};
uint16_t delayval;
uint8_t r,c,i;
uint8_t delay_after=0;
uint8_t delay_before=0;
uint8_t ledmacro=0;
uint8_t hexaKeys0[ROWS][COLS] = {
	{MACRO2,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_0,KEY_MINUS,KEY_EQUAL,KEY_BACKSPACE},
	{KEY_TAB,KEY_Q,KEY_W,KEY_E,KEY_R,KEY_T,KEY_Y,KEY_U,KEY_I,KEY_O,KEY_P,KEY_LEFT_BRACE,KEY_RIGHT_BRACE,KEY_BACKSLASH},
	{KEY_CAPS_LOCK,KEY_A,KEY_S,KEY_D,KEY_F,KEY_G,KEY_H,KEY_J,KEY_K,KEY_L,KEY_SEMICOLON,KEY_QUOTE,0x00,KEY_ENTER},
	{KEY_LEFT_SHIFT,0x00,KEY_Z,KEY_X,KEY_C,KEY_V,KEY_B,KEY_N,KEY_M,KEY_COMMA,KEY_PERIOD,KEY_SLASH,0x00,KEY_RIGHT_SHIFT},
	{KEY_LEFT_CTRL,KEY_FN,KEY_LEFT_ALT,0x00,0x00,KEY_SPACE,0x00,0x00,0x00,0x00,KEY_RIGHT_ALT,KEY_FN,KEY_FN, KEY_RIGHT_CTRL}
};
uint8_t hexaKeys1[ROWS][COLS] = {
	{KEY_TILDE,KEY_F1,KEY_F2,KEY_F3,KEY_F4,KEY_F5,KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,KEY_F11,KEY_F12, KEY_DELETE},
	{KEY_TAB,0x00,KEY_UP,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{KEY_CAPS_LOCK,KEY_LEFT,KEY_DOWN,KEY_RIGHT,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,KEY_ENTER},
	{KEY_LEFT_SHIFT,0x00,KEY_NUM_LOCK,KEY_SCROLL_LOCK,KEY_INSERT,KEY_PRINTSCREEN,MACRO0,MACRO1,MACRO3,AUDIO_VOL_DOWN,AUDIO_VOL_UP,AUDIO_MUTE,0x00,KEY_RIGHT_SHIFT},
	{KEY_LEFT_CTRL,KEY_FN,KEY_LEFT_ALT,0x00,0x00,KEY_SPACE,0x00,0x00,0x00,0x00,KEY_RIGHT_ALT,KEY_FN,KEY_FN,KEY_RIGHT_CTRL}
};
//keymask_bits:7-press 654-hexatype0 3-press 210-hexatype1
//type: 1-key 2-modifykey 3-mousekey 4-systemkey 5-consumerkey 6-FN 7-macro
uint8_t keymask[ROWS][COLS] = {
	{0x71,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11},
	{0x11,0x10,0x11,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10},
	{0x11,0x11,0x11,0x11,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x11},
	{0x22,0x00,0x11,0x11,0x11,0x11,0x17,0x17,0x17,0x15,0x15,0x15,0x00,0x22},
	{0x22,0x66,0x22,0x00,0x00,0x11,0x00,0x00,0x00,0x00,0x22,0x66,0x66,0x22}
};
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
void Open_LED(){
	digitalWrite(ledcaps,LOW);
}
void Close_LED(){
	digitalWrite(ledcaps,HIGH);
}

void init_LED(){
	pinMode(ledcaps,OUTPUT);
	digitalWrite(ledcaps,LOW);
	pinMode(fullled,OUTPUT);
	digitalWrite(fullled,LOW);
	ledmacro=0;
	/*
	WS2812Setup();delayval=Maxdelay;
	WS2812Clear();
	WS2812Send();
	*/
}
void LED(){
	if((keyboard_buffer.keyboard_leds&(1<<1))==(1<<1)){
	digitalWrite(ledcaps,LOW);}else{ digitalWrite(ledcaps,HIGH);}
	if(ledmacro & (1<<0)){digitalWrite(fullled,LOW);}else{digitalWrite(fullled,HIGH);}
	/*
	if(delayval>=Maxdelay){
		if(ledmacro & (1<<1)){
			for(uint8_t i=0;i<WS2812_COUNT;i++){
				uint8_t r=pgm_read_byte(Rcolors+cindex[i]);
				uint8_t g=pgm_read_byte(Gcolors+cindex[i]);
				uint8_t b=pgm_read_byte(Bcolors+cindex[i]);
				WS2812SetRGB(i,r,g,b);
				cindex[i]++;
				if(cindex[i]>=WS2812ColorCount) cindex[i]=0;
			}
			}else{
			WS2812Clear();
		}
		delayval--;
		WS2812Send();
		}else{
		if(delayval){delayval--;}
		else {delayval=Maxdelay;}
	}
	*/
}
uint8_t usb_macro_send(){
	ledmacro^=macroreport;
	return 0;
}
/////////////////////////////////////////////////////////////////////
void XDMode(){
	FN=0xF0;
	for (r = 0; r < ROWS; r++) {
		pinMode(rowPins[r],OUTPUT);
		digitalWrite(rowPins[r],LOW);
		for (c = 0; c < COLS; c++) {
			if (digitalRead(colPins[c])) {keymask[r][c]&= ~0x88;}
			else {keymask[r][c]|= 0x88;delay_after=_delay_after;}
			if(keymask[r][c]==0xEE )FN=0x0F;
		}
		init_rows();
	}
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
	////////////////////////////////////////////////
	init_cols();
	init_rows();
	while (1) {//÷ÿ∆Ù
		init_LED();
		EnableRecv=1;
		keyboard_buffer.enable_pressing=1;
		//ResetMatrixFormEEP();
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
				XDMode();
				LED();
			}
		}
	}
	return 0;
}
#endif