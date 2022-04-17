#include "nes.h"
#ifdef D_NES_CMODEL
#include "8051.h"
#else
#include <reg52.h>
#include <8051.h>
#endif
#include "Var_Def.h"
#include <string.h>
#include <UsbReg.h>
#include "USB_String.h"
#include "MCU_Reg.h"
#include "sys_api.h"
#include "usb_def.h"
#include "Interrupt_API.h"
#include "GPIO_Reg.h"
#include "GPIO_API.h"
#include "SPI_Flash_API.h"
#include "Buf_Def.h"
#include "Operator.h"
#include "pba_api.h"
#include "pba_test_api.h"
#include "DMA_Reg.h"
#include "USB_API.h"
#include "basetype.h"
#include "a.h"

//==================================================================================================================

BYTE xdata gb_Interface;
BYTE xdata gb_Link_Error_count;
SWORD data gw_In_EP_Offset, gw_Out_EP_Offset;


//==================================================================================================================



void Trig_USB(SWORD aw_EPn_Offset)
{
    USBREG[EPn_BUF_START0 + aw_EPn_Offset] = 0x00;
    USBREG[EPn_TRIG_CFG1 + aw_EPn_Offset] = EPn_TRIG_EN;
}

#if 0
void Get_LBA()
{
	if ((X_CMDBLOCK[0]&0xF0) || (X_CMDBLOCK[0]==0x06))//0x28 command
	{
		LBA.BYTEMODE.BYTE4 = X_CMDBLOCK[2];
		LBA.BYTEMODE.BYTE3 = X_CMDBLOCK[3];
		LBA.BYTEMODE.BYTE2 = X_CMDBLOCK[4];
		LBA.BYTEMODE.BYTE1 = X_CMDBLOCK[5];
		SEC_CNT = (X_CMDBLOCK[7]<<8) | X_CMDBLOCK[8];
	}
	else
	{
		LBA.BYTEMODE.BYTE4 = 0x00;
		LBA.BYTEMODE.BYTE3 = X_CMDBLOCK[1] & 0x1F;
		LBA.BYTEMODE.BYTE2 = X_CMDBLOCK[2];
		LBA.BYTEMODE.BYTE1 = X_CMDBLOCK[3];
		SEC_CNT = X_CMDBLOCK[4];
	}    
}
#endif



void USB_Trig_Receive(bit is_hold)
{
	while (USBREG[EPn_TRIG_CFG1+EP2] & EPn_TRIG_EN) {}
	Buf_Flag_Setting(0x80, 0x00);   //Set empty

	USBREG[EPn_BUF_START0+EP2] = 0x00;//X_BUFBUFP;

	USBREG[EPn_T_SEC_CNT_H_0x219+EP2] = 0x00;
	USBREG[EPn_T_SEC_CNT_L_0x218+EP2] = 0x01;

	USBREG[EPn_TRIG_CFG1+EP2] = (is_hold)?(EPn_TRIG_EN|OTF_MD):(EPn_TRIG_EN);

	while (USBREG[EPn_TRIG_CFG1+EP2] & EPn_TRIG_EN) {}
}

void Receive_All_data(void)
{
	if (MassDirection)
	{
		gucStallOnGoing=0x01U; /*set the flag to denote the STALL in on going on IN.*/
		USBREG[EPn_TRIG_CFG1 + ENDPOINT1] = STALL_ONCE;  /* stall EP1 */
		X_CSWStatus = 0x01U;
		MassError = 1U;
	}
	else
	{
		/*consider case 511,512,513*/
		while (X_MASS_XFR_LEN.All>512U)
		{
			X_MASS_XFR_LEN.All-=512U;
			USB_Trig_Receive(1U);/*must in common area*/
		}
		if (X_MASS_XFR_LEN.All)
		{
			USB_Trig_Receive(0U);/*must in common area*/
		}
	}
}



void Usb_Stall_Bulk(void)
{
	if (X_MASS_XFR_LEN.All)
	{
		if (MassDirection)
		{
			gucStallOnGoing=0x01; //set the flag to denote the STALL in on going on IN.
			USBREG[EPn_TRIG_CFG1 + EP1] = STALL_ONCE;  //stall EP1

		}
		else
		{
			if(X_MASS_XFR_LEN.All <= 1024)//4096)//1024)
			{
				Receive_All_data();
			}
			else
			{
				gucStallOnGoing=0x02; //set the flag to denote the STALL  on going for OUT.
				USBREG[EPn_TRIG_CFG1 + 0x40] = STALL_ONCE;  //stall EP2
		
			}
		}
	}// end of if(X_MASS_XFR_LEN.All...
	else
	{
		gucStallOnGoing=0x01; //set the flag to denote the STALL in on going on IN.
		USBREG[EPn_TRIG_CFG1 + 0x00] = STALL_ONCE;  //stall EP1
	}

	return;
}




void FifoShortPkt(BYTE *ab_ptrString, SWORD aw_Length)
{
    //Operation:
    //1. put data to the register for data IN.
    //2. trigger th edata sending
    //Parameters:
    //ptrCodeString is a pointer pointed to the spurce string
    //ucLength is the length of data to be sent

    SWORD rw_i;

    
		if(Resetting)
			return;
		

    for (rw_i = 0; rw_i < aw_Length; rw_i++)
    {
        USBREG[EPn_SERIAL_D + gw_In_EP_Offset] = ab_ptrString[rw_i];
    }

    USBREG[EPn_T_BC0 + gw_In_EP_Offset] = (BYTE) aw_Length;         //length, low byte
    USBREG[EPn_T_BC1 + gw_In_EP_Offset] = (BYTE) (aw_Length >> 8);  //length, mid byte
    USBREG[EPn_T_BC2 + gw_In_EP_Offset] = 0;                        //length, high byte
    USBREG[EPn_T_BC3 + gw_In_EP_Offset] = 0;                        //length, high high byte

    USBREG[EPn_TRIG_CFG1 + gw_In_EP_Offset] |= 0x40;    //send data

    while (USBREG[EPn_TRIG_CFG1 + gw_In_EP_Offset] & 0x40);
}



void errASCSenseKey(BYTE ab_ASC, BYTE ab_SenseKey)
{
    X_bASC[SEL_MEDIA] = ab_ASC;          //ASC=Additional Sense Code.
    X_bASCQ[SEL_MEDIA] = 0x00;               //ASCQ=Additional Sense Code Qualifier.
    X_SenseKey[SEL_MEDIA] = ab_SenseKey;    //EVAN: Sense Key.
		X_CSWStatus = 0x01U;
		MassError = 1U;
}



