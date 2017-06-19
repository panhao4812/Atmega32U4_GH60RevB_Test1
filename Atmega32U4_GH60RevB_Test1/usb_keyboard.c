#include "usb_keyboard.h"

/**************************************************************************
 *
 *  Variables - these are the only non-stack RAM usage
 *
 **************************************************************************/
// zero when we are not configured, non-zero when enumerated
static volatile uint8_t usb_configuration=0;

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

static const uint8_t PROGMEM endpoint_config_table[] = {		
	1, EP_TYPE_INTERRUPT_IN,  EP_SIZE(KEYBOARD_SIZE) | KEYBOARD_BUFFER,
	1, EP_TYPE_INTERRUPT_IN,  EP_SIZE(RAW_EPSIZE) | EP_DOUBLE_BUFFER,
	1, EP_TYPE_INTERRUPT_OUT,  EP_SIZE(RAW_EPSIZE) | EP_DOUBLE_BUFFER,
	0
};
const uint8_t PROGMEM device_descriptor[] = {
	18,					// bLength
	1,					// bDescriptorType
	0x00, 0x02,				// bcdUSB
	0,					// bDeviceClass
	0,					// bDeviceSubClass
	0,					// bDeviceProtocol
	ENDPOINT0_SIZE,				// bMaxPacketSize0
	LSB(VENDOR_ID), MSB(VENDOR_ID),		// idVendor
	LSB(PRODUCT_ID), MSB(PRODUCT_ID),	// idProduct
	0x00, 0x01,				// bcdDevice
	1,					// iManufacturer
	2,					// iProduct
	0,					// iSerialNumber
	1					// bNumConfigurations
};
const uint8_t PROGMEM KeyboardReport[] = {
        0x05, 0x01,          // Usage Page (Generic Desktop),
        0x09, 0x06,          // Usage (Keyboard),
        0xA1, 0x01,          // Collection (Application),
        0x75, 0x01,          //   Report Size (1),
        0x95, 0x08,          //   Report Count (8),
        0x05, 0x07,          //   Usage Page (Key Codes),
        0x19, 0xE0,          //   Usage Minimum (224),
        0x29, 0xE7,          //   Usage Maximum (231),
        0x15, 0x00,          //   Logical Minimum (0),
        0x25, 0x01,          //   Logical Maximum (1),
        0x81, 0x02,          //   Input (Data, Variable, Absolute), ;Modifier byte
        0x95, 0x01,          //   Report Count (1),
        0x75, 0x08,          //   Report Size (8),
        0x81, 0x03,          //   Input (Constant),                 ;Reserved byte
        0x95, 0x05,          //   Report Count (5),
        0x75, 0x01,          //   Report Size (1),
        0x05, 0x08,          //   Usage Page (LEDs),
        0x19, 0x01,          //   Usage Minimum (1),
        0x29, 0x05,          //   Usage Maximum (5),
        0x91, 0x02,          //   Output (Data, Variable, Absolute), ;LED report
        0x95, 0x01,          //   Report Count (1),
        0x75, 0x03,          //   Report Size (3),
        0x91, 0x03,          //   Output (Constant),                 ;LED report padding
        0x95, 0x06,          //   Report Count (6),
        0x75, 0x08,          //   Report Size (8),
        0x15, 0x00,          //   Logical Minimum (0),
        0x25, 0x68,          //   Logical Maximum(104),
        0x05, 0x07,          //   Usage Page (Key Codes),
        0x19, 0x00,          //   Usage Minimum (0),
        0x29, 0x68,          //   Usage Maximum (104),
        0x81, 0x00,          //   Input (Data, Array),
        0xc0                 // End Collection
};
const PROGMEM uint8_t  RawReport[] =
{
	0x06, 0x31 ,0xFF,//Usage Page (Vendor-Defined 50 31FF)
	0x09 ,0x74,//Usage (Vendor-Defined 116)
	0xA1, 0x01,//Collection (Application)
	0x09 ,0x75,//Usage (Vendor-Defined 117)
	0x15 ,0x00,//Logical Minimum (0)
	0x26, 0xFF ,0x00,//Logical Maximum (255 FF00)
	0x95 ,0x08 ,//Report Count (8)
	0x75 ,0x08 ,//Report Size (8)
	0x81 ,0x02 ,//Input (Data,Var,Abs,NWrp,Lin,Pref,NNul,Bit)
	0x09 ,0x76 ,//Usage (Vendor-Defined 118)
	0x15, 0x00 ,//Logical Minimum (0)
	0x26 ,0xFF ,0x00 ,//Logical Maximum (255)
	0x95 ,0x08 , //Report Count (8)
	0x75 ,0x08 ,//Report Size (8)
	0x91 ,0x02, //Output (Data,Var,Abs,NWrp,Lin,Pref,NNul,NVol,Bit)
	0xC0 //End Collection
};
#define CONFIG1_DESC_SIZE        (9+9+9+7+9+9+7+7)
#define KEYBOARD_HID_DESC_OFFSET (9+9)
#define RAW_HID_DESC_OFFSET (9+9+9+7+9)
const uint8_t PROGMEM config1_descriptor[] = {
	9,
	0x02,
	CONFIG1_DESC_SIZE,0x00, //9+(9+9+7)+(9+9+7+7)
	0x02,          /* number of interfaces in this configuration */
	1,          /* index of this configuration */
	0,          /* configuration name string index */
	0xA0,
	0x32, /* max USB current in 2mA units */
	//Interface Descriptor 0/0 HID, 1 Endpoint
	9,          /* sizeof(usbDescrInterface): length of descriptor in bytes */
	0x04, /* descriptor type */
	0,          /* index of this interface */
	0,          /* alternate setting for this interface */
	0x01,
	0x03,
	0x01,
	0x01,
	0,          /* string index for interface */
	//HID descriptor
	9,          /* sizeof(usbDescrHID): length of descriptor in bytes */
	0x21,   /* descriptor type: HID */
	0x01, 0x11, /* BCD representation of HID version */
	0x00,       /* target country code */
	0x01,       /* number of HID Report (or other HID class) Descriptor infos to follow */
	0x22,       /* descriptor type: report */
	sizeof(KeyboardReport), 0x00,  /* total length of report descriptor */
	//endpoint descriptor for endpoint 1
	0x07,          /* sizeof(usbDescrEndpoint) */
	0x05,  /* descriptor type = endpoint */
	0x81, /* IN endpoint number 1 */
	0x03,       /* attrib: Interrupt endpoint */
	0x08,0x00,       /* maximum packet size */
	0x01, /* in ms */
	//Interface Descriptor 1/0 HID, 2 Endpoints
	0x09,
	0x04,
	0x01,
	0x00,
	0x02,
	0x03,
	0x00,
	0x00,
	0x00,
	//HID descriptor
	0x09,
	0x21,
	0x01,0x11,
	0x00,
	0x01,
	0x22,
	sizeof(RawReport),0x00,
	//endpoint descriptor for endpoint 1
	0x07,          /* sizeof(usbDescrEndpoint) */
	0x05,  /* descriptor type = endpoint */
	0x82, /* IN endpoint number 1 */
	0x03,       /* attrib: Interrupt endpoint */
	0x08,0x00,       /* maximum packet size */
	0x01, /* in ms */
	//endpoint descriptor for endpoint 1
	0x07,          /* sizeof(usbDescrEndpoint) */
	0x05,  /* descriptor type = endpoint */
	0x03, /* IN endpoint number 1 */
	0x03,       /* attrib: Interrupt endpoint */
	0x08,0x00,       /* maximum packet size */
	0x01 /* in ms */
};
struct usb_string_descriptor_struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	int16_t wString[];
};
const struct usb_string_descriptor_struct PROGMEM string0 = {
	4,
	3,
	{0x0409}
};
const struct usb_string_descriptor_struct PROGMEM string1 = {
	sizeof(STR_MANUFACTURER),
	3,
	STR_MANUFACTURER
};
const struct usb_string_descriptor_struct PROGMEM string2 = {
	sizeof(STR_PRODUCT),
	3,
	STR_PRODUCT
};
const struct descriptor_list_struct {
	uint16_t	wValue;
	uint16_t	wIndex;
	const uint8_t	*addr;
	uint8_t		length;
} PROGMEM descriptor_list[] = {
	{0x0100, 0x0000, device_descriptor, sizeof(device_descriptor)},
	{0x0200, 0x0000, config1_descriptor, sizeof(config1_descriptor)},
	{0x2200, KEYBOARD_INTERFACE, KeyboardReport, sizeof(KeyboardReport)},
	{0x2100, KEYBOARD_INTERFACE, config1_descriptor+KEYBOARD_HID_DESC_OFFSET, 9},
	{0x2200, RAW_INTERFACE, RawReport, sizeof(RawReport)},
	{0x2100, RAW_INTERFACE, config1_descriptor+RAW_HID_DESC_OFFSET, 9},
	{0x0300, 0x0000, (const uint8_t *)&string0, 4},
	{0x0301, 0x0409, (const uint8_t *)&string1, sizeof(STR_MANUFACTURER)},
	{0x0302, 0x0409, (const uint8_t *)&string2, sizeof(STR_PRODUCT)}
};
#define NUM_DESC_LIST (sizeof(descriptor_list)/sizeof(struct descriptor_list_struct))

