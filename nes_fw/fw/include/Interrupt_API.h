#ifndef __INTERRUPT_API_H__
#define __INTERRUPT_API_H__
//==================================================================================================================

//Global Variable
extern volatile BYTE data TH0_Reload, TL0_Reload;
extern volatile BYTE gb_USB_INT1_EVT[4];
extern volatile U16  gb_Time0_Cnt;

//==================================================================================================================

//Global Function

extern void enable_intr();
extern void RTC_Delay(SWORD);
#endif  //#ifndef __INTERRUPT_API_H__