void Set_Media_OK(void)
{
  X_bASC[SEL_MEDIA] = 0x00;
	X_bASCQ[SEL_MEDIA] = 0x00;
	X_SenseKey[SEL_MEDIA] = 0x00;
	X_CSWStatus = 0x00;
	MassError = 0;
}


void Set_Media_NG(void)
{
    errASCSenseKey(0x00, 0x00);
    X_CSWStatus = 1;
}

void Set_NO_Media(void)
{
  X_bASC[SEL_MEDIA] = 0x3A;
	X_bASCQ[SEL_MEDIA] = 0x00;
	X_SenseKey[SEL_MEDIA] = 0x02;
	X_CSWStatus = 0x01;
	MassError = 1;
}


void Set_LUN_Error(void)
{
  X_CSWStatus = 0x01;
	MassError = 1;
	Usb_Stall_Bulk();
}




void Invalid_Cmd(void)
{
	if (MASS_LUN>=X_MaxCard)
	{
		Set_LUN_Error();
	}
	else
	{
		MassError = 1;
		X_CSWStatus = 1;

		X_bASC[SEL_MEDIA] = 0x20;
		X_bASCQ[SEL_MEDIA] = 0x00;
		X_SenseKey[SEL_MEDIA] = 0x05;
		Usb_Stall_Bulk();   
	}

	return;
}


void Invalid_Field_In_Command(void)
{
	X_bASC[SEL_MEDIA] = 0x24;
	X_bASCQ[SEL_MEDIA] = 0x00;
	X_SenseKey[SEL_MEDIA] = 0x05;
	X_CSWStatus = 0x01;
	MassError = 1;
}




void USB_XTAL_Mode(BYTE ab_Enable)
{
    if (ab_Enable)
    {
        CLKREG[CLK_SEL] |= PHY_XTALSEL; //Set USB PHY run XTAL mode
        USBREG[PAGE_NO] = 0x05; // page# 5
        USBREG[CR_OP_GLB_OP0] = PHY_XTAL_SEL; // use xtal, make sure PHY_TRACK[29:28] & PHY_TRACK[1:0] also be 0 for xtal mode
        USBREG[PHY_TRACK_0] = 0x30;   //Crystal mode U3
        USBREG[PHY_TRACK_3] = 0x00;   //Crystal mode U2
        USBREG[PAGE_NO] = 0x00;   // page# 0
    }
    else
    {
        CLKREG[CLK_SEL] &= ~(PHY_XTALSEL);  //Set USB PHY run nonXTAL mode
        USBREG[PAGE_NO] = 0x05; // page# 5
        USBREG[CR_OP_GLB_OP0] &= ~(PHY_XTAL_SEL); // use xtal, make sure PHY_TRACK[29:28] & PHY_TRACK[1:0] also be 0 for xtal mode
        USBREG[PHY_TRACK_0] = 0x33;   //NonCrystal mode U3
        USBREG[PHY_TRACK_3] = 0x30;   //NonCrystal mode U2
        USBREG[PAGE_NO] = 0x00;   // page# 0
    }
}


void PHY_Set_Reg(void)
{
    /* Set nonXTAL mode for BootCode, XTAL mode for FW code */

    USB_XTAL_Mode(1);   //0 = nonXTAL mode, 1 = XTAL mode

    USBREG[PAGE_NO]   = 0x00;
}


/*
void u30_Phy_Reset(void)
{
   
}


void u30_LinkUp(void)
{
    //USBREG[CONN_SPD] = CR_CONNECT;                //connect and try to enter high speed mode
    USBREG[CONN_SPD] = CR_CONNECT | SUPER_SPD;  //connect and try to enter SuperSpeed mode
}
*/


void usb_Connect(void)
{

	USBREG[PAGE_NO] = 0x06;

	USBREG[0x283] |= BIT5;		// U3 Wake LFPS 1us
	USBREG[0x28E] = 0x9C;  // Rest phy

	USBREG[0x290] = 0xD7; //0x06; //0x07; //for U3 LPS timing ( Suspend debug)

	USBREG[0x320] = 0xf0; //0xe0;  // PCS[7:6]=11b

	USBREG[0x321] = 0xf0;  // PCS[9:8]=11b
	USBREG[0x322] = 0x0A; //0x8b; //0x8A; //0x8b;  //james for EB issue
	USBREG[0x323] = 0x4E; //0x2d; //0x2e;  //james for EB issue

	USBREG[0x324] = 0x21;
	USBREG[0x325] = 0x21;  //james for ( bit 5 is for turn off OSC in P3)
	USBREG[0x326] = 0xC0;  //james for
	USBREG[0x327] = 0x01; //0x04; //0x00; //0x05;  //james for db port
	USBREG[0x328] = 0x23;
	USBREG[0x329] = 0x72; //0xf2;

	USBREG[0x301]=0x29;  // fine tune clock, default is 0x09
	USBREG[0x302]=0x70;  // set band register

	USBREG[0x306] = 0x85; //0x05; //ipx preramp
	USBREG[0x307] = 0x00; //common mode PCM volt

	USBREG[0x30A] = 0x08; // Kp=18, Ki=12
	USBREG[0x30D] = 0x13; // Set EQ

	USBREG[0x30E] = 0xBB;


	USBREG[PAGE_NO] = 0x07;
	USBREG[0x324] = 0xc4;
	USBREG[0x325] = 0x39;
	USBREG[0x327] = 0x10;
	USBREG[0x328] = 0x01;


	USBREG[PAGE_NO] = 0x07;
	/* Register Mode */
	USBREG[0x280] = 0xB5;

	USBREG[PAGE_NO] = 0x00;
	/*HW always replys U1/U2 */
	USBREG[ENTER_UX ] |= LXU_U2|LXU_U1;
	USBREG[CONN_SPD] = (CR_CONNECT|SUPER_SPD); /* Connect & Try To Enter SuperSpeed Mode */


}



void usb_send_EP0ACK(void)
{
    //for normal response without data phase
    //EX1 = 0; //so the interrupt flag from EP0_STSED_STA won't be set or cleared by ISR
    USBREG[EPC_RESPONSE] = STAT_NORM;
}


#if 0
bit BotHxDn()
{
	Set_Media_OK();

	X_Residue.All=X_MASS_XFR_LEN.All;
	if (!MassDirection)
	{
		if (X_MASS_XFR_LEN.All)
		{/*BOT case 9, Ho>Dn*/
			Receive_All_data();/*Receive_All_data();*/
			X_CSWStatus = 0x01U;
			return 1U;
		}
	}
	else if (X_MASS_XFR_LEN.All)
	{/*BOT case 4, Hi>Dn*/
		Usb_Stall_Bulk();
		X_CSWStatus = 0x01U;
		return 1U;
	}
	else
	{
		/*This area is reserved*/
	}
	return 0U;
}
#endif


