#ifndef USB_KEYBOARD_H_
#define USB_KEYBOARD_H_

#include <stdint.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>
//////////////////////////////////////////////////////
//#define  xd60
#define CNY
//#define oh77

#if defined(xd60)
#define ROWS  5
#define COLS  14
#define VENDOR_ID		0x32C4//0x16C0
#define PRODUCT_ID		0x0160//0x047C
#elif defined(CNY)
#define ROWS  5
#define COLS  14
#define VENDOR_ID		0x32C4//0x16C0
#define PRODUCT_ID		0x0260//0x047C
#elif defined(oh77)
#define ROWS  11
#define COLS  7
#define VENDOR_ID		0x32C4//0x16C0
#define PRODUCT_ID		0x0375//0x047C
#else
#define ROWS  5
#define COLS  14
#define VENDOR_ID		0x32C4//0x16C0
#define PRODUCT_ID		0x0060//0x047C
#endif



#define SUPPORT_ENDPOINT_HALT //是否立即中断ep

#define ENDPOINT0_SIZE		8

#define KEYBOARD_INTERFACE	0
#define KEYBOARD_ENDPOINT	1
#define KEYBOARD_SIZE		8

#define MOUSE_INTERFACE KEYBOARD_INTERFACE+1
#define MOUSE_ENDPOINT	KEYBOARD_ENDPOINT+1
#define MOUSE_SIZE		8

#define RAW_INTERFACE	 MOUSE_INTERFACE+1
#define RAW_ENDPOINT_IN	 MOUSE_ENDPOINT+1
#define RAW_ENDPOINT_OUT MOUSE_ENDPOINT+2
#define RAW_EPSIZE  8

void usb_init();			// initialize everything
uint8_t usb_configured();		// is the USB port configured
void releaseAllkeyboardkeys();
uint8_t releasekey(uint8_t key);
uint8_t presskey(uint8_t key);
void pressModifierKeys(uint8_t key);
void releaseModifierKeys(uint8_t key);
volatile uint8_t EnableRecv;//eep change
void ClearKeyboard();
void ClearMouse();
void ClearRaw();
void releaseAllmousekeys();
void releasemousekey(uint8_t key);
void pressmousekey(uint8_t key);
void releasesystemkey(uint8_t key);
void presssystemkey(uint8_t key);
void releaseconsumerkey(uint8_t key);
void pressconsumerkey(uint8_t key);
uint8_t usb_keyboard_send_required();		// initialize everything
uint8_t usb_keyboard_send();
uint8_t usb_mouse_send_required();
uint8_t usb_mouse_send();
uint8_t usb_recv(uint8_t endpoint,uint8_t *buffer, uint8_t buffersize,uint8_t timeout);
uint8_t usb_send(uint8_t endpoint,const uint8_t *buffer, uint8_t buffersize,uint8_t timeout);
void EVENT_USB_Device_StartOfFrame();

typedef struct  {
	uint8_t bLength;
	uint8_t bDescriptorType;
	int16_t wString[];
}__attribute__ ((packed)) usb_string_descriptor_struct;
typedef struct {
	uint8_t report_id;
	uint8_t buttons;
	int8_t x;
	int8_t y;
	int8_t v;
	int8_t h;
} __attribute__ ((packed)) report_mouse0_t;
typedef struct {
	uint8_t  report_id;
	uint16_t usage;
} __attribute__ ((packed)) report_extra_t;
typedef struct {
	report_mouse0_t mouse;
	report_extra_t system_keys;
	report_extra_t consumer_keys;
} __attribute__ ((packed)) report_mouse_t;
typedef struct {
	uint8_t modifier;
	uint8_t reserved;
	uint8_t keycode[6];
}__attribute__ ((packed))  report_keyboard_t;
typedef  union  {
	uint16_t      word[RAW_EPSIZE/2];
	uint8_t       bytes[RAW_EPSIZE];
}__attribute__ ((packed))report_raw_t;

typedef struct {
	uint8_t mouse_keys;
	int8_t x;
	int8_t y;
	int8_t v;
	int8_t h;
	uint16_t system_keys;
	uint16_t consumer_keys;
	// protocol setting from the host.  We use exactly the same report
	// either way, so this variable only stores the setting since we
	// are required to be able to report which setting is in use.
	uint8_t mouse_protocol;
	uint8_t Send_Required;
} __attribute__ ((packed)) buffer_mouse_t;
typedef struct {
	uint8_t keyboard_modifier_keys;
	uint8_t keyboard_keys[6];
	uint8_t keyboard_leds;
	uint8_t enable_pressing;
	// the idle configuration, how often we send the report to the
	// host (ms * 4) even when it hasn't changed
	uint8_t keyboard_idle_config;
	uint8_t keyboard_idle_count;
	// protocol setting from the host.  We use exactly the same report
	// either way, so this variable only stores the setting since we
	// are required to be able to report which setting is in use
	uint8_t keyboard_protocol;
	uint8_t Send_Required;
}__attribute__ ((packed))  buffer_keyboard_t;

