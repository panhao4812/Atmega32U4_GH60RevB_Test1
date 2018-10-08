#include "usb_keyboard.h"

/**************************************************************************
*
*  Variables - these are the only non-stack RAM usage
*
**************************************************************************/
// zero when we are not configured, non-zero when enumerated
static volatile uint8_t usb_configuration=0;

const uint8_t PROGMEM device_descriptor[] = {
	18,					// bLength
	1,					// bDescriptorType
	0x00,0x02,				// bcdUSB
	0,					// bDeviceClass
	0,					// bDeviceSubClass
	0,					// bDeviceProtocol
	ENDPOINT0_SIZE,				// bMaxPacketSize0
	LSB(VENDOR_ID), MSB(VENDOR_ID),		// idVendor
	LSB(PRODUCT_ID), MSB(PRODUCT_ID),	// idProduct
	0x00, 0x02,				// bcdDevice
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
	   0x26, 0xFF, 0x00,    //   Logical Maximum(255),
	   0x05, 0x07,          //   Usage Page (Key Codes),
	   0x19, 0x00,          //   Usage Minimum (0),
	   0x29, 0xFF,          //   Usage Maximum (255),
	   0x81, 0x00,          //   Input (Data, Array),
	   0xc0                 // End Collection
};
const  PROGMEM  uint8_t MouseReport[] =
{
	/* mouse */
	0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
	0x09, 0x02,                    // USAGE (Mouse)
	0xa1, 0x01,                    // COLLECTION (Application)
	0x85, REPORT_ID_MOUSE,         //   REPORT_ID (1)
	0x09, 0x01,                    //   USAGE (Pointer)
	0xa1, 0x00,                    //   COLLECTION (Physical)
	// ----------------------------  Buttons
	0x05, 0x09,                    //     USAGE_PAGE (Button)
	0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
	0x29, 0x05,                    //     USAGE_MAXIMUM (Button 5)
	0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
	0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
	0x75, 0x01,                    //     REPORT_SIZE (1)
	0x95, 0x05,                    //     REPORT_COUNT (5)
	0x81, 0x02,                    //     INPUT (Data,Var,Abs)
	0x75, 0x03,                    //     REPORT_SIZE (3)
	0x95, 0x01,                    //     REPORT_COUNT (1)
	0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
	// ----------------------------  X,Y position
	0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
	0x09, 0x30,                    //     USAGE (X)
	0x09, 0x31,                    //     USAGE (Y)
	0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
	0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
	0x75, 0x08,                    //     REPORT_SIZE (8)
	0x95, 0x02,                    //     REPORT_COUNT (2)
	0x81, 0x06,                    //     INPUT (Data,Var,Rel)
	// ----------------------------  Vertical wheel
	0x09, 0x38,                    //     USAGE (Wheel)
	0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
	0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
	0x35, 0x00,                    //     PHYSICAL_MINIMUM (0)        - reset physical
	0x45, 0x00,                    //     PHYSICAL_MAXIMUM (0)
	0x75, 0x08,                    //     REPORT_SIZE (8)
	0x95, 0x01,                    //     REPORT_COUNT (1)
	0x81, 0x06,                    //     INPUT (Data,Var,Rel)
	// ----------------------------  Horizontal wheel
	0x05, 0x0c,                    //     USAGE_PAGE (Consumer Devices)
	0x0a, 0x38, 0x02,              //     USAGE (AC Pan)
	0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
	0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
	0x75, 0x08,                    //     REPORT_SIZE (8)
	0x95, 0x01,                    //     REPORT_COUNT (1)
	0x81, 0x06,                    //     INPUT (Data,Var,Rel)
	0xc0,                          //   END_COLLECTION
	0xc0,                          // END_COLLECTION
	/* system control */
	0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
	0x09, 0x80,                    // USAGE (System Control)
	0xa1, 0x01,                    // COLLECTION (Application)
	0x85, REPORT_ID_SYSTEM,        //   REPORT_ID (2)
	0x15, 0x01,                    //   LOGICAL_MINIMUM (0x1)
	0x26, 0xb7, 0x00,              //   LOGICAL_MAXIMUM (0xb7)
	0x19, 0x01,                    //   USAGE_MINIMUM (0x1)
	0x29, 0xb7,                    //   USAGE_MAXIMUM (0xb7)
	0x75, 0x10,                    //   REPORT_SIZE (16)
	0x95, 0x01,                    //   REPORT_COUNT (1)
	0x81, 0x00,                    //   INPUT (Data,Array,Abs)
	0xc0,                          // END_COLLECTION
	/* consumer */
	0x05, 0x0c,                    // USAGE_PAGE (Consumer Devices)
	0x09, 0x01,                    // USAGE (Consumer Control)
	0xa1, 0x01,                    // COLLECTION (Application)
	0x85, REPORT_ID_CONSUMER,      //   REPORT_ID (3)
	0x15, 0x01,                    //   LOGICAL_MINIMUM (0x1)
	0x26, 0x9c, 0x02,              //   LOGICAL_MAXIMUM (0x29c)
	0x19, 0x01,                    //   USAGE_MINIMUM (0x1)
	0x2a, 0x9c, 0x02,              //   USAGE_MAXIMUM (0x29c)
	0x75, 0x10,                    //   REPORT_SIZE (16)
	0x95, 0x01,                    //   REPORT_COUNT (1)
	0x81, 0x00,                    //   INPUT (Data,Array,Abs)
	0xc0                        // END_COLLECTION
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
#define CONFIG1_DESC_SIZE        (9+9+9+7+9+9+7+9+9+7+7)
#define KEYBOARD_HID_DESC_OFFSET (9+9)
#define MOUSE_HID_DESC_OFFSET    (9+9+9+7+9)
#define RAW_HID_DESC_OFFSET      (9+9+9+7+9+9+7+9)
const uint8_t PROGMEM config1_descriptor[] = {
	9,
	0x02,
	CONFIG1_DESC_SIZE,0x00, //9+(9+9+7)+(9+9+7+7)
	0x03,          /* number of interfaces in this configuration */
	1,          /* index of this configuration */
	0,          /* configuration name string index */
	0xA0,
	0x32, /* max USB current in 2mA units */
	//Interface Descriptor 0/0 HID, 1 Endpoint
	9,          /* sizeof(usbDescrInterface): length of descriptor in bytes */
	0x04, /* descriptor type */
	KEYBOARD_INTERFACE,          /* index of this interface */
	0,          /* alternate setting for this interface */
	0x01,
	0x03,
	0x01,
	0x01,
	0,          /* string index for interface */
	//HID descriptor
	9,          /* sizeof(usbDescrHID): length of descriptor in bytes */
	0x21,   /* descriptor type: HID */
	0x11, 0x01, /* BCD representation of HID version */
	0x00,       /* target country code */
	0x01,       /* number of HID Report (or other HID class) Descriptor infos to follow */
	0x22,       /* descriptor type: report */
	sizeof(KeyboardReport), 0x00,  /* total length of report descriptor */
	//endpoint descriptor for endpoint 1
	0x07,          /* sizeof(usbDescrEndpoint) */
	0x05,  /* descriptor type = endpoint */
	0x80+KEYBOARD_ENDPOINT, /* IN endpoint number 1 */
	0x03,       /* attrib: Interrupt endpoint */
	KEYBOARD_SIZE,0x00,       /* maximum packet size */
	0x01, /* in ms */
	
	//Interface Descriptor 1/0 HID, 1 Endpoints
	0x09,
	0x04,  //interface
	MOUSE_INTERFACE,  //interface number
	0x00,
	0x01,
	0x03, //hid
	0x01, //boot interface
	0x02, //mouse
	0x00,
	//HID descriptor
	0x09,
	0x21,
	0x11, 0x01,
	0x00,
	0x01,
	0x22,
	sizeof(MouseReport),0x00,
	//endpoint descriptor for endpoint 4
	0x07,          /* sizeof(usbDescrEndpoint) */
	0x05,  /* descriptor type = endpoint */
	0x80+MOUSE_ENDPOINT, /* IN endpoint number 1 */
	0x03,       /* attrib: Interrupt endpoint */
	MOUSE_SIZE,0x00,       /* maximum packet size */
	0x01, /* in ms */
	//Interface Descriptor 1/0 HID, 2 Endpoints
	0x09,
	0x04,
	RAW_INTERFACE, //interface number
	0x00,
	0x02,
	0x03,
	0x00,
	0x00,
	0x00,
	//HID descriptor
	0x09,
	0x21,
	0x11, 0x01,
	0x00,
	0x01,
	0x22,
	sizeof(RawReport),0x00,
	//endpoint descriptor for endpoint 2
	0x07,          /* sizeof(usbDescrEndpoint) */
	0x05,  /* descriptor type = endpoint */
	0x80+RAW_ENDPOINT_IN, /* IN endpoint number 1 */
	0x03,       /* attrib: Interrupt endpoint */
	RAW_EPSIZE,0x00,       /* maximum packet size */
	0x01, /* in ms */
	//endpoint descriptor for endpoint 3
	0x07,          /* sizeof(usbDescrEndpoint) */
	0x05,  /* descriptor type = endpoint */
	RAW_ENDPOINT_OUT, /* IN endpoint number 1 */
	0x03,       /* attrib: Interrupt endpoint */
	RAW_EPSIZE,0x00,       /* maximum packet size */
	0x01 /* in ms */
};
const  usb_string_descriptor_struct PROGMEM string0 = {
	4,
	3,
	{0x0409} // language index (0x0409 = US-English)
};
const  usb_string_descriptor_struct PROGMEM string1 = {
	0x0C,
	3,
	{0x007A,0x0069,0x0061,0x006E,0x0031}//zian1
};
const  usb_string_descriptor_struct PROGMEM string2 = {
	0x1C,
	3,
	{0x007A,0x0069,0x0061,0x006E,0x005F,
		0x006B,0x0065,0x0079,0x0062,0x006F,
	0x0061,0x0072,0x0064}  //zian_keyboard
};

/**************************************************************************
*
*  Public Functions - these are the API intended for the user
*
**************************************************************************/

// initialize USB

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
	ClearMacro();
}
// return 0 if the USB is not configured, or the configuration
// number selected by the HOST
uint8_t usb_configured(void)
{
	return usb_configuration;
}
void Recv(volatile uint8_t* data, uint8_t count)
{
	while (count--)
	*data++ = UEDATX;
}
// send the contents of keyboard_keys and keyboard_modifier_keys