void usb_Set_Configuration(void)
{
	xdata unsigned char ucCmdErr=0U;

	ucCmdErr = 0U;

	if (gbEPxHalt)
	{
		ucCmdErr=1U;
	}
	else
	{
		if (!gbAddressed)
		{
			ucCmdErr=1U;
		}
		else if (gucValue_H)
		{
			ucCmdErr=1U;
		}
		else if (!gucValue_L)
		{
			gucConfiguredValue = 0U;
		}
		else if (gucValue_L==1U)
		{
			gucConfiguredValue = 0x10U|0x01U; /*configured and the configured value is 1*/
		}
		else
		{
			ucCmdErr = 1U;
		}
	}

	if (ucCmdErr)
	{
		gbStallEP0Cmd = 1U;
	}
	else
	{
		USBREG[EPn_TRIG_CFG1] = 0x00;
   	USBREG[EPn_TRIG_CFG1 + 0x40] = 0x00;
		gucStallOnGoing = 0U;
		usb_send_EP0ACK();
	}
	gbEP0CmdFinish = 0U;
	gbDoCSW=0U;

	if (gucConfiguredValue)
	{
		/* put statements to blink LED for a while.//FLASH_LED = 0; FLASH_LED_CNT = 10;*/
		/* put a statement to enabale timer 0.     //ET0 = 1;*/
	}
	else
	{
		/* put statements to use slower CPU clock  //FAIRY[CLKDIV] = 0xA9;    //init : 0xA0*/
		/* put a statement to turn off LED here.   //FLASH_LED = 1;*/
		/* put a ststement to disbale timer 0.     //ET0 = 0;*/
	}
}


static void Buffer_Set(void)
{

#if 0
	/*USB RING SETTING*/
	USBREG[EPn_BUF_TOP0+EP1] = 0x00;
	USBREG[EPn_BUF_TOP1+EP1] = 0x01;
	USBREG[EPn_BUF_TOP2+EP1] = 0x00;
	USBREG[EPn_BUF_LEN0+EP1] = 0x40;
	USBREG[EPn_BUF_LEN1+EP1] = 0x00;

	USBREG[EPn_BUF_TOP0+EP2] = 0x00;
	USBREG[EPn_BUF_TOP1+EP2] = 0x01;
	USBREG[EPn_BUF_TOP2+EP2] = 0x00;
	USBREG[EPn_BUF_LEN0+EP2] = 0x40;
	USBREG[EPn_BUF_LEN1+EP2] = 0x00;
#endif

#if 1	
    //EP0 ring buffer top buffer number(0x000134->buffer number 0x9A)
    USBREG[EP0_BUF_TOP0] = USBEP0BUF_START_ADR0;
    USBREG[EP0_BUF_TOP1] = USBEP0BUF_START_ADR1;
    USBREG[EP0_BUF_TOP2] = USBEP0BUF_START_ADR2;
    //EP0 ring buffer max length(1 sector)
    USBREG[EP0_BUF_LEN0] = USBEP0BUF_LEN0;
    USBREG[EP0_BUF_LEN1] = USBEP0BUF_LEN1;



    //EP1 ring buffer top buffer number(0x000180->buffer number 0xC0)
    USBREG[EPn_BUF_TOP0 + EP1_OFFSET] = USBEPNBUF_START_ADR0;
    USBREG[EPn_BUF_TOP1 + EP1_OFFSET] = USBEPNBUF_START_ADR1;
    USBREG[EPn_BUF_TOP2 + EP1_OFFSET] = USBEPNBUF_START_ADR2;
    //EP1 ring buffer max length(64 sector)
    USBREG[EPn_BUF_LEN0 + EP1_OFFSET] = USBEPNBUF_LEN0;
    USBREG[EPn_BUF_LEN1 + EP1_OFFSET] = USBEPNBUF_LEN1;

    //EP2 ring buffer top buffer number(0x000180->buffer number 0xC0)
    USBREG[EPn_BUF_TOP0 + EP2_OFFSET] = USBEPNBUF_START_ADR0;
    USBREG[EPn_BUF_TOP1 + EP2_OFFSET] = USBEPNBUF_START_ADR1;
    USBREG[EPn_BUF_TOP2 + EP2_OFFSET] = USBEPNBUF_START_ADR2;
    //EP2 ring buffer max length(64 sector)
    USBREG[EPn_BUF_LEN0 + EP2_OFFSET] = USBEPNBUF_LEN0;
    USBREG[EPn_BUF_LEN1 + EP2_OFFSET] = USBEPNBUF_LEN1;


    //EP3 ring buffer top buffer number(0x000180->buffer number 0xC0)
    USBREG[EPn_BUF_TOP0 + EP3_OFFSET] = USBEPNBUF_START_ADR0;
    USBREG[EPn_BUF_TOP1 + EP3_OFFSET] = USBEPNBUF_START_ADR1;
    USBREG[EPn_BUF_TOP2 + EP3_OFFSET] = USBEPNBUF_START_ADR2;
    //EP3 ring buffer max length(64 sector)
    USBREG[EPn_BUF_LEN0 + EP3_OFFSET] = USBEPNBUF_LEN0;
    USBREG[EPn_BUF_LEN1 + EP3_OFFSET] = USBEPNBUF_LEN1;

    //EP4 ring buffer top buffer number(0x000180->buffer number 0xC0)
    USBREG[EPn_BUF_TOP0 + EP4_OFFSET] = USBEPNBUF_START_ADR0;
    USBREG[EPn_BUF_TOP1 + EP4_OFFSET] = USBEPNBUF_START_ADR1;
    USBREG[EPn_BUF_TOP2 + EP4_OFFSET] = USBEPNBUF_START_ADR2;
    //EP4 ring buffer max length(64 sector)
    USBREG[EPn_BUF_LEN0 + EP4_OFFSET] = USBEPNBUF_LEN0;
    USBREG[EPn_BUF_LEN1 + EP4_OFFSET] = USBEPNBUF_LEN1;

    //EP5 ring buffer top buffer number(0x000180->buffer number 0xC0)
    USBREG[EPn_BUF_TOP0 + EP5_OFFSET] = USBEPNBUF_START_ADR0;
    USBREG[EPn_BUF_TOP1 + EP5_OFFSET] = USBEPNBUF_START_ADR1;
    USBREG[EPn_BUF_TOP2 + EP5_OFFSET] = USBEPNBUF_START_ADR2;
    //EP5 ring buffer max length(64 sector)
    USBREG[EPn_BUF_LEN0 + EP5_OFFSET] = USBEPNBUF_LEN0;
    USBREG[EPn_BUF_LEN1 + EP5_OFFSET] = USBEPNBUF_LEN1;

    //EP6 ring buffer top buffer number(0x000180->buffer number 0xC0)
    USBREG[EPn_BUF_TOP0 + EP6_OFFSET] = USBEPNBUF_START_ADR0;
    USBREG[EPn_BUF_TOP1 + EP6_OFFSET] = USBEPNBUF_START_ADR1;
    USBREG[EPn_BUF_TOP2 + EP6_OFFSET] = USBEPNBUF_START_ADR2;
    //EP6 ring buffer max length(64 sector)
    USBREG[EPn_BUF_LEN0 + EP6_OFFSET] = USBEPNBUF_LEN0;
    USBREG[EPn_BUF_LEN1 + EP6_OFFSET] = USBEPNBUF_LEN1;
#endif

}


