#include "Functions.h"
#include "ws2812.h"
#ifdef oh77

uint8_t i,delayval;
uint8_t rowPins[ROWS]={21,20,19,18,17,5,4,0,1,2,3};
uint8_t colPins[COLS]={16,6,7,8,23,22,11};
//row F0 F1 F4 F5 F6 D0 B7 B0 B1 B2 B3
//col F7 D1 D2 D3 D5 D4 D6

uint8_t hexaKeys0[ROWS][COLS] = {
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r1
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r2
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r3
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r4
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r5
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r6
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r7
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r8
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r9
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r10
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r11
};
uint8_t hexaKeys1[ROWS][COLS] = {
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r1
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r2
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r3
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r4
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r5
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r6
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r7
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r8
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r9
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r10
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r11
};
//keymask_bits:7-press 654-hexatype0 3-press 210-hexatype1
//type: 1-key 2-modifykey 3-mousekey 4-systemkey 5-consumerkey 6-FN 7-consumerkeyAL,8-consumerkeyAC

uint8_t keymask[ROWS][COLS] = {
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r1
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r2
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r3
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r4
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r5
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r6
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r7
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r8
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r9
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r10
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00},//r11
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
//ws2812 D7
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
}
/////////////////////////////////////////////////////////////////////
uint8_t r,c;
uint8_t delay_after=0;
uint8_t delay_before=0;
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
			}
		}
	}
	if(usb_keyboard_send_required())delay_before=_delay_before;
	if(usb_mouse_send_required())delay_before=_delay_before;
	if(delay_after==_delay_after && delay_before==1){usb_keyboard_send();usb_mouse_send();}
	if(delay_after==1){usb_keyboard_send();usb_mouse_send();}
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