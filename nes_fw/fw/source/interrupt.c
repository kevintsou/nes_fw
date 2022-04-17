#include "nes.h"
#ifdef D_NES_CMODEL
#include "8051.h"
#else
#include <reg52.h>
#include <8051.h>
#endif

#include "Var_Def.h"
#include <UsbReg.h>
#include "MCU_reg.h"
#include "GPIO_api.h"
#include "USB_API.h"
#include "WDT_API.h"
#include "Basetype.h"
#include "a.h"

//==================================================================================================================

//Global variable
volatile BYTE data TH0_Reload, TL0_Reload;
volatile BYTE data gb_USB_INT1_EVT[4];
volatile U16 data gb_Time0_Cnt;


//==================================================================================================================

void clear_interrupt_flags(void)
{
    //
    //USB_REG[USB_INT_STA_11] = USB_REG[USB_INT_STA_11];    //clear interrupt flags for USB2 bus events
    //USB_REG[USB_INT_STA_12] = USB_REG[USB_INT_STA_12];    //clear interrupt flags for USB3 bus events
    //USB_REG[USB_INT_STA_13] = USB_REG[USB_INT_STA_13];    //clear interrupt flags for USB3 bus events
    //USB_REG[PIPE0_INT_STA2] = EP0_SETUP_STA|EP0_STSST_STA|EP0_STSED_STA;  //clear EP0 Interrupt flags.
    //USB_REG[PIPE_COMMON] = PIPE_NUM_EP2;
    //USB_REG[PIPEn_INT_STA0]=Pn_BFRDY_STA; //clear EP2 buffer ready flag
    //USB_REG[PIPEn_INT_STA3]=Pn_CBW_STA;   //clear CBW flag
    IE0 = 0;    //flag for 8051 nINT0
    IE1 = 0;    //flag for 8051 nINT1
}

void enable_intr(void)
{
    clear_interrupt_flags();
    //EX1 = 1;  //enable the interrupt for Setup and BulkOut
    //EA = 1;   //enable the all-interrupt gate
}

void RTC_Delay(SWORD delay_time)    // Unit : 1ms
{
    En_Real_Timer();

    while (Rd_Real_Timer() < delay_time);

    Dis_Real_Timer();
}


void int0_isr(void) /*interrupt 0*/
{


	usb_interrupt_Event = USBREG[INT0EXT_EVT3];

	if(usb_interrupt_Event&WAKEUP)
		USBREG[INT0EXT_EVT3] = WAKEUP;

//Suspend_Support
	if(usb_interrupt_Event&SUSPEND)  // Suspend 
		USBREG[INT0EXT_EVT3] = SUSPEND;

	if(usb_interrupt_Event&ENUM_DONE)
	{
		USBREG[INT0EXT_EVT3]=ENUM_DONE; // clear bit
		gucUsbSpeed=0;
		if((USBREG[ENUM_SPD]&SPD_MASK)==SUPER_SPD)
			gucUsbSpeed = 3;
		else
		{
			if((USBREG[ENUM_SPD]&SPD_MASK)==HIGH_SPD)
				gucUsbSpeed = 2;
			else if((USBREG[ENUM_SPD]&SPD_MASK)==FULL_SPD)
				gucUsbSpeed = 1;
			else 
				gucUsbSpeed = 0;
		}
	}
		 
 
	if(usb_interrupt_Event&RCOSC_SW_REQ)
		USBREG[INT0EXT_EVT3]=RCOSC_SW_REQ; // clear bit
		

 
	usb_interrupt_level2_H = (USBREG[INT0_EVT6] | USBREG[INT0_EVT7]<<8); //usb_interrupt_level2_H -->  bit48 ~ 63 
	usb_interrupt_level2_L= (USBREG[INT0_EVT0] | USBREG[INT0_EVT2]<<8);		//usb_interrupt_level2_L -->  bit 0~7 , 16~23    
		
	
	if((usb_interrupt_level2_L&(HOT_RST<<8))||(usb_interrupt_level2_L&(WARM_BUS_RST<<8)))
	{
		if(usb_interrupt_level2_L&(WARM_BUS_RST<<8))//warm reset
		{
			USBREG[INT0_EVT2] = WARM_BUS_RST;
		}
		else
		{
			USBREG[INT0_EVT2] = HOT_RST; // Hot reset 
			USBREG[RST_ST]=WIRE_HOT_RST;
		}

		Resetting=1;	
	}


	if ( usb_interrupt_level2_L & EP0_SETUP )    /* setup command In*/
	{
		USBREG[INT0_EVT0] = EP0_SETUP;

		if(USBREG[EPC_RESPONSE]&EPC_S_DPP_VLD)
		{
			gucRequestType = USBREG[SETUP_DATA0];/*X_CMDEP0[0];*/
			gucRequest = USBREG[SETUP_DATA1];/*X_CMDEP0[1];*/
			gucValue_L = USBREG[SETUP_DATA2];/*X_CMDEP0[2];*/
			gucValue_H = USBREG[SETUP_DATA3];/*X_CMDEP0[3];*/
			gucIndex_L = USBREG[SETUP_DATA4];/*X_CMDEP0[4];*/
			gucIndex_H = USBREG[SETUP_DATA5];/*X_CMDEP0[5];*/
			gucxLength_L = USBREG[SETUP_DATA6];/*X_CMDEP0[6];*/
			gucxLength_H = USBREG[SETUP_DATA7];/*X_CMDEP0[7];*/
		}
	}
 

}