report_mouse_t mouse_report;
buffer_mouse_t mouse_buffer;
#define maxEEP (uint16_t)0x03FF // (1k eeprom)
report_raw_t raw_report_in;
report_raw_t raw_report_out;
report_keyboard_t keyboard_report;
buffer_keyboard_t keyboard_buffer;

// This file does not include the HID debug functions, so these empty
// macros replace them with nothing, so users can compile code that
// has calls to these functions.
#define usb_debug_putchar(c)
#define usb_debug_flush_output()


#define KEY_CTRL	0x01
#define KEY_SHIFT	0x02
#define KEY_ALT		0x04
#define KEY_GUI		0x08
#define KEY_LEFT_CTRL	0x01
#define KEY_LEFT_SHIFT	0x02
#define KEY_LEFT_ALT	0x04
#define KEY_LEFT_GUI	0x08
#define KEY_RIGHT_CTRL	0x10
#define KEY_RIGHT_SHIFT	0x20
#define KEY_RIGHT_ALT	0x40
#define KEY_RIGHT_GUI	0x80

#define KEY_A		4
#define KEY_B		5
#define KEY_C		6
#define KEY_D		7
#define KEY_E		8
#define KEY_F		9
#define KEY_G		10
#define KEY_H		11
#define KEY_I		12
#define KEY_J		13
#define KEY_K		14
#define KEY_L		15
#define KEY_M		16
#define KEY_N		17
#define KEY_O		18
#define KEY_P		19
#define KEY_Q		20
#define KEY_R		21
#define KEY_S		22
#define KEY_T		23
#define KEY_U		24
#define KEY_V		25
#define KEY_W		26
#define KEY_X		27
#define KEY_Y		28
#define KEY_Z		29
#define KEY_1		30
#define KEY_2		31
#define KEY_3		32
#define KEY_4		33
#define KEY_5		34
#define KEY_6		35
#define KEY_7		36
#define KEY_8		37
#define KEY_9		38
#define KEY_0		39
#define KEY_ENTER	40
#define KEY_ESC		41
#define KEY_BACKSPACE	42
#define KEY_TAB		43
#define KEY_SPACE	44
#define KEY_MINUS	45
#define KEY_EQUAL	46
#define KEY_LEFT_BRACE	47
#define KEY_RIGHT_BRACE	48
#define KEY_BACKSLASH	49
#define KEY_NUMBER	50
#define KEY_SEMICOLON	51
#define KEY_QUOTE	52
#define KEY_TILDE	53
#define KEY_COMMA	54
#define KEY_PERIOD	55
#define KEY_SLASH	56
#define KEY_CAPS_LOCK	57
#define KEY_F1		58
#define KEY_F2		59
#define KEY_F3		60
#define KEY_F4		61
#define KEY_F5		62
#define KEY_F6		63
#define KEY_F7		64
#define KEY_F8		65
#define KEY_F9		66
#define KEY_F10		67
#define KEY_F11		68
#define KEY_F12		69
#define KEY_PRINTSCREEN	70
#define KEY_SCROLL_LOCK	71
#define KEY_PAUSE	72
#define KEY_INSERT	73
#define KEY_HOME	74
#define KEY_PAGE_UP	75
#define KEY_DELETE	76
#define KEY_END		77
#define KEY_PAGE_DOWN	78
#define KEY_RIGHT	79
#define KEY_LEFT	80
#define KEY_DOWN	81
#define KEY_UP		82
#define KEY_NUM_LOCK	83
#define KEYPAD_SLASH	84	// /
#define KEYPAD_ASTERIX	85	// *
#define KEYPAD_MINUS	86
#define KEYPAD_PLUS	87
#define KEYPAD_ENTER	88
#define KEYPAD_1	89
#define KEYPAD_2	90
#define KEYPAD_3	91
#define KEYPAD_4	92
#define KEYPAD_5	93
#define KEYPAD_6	94
#define KEYPAD_7	95
#define KEYPAD_8	96
#define KEYPAD_9	97
#define KEYPAD_0	98
#define KEYPAD_PERIOD	99		//.

#define MOUSE_LEFT	1<<0
#define MOUSE_RIGHT	1<<1
#define MOUSE_MID	1<<2
#define MOUSE_4	1<<3
#define MOUSE_5	1<<4

#define KEY_FN 0
#define REPORT_ID_MOUSE     1
#define REPORT_ID_SYSTEM    2
#define REPORT_ID_CONSUMER  3

