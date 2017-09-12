#include "../functions.h"
#include "../ws2812.h"
#ifdef staryu
uint8_t i,FN;
uint8_t ledmacro=0;//��¼led״̬
uint16_t delayval;//rgb����
uint8_t r,c;
uint8_t delay_after=0;//backswing
uint8_t delay_before=0;//windup
uint16_t cindex[WS2812_COUNT]={0};
//SW D0 D1 D2 D3 D4
//LED C2 C7 D5 D6 B0
//RGB C6
uint8_t colPins[COLS]={16,17,18,19,20};
uint8_t ledPins[COLS]={10,15,21,22,0};
uint8_t rowPins[ROWS]={0xFF};
uint8_t hexaKeys0[ROWS][COLS]={
	{KEY_UP,MACRO4,KEY_RIGHT,KEY_DOWN,KEY_LEFT}
};
uint8_t hexaKeys1[ROWS][COLS]={
	{MACRO0,MACRO4,KEY_C,KEY_X,KEY_Z}
};
uint8_t keymask[ROWS][COLS]={
	{0x17,0x66,0x17,0x17,0x17}
};
uint8_t keymaskled[ROWS][COLS] = {
	{0x00,0x00,0x00,0x00,0x00}
};
void init_cols(){
	DDRD &=~0x1F;
	PORTD|= 0x1F;
}
void init_rows(){
}
void Open_LED(){
	for ( i=0; i<COLS; i++){
		digitalWrite(ledPins[i],LOW);
	}
}
void Close_LED(){
	for ( i=0; i<COLS; i++){
		digitalWrite(ledPins[i],HIGH);
	}
}
void init_LED(){
	for(i=0;i<COLS;i++){
		pinMode(ledPins[i],OUTPUT);
		digitalWrite(ledPins[i],HIGH);
	}
	WS2812Setup();delayval=Maxdelay;
	WS2812Clear();
	WS2812Send();
	ledmacro=0;
}
uint8_t usb_macro_send(){
	ledmacro^=macroreport;
	return 0;
}
void LED_macro1(){
	if((ledmacro & (1<<0))==0){
		for (c = 0; c < COLS; c++) {
			if(keymaskled[0][c]){digitalWrite(ledPins[c],HIGH);}else{digitalWrite(ledPins[c],LOW);}
			if(keymaskled[0][c])keymaskled[0][c]--;
		}
	}
}
void LED(){
	//////////////////////////////FULL LRF/////////////////////
	if(ledmacro & (1<<0))
	{Open_LED();}else{Close_LED();}
	////////////////////////////RGB////////////////////////
	if(delayval>=Maxdelay){
		if((ledmacro & (1<<1))||(keyboard_buffer.keyboard_leds&(1<<2))){
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
		WS2812Send2();
		}else{
		if(delayval){delayval--;}
		else {delayval=Maxdelay;}
	}
}

void StaryuMode(){
	if(ledmacro&0x10){FN=0x0F;}else{FN=0xF0;}
	for (r = 0; r < ROWS; r++) {
		for (c = 0; c < COLS; c++) {
			if (digitalRead(colPins[c])) {keymask[r][c]&= ~0x88;}
			else {keymask[r][c]|= 0x88;delay_after=_delay_after;keymaskled[r][c]=0x6C;}
			if(keymask[r][c]==0xEE ){
				if(ledmacro&0x10){FN=0xF0;}else{FN=0x0F;}
			}
		}
		init_rows();
	}
	LED_macro1();
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
int init_main(void)
{
	CPU_PRESCALE(CPU_16MHz);//16M??????????
	#if  (defined(__AVR_ATmega32U4__))
	void closeJtag();
	#endif
	usb_init();
	while (!usb_configured()){_delay_ms(300);}
	init_cols();
	init_rows();
	while (1) {//����
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
				StaryuMode();
				LED();
			}
		}
	}
	return 0;
}
#endif