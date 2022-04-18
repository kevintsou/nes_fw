
#include "nes.h"

#ifdef D_NES_CMODEL
#include "8051.h"
#else
#include <reg52.h>
#include <8051.h>
#endif
#include "Var_Def.h"
#include "MCU_Reg.h"
#include "interrupt_api.h"
#include "sys_api.h"
#include "USB_API.h"
#include "GPIO_reg.h"
#include "GPIO_API.h"
#include "IO_Macro.h"
#include "WDT_API.h"
#include "UsbReg.h"
#include "Basetype.h"
#include "All.h"



void main11()
{
   	
    Init8051Reg();
    Operate_Mode_Set();
    enable_intr();
		

		PhyInit();
		UsbInit();
        Init_PBA();
		while(1)
		{
			if (usb_interrupt_level2_L  || usb_interrupt_level2_H || usb_interrupt_Event)
				UsbHandler();

			if (usb_interrupt_level1) 
				UsbHandler_Bulk();

			if (gbDoCSW) 
				Return_CSW();

		}


    while (1);
}





#if 0
void USB_DEBUG_MSG(unsigned char a, unsigned char b)
{
	//if (timer_ESDDebug>200)	// wait 10ms*200
	//unsigned long count;
	
	//for(count=0; count < 5000 ; count++ );
	//{
		USBREG[0x72] = b;
		USBREG[0x73] = a;
		USBREG[DN_SEND] = SS_DN_SEND;
		while ((USBREG[DN_SEND] & SS_DN_SEND));
	//}
}
#endif