void PhyInit(void)
{
 
    int cnt = 0;

    

	USBREG[PAGE_NO] = 0x05;


	USBREG[0x200] |= PHY_XTAL_SEL;
	USBREG[0x210]  = 0x30;
	USBREG[0x213]  = 0x00;

	USBREG[0x211] = 0x64;
	USBREG[0x214] = 0x4D;


	USBREG[PAGE_NO] = 0x05;
	USBREG[0x228] = 0x24; // ·Å«×¨t¼Æ


	USBREG[PAGE_NO] = 0x00;

	// HW Auto RCOSC switch
	USBREG[PHY_PLL_ST] &= (~0x40);



	// wait for PLL lock
	SYSREG[0x6F] |= (BIT4);
	while (!(USBREG[PHY_PLL_ST]&(0x03)));

	SYSREG[0x6B] = 0xff;


}




void UsbInit(void)
{
    /* Select USB reg mapping Page#0 */
    USBREG[PAGE_NO] = 0x00;

    MediaStatus[0] = 0;
    MediaStatus[1] = 0;
    //memset(FAT_MBR_Base, 0x00, 2048);
   // memcpy(FAT_MBR_Base, FDISK_FAT_MBR, FAT_MBR_SIZE);
    //memcpy(FAT_PBS_Base,FDISK_FAT_PBS,FAT_PBS_SIZE);

    /* Set EP0,1,2,3,4 buffer */
    Buffer_Set();

    //USBREG[PHY_PLL_ST] |= HW_AUTO_SW_RCOSC;

    EX1 = 1;
    EX0 = 1;
       
    Resetting=0;

  
    USBREG[INT1_DET_EN0] = CR_EP2_ABFVLD_EN ;   // en EP2

    /* We could disable U1/U2 suspend temptaryly for V7 FPGA ICE mode */
    USBREG[ENTER_UX] = (LXU_U2 | LXU_U1);

    usb_Connect();

    gb_Link_Error_count = 0;
}

void usb_EP0_Tx_Data(void)
{
    SWORD rw_i;

    for (rw_i = 0; rw_i < gu16EP0DataCount; rw_i++)
    {
        //put data to the EP0 IN buffer
        if (gbGetFromXdata)
        {
            USBREG[EP0_SERIAL_D] = ((BYTE xdata*)ptrEP0DataPointer)[rw_i];
        }
        else
        {
            USBREG[EP0_SERIAL_D] = ptrEP0DataPointer[rw_i];
        }
    }

    USBREG[EP0_TRIG_BCNT0] = gu16EP0DataCount;
    USBREG[EP0_TRIG_BCNT1] = gu16EP0DataCount >> 8;

    //while( !(USBREG[PIPE0_INT_STA0] & EP0_BFRDY_STA)) //wait untill the data been sent
    //{
    //if(GetBusEvent())
    //return;
    //}

    //USBREG[PIPE0_INT_STA0] = EP0_BFRDY_STA;           //clear the flag

    USBREG[EP0_TRIG_CFG] |= INC_PTR;            //increase data pointer
    USBREG[EPC_RESPONSE] = DATA_NORM | STAT_NORM;   //return ACK
    gbGetFromXdata = 0; //reset the flag !
}

void usb_Clear_Feature(void)
{

	if (!gucConfiguredValue)
	{
		if ((gucRequestType==0x80U)&&(gucValue_H==0U)&&(gucValue_L==DEVICE_REMOTE_WAKEUP)) {}
		else if ((gucRequestType==0x82U)&&((gucIndex_H | gucIndex_L | gucValue_H | gucValue_L)==0U))
		{
			gbEPxHalt = 0U;
		}
		else
		{
			gbStallEP0Cmd = 1U;
		}
	}
	else
	{
		if (gucValue_H | gucIndex_H)
		{
			gbStallEP0Cmd = 1U;
		}
		else
		{
			switch (gucValue_L)
			{
				case ENDPOINT_STALL:
					if (gucRequestType==2U)
					{
						if (gucIndex_L==0U)
						{
							gbEPxHalt=0U;
						}
						else if ((gucIndex_L==0x81U)||(gucIndex_L==0x01U)) /*for endpoint 1*/
						{
							if (gbBadBulkCmd)
							{
								break; /*don't clear the STALL when bad bulk command is received*/
							}
							gucStallOnGoing &= ~0x01U; /*clear the flag of STALL on IN*/
							if (!gucStallOnGoing)
							{
								gbEPxHalt = 0U; /*clear the Flag for halt*/
							}
							USBREG[EPn_TRIG_CFG1] = 0x00;
							USBREG[EPn_TRIG_CFG1 + ENDPOINT2] = 0x00;
						}
						else if (gucIndex_L==2U) /*for endpoint 2*/
						{
							if (gbBadBulkCmd)
							{
								break; /*don't clear the STALL when bad bulk command is received*/
							}
							gucStallOnGoing &= ~0x02U; /*clear the flag of STALL on OUT*/
							if (!gucStallOnGoing)
							{
								gbEPxHalt = 0U; /*clear the Flag for halt*/
							}
							USBREG[EPn_TRIG_CFG1] = 0x00;
							USBREG[EPn_TRIG_CFG1 + ENDPOINT2] = 0x00;
						}
						else
						{
							gbStallEP0Cmd = 1U;
						}
					}
					else
					{
						gbStallEP0Cmd = 1U;
					}
					break;

				case DEVICE_REMOTE_WAKEUP:
					if (gucRequestType)
					{
						gbStallEP0Cmd = 1U;
					}
					break;

				default:
					gbStallEP0Cmd = 1U;
					break;
			}
		}
	}

	if (!gbStallEP0Cmd)
	{
		gbEP0CmdFinish = 1U;
		usb_send_EP0ACK();
	}
}



