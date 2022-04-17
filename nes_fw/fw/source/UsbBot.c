
#include "nes.h"
#ifdef D_NES_CMODEL
#include "8051.h"
#else
#include <reg52.h>
#include <8051.h>
#endif
#include "Var_Def.h"
#include "MCU_Reg.h"
#include <UsbReg.h>
#include "USB_String.h"
#include "SCSI_Table.h"
#include "USB_API.h"
#include "usb_def.h"
#include "SCSI_CMD_API.h"
#include "string.h"
#include "SYS_API.h"
#include "PH_AP_API.h"
#include "Interrupt_API.h"
#include "GPIO_API.h"
#include "GPIO_Reg.h"
#include "Buf_DEF.h"
#include "Basetype.h"
#include "a.h"


//==================================================================================================================

//Glocbal variable

xdata BYTE gb_ErrorCode;
BYTE data  gb_CBW_Offset, gb_CSW_Offset;

//==================================================================================================================

//Define
#define SET_ABRT    0x04    //Aborted command
//==================================================================================================================



void Return_CSW(void)
{ 

	if (Resetting) //20131014 for reset test
	{
		gbDoCSW=0;
		X_Residue.All = 0;
		return;
	}
	
	while(USBREG[EPn_TRIG_CFG1 + EP1] & STALL_ONCE );

	while(!(USBREG[EPn_TLU_STATUS] & 0x80))
	{ 
		if((USBREG[0x10]&0x20) == 0x00)
		{   
			gbDoCSW=0;
		  X_Residue.All = 0;
			return;
		}
	};//wait untill the EPC data buffer is available
	

	USBREG[EPn_SERIAL_D] = 0x55; 
	USBREG[EPn_SERIAL_D] = 0x53;
	USBREG[EPn_SERIAL_D] = 0x42;
	USBREG[EPn_SERIAL_D] = 0x53;

	USBREG[EPn_SERIAL_D] = X_MASS_TAG.BYTEMODE.BYTE1;
	USBREG[EPn_SERIAL_D] = X_MASS_TAG.BYTEMODE.BYTE2;
	USBREG[EPn_SERIAL_D] = X_MASS_TAG.BYTEMODE.BYTE3;
	USBREG[EPn_SERIAL_D] = X_MASS_TAG.BYTEMODE.BYTE4;

	USBREG[EPn_SERIAL_D] = X_Residue.BYTEMODE.BYTE1;
	USBREG[EPn_SERIAL_D] = X_Residue.BYTEMODE.BYTE2;
	USBREG[EPn_SERIAL_D] = X_Residue.BYTEMODE.BYTE3;
	USBREG[EPn_SERIAL_D] = X_Residue.BYTEMODE.BYTE4;

	USBREG[EPn_SERIAL_D] = X_CSWStatus;

	USBREG[EPn_T_BC0] = 13;      //length, low byte
	USBREG[EPn_T_BC1] = 0;       //length, high byte

	USBREG[EPn_TRIG_CFG1] = INC_PTR;   //send data

	gbDoCSW=0U;
	gucStallMask = 0x01U;
	X_Residue.All = 0;
	
}



void SCSI_CMD_PROCESS(void)
{
	MassError = 0;
	WriteCheck = 0;
	ReadCheck = 0;
	ErrorCode = 0;
	Resetting = 0;

	switch (X_CMDBLOCK[0])
	{
		case 0x00 : /* TUR */
			Test_Unit_Ready();
		break;

		case 0x2F : /* Verify */
		case 0x2B : /* Seek 10 */
			if (MASS_LUN>=X_MaxCard)
			{
				Set_LUN_Error();
			}
			else
			{
				Set_NO_Media();
			}
		break; 

		case 0x28 : /* read 10 */
		case 0x2A : /* Write 10 */
		case 0x25 : /* read capacity */
		case 0x2E : /* Write Verify */
		case 0x04: /* Format_Unit */
			Set_NO_Media();
			Usb_Stall_Bulk();
		break;

		case 0x12 :
			Inquiry();
		break;

		case 0x03 :
			Request_Sense();
		break;

		case 0x1A :
			Mode_Sense6();
		break;

		case 0x5A :
			Mode_Sense();
		break;

		case 0x01: /* Rezero_Unit */
		case 0x1B: /* Start_Stop */
		case 0x1D: /* Send_Diagnostic */
			Set_Media_OK();
		break;

		case 0x1E :
			Prevent_Allow_Removal();
		break;

		case 0x23 :
			Read_Format_Capacity();
		break;

		case 0xc7 : /* for Mac & linux */
		case 0xc6 :
			X_CMDBLOCK[1]&=0x1f;
		case 0x06 :
			Vendor_Phison_Cmd();
		break;

		case CARDREADER_CMD :   //Card reader have to use 0xCA to its vendor cmd
	        CardReader_Cmd();
        break;

		default :
			Invalid_Cmd();
		break;

	}
	return;
}



