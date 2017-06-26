#include "Functions.h"
#include "ws2812.h"
#ifdef xd60
//lrow B6 F5 F6 F7 F4 B2
//row D0 D1 D2 D3 D5
//col F0,F1,E6,C7,C6,B6,D4,B1,B7,B5,B4,D7,D6,B3
uint8_t i,delayval;
uint8_t rowPins[ROWS]={5,6,7,8,23};
uint8_t colPins[COLS]={21,20,24,10,9,15,22,1,4,14,13,12,11,3};
//                     1  2  3  4  5  6  7 8 9 10 11 12 13 14

uint8_t hexaKeys0[ROWS][COLS] = {
	{KEY_ESC,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_0,KEY_MINUS,KEY_EQUAL,KEY_BACKSPACE},
	{KEY_TAB,KEY_Q,KEY_W,KEY_E,KEY_R,KEY_T,KEY_Y,KEY_U,KEY_I,KEY_O,KEY_P,KEY_LEFT_BRACE,KEY_RIGHT_BRACE,KEY_BACKSLASH},
	{KEY_CAPS_LOCK,KEY_A,KEY_S,KEY_D,KEY_F,KEY_G,KEY_H,KEY_J,KEY_K,KEY_L,KEY_SEMICOLON,KEY_QUOTE,0x00,KEY_ENTER},
	{KEY_LEFT_SHIFT,0x00,KEY_Z,KEY_X,KEY_C,KEY_V,KEY_B,KEY_N,KEY_M,KEY_COMMA,KEY_PERIOD,0x00, KEY_SLASH,KEY_UP},
	{KEY_LEFT_CTRL,KEY_FN,KEY_LEFT_ALT,0x00,0x00,KEY_SPACE,0x00,KEY_SLASH,KEY_LEFT,0x00,KEY_FN,KEY_RIGHT_CTRL,KEY_DOWN, KEY_RIGHT}
};
uint8_t hexaKeys1[ROWS][COLS] = {
	{KEY_TILDE,KEY_F1,KEY_F2,KEY_F3,KEY_F4,KEY_F5,KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,KEY_F11,KEY_F12, KEY_DELETE},
	{KEY_TAB,KEYPAD_1,KEYPAD_2,KEYPAD_3,KEYPAD_4,KEYPAD_5,KEYPAD_6,KEYPAD_7,KEYPAD_8,KEYPAD_9,KEYPAD_0,KEYPAD_MINUS,KEYPAD_PLUS,KEY_BACKSLASH},
	{KEY_CAPS_LOCK, MOUSE_LEFT,MOUSE_MID,MOUSE_RIGHT,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,KEY_ENTER},
	{KEY_LEFT_SHIFT,0x00,KEY_NUM_LOCK ,KEY_SCROLL_LOCK,KEY_INSERT,KEY_PRINTSCREEN,0x00,0x00,0x00,AUDIO_VOL_DOWN,AUDIO_VOL_UP,0x00,0x00,KEY_UP},
	{KEY_LEFT_CTRL,KEY_FN,KEY_LEFT_ALT,0x00,0x00,KEY_SPACE,0x00,0x00,KEY_LEFT,0x00,KEY_FN,KEY_RIGHT_CTRL,KEY_DOWN,KEY_RIGHT}
};
//keymask_bits:7-press 654-hexatype0 3-press 210-hexatype1
//type: 1-key 2-modifykey 3-mousekey 4-systemkey 5-consumerkey 6-FN 7-consumerkeyAL,8-consumerkeyAC

uint8_t keymask[ROWS][COLS] = {
	{0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11},
	{0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11},
	{0x11,0x13,0x13,0x13,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x11},
	{0x22,0x00,0x11,0x11,0x11,0x11,0x10,0x10,0x10,0x15,0x15,0x00,0x10,0x11},
	{0x22,0x66,0x22,0x00,0x00,0x11,0x00,0x10,0x11,0x00,0x66,0x22,0x11,0x11}
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
/////////////////////////////////////////////////////////////////////
#define ledcount 2
uint8_t ledPins[ledcount]={18,2};
void Open_LED(){
	for ( i=0; i<ledcount; i++){
		digitalWrite(ledPins[i],LOW);
	}
}
void Close_LED(){

	for ( i=0; i<ledcount; i++){
		digitalWrite(ledPins[i],HIGH);
	}
}
/*
uint8_t LED_caps=2;
uint8_t LED_GPIO1=17;
uint8_t LED_GPIO2=18;
*/
void init_LED(){
	for ( i=0; i<ledcount; i++){
		pinMode(ledPins[i],OUTPUT);
		digitalWrite(ledPins[i],LOW);
	}
	WS2812Setup();delayval=Maxdelay;
	WS2812Clear();
	WS2812Send();
}
uint8_t red,green,blue;
uint8_t redm,greenm,bluem;
void LED(){
	for ( i=0; i<ledcount; i++){
		if((keyboard_buffer.keyboard_leds&(1<<i))==(1<<i)){ digitalWrite(ledPins[i],LOW);}
		else{ digitalWrite(ledPins[i],HIGH);}
	}
	if(delayval>=Maxdelay){
	if(redm)red-=5;else red+=5;	
	if(greenm)green-=3;else green+=3;
	if(bluem)blue-=4;else blue+=4;
		if((keyboard_buffer.keyboard_leds&(1<<2))==(1<<2)){
			for(char i = 0; i<WS2812_COUNT; i++)
			{
				WS2812SetRGB(i, red, green, blue);
			}
			}else{
			WS2812Clear();
		}
		WS2812Send();
		delayval--;
		if(red>=255){red=255;redm=1;}
		if(green>=255){green=255;greenm=1;}
		if(blue>=255){blue=255;bluem=1;}
		if(red<=5){red=0;redm=5;}
		if(green<=5){green=0;greenm=5;}
		if(blue<=5){blue=0;bluem=5;}
	}
	else{
		if(delayval>0 ){delayval--;}
		else {delayval=Maxdelay;}
	}
}
/////////////////////////////////////////////////////////////////////
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
	while (1) {//÷ÿ∆Ù
		init_LED();
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
				XDMode();
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