uint8_t usb_recv(uint8_t endpoint,uint8_t *buffer, uint8_t buffersize,uint8_t timeout)
{
	uint8_t intr_state;
	if (!usb_configuration) return 1;
	intr_state = SREG;
	cli();
	timeout = FrameNumber() + timeout;
	SetEP(endpoint);
	while (1) {
		if (ReadWriteAllowed()) break;
		SREG = intr_state;
		if (!usb_configuration) return 1;
		if (FrameNumber() == timeout) return 1;
		intr_state = SREG;
		cli();
		SetEP(endpoint);
	}
	for(int i=0;i<buffersize;i++){*buffer++ = UEDATX;}
	ReleaseTX();
	SREG = intr_state;
	return 0;
}
uint8_t usb_send(uint8_t endpoint,const uint8_t *buffer, uint8_t buffersize,uint8_t timeout)
{
	uint8_t intr_state;
	if (!usb_configuration) return 1;
	intr_state = SREG;
	cli();
	timeout = FrameNumber() + timeout;
	SetEP(endpoint);
	while (1) {
		if (ReadWriteAllowed()) break;
		SREG = intr_state;
		if (!usb_configuration) return 1;
		if (FrameNumber() == timeout) return 1;
		intr_state = SREG;
		cli();
		SetEP(endpoint);
	}
	for(int i=0;i<buffersize;i++){UEDATX = *buffer++;}
	ReleaseTX();
	SREG = intr_state;
	return 0;
}