unsigned int Get_Serial_USB_Data(unsigned long u32Index)
{

    unsigned int uData = 0;
    unsigned char u8Cnt =0;

    uData = USBREG[EPn_SERIAL_D+u32Index];

    return uData;

}


void Usb_CBW_Out(void)
{
	
		unsigned char ucBulkCmdIn,i;
		unsigned int GetCmd_H = 0,GetCmd_L = 0;
		fBadCBW=1;

		if(!(USBREG[EPn_TLU_STATUS+0x40] & RAM_VALID))
		{
		    while(1);
		    return;
		}

		ucBulkCmdIn = 0;
		X_Residue.All = 0;

		if((i=USBREG[EPn_DATA_LEN_L+0x40]) != 31)
		{
		    USBREG[EPn_TRIG_CFG1+EP2] = 0x40;
		    i=0;
		}
		else
		{
 		    GetCmd_L = Get_Serial_USB_Data(EP2);
		    GetCmd_L |= (Get_Serial_USB_Data(EP2))<<8;
		    GetCmd_H = (Get_Serial_USB_Data(EP2));
		    GetCmd_H |= (Get_Serial_USB_Data(EP2))<<8;

 		    if((GetCmd_L==0x5355)&&(GetCmd_H==0x4342))
		    {
 		        fBadCBW=0;
		        ucBulkCmdIn = 1;

		        X_MASS_TAG.BYTEMODE.BYTE1 = Get_Serial_USB_Data(EP2);
		        X_MASS_TAG.BYTEMODE.BYTE2 = Get_Serial_USB_Data(EP2);
		        X_MASS_TAG.BYTEMODE.BYTE3 = Get_Serial_USB_Data(EP2);
		        X_MASS_TAG.BYTEMODE.BYTE4 = Get_Serial_USB_Data(EP2);

		        X_MASS_XFR_LEN.BYTEMODE.BYTE1 = Get_Serial_USB_Data(EP2);
		        X_MASS_XFR_LEN.BYTEMODE.BYTE2 = Get_Serial_USB_Data(EP2);
		        X_MASS_XFR_LEN.BYTEMODE.BYTE3 = Get_Serial_USB_Data(EP2);

		        X_MASS_XFR_LEN.BYTEMODE.BYTE4 = Get_Serial_USB_Data(EP2);
		        MassDirection = Get_Serial_USB_Data(EP2);
		        MASS_LUN = (Get_Serial_USB_Data(EP2)) & 0x0F;

		        X_Residue.BYTEMODE.BYTE1 = Get_Serial_USB_Data(EP2);

						
		        for(i=0; i<16; i++)
		        {
		            X_CMDBLOCK[i] = Get_Serial_USB_Data(EP2);

		        }

		        USBREG[EPn_TRIG_CFG1+EP2] = 0x40;

		        X_Residue.All = 0;

		        SCSI_CMD_PROCESS();
		        gbDoCSW=1U;


		    }
		    else
		        USBREG[EPn_TRIG_CFG1+EP2] = 0x40;
		}


		if(fBadCBW)
		    USBREG[EPn_TRIG_CFG1 + 0x40] = STALL_ALWAYS;  //stall EP2


}


