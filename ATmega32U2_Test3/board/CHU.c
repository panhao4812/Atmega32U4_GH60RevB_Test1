#include "../functions.h"
#include "../ws2812.h"
#ifdef CHU
int init_main(void)
{
	CPU_PRESCALE(CPU_16MHz);//16M??????????
	#if  (defined(__AVR_ATmega32U4__))
	void closeJtag();
	#endif
	usb_init();
	while (!usb_configured()){_delay_ms(300);}
	usb_send(KEYBOARD_ENDPOINT,(uint8_t *)&keyboard_report,8,50);
	//LED C5 C6
	//    D0  B7

	DDRD|=(1<<0);
	PORTD|=(1<<0);
	DDRC|=(1<<5)|(1<<6);
	PORTC|=(1<<5)|(1<<6);
	DDRB|=(1<<7);
	PORTB|=(1<<7);
	while (1)
	{
		

	}
	return 0;
}
#endif