/**************************************************************************
*
*  Private Functions - not intended for general user consumption....
*
**************************************************************************/
// USB Device Interrupt - handle all device-level events
// the transmit buffer flushing is triggered by the start of frame
//	General interrupt
void EVENT_USB_Device_StartOfFrame()
{
	static uint8_t count;
	if (++count % 50) return;
	count = 0;
	if (ReadWriteAllowed()&& EnableRecv){
		if(keyboard_buffer.enable_pressing==0)	{EnableRecv=usb_recv(RAW_ENDPOINT_OUT,(uint8_t *)&raw_report_out,RAW_EPSIZE ,0);}
		else { EnableRecv=usb_recv(RAW_ENDPOINT_OUT,(uint8_t *)&raw_report_out,2, 0);}
	}
}
ISR(USB_GEN_vect)
{
	uint8_t intbits;
	static uint8_t div4=0;
	intbits = UDINT;
	UDINT = 0;
	if (intbits & (1<<EORSTI)) {
		SetEP(0);
		UECONX = 1;
		UECFG0X = EP_TYPE_CONTROL;
		UECFG1X = EP_SIZE(ENDPOINT0_SIZE) | EP_SINGLE_BUFFER;
		UEIENX = (1<<RXSTPE);
		usb_configuration = 0;
	}
	//////////////////////////////////////////
	if ((intbits & (1<<SOFI)) && usb_configuration) {
		EVENT_USB_Device_StartOfFrame();
		if (keyboard_buffer.keyboard_idle_config && (++div4 & 3) == 0) {
			SetEP(KEYBOARD_ENDPOINT);
			if (ReadWriteAllowed()) {
				keyboard_buffer.keyboard_idle_count++;
				if (keyboard_buffer.keyboard_idle_count == keyboard_buffer.keyboard_idle_config) {
					keyboard_buffer.keyboard_idle_count = 0;
					UEDATX=keyboard_report.modifier;
					UEDATX=keyboard_report.reserved;
					for (int i=0; i<6; i++) {
						UEDATX = keyboard_report.keycode[i];
					}
					ReleaseTX();
				}
			}
		}
		/////////////////////////////////
	}
}
// USB Endpoint Interrupt - endpoint 0 is handled here.  The
// other endpoints are manipulated by the user-callable
// functions, and the start-of-frame interrupt.
//	Endpoint 0 interrupt
static const uint8_t PROGMEM endpoint_config_table[] = {
	1, EP_TYPE_INTERRUPT_IN,  EP_SIZE(KEYBOARD_SIZE) | EP_DOUBLE_BUFFER,
	1, EP_TYPE_INTERRUPT_IN,  EP_SIZE(MOUSE_SIZE) | EP_DOUBLE_BUFFER,
	1, EP_TYPE_INTERRUPT_IN,  EP_SIZE(RAW_EPSIZE) | EP_DOUBLE_BUFFER,
	1, EP_TYPE_INTERRUPT_OUT, EP_SIZE(RAW_EPSIZE) | EP_DOUBLE_BUFFER,
	0,
	0
};
//{UECONX,UECFG0X,UECFG1X}
//UECONX寄存器相关 -，-，stallRQ，stallRQC，RSTDT，-，-，EPen
//EPen代表是否启用ep
//UECFG0X寄存器相关 EPtype,EPtype,-,-,-,-,-,EPdir
//EPtype 00 control 10 bulk 01 isochronous 11 interrupt
//EPdir代表in 还是 out
//UECFG1X寄存器相关 -,EPsize,EPsize,EPsize,EPbk,EPbk,alloc,-
//EPsize=0 size=8 | EPsize=1 size=16 | EPsize=2 size=32 | EPsize=3 size=64
//0 one bank 1 double bank 
//0 Clear Endpoint 1 allocate endpoint 