void usb_Set_Interface(void)
{
	if ( (gbEPxHalt) || (!gucConfiguredValue) || (gucValue_L>2U) ||
	        (gucIndex_H | gucIndex_L | gucValue_H ))
	{
		gbStallEP0Cmd = 1U;
	}
	else
	{
		gucInterface = gucValue_L;

		/*set endpoint 1 and 2*/
		usb_send_EP0ACK(); /* Send an ACK_TP*/
		gbEP0CmdFinish = 1U;
	}
}


void usb_Get_Interface(void)
{
	xdata unsigned char ucxTmp=0U;

	if ( (gbEPxHalt) || (!gucConfiguredValue) ||
	        (gucIndex_L | gucIndex_H | gucValue_L | gucValue_H))
	{
		gbStallEP0Cmd = 1U;
	}
	else
	{
		/*to send a byte for the number of avaliable interface*/
		gu16EP0DataCount=1U;
		ucxTmp=gucInterface;
		ptrEP0DataPointer=&ucxTmp;
		gbGetFromXdata=1U;
		usb_EP0_Tx_Data();
		gbEP0CmdFinish = 1U;
	}
}


void usb_Get_Configuration()
{
	xdata unsigned char ucCmdErr=0U;
	xdata unsigned char ucxTmp=0U; 

	ucCmdErr = 0U;
	if (gbEPxHalt)
	{
		ucCmdErr = 1U;
	}
	else
	{
		/*to send a byte for the number of avaliable configuration*/
		gu16EP0DataCount=1U;
		ucxTmp=gucConfiguredValue&0x0fU;
		ptrEP0DataPointer=&ucxTmp;
		gbGetFromXdata=1U;
		usb_EP0_Tx_Data();
		gbEP0CmdFinish = 1U;
	}
}


void usb_Get_Descriptor(void)
{
	xdata unsigned char ucCMD_ERR=0U;

	ucCMD_ERR = 0U;
	if (gbEPxHalt)
	{
		ucCMD_ERR = 1U;
	}
	else
	{
		if ((gucValue_H==STRING) && (gucValue_L > 3U))
		{
			ucCMD_ERR = 1U;
		}
		else if ((gucValue_H==BOS_ID) && (gucValue_L | gucIndex_L | gucIndex_H))
		{
			ucCMD_ERR = 1U;
		}
		else if ((gucValue_H!=STRING) && (gucValue_L | gucIndex_L | gucIndex_H))
		{
			ucCMD_ERR = 1U;
		}
		else
		{
			gbGetFromXdata=1U;
			switch (gucValue_H)
			{
			case DEVICE:
				/*EP0DataPointer = CbDeviceDescriptor;*/
				gu16EP0DataCount = 0x12U;
				if (gucUsbSpeed==3U)
				{
					memcpy(((void *)(&gucxMiscDescriptors[0U])), ((void *)(gucSsDeviceDescriptor)), ((signed int)(gu16EP0DataCount)));/*should copy from info block*/
				}
				else
				{
					memcpy(((void *)(&gucxMiscDescriptors[0U])), ((void *)(guc20DeviceDescriptor)), ((signed int)(gu16EP0DataCount)));/*should copy from info block*/
					/*if(SL_RESUME_SRAM1==0x5a5a) gucxMiscDescriptors[2]=0x10;//LPM cheating*/
				}
				ptrEP0DataPointer = &gucxMiscDescriptors[0U];
				break;

			case CONFIGURATION:
				if (gucUsbSpeed==3U)
				{
					gu16EP0DataCount = CONFIG_LENGTH;
					memcpy(((void *)(&gucxMiscDescriptors[0U])), ((void *)(&gucSsConfigDescriptor[0U])), ((signed int)(CONFIG_LENGTH)));
				}
				else if (gucUsbSpeed==2U)
				{
					gu16EP0DataCount = CONFIG_LENGTH20;
					memcpy(((void *)(&gucxMiscDescriptors[0U])), ((void *)(&gucHSConfigDescriptor[0])), ((signed int)(CONFIG_LENGTH)));
				}
				else
				{
					/*EP0DataPointer = CbConfigDescriptorFS;*/
					gu16EP0DataCount = CONFIG_LENGTH20;
					memcpy(((void *)(&gucxMiscDescriptors[0U])), ((void *)(gucFSConfigDescriptor)), ((signed int)(CONFIG_LENGTH)));
				}
				ptrEP0DataPointer = &gucxMiscDescriptors[0U];
				break;

			case STRING:
				/*memset(&gucxMiscDescriptors,0x00,52);*/
				if (gucValue_L==0U)
				{
					memcpy(((void *)(&gucxMiscDescriptors[0U])), ((void *)(&gucStringDescriptorLid[0])), ((signed int)(gucStringDescriptorLid[0])));
				}
				else if (gucValue_L==1U)
				{
					memcpy(((void *)(&gucxMiscDescriptors[0U])), ((void *)(&gucStringDescriptor1[0])), ((signed int)(gucStringDescriptor1[0])));
				}
				else if (gucValue_L==2U)
				{
					memcpy(((void *)(&gucxMiscDescriptors[0U])), ((void *)(&gucStringDescriptor2[0])), ((signed int)(gucStringDescriptor2[0])));
				}
				else if (gucValue_L==3U)
				{
					memcpy(((void *)(&gucxMiscDescriptors[0U])), ((void *)(&gucSerialDescriptor[0])), ((signed int)(gucSerialDescriptor[0])));
				}
				else
				{
					/*This area is reserved*/
				}
				gu16EP0DataCount = gucxMiscDescriptors[0U];
				ptrEP0DataPointer = &gucxMiscDescriptors[0U];
				break;

			case DEVICE_QUALIFIER:
				if (gucUsbSpeed==2U) /* High Speed*/
				{
					ptrEP0DataPointer = &gucHSDeviceQualifier[0U];
				}
				else
				{
					ptrEP0DataPointer = &gucFSDeviceQualifier[0U];
				}
				gu16EP0DataCount = DEVICE_QUALIFIER_LENGTH;
				gbGetFromXdata=0U;
				break;

			case OTHER_SPEED_CONFIGURATION:
				if (gucUsbSpeed==2U)
				{ /* High Speed*/
					gu16EP0DataCount = OTHER_SPEED_CONFIGURATION_LENGTH;
					memcpy(((void *)(&gucxMiscDescriptors[0U])), ((void *)(gucOtherSpeedConfigurationHS)), ((signed int)(OTHER_SPEED_CONFIGURATION_LENGTH)));
					/*@gucxMiscDescriptors[8]=0x32;//set by info block//power consumption*/
				}
				else
				{
					gu16EP0DataCount = OTHER_SPEED_CONFIGURATION_LENGTH;
					memcpy(((void *)(&gucxMiscDescriptors[0U])), ((void *)(gucOtherSpeedConfigurationFS)), ((signed int)(OTHER_SPEED_CONFIGURATION_LENGTH)));
					/*@gucxMiscDescriptors[8]=0x32;//set by info block//power consumption*/
				}
				ptrEP0DataPointer = &gucxMiscDescriptors[0U];
				break;

			case BOS_ID:
				if (gucUsbSpeed==3U)
				{
					gu16EP0DataCount = BOS_LENGTH;
					memcpy(((void *)(&gucxMiscDescriptors[0U])), ((void *)(&gucBinaryObjectStore[0])), ((signed int)(BOS_LENGTH)));
					ptrEP0DataPointer = &gucxMiscDescriptors[0U];
				}
				else
				{
					/*if(SL_RESUME_SRAM1==0x5a5a)*/
					{
						gu16EP0DataCount = BOS_LENGTH;
						memcpy(((void *)(&gucxMiscDescriptors[0U])), ((void *)(&gucBinaryObjectStore[0])), ((signed int)(BOS_LENGTH)));
						ptrEP0DataPointer = &gucxMiscDescriptors[0U];
					}
					/*else*/
					/*  {*/
					/*    ucCMD_ERR=1;//LPM Cheating*/
					/*  }*/
				}
				break;

			default:
				ucCMD_ERR=1U;
				gbGetFromXdata=0U;
				break;
			}/*end of switch()...*/
		}
	}/*end of if (gbEPxHalt) else...*/

	if (!gucxLength_H)
	{
		if (gu16EP0DataCount>gucxLength_L)
		{
			gu16EP0DataCount = gucxLength_L;
		}
	}

	if (ucCMD_ERR)
	{
		gbStallEP0Cmd = 1U;
	}
	else
	{
		usb_EP0_Tx_Data();
	}
}