/**************************************************************************
 *
 *  Public Functions - these are the API intended for the user
 *
 **************************************************************************/

// initialize USB
void ClearMouse(){
	memset(&mouse_report, 0, sizeof(mouse_report));
	memset(&mouse_buffer,0,sizeof(mouse_buffer));
	mouse_report.mouse.report_id= REPORT_ID_MOUSE;
	mouse_report.system_keys.report_id= REPORT_ID_SYSTEM;
	mouse_report.consumer_keys.report_id= REPORT_ID_CONSUMER;
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
void usb_init(void)
{
	HW_CONFIG();
	USB_FREEZE();	// enable USB
	PLL_CONFIG();				// config PLL
        while (!(PLLCSR & (1<<PLOCK))) ;	// wait for PLL lock
        USB_CONFIG();				// start USB clock
        UDCON = 0;				// enable attach resistor
	usb_configuration = 0;
        UDIEN = (1<<EORSTE)|(1<<SOFE);
	sei();
	ClearKeyboard();
	ClearMouse();
	ClearRaw();
}
// return 0 if the USB is not configured, or the configuration
// number selected by the HOST
uint8_t usb_configured(void)
{
	return usb_configuration;
}
// send the contents of keyboard_keys and keyboard_modifier_keys
uint8_t usb_keyboard_send(void)
{
	uint8_t i, intr_state, timeout;

	if (!usb_configuration) return 1;
	intr_state = SREG;
	cli();
	UENUM = KEYBOARD_ENDPOINT;
	timeout = UDFNUML + 50;
	while (1) {
		// are we ready to transmit?
		if (UEINTX & (1<<RWAL)) break;
		SREG = intr_state;
		// has the USB gone offline?
		if (!usb_configuration) return 1;
		// have we waited too long?
		if (UDFNUML == timeout) return 1;
		// get ready to try checking again
		intr_state = SREG;
		cli();
		UENUM = KEYBOARD_ENDPOINT;
	}
	UEDATX=keyboard_report.modifier;UEDATX=keyboard_report.reserved;
	for (i=0; i<6; i++) {
		UEDATX = keyboard_report.keycode[i];
	}
	UEINTX = 0x3A;
	keyboard_buffer.keyboard_idle_count = 0;
	SREG = intr_state;
	return 0;
}
uint8_t usb_keyboard_send_required(){
	uint8_t send_required=0;
		if(keyboard_report.modifier!=keyboard_buffer.keyboard_modifier_keys){keyboard_report.modifier = keyboard_buffer.keyboard_modifier_keys;send_required=1;}
		if(keyboard_report.keycode[0]!=keyboard_buffer.keyboard_keys[0]){keyboard_report.keycode[0]=keyboard_buffer.keyboard_keys[0];send_required=1;}
		if(keyboard_report.keycode[1]!=keyboard_buffer.keyboard_keys[1]){keyboard_report.keycode[1]=keyboard_buffer.keyboard_keys[1];send_required=1;}
		if(keyboard_report.keycode[2]!=keyboard_buffer.keyboard_keys[2]){keyboard_report.keycode[2]=keyboard_buffer.keyboard_keys[2];send_required=1;}
		if(keyboard_report.keycode[3]!=keyboard_buffer.keyboard_keys[3]){keyboard_report.keycode[3]=keyboard_buffer.keyboard_keys[3];send_required=1;}
		if(keyboard_report.keycode[4]!=keyboard_buffer.keyboard_keys[4]){keyboard_report.keycode[4]=keyboard_buffer.keyboard_keys[4];send_required=1;}
		if(keyboard_report.keycode[5]!=keyboard_buffer.keyboard_keys[5]){keyboard_report.keycode[5]=keyboard_buffer.keyboard_keys[5];send_required=1;}
	return send_required;
}
/**************************************************************************
 *
 *  Private Functions - not intended for general user consumption....
 *
 **************************************************************************/
// USB Device Interrupt - handle all device-level events
// the transmit buffer flushing is triggered by the start of frame
//
ISR(USB_GEN_vect)
{
	uint8_t intbits, i;
	static uint8_t div4=0;

        intbits = UDINT;
        UDINT = 0;
        if (intbits & (1<<EORSTI)) {
		UENUM = 0;
		UECONX = 1;
		UECFG0X = EP_TYPE_CONTROL;
		UECFG1X = EP_SIZE(ENDPOINT0_SIZE) | EP_SINGLE_BUFFER;
		UEIENX = (1<<RXSTPE);
		usb_configuration = 0;
        }
	if ((intbits & (1<<SOFI)) && usb_configuration) {
		if (keyboard_buffer.keyboard_idle_config && (++div4 & 3) == 0) {
			UENUM = KEYBOARD_ENDPOINT;
			if (UEINTX & (1<<RWAL)) {
				keyboard_buffer.keyboard_idle_count++;
				if (keyboard_buffer.keyboard_idle_count == keyboard_buffer.keyboard_idle_config) {
					keyboard_buffer.keyboard_idle_count = 0;					
					UEDATX=keyboard_report.modifier;UEDATX=keyboard_report.reserved;
					for (i=0; i<6; i++) {
						UEDATX = keyboard_report.keycode[i];
					}
					UEINTX = 0x3A;
				}
			}
		}
	}
}



// Misc functions to wait for ready and send/receive packets
static inline void usb_wait_in_ready(void)
{
	while (!(UEINTX & (1<<TXINI))) ;
}
static inline void usb_send_in(void)
{
	UEINTX = ~(1<<TXINI);
}
static inline void usb_wait_receive_out(void)
{
	while (!(UEINTX & (1<<RXOUTI))) ;
}
static inline void usb_ack_out(void)
{
	UEINTX = ~(1<<RXOUTI);
}



// USB Endpoint Interrupt - endpoint 0 is handled here.  The
// other endpoints are manipulated by the user-callable
// functions, and the start-of-frame interrupt.
//
ISR(USB_COM_vect)
{
        uint8_t intbits;
	const uint8_t *list;
        const uint8_t *cfg;
	uint8_t i, n, len, en;
	uint8_t bmRequestType;
	uint8_t bRequest;
	uint16_t wValue;
	uint16_t wIndex;
	uint16_t wLength;
	uint16_t desc_val;
	const uint8_t *desc_addr;
	uint8_t	desc_length;

        UENUM = 0;
	intbits = UEINTX;
        if (intbits & (1<<RXSTPI)) {
                bmRequestType = UEDATX;
                bRequest = UEDATX;
                wValue = UEDATX;
                wValue |= (UEDATX << 8);
                wIndex = UEDATX;
                wIndex |= (UEDATX << 8);
                wLength = UEDATX;
                wLength |= (UEDATX << 8);
                UEINTX = ~((1<<RXSTPI) | (1<<RXOUTI) | (1<<TXINI));
			//////////////////////////////////////////////////
                if (bRequest == GET_DESCRIPTOR) {
			list = (const uint8_t *)descriptor_list;
			for (i=0; ; i++) {
				if (i >= NUM_DESC_LIST) {
					UECONX = (1<<STALLRQ)|(1<<EPEN);  //stall
					return;
				}
				desc_val = pgm_read_word(list);
				if (desc_val != wValue) {
					list += sizeof(struct descriptor_list_struct);
					continue;
				}
				list += 2;
				desc_val = pgm_read_word(list);
				if (desc_val != wIndex) {
					list += sizeof(struct descriptor_list_struct)-2;
					continue;
				}
				list += 2;
				desc_addr = (const uint8_t *)pgm_read_word(list);
				list += 2;
				desc_length = pgm_read_byte(list);
				break;
			}
			/////////////////////////////////////////////////////////
			len = (wLength < 256) ? wLength : 255;
			if (len > desc_length) len = desc_length;
			do {
				// wait for host ready for IN packet
				do {
					i = UEINTX;
				} while (!(i & ((1<<TXINI)|(1<<RXOUTI))));
				if (i & (1<<RXOUTI)) return;	// abort
				// send IN packet
				n = len < ENDPOINT0_SIZE ? len : ENDPOINT0_SIZE;
				for (i = n; i; i--) {
					UEDATX = pgm_read_byte(desc_addr++);
				}
				len -= n;
				usb_send_in();
			} while (len || n == ENDPOINT0_SIZE);
			return;
                }
		if (bRequest == SET_ADDRESS) {
			usb_send_in();
			usb_wait_in_ready();
			UDADDR = wValue | (1<<ADDEN);
			return;
		}
		if (bRequest == SET_CONFIGURATION && bmRequestType == 0) {
			usb_configuration = wValue;
			usb_send_in();
			cfg = endpoint_config_table;
			for (i=1; i<=MAX_ENDPOINT; i++) {
				UENUM = i;
				en = pgm_read_byte(cfg++);
				UECONX = en;
				if (en) {
					UECFG0X = pgm_read_byte(cfg++);
					UECFG1X = pgm_read_byte(cfg++);
				}
			}
        		UERST = 0x1E;
        		UERST = 0;
			return;
		}
		if (bRequest == GET_CONFIGURATION && bmRequestType == 0x80) {
			usb_wait_in_ready();
			UEDATX = usb_configuration;
			usb_send_in();
			return;
		}

		if (bRequest == GET_STATUS) {
			usb_wait_in_ready();
			i = 0;
			#ifdef SUPPORT_ENDPOINT_HALT
			if (bmRequestType == 0x82) {
				UENUM = wIndex;
				if (UECONX & (1<<STALLRQ)) i = 1;
				UENUM = 0;
			}
			#endif
			UEDATX = i;
			UEDATX = 0;
			usb_send_in();
			return;
		}
		#ifdef SUPPORT_ENDPOINT_HALT
		if ((bRequest == CLEAR_FEATURE || bRequest == SET_FEATURE)
		  && bmRequestType == 0x02 && wValue == 0) {
			i = wIndex & 0x7F;
			if (i >= 1 && i <= MAX_ENDPOINT) {
				usb_send_in();
				UENUM = i;
				if (bRequest == SET_FEATURE) {
					UECONX = (1<<STALLRQ)|(1<<EPEN);
				} else {
					UECONX = (1<<STALLRQC)|(1<<RSTDT)|(1<<EPEN);
					UERST = (1 << i);
					UERST = 0;
				}
				return;
			}
		}
		#endif
		if (wIndex == KEYBOARD_INTERFACE) {
			if (bmRequestType == 0xA1) {
				if (bRequest == HID_GET_REPORT) {
					usb_wait_in_ready();
					UEDATX=keyboard_report.modifier;UEDATX=keyboard_report.reserved;
					for (i=0; i<6; i++) {
						UEDATX = keyboard_report.keycode[i];
					}
					usb_send_in();
					return;
				}
				if (bRequest == HID_GET_IDLE) {
					usb_wait_in_ready();
					UEDATX = keyboard_buffer.keyboard_idle_config;
					usb_send_in();
					return;
				}
				if (bRequest == HID_GET_PROTOCOL) {
					usb_wait_in_ready();
					UEDATX = keyboard_buffer.keyboard_protocol;
					usb_send_in();
					return;
				}
			}
			if (bmRequestType == 0x21) {
				if (bRequest == HID_SET_REPORT) {
					usb_wait_receive_out();
					keyboard_buffer.keyboard_leds = UEDATX;
					usb_ack_out();
					usb_send_in();
					return;
				}
				if (bRequest == HID_SET_IDLE) {
					keyboard_buffer.keyboard_idle_config = (wValue >> 8);
					keyboard_buffer.keyboard_idle_count = 0;
					usb_send_in();
					return;
				}
				if (bRequest == HID_SET_PROTOCOL) {
					keyboard_buffer.keyboard_protocol = wValue;
					usb_send_in();
					return;
				}
			}
		}
	}
	UECONX = (1<<STALLRQ) | (1<<EPEN);	// stall
}