ISR(USB_COM_vect)
{
	uint8_t bmRequestType;
	uint8_t bRequest;
	uint16_t wValue;
	uint16_t wIndex;
	uint16_t wLength;

	const uint8_t *cfg;
	const uint8_t *desc_addr;
	uint8_t i, n, len, en;
	SetEP(0);
	if (ReceivedSetupInt()) {
		bmRequestType = UEDATX;
		bRequest = UEDATX;
		wValue = UEDATX;
		wValue |= (UEDATX << 8);
		wIndex = UEDATX;
		wIndex |= (UEDATX << 8);
		wLength = UEDATX;
		wLength |= (UEDATX << 8);
		ClearSetupInt();
		//////////////////////////////////////////////////
		if (bRequest == GET_DESCRIPTOR) {
			     if((wValue==0x0100)&&(wIndex==0x0000)){            desc_addr=device_descriptor; len= sizeof(device_descriptor);}
			else if((wValue==0x0200)&&(wIndex==0x0000)){            desc_addr=config1_descriptor;len= sizeof(config1_descriptor);}
			else if((wValue==0x2200)&&(wIndex==KEYBOARD_INTERFACE)){desc_addr=KeyboardReport;    len= sizeof(KeyboardReport);}
			else if((wValue==0x2100)&&(wIndex==KEYBOARD_INTERFACE)){desc_addr=config1_descriptor+KEYBOARD_HID_DESC_OFFSET; len=9;}
			else if((wValue==0x2200)&&(wIndex==RAW_INTERFACE)){     desc_addr=RawReport;         len= sizeof(RawReport);}
			else if((wValue==0x2100)&&(wIndex==RAW_INTERFACE)){     desc_addr=config1_descriptor+RAW_HID_DESC_OFFSET; len=9;}
			else if((wValue==0x2200)&&(wIndex==MOUSE_INTERFACE)){   desc_addr=MouseReport;       len= sizeof(MouseReport);}
			else if((wValue==0x2100)&&(wIndex==MOUSE_INTERFACE)){   desc_addr=config1_descriptor+MOUSE_HID_DESC_OFFSET; len=9;}			
			else if(wValue==0x0300){desc_addr=(const uint8_t *)&string0; len= 4;}
			else if(wValue==0x0301){desc_addr=(const uint8_t *)&string1; len= 0x0C ;} //sizeof(STR_MANUFACTURER)
			else if(wValue==0x0302){desc_addr=(const uint8_t *)&string2; len= 0x1C;}//sizeof(STR_PRODUCT)
			else {Stall();return;}
			/////////////////////////////////////////////////////////
			if(wLength>0xff)wLength=0xff;
			if(wLength<len)len=(uint8_t)wLength;
			do {
				if(WaitForINOrOUT()){
					n = len < ENDPOINT0_SIZE ? len : ENDPOINT0_SIZE;
					for (i = n; i; i--) {
						UEDATX = pgm_read_byte(desc_addr++);
					}
					len -= n;
					ClearIN();
				}
				else return;
			} while (len || n == ENDPOINT0_SIZE);
			return;
		}
		if (bRequest == SET_ADDRESS) {
			ClearIN();
			WaitIN();
			UDADDR = wValue | (1<<ADDEN);
			return;
		}
		if (bRequest == SET_CONFIGURATION && bmRequestType == 0) {
			usb_configuration = wValue;
			ClearIN();
			cfg = endpoint_config_table;
			for (i=1; i<MAX_ENDPOINT; i++) {
				SetEP(i);
				en = pgm_read_byte(cfg++);
				UECONX = en;	
				if (en) {
					UECFG0X = pgm_read_byte(cfg++);
					UECFG1X = pgm_read_byte(cfg++);
				}
			}
			UERST = UERST_MASK;//ep全部重置
			UERST = 0;         //ep全部启用
			return;
		}
		if (bRequest == GET_CONFIGURATION && bmRequestType == 0x80) {
			WaitIN();
			UEDATX = usb_configuration;
			ClearIN();
			return;
		}

		if (bRequest == GET_STATUS) {
			WaitIN();
			i = 0;
			#ifdef SUPPORT_ENDPOINT_HALT
			if (bmRequestType == 0x82) {
				SetEP(wIndex);
				if (UECONX & (1<<STALLRQ)) i = 1;
				SetEP(0);
			}
			#endif
			UEDATX = i;
			UEDATX = 0;
			ClearIN();
			return;
		}
		#ifdef SUPPORT_ENDPOINT_HALT
		if ((bRequest == CLEAR_FEATURE || bRequest == SET_FEATURE)
		&& bmRequestType == 0x02 && wValue == 0) {
			i = wIndex & 0x7F;
			if (i >0 && i < MAX_ENDPOINT) {
				ClearIN();
				SetEP(i);
				if (bRequest == SET_FEATURE) {
					Stall();
					} else {
					UECONX = (1<<STALLRQC)|(1<<RSTDT)|(1<<EPEN);
					UERST = (1 << i); //UERST表示ep重置 第N位为1就表示epN重置
					UERST = 0;				
				}
				return;
			}
		}
		#endif
		if (wIndex == KEYBOARD_INTERFACE) {
			if (bmRequestType == 0xA1) {
				if (bRequest == HID_GET_REPORT) {
					WaitIN();
					UEDATX=keyboard_report.modifier;UEDATX=keyboard_report.reserved;
					for (i=0; i<6; i++) {
						UEDATX = keyboard_report.keycode[i];
					}
					ClearIN();
					return;
				}
				if (bRequest == HID_GET_IDLE) {
					WaitIN();
					UEDATX = keyboard_buffer.keyboard_idle_config;
					ClearIN();
					return;
				}
				if (bRequest == HID_GET_PROTOCOL) {
					WaitIN();
					UEDATX = keyboard_buffer.keyboard_protocol;
					ClearIN();
					return;
				}
			}
			if (bmRequestType == 0x21) {
				if (bRequest == HID_SET_REPORT) {
					WaitOUT();
					keyboard_buffer.keyboard_leds = UEDATX;
					ClearOUT();
					ClearIN();
					return;
				}
				if (bRequest == HID_SET_IDLE) {
					keyboard_buffer.keyboard_idle_config = (wValue >> 8);
					keyboard_buffer.keyboard_idle_count = 0;
					ClearIN();
					return;
				}
				if (bRequest == HID_SET_PROTOCOL) {
					keyboard_buffer.keyboard_protocol = wValue;
					ClearIN();
					return;
				}
			}
		}
		if (wIndex == RAW_INTERFACE) {
			if (bmRequestType == 0xA1){
				if ( bRequest == HID_GET_REPORT) {
					len = RAW_EPSIZE;
					do {
						// wait for host ready for IN packet
						do {
							i = UEINTX;
						} while (!(i & ((1<<TXINI)|(1<<RXOUTI))));
						if (i & (1<<RXOUTI)) return;	// abort
						// send IN packet
						n = len < ENDPOINT0_SIZE ? len : ENDPOINT0_SIZE;
						for (i = n; i; i--) {
							// just send zeros
							UEDATX = 0;
						}
						len -= n;
						ClearIN();
					} while (len || n == ENDPOINT0_SIZE);
					return;
				}
			}
			if (bmRequestType == 0x21 ){
				if (bRequest == HID_SET_IDLE) {
					ClearIN();
					return;
				}
				if (bRequest == HID_SET_REPORT) {
					len = RAW_EPSIZE;
					do {
						n = len < ENDPOINT0_SIZE ? len : ENDPOINT0_SIZE;
						WaitOUT();
						// ignore incoming bytes
						ClearOUT();
						len -= n;
					} while (len);
					WaitIN();
					ClearIN();
					return;
				}
			}
		}
		if (wIndex == MOUSE_INTERFACE) {
			if (bmRequestType == 0xA1) {
				if (bRequest == HID_GET_REPORT) {
					WaitIN();
					UEDATX = mouse_report.mouse.report_id;
					UEDATX = mouse_report.mouse.buttons;
					UEDATX = mouse_report.mouse.x;
					UEDATX = mouse_report.mouse.y;
					UEDATX = mouse_report.mouse.v;
					UEDATX = mouse_report.mouse.h;
					ClearIN();
					return;
				}
				if (bRequest == HID_GET_PROTOCOL) {
					WaitIN();
					UEDATX = mouse_buffer.mouse_protocol;
					ClearIN();
					return;
				}
			}
			if (bmRequestType == 0x21) {
				if (bRequest == HID_SET_PROTOCOL) {
					mouse_buffer.mouse_protocol = wValue;
					ClearIN();
					return;
				}
				if (bRequest == HID_SET_IDLE) {
					ClearIN();
					return;
				}
			}
		}
	}
	Stall();	// stall
}

/*
* Appendix G: HID Request Support Requirements
*
* The following table enumerates the requests that need to be supported by various types of HID class devices.
* Device type     GetReport   SetReport   GetIdle     SetIdle     GetProtocol SetProtocol
* ------------------------------------------------------------------------------------------
* Boot Mouse      Required    Optional    Optional    Optional    Required    Required
* Non-Boot Mouse  Required    Optional    Optional    Optional    Optional    Optional
* Boot Keyboard   Required    Optional    Required    Required    Required    Required
* Non-Boot Keybrd Required    Optional    Required    Required    Optional    Optional
* Other Device    Required    Optional    Optional    Optional    Optional    Optional
*/