void usb_Get_Status(void)
{
	unsigned char ucCmdErr;
	unsigned char xdata ucxStatus[2]; 
	
	ucxStatus[1] = ucxStatus[0]= ucCmdErr =0x00;

	if (!gucConfiguredValue)
	{
		if (gucRequestType==0x80)
		{}
		else if ((gucRequestType==0x82)&&((gucIndex_H | gucIndex_L)==0))
		{
			ucxStatus[0] =  gbEPxHalt ? 0x01 : 0x00;
		}
		else 
			ucCmdErr=1;
	}
	else
	{		
		if(gucRequestType==0x82) //for endpoint
		{
			if (gucIndex_H) 
				ucCmdErr = 1;
			else
			{
				if (gucIndex_L==0)
				{//for EP0
				 	ucxStatus[0] = (gbEPxHalt && (gucStallOnGoing==0))? 0x01 : 0x00;
				}
				else if ((gucIndex_L==0x81) || (gucIndex_L==0x01))
				{//for EP1		    
				 	ucxStatus[0] = (gucStallOnGoing & 0x01) ? 0x01 : 0x00;
				}
				else if (gucIndex_L==2)
				{//for EP2
					ucxStatus[0] = (gucStallOnGoing & 0x02) ? 0x01 : 0x00;
				}
				else 
					ucCmdErr = 1;
			}
		} //end of if(gucRequestType==0x82)...    
	} //end of if (!gucConfiguredValue) else...

	if (ucCmdErr) 
		gbStallEP0Cmd = 1;
	else
	{
		gu16EP0DataCount=2;
		ptrEP0DataPointer=&ucxStatus[0];
		gbGetFromXdata=1;
		usb_EP0_Tx_Data();
	}
}


void usb_Set_Feature(void)
{
	xdata unsigned char ucTestMode=0U;

	gbStallEP0Cmd = 0U;
	ucTestMode = 0U;

	if ((gucUsbSpeed!=3U) && (gucIndex_H) && (gucIndex_H<=5U) )
	{

		ucTestMode=1U;
	}
	else if (gucRequestType==0x01U)
	{/*Interface*/
		if (gucConfiguredValue)
		{/*configured*/
			if (gucIndex_L==0x00U)
			{/*suspend option=0x00*/
			}
			else
			{/*suspend option != 0x00*/
			}
		}
		else
		{
			gbStallEP0Cmd = 1U;
		}
	}
	else if ((gucRequestType==0x02U)&&((gucIndex_L==0x81U)||(gucIndex_L<=2U)))
	{/*endpoint*/
		gbEPxHalt = 1U;
		gucStallOnGoing = gucIndex_L&0x03U;
	}
	else if (gucRequestType==0x00U)
	{
		if (!gucConfiguredValue)
		{
			gbStallEP0Cmd = 1U;
		}


	} /*end of else if(gucRequestType==0x00)...*/
	else
	{
		gbStallEP0Cmd = 1U;
	}

	if (!gbStallEP0Cmd)
	{
		usb_send_EP0ACK(); /* Send an ACK_TP*/
		if (ucTestMode)
		{

		}/*end of  if (gucUsbSpeed!=3) ...*/
	}/*end of if(ucCMD_ERR) else ...*/
}

void usb_Set_Address(void) 
{
	xdata unsigned char ucCmdErr=0U;

	if (gucUsbSpeed==2U)
	{
	}

	ucCmdErr = 0U;
	if (gbEPxHalt)
	{
		ucCmdErr = 1U;
	}
	else
	{
		if ((gucValue_H) || (gucValue_L>127U))
		{
			ucCmdErr=1U;
		}
		else
		{
			if (gucValue_L)
			{
				gbAddressed = 1U;
			}
			else
			{
				gbAddressed = 0U;
			}
		}
	}

	if (ucCmdErr)
	{
		gbStallEP0Cmd = 1U;
	}
	else
	{

		gucStallOnGoing = 0x00U; /*clear the flag of STALL on IN*/

		
		gbDoCSW=0U;

		
		gbEP0CmdFinish = 1U;
		usb_send_EP0ACK();
	}
}



void StandardCommand(void)
{
	gbStallEP0Cmd = 0;
	switch (gucRequest)
	{
		case 0: // get status
			usb_Get_Status();
			break;

		case 3: // set feature
			usb_Set_Feature();
			break;

		case 1: // clear feature. need to do this in interrupt?
			usb_Clear_Feature();
			break;

		case 5: // set address
			usb_Set_Address();
			break;

		case 6: // get descriptor
			usb_Get_Descriptor();
			break;

		case 8: // get configuration
			usb_Get_Configuration();
			break;

		case 9: // set configuration
			usb_Set_Configuration();
			break;

		case 10:// get interface
			usb_Get_Interface();
			break;

		case 11:// set interface
			usb_Set_Interface();
			break;

		default:
			gbStallEP0Cmd = 1;
			break;
	}

	if (gbStallEP0Cmd)
	{
		if(gucxLength_L || gucxLength_H)
			USBREG[EPC_RESPONSE] = DATA_STALL;
		else
			USBREG[EPC_RESPONSE] = STAT_STALL;
	}
	return;
}