#define AUDIO_MUTE              0xE2
#define AUDIO_VOL_UP            0xE9
#define AUDIO_VOL_DOWN          0xEA
#define TRANSPORT_NEXT_TRACK    0xB5
#define TRANSPORT_PREV_TRACK    0xB6
#define TRANSPORT_STOP          0xB7
#define TRANSPORT_STOP_EJECT    0xCC
#define TRANSPORT_PLAY_PAUSE    0xCD

/* Generic Desktop Page(0x01) - system power control */
#define SYSTEM_POWER_DOWN       0x81
#define SYSTEM_SLEEP            0x82
#define SYSTEM_WAKE_UP          0x83

/*
//application launch 
#define AL_CC_CONFIG            0x0183
#define AL_EMAIL                0x018A
#define AL_CALCULATOR           0x0192
#define AL_LOCAL_BROWSER        0x0194
//application control 
#define AC_SEARCH               0x0221
#define AC_HOME                 0x0223
#define AC_BACK                 0x0224
#define AC_FORWARD              0x0225
#define AC_STOP                 0x0226
#define AC_REFRESH              0x0227
#define AC_BOOKMARKS            0x022A
//supplement for Bluegiga iWRAP HID(not supported by Windows?) 
#define AL_LOCK                 0x019E
#define TRANSPORT_RECORD        0x00B2
#define TRANSPORT_FAST_FORWARD  0x00B3
#define TRANSPORT_REWIND        0x00B4
#define TRANSPORT_EJECT         0x00B8
#define AC_MINIMIZE             0x0206
//*/

// Everything below this point is only intended for usb_serial.c
// Misc functions to wait for ready and send/receive packets
/*
uint8_t USB_INT_HasOccurred(const uint8_t Interrupt){
switch (Interrupt)
{
case USB_INT_WAKEUPI:
return (UDINT  & (1 << WAKEUPI));
case USB_INT_SUSPI:
return (UDINT  & (1 << SUSPI));
case USB_INT_EORSTI:
return (UDINT  & (1 << EORSTI));
case USB_INT_SOFI:
return (UDINT  & (1 << SOFI));
case USB_INT_RXSTPI:
return (UEINTX & (1 << RXSTPI));
default:
return 0;
}
}
uint8_t USB_INT_IsEnabled(const uint8_t Interrupt)
{
switch (Interrupt)
{
case USB_INT_WAKEUPI:
return (UDIEN  & (1 << WAKEUPE));
case USB_INT_SUSPI:
return (UDIEN  & (1 << SUSPE));
case USB_INT_EORSTI:
return (UDIEN  & (1 << EORSTE));
case USB_INT_SOFI:
return (UDIEN  & (1 << SOFE));
case USB_INT_RXSTPI:
return (UEIENX & (1 << RXSTPE));
default:
return 0;
}
}
void USB_INT_Clear(const uint8_t Interrupt){
switch (Interrupt)
{
case USB_INT_WAKEUPI:
UDINT  &= ~(1 << WAKEUPI);
break;
case USB_INT_SUSPI:
UDINT  &= ~(1 << SUSPI);
break;
case USB_INT_EORSTI:
UDINT  &= ~(1 << EORSTI);
break;
case USB_INT_SOFI:
UDINT  &= ~(1 << SOFI);
break;
case USB_INT_RXSTPI:
UEINTX &= ~(1 << RXSTPI);
break;
default:
return false;
}
}
*/
static inline void WaitIN(void)
{
	while (!(UEINTX & (1<<TXINI)));
}
static inline void ClearIN(void)
{
	UEINTX = ~(1<<TXINI);
}
static inline void WaitOUT(void)
{
	while (!(UEINTX & (1<<RXOUTI)));
}
static inline uint8_t WaitForINOrOUT()
{
	while (!(UEINTX & ((1<<TXINI)|(1<<RXOUTI))));
	return (UEINTX & (1<<RXOUTI)) == 0;
}
static inline void ClearOUT(void)
{
	UEINTX = ~(1<<RXOUTI);
}