//==================================================================================================================
//  FUNCTION DESCRIPTION:
//      10m sec timer.
//
//  INPUT:
//      None
//
//  OUTPUT:
//      None
//
//  MODIFIED GLOBAL VARIABLE:
//      None
//
//==================================================================================================================
void timer0_isr(void) /*interrupt 1*/ {
    TR0 = 0;//stop the progress of TH0 and TL0

    TH0 = TH0_Reload;
    TL0 = TL0_Reload;
    gb_Time0_Cnt++;
    TR0 = 1;//start the progress of TH0 and TL0
}

void int1_isr(void) /*interrupt 2*/
{

	usb_interrupt_level1 = (USBREG[INT1_EVT0]&EP2_DATA_AV);

	if (usb_interrupt_level1)
	{
		USBREG[INT1_EVT0]=EP2_DATA_AV; // clear bit
		USBREG[INT1_DET_EN0]&=~(CR_EP2_ABFVLD_EN);  //  disable interrupt EP2 , So not need disable 8051 interrupt 
	}
	
}

//==================================================================================================================
//  FUNCTION DESCRIPTION:
//      timer1 interupt.
//
//  INPUT:
//      None
//
//  OUTPUT:
//      None
//
//MODIFIED GLOBAL VARIABLE:
//      None
//
//==================================================================================================================
void timer1_isr(void) /*interrupt 3*/ {
    TL1 = (unsigned char) 54536;
    TH1 = (unsigned char) (54536 >> 8);
}

//==================================================================================================================
//  FUNCTION DESCRIPTION:
//      Wait define.
//
//  INPUT:
//      None
//
//  OUTPUT:
//      None
//
//  MODIFIED GLOBAL VARIABLE:
//      None
//
//==================================================================================================================
void serialport_isr(void) /*interrupt 4*/ {
   

    if (RI == 1) {
        RI = 0; //Clear RI (Receive Interrupt).
    }

    if (TI == 1) {
        TI = 0; //Clear TI (Transmit Interrupt).
    }
}

//==================================================================================================================
//  FUNCTION DESCRIPTION:
//      Wait define.
//
//  INPUT:
//      None
//
//  OUTPUT:
//      None
//
//  MODIFIED GLOBAL VARIABLE:
//      None
//
//==================================================================================================================
void timer2_isr(void) /*interrupt 5*/ {
}

//==================================================================================================================
//  FUNCTION DESCRIPTION:
//      external interrupt 2
//
//  INPUT:
//      None
//
//  OUTPUT:
//      None
//
//  MODIFIED GLOBAL VARIABLE:
//      None
//
//==================================================================================================================
void Ext_INT2_isr(void) /*interrupt 8 using 2*/ {
}