void usb_Bulk_Reset(void)
{
  if (gucValue_H | gucValue_L |gucIndex_H | gucIndex_L|gucxLength_H |gucxLength_L)
	{
		gbStallEP0Cmd = 1U;
		return;
	}
	
	USBREG[EPn_TRIG_CFG1] = 0x00;
  USBREG[EPn_TRIG_CFG1 + 0x40] = 0x00;

	gucStallOnGoing = 0U;
	gbBadBulkCmd=0U;

	usb_send_EP0ACK();
}



void usb_Bulk_Get_Lun(void)
{
	if ((gucValue_H | gucValue_L | gucIndex_H | gucIndex_L | gucxLength_H |(gucxLength_L!=1)))/*use | to save code size*/
	{
		gbStallEP0Cmd = 1U;
		return;
	}

	if (gucxLength_L)
	{
		gu16EP0DataCount = 1U;
		gucxMiscDescriptors[0U]=0U;
		ptrEP0DataPointer = &gucxMiscDescriptors[0U];
		gbGetFromXdata=1U;
		usb_EP0_Tx_Data();
	}
	else
	{
		usb_send_EP0ACK();
	}
}

void ClassCommand(void)
{
    gbStallEP0Cmd = 0;

    switch (gucRequest)
    {
            //case 0x00 :
            //CBI_Cmd();    //CBI
            //break;

        case 0xFF :
            usb_Bulk_Reset();//Bulk_reset();
            break;

        case 0xFE :
            usb_Bulk_Get_Lun();//Bulk_Get_Lun();
            break;

        default :
            gbStallEP0Cmd = 1;
            break;
    }

    if (gbStallEP0Cmd)
    {
        USBREG[EPC_RESPONSE] = DATA_STALL | STAT_STALL;
    }
}


void VendorCommand(void)
{
    return;
}


void Usb_ep0setup(void)
{
	if (!(gucRequestType & (UCHAR)0x60))
	{
		StandardCommand();
	} 
	else if ((UCHAR)(gucRequestType & (UCHAR)0x60)==(UCHAR)0x20) /* class command*/
	{
		ClassCommand();
	}
	else if ((UCHAR)(gucRequestType & (UCHAR)0x60)==(UCHAR)0x40)	/* vendor command*/
	{
		VendorCommand();
	}
	else
	{
	/*put STALL here*/
	}

//	USB_REG[PIPE0_INT_STA2] =  (UCHAR)EP0_SETUP_STA;  /*clear the interruptflag*/
	return;
}


void USBBulkInSectorFromPageBuffer(BYTE Buf_No, SWORD Sec_Cnt, BYTE Buf_Mode)
{
    TWORD ruw_buf_tmp;
    BYTE rb_EPn_BUF_TOP0, rb_EPn_BUF_TOP1, rb_EPn_BUF_TOP2;



    gw_In_EP_Offset = EP1_OFFSET; // Add By CC
    while(USBREG[EPn_TRIG_CFG1 + EP1_OFFSET] & EPn_TRIG_EN);
    //while ((USBREG[EPn_TLU_STATUS + gw_In_EP_Offset] & RAM_VALID) == 0); // ??? maybe error check this

    ruw_buf_tmp.Word = Buf_No;
    ruw_buf_tmp.Word <<= 1;     // for match HW register offset bit.1

    //Backup & set new EPn buffer point
    rb_EPn_BUF_TOP0 = USBREG[EPn_BUF_TOP0 + EP1_OFFSET];
    rb_EPn_BUF_TOP1 = USBREG[EPn_BUF_TOP1 + EP1_OFFSET];
    rb_EPn_BUF_TOP2 = USBREG[EPn_BUF_TOP2 + EP1_OFFSET];

    USBREG[EPn_BUF_TOP0 + EP1_OFFSET] = ruw_buf_tmp.HLMODE.L;
    USBREG[EPn_BUF_TOP1 + EP1_OFFSET] = ruw_buf_tmp.HLMODE.H;
    USBREG[EPn_BUF_TOP2 + EP1_OFFSET] = 0x00;

    //EP2 buffer length, 32 sectors
    //USBREG[EPn_BUF_LEN0 + gw_In_EP_Offset] = 0x20;
    //USBREG[EPn_BUF_LEN1 + gw_In_EP_Offset] = 0x00;

    USBREG[EPn_T_SC0 + EP1_OFFSET] =   (BYTE)Sec_Cnt;
    USBREG[EPn_T_SC1 + EP1_OFFSET] =   (BYTE)(Sec_Cnt >> 8);

    if (Buf_Mode)
    {
        Buf_Flag_Setting(Buf_No, 0x00); //Set empty
        Trig_USB(EP1_OFFSET);

        while (USBREG[EPn_TRIG_CFG1 + EP1_OFFSET] & EPn_TRIG_EN);
    }
    else
    {


        USBREG[EPn_TRIG_CFG0 + EP1_OFFSET] |= OTF_MD;

        Trig_USB(EP1_OFFSET);

        while (USBREG[EPn_TRIG_CFG1 + EP1_OFFSET] & EPn_TRIG_EN);


        USBREG[EPn_TRIG_CFG0 + EP1_OFFSET] &= ~(OTF_MD);
    }

    //Buffer_Set();
    //Restore EPn buffer point
    USBREG[EPn_BUF_TOP0 + EP1_OFFSET] = rb_EPn_BUF_TOP0;
    USBREG[EPn_BUF_TOP1 + EP1_OFFSET] = rb_EPn_BUF_TOP1;
    USBREG[EPn_BUF_TOP2 + EP1_OFFSET] = rb_EPn_BUF_TOP2;


}