static inline uint8_t Recv8()
{
	return UEDATX;
}
static inline void Send8(uint8_t d)
{
	UEDATX = d;
}
static inline void SetEP(uint8_t ep)
{
	UENUM = ep;
}
static inline uint8_t FifoByteCount()
{
	return UEBCLX;
}
static inline uint8_t ReceivedSetupInt()
{
	return UEINTX & (1<<RXSTPI);
}
static inline void ClearSetupInt()
{
	UEINTX = ~((1<<RXSTPI) | (1<<RXOUTI) | (1<<TXINI));
}
static inline void Stall()
{
	UECONX = (1<<STALLRQ) | (1<<EPEN);
}
static inline uint8_t ReadWriteAllowed()
{
	return UEINTX & (1<<RWAL);
}
static inline uint8_t Stalled()
{
	return UEINTX & (1<<STALLEDI);
}
static inline uint8_t FifoFree()
{
	return UEINTX & (1<<FIFOCON);
}
static inline void ReleaseRX()
{
	UEINTX = 0x6B;	// FIFOCON=0 NAKINI=1 RWAL=1 NAKOUTI=0 RXSTPI=1 RXOUTI=0 STALLEDI=1 TXINI=1
}
static inline void ReleaseTX()
{
	UEINTX = 0x3A;	// FIFOCON=0 NAKINI=0 RWAL=1 NAKOUTI=1 RXSTPI=1 RXOUTI=0 STALLEDI=1 TXINI=0
}
static inline uint8_t FrameNumber()
{
	return UDFNUML;
}
////////////////////////////////////////
#define USB_INT_WAKEUPI  2
#define USB_INT_SUSPI    3
#define USB_INT_EORSTI   4
#define USB_INT_SOFI     5
#define USB_INT_RXSTPI   6
#define USB_SERIAL_PRIVATE_INCLUDE
#ifdef USB_SERIAL_PRIVATE_INCLUDE
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define EP_TYPE_CONTROL			0x00
#define EP_TYPE_BULK_IN			0x81
#define EP_TYPE_BULK_OUT		0x80
#define EP_TYPE_INTERRUPT_IN		0xC1
#define EP_TYPE_INTERRUPT_OUT		0xC0
#define EP_TYPE_ISOCHRONOUS_IN		0x41
#define EP_TYPE_ISOCHRONOUS_OUT		0x40

#define EP_SINGLE_BUFFER		0x02
#define EP_DOUBLE_BUFFER		0x06

#define EP_SIZE(s)	((s) == 64 ? 0x30 :	\
((s) == 32 ? 0x20 :	\
((s) == 16 ? 0x10 :	\
0x00)))
//size只能为8 16 32 64 128 256 512


#define LSB(n) (n & 255)
#define MSB(n) ((n >> 8) & 255)

#if defined (__AVR_AT90USB162__) || defined (__AVR_AT90USB82__)
#   define MAX_ENDPOINT     5
#   define UERST_MASK       0x1E
#else
#   define MAX_ENDPOINT     7
#   define UERST_MASK       0x7E
//0b 01111110 UERST是代表ep是否重置，0-6位分别表示ep0-6 第7位锁死为0 1代表重置，0代表不重置
#endif

#if defined(__AVR_AT90USB162__)
#define HW_CONFIG()
#define PLL_CONFIG() (PLLCSR = ((1<<PLLE)|(1<<PLLP0)))
#define USB_CONFIG() (USBCON = (1<<USBE))
#define USB_FREEZE() (USBCON = ((1<<USBE)|(1<<FRZCLK)))
#elif defined(__AVR_ATmega32U4__)
#define HW_CONFIG() (UHWCON = 0x01)
#define PLL_CONFIG() (PLLCSR = 0x12)
#define USB_CONFIG() (USBCON = ((1<<USBE)|(1<<OTGPADE)))
#define USB_FREEZE() (USBCON = ((1<<USBE)|(1<<FRZCLK)))
#elif defined(__AVR_AT90USB646__)
#define HW_CONFIG() (UHWCON = 0x81)
#define PLL_CONFIG() (PLLCSR = 0x1A)
#define USB_CONFIG() (USBCON = ((1<<USBE)|(1<<OTGPADE)))
#define USB_FREEZE() (USBCON = ((1<<USBE)|(1<<FRZCLK)))
#elif defined(__AVR_AT90USB1286__)
#define HW_CONFIG() (UHWCON = 0x81)
#define PLL_CONFIG() (PLLCSR = 0x16)
#define USB_CONFIG() (USBCON = ((1<<USBE)|(1<<OTGPADE)))
#define USB_FREEZE() (USBCON = ((1<<USBE)|(1<<FRZCLK)))
#endif

// standard control endpoint request types
#define GET_STATUS			0
#define CLEAR_FEATURE			1
#define SET_FEATURE			3
#define SET_ADDRESS			5
#define GET_DESCRIPTOR			6
#define GET_CONFIGURATION		8
#define SET_CONFIGURATION		9
#define GET_INTERFACE			10
#define SET_INTERFACE			11
// HID (human interface device)
#define HID_GET_REPORT			1
#define HID_GET_IDLE			2
#define HID_GET_PROTOCOL		3
#define HID_SET_REPORT			9
#define HID_SET_IDLE			10
#define HID_SET_PROTOCOL		11
// CDC (communication class device)
#define CDC_SET_LINE_CODING		0x20
#define CDC_GET_LINE_CODING		0x21
#define CDC_SET_CONTROL_LINE_STATE	0x22
#endif
#endif