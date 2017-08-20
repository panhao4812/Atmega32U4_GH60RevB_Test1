
#ifndef ENDPOINT_H_
#define ENDPOINT_H_


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
static inline uint8_t ReceivedSetupInt()
{
	return UEINTX & (1<<RXSTPI);
}
static inline void ClearSetupInt()
{
	UEINTX = ~((1<<RXSTPI) | (1<<RXOUTI) | (1<<TXINI));
}
static inline void ClearSETUP(void)
{
	UEINTX &= ~(1 << RXSTPI);
}
static inline uint8_t IsINReady(void)
{
	return UEINTX & (1 << TXINI);
}
static inline uint8_t IsOUTReceived(void)
{
	return UEINTX & (1 << RXOUTI);
}
static inline void WaitIN(void)
{
	while (!(UEINTX & (1<<TXINI)));
}
static inline void WaitOUT(void)
{
	while (!(UEINTX & (1<<RXOUTI)));
}
static inline void ClearIN(void)
{
	UEINTX = ~(1<<TXINI);
}
static inline void ClearOUT(void)
{
	UEINTX = ~(1<<RXOUTI);
}
static inline uint8_t WaitForINOrOUT()
{
	while (!(UEINTX & ((1<<TXINI)|(1<<RXOUTI))));
	return (UEINTX & (1<<RXOUTI)) == 0;
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
///////////////////////////////////////////////////
static inline uint8_t Endpoint_IsStalled(void)
{
	return UECONX & (1 << STALLRQ);
}
static inline void Endpoint_StallTransaction(void)
{
	UECONX |= (1 << STALLRQ);
}
static inline void Endpoint_ClearStall(void)
{
	UECONX |= (1 << STALLRQC);
}
static inline void Endpoint_EnableEndpoint(void)
{
	UECONX |= (1 << EPEN);
}
static inline void Endpoint_DisableEndpoint(void)
{
	UECONX &= ~(1 << EPEN);
}
static inline uint8_t Endpoint_IsEnabled(void)
{
	return UECONX & (1 << EPEN);
}
static inline void Stall()
{
	UECONX = (1<<STALLRQ) | (1<<EPEN);
}
////////////////////////////////////////////////////////
static inline uint8_t Recv8()
{
	return UEDATX;
}
static inline void Send8(uint8_t d)
{
	UEDATX = d;
}
////////////////////////////////////////////////////
static inline void SetEP(uint8_t ep)
{
	UENUM = ep;
}
static inline uint8_t FifoByteCount()
{
	return UEBCLX;
}
static inline uint8_t FrameNumber()
{
	return UDFNUML;
}
/////////////////////////////////////////////////
static inline uint8_t GetEndpointInterrupts(void)
{
	return UEINT;
}

#endif /* ENDPOINT_H_ */