void USBBulkOutSectorToPageBuffer(BYTE Buf_No, SWORD Sec_Cnt, BYTE Buf_Mode)
{
    TWORD ruw_buf_tmp;
    BYTE rb_EPn_BUF_TOP0, rb_EPn_BUF_TOP1, rb_EPn_BUF_TOP2;

    gw_Out_EP_Offset =EP2_OFFSET;

    while (USBREG[EPn_TRIG_CFG1 + EP2_OFFSET] & EPn_TRIG_EN);

    ruw_buf_tmp.Word = Buf_No;
    ruw_buf_tmp.Word <<= 1;

    //Backup & set new EPn buffer point
    rb_EPn_BUF_TOP0 = USBREG[EPn_BUF_TOP0 + EP2_OFFSET];
    rb_EPn_BUF_TOP1 = USBREG[EPn_BUF_TOP1 + EP2_OFFSET];
    rb_EPn_BUF_TOP2 = USBREG[EPn_BUF_TOP2 + EP2_OFFSET];
	
    USBREG[EPn_BUF_TOP0 + EP2_OFFSET] = ruw_buf_tmp.HLMODE.L;
    USBREG[EPn_BUF_TOP1 + EP2_OFFSET] = ruw_buf_tmp.HLMODE.H;
    USBREG[EPn_BUF_TOP2 + EP2_OFFSET] = 0x00;

    USBREG[EPn_T_SC0 + EP2_OFFSET] =  (BYTE)Sec_Cnt;
    USBREG[EPn_T_SC1 + EP2_OFFSET] =  (BYTE)(Sec_Cnt >> 8);


    if (Buf_Mode)
    {
        Buf_Flag_Setting(Buf_No, 0x00); //Set empty
        Trig_USB(EP2_OFFSET);

        while (USBREG[EPn_TRIG_CFG1 + EP2_OFFSET] & EPn_TRIG_EN);
    }
    else
    {


        USBREG[EPn_TRIG_CFG0 + EP2_OFFSET] |= OTF_MD;

        Trig_USB(EP2_OFFSET);

        while (USBREG[EPn_TRIG_CFG1 + EP2_OFFSET] & EPn_TRIG_EN);

        USBREG[EPn_TRIG_CFG0 + EP2_OFFSET] &= ~(OTF_MD);


				
    }

    //Buffer_Set();
    //Restore EPn buffer point
    USBREG[EPn_BUF_TOP0 + EP2_OFFSET] = rb_EPn_BUF_TOP0;
    USBREG[EPn_BUF_TOP1 + EP2_OFFSET] = rb_EPn_BUF_TOP1;
    USBREG[EPn_BUF_TOP2 + EP2_OFFSET] = rb_EPn_BUF_TOP2;

}


void Send_Link_Err_Count(void)
{
    xdata BYTE *rb_ptr = 0x20000;

    rb_ptr[0] = gb_Link_Error_count;

    USBBulkInSectorFromPageBuffer(0x80, 1, 0);
}


void UsbHandler(void)
{
	if (usb_interrupt_Event )
	{
		
		if(usb_interrupt_Event&RCOSC_SW_REQ)  // RCOSC Enent 
		{
				if(USBREG[ENUM_SPD]&SS_POLL_FAI)
					poll_fail_count++;
				else
				{
					USBREG[PHY_PLL_ST]=FW_RCOSC_SW_REQ;
					//return;
					goto EVENT_EXIT;
				}

				if(poll_fail_count >=3)
				{
					USBREG[PHY_PLL_ST]=FW_RCOSC_SW_REQ;
					//return;
					goto EVENT_EXIT;
				}

				USBREG[CONN_SPD]&=~(CR_CONNECT);
				USBREG[INT1_DET_EN0]=CR_EP2_ABFVLD_EN;
				USBREG[EPn_TRIG_CFG1 + EP1] = 0x00;
				USBREG[EPn_TRIG_CFG1 + EP2] = 0x00;
				USBREG[EPn_TRIG_CFG1 + EP3] = 0x00;
				USBREG[EPn_TRIG_CFG1 + EP4] = 0x00;

				
				USBREG[CONN_SPD]=CR_CONNECT;	
					
		}

			
		if(usb_interrupt_Event&ENUM_DONE)  // ENUM_DONE Enent 
		{
				USBREG[INT1_DET_EN0]=CR_EP2_ABFVLD_EN;
				USBREG[EPn_TRIG_CFG1 + EP1] = 0x00;
				USBREG[EPn_TRIG_CFG1 + EP2] = 0x00;
				USBREG[EPn_TRIG_CFG1 + EP3] = 0x00;
				USBREG[EPn_TRIG_CFG1 + EP4] = 0x00;
		}

EVENT_EXIT:

		usb_interrupt_Event = 0;

	}
	else
	{
#if 1
		if((usb_interrupt_level2_L&(HOT_RST<<8))||(usb_interrupt_level2_L&(WARM_BUS_RST<<8)))
		{
				USBREG[INT1_DET_EN0]=CR_EP2_ABFVLD_EN;
				USBREG[EPn_TRIG_CFG1 + EP1] = 0x00;
				USBREG[EPn_TRIG_CFG1 + EP2] = 0x00;
				USBREG[EPn_TRIG_CFG1 + EP3] = 0x00;
				USBREG[EPn_TRIG_CFG1 + EP4] = 0x00;
				USBREG[INT0_DET_EN2] =0x00;
				USBREG[INT0_DET_EN6] =0x00;
				
				if(usb_interrupt_level2_L&(HOT_RST<<8))
					USBREG[RST_ST]=WIRE_HOT_RST;

				goto Hander_EXIT;
		}
#endif
		 if (usb_interrupt_level2_L & EP0_SETUP)
		 	{
		 		Usb_ep0setup();
		 	}
	
Hander_EXIT:

		usb_interrupt_level2_H = 0;
		usb_interrupt_level2_L = 0;
	}

  EX0 = 1;
	
}




void UsbHandler_Bulk(void)
{
	if (usb_interrupt_level1 & BIT1)
	{
		Usb_CBW_Out();
		
	}
	
	usb_interrupt_level1 = 0;
	USBREG[INT1_DET_EN0]=CR_EP2_ABFVLD_EN; // Enable EP2 Interrupt 


}


void Init_PBA()
{
 	unsigned char byCeIdx,j;
  volatile unsigned char* pbyPBACEReg;

  // IOW F121 00, enable IP rst
  CLKREG[CLK_GATE] = 0x00;
  //IOW F124 01, enable IP rst
  CLKREG[CLK_GATE1] = 0x01;
  //IOW F12F 20, PLL EN
  CLKREG[CLK_WUCNT] = 0x20;
  //IOW F180 55, test wr
  DMAREG[DMA_SRHB_L] = 0x55;

  //IOW F000 80, PBA RST
  PBAREG[CRBYTE0] = 0x80;//0x80;

  for (byCeIdx = 0; byCeIdx <  8; byCeIdx++)
  {
      pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;
      pbyPBACEReg[CRBYTE0_CE] &= MASK_MODE;

      pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN0] = 0x55;
      pbyPBACEReg[SET_BSY8_5_BUSY_RDY_LUN0] = 0x55;

      for (j = 0x00; j < 0x0E; j++)
      {
          pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN1 + j] = 0x55;
      }


  }
 

}


