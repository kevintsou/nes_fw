
#include "nes.h"
#ifdef D_NES_CMODEL
#include "8051.h"
#else
#include <reg52.h>
#include <intrins.h>
#include <8051.h>
#endif

#include "Var_Def.h"
#include "MCU_Reg.h"
#include <UsbReg.h>
#include "USB_String.h"
#include "USB_API.h"
#include "usb_def.h"
#include "string.h"
#include "SYS_API.h"
#include "PH_AP_API.h"
#include "Interrupt_API.h"
#include "GPIO_API.h"
#include "BUF_Def.h"
#include "a.h"

unsigned char code InquiryData[] =  //head of inquiry data
{
    0x00,	// (Byte 0) bit7: Reserved, bit6~0: Peripheral Device Type
    0x80,	// (Byte 1) bit7: RMB, bit6~0: Reserved
    0x00,	// (Byte 2) ISO, ECMA, ANSI Version
    0x01,	// (Byte 3) bit7~4: ATAPI Version, bit3~0: Response Data Format
    0x1F,	// (Byte 4) Additional Length (n-4)
    0x00, 0x00, 0x00,	// (Byte 5 ~ 7) Reserved
};

unsigned char code Mode_Sense_PageCode01[] =
{
    0x00, 0x12, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x0a, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00,
};

unsigned char code Mode_Sense_PageCode1A[] =
{
    0x00, 0x12, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x1a, 0x0a, 0x00, 0x03
};

unsigned char code Mode_Sense_PageCode2A[] =
{
    0x00, 0x3E, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2A, 0x36, 0x05, 0x05, 0xF5, 0x73, 0x29, 0x23,
    0x1B, 0x80, 0x00, 0xFF, 0x08, 0x00, 0x05, 0x80,
    0x00, 0x10, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06
};

unsigned char code DefaultInfoBlk[] =
{//send from host
 //--- 0~17, device descriptor
 //---18~43, language id
 //---44~69, manufactory string
 //---70~95, product string
 //---96~121, serial string
 //---122~147, string, not used 
 0x00, //0, year
 0x00, //1, year
 0x00, //2, year
 0x00, //3, year
 0x00, //4, month
 0x00, //5, month   
 0x00, //6, day
 0x00, //7, day 
 'P','S','3','4','3','2',' ','N',//8~23, vendor inquiry product name (156)
 'A','N','D','_','E','m','u',' ',

 'p','h','i','s','o','n',' ',' ', //24~31, vendor inquiry manufacture name (176)
 '2','.','0','0', //32~35, vendor inquiry version (184)
 ' ',0x16,0x03,0x18,'C','H',' ',' ' //36~43 PMAP1234
};


unsigned char code FWDateIC[]=
{
//data for last modified date of F/W and the name of IC
    'V', //378
    'R', //379
    0xAC, 0x18,//380, 381, Firmware modify date, 2010/12/21
    0x23, 0x07 //382~383, PS2260
};

unsigned char code Mbr_Sign[] =
{
    'B', 't', 'P', 'r', 'a', 'm', 'C', 'd' // Boot PRAM Code
};

unsigned char code Mode_Sense_PageCode1DB9[10U] =  /* 10 bytes*/
{
	0x1dU, 0x08U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x08U, 0x00U, 0x08U
};

unsigned char code Mode_Sense_PageCode01B9[9U] =  /* 12 bytes*/
{
	0x01U, 0x0aU, 0x00U, 0xffU, 0x00U, 0x00U, 0x00U, 0x00U, 0xffU
};

unsigned char code Mode_Sense_PageCode05B9[16U] =  /* 16 bytes*/
{
	0x05U, 0x32U, 0x40U, 0x25U, 0x08U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
	0x00U, 0x00U, 0x00U, 0x10U, 0x00U, 0x96U
};


#if 0
void Usb_Bulk_In_512(UCHAR ucSecCount, WORD wByteCount)
{

    USB_DEBUG_MSG(0xAA,0x01);

    if ((ucSecCount==0)&&(wByteCount==0) ) return;

    wByteCount = wByteCount + ((WORD)ucSecCount<<9);


    USBREG[EPn_BUF_START0 + EP1] = 0;
    USBREG[EPn_T_BC0 + EP1] =  (wByteCount & 0xFF);
    USBREG[EPn_T_BC1 + EP1] =  ((wByteCount>>8)&0xFF);
    USBREG[EPn_T_BC2 + EP1] =  0x00;

    //USBREG[EPn_TRIG_CFG1+EP1] = EPn_TRIG_EN|0x08;

    //while(USBREG[EPn_TRIG_CFG1+EP1] & EPn_TRIG_EN){}


    USBREG[EPn_TRIG_CFG1+EP1] |= EPn_TRIG_EN;

    while (USBREG[EPn_TRIG_CFG1+EP1] & EPn_TRIG_EN) {}


    USB_DEBUG_MSG(0xAA,0x02);

    return;
}

#endif

void USB_Trig_Send(unsigned char Cnt)
{
    unsigned char i;

    while (!(USBREG[EPn_TLU_STATUS] & RAM_VALID))
    {
        if ((USBREG[TLU_ENCLR]&TLU_EPN_EN) == 0x00)
        {
            return;
        }
    };

    for (i=0; i<Cnt; i++)
        USBREG[EPn_SERIAL_D] = ShortPkt_Base[i];


    USBREG[EPn_T_BC0] = Cnt; /* length, low byte */
    USBREG[EPn_T_BC1] = 0;        /* length, high byte */
    USBREG[EPn_T_BC2] = 0;        /* length, high byte */

    USBREG[EPn_TRIG_CFG1] = INC_PTR; /* send data */


}



bit CheckWriteProtect()
{
	return 1; /* always set write protect */
}




void Inquiry()
{

	uint8 u8Cnt =0;
	UCHAR ucLen =(UCHAR)44;

	
	if (X_MASS_XFR_LEN.All<(WORD)44)
	{
		ucLen = X_MASS_XFR_LEN.BYTEMODE.BYTE1;
	}

	
	if(MASS_LUN >= (UCHAR)X_MaxCard)
	{
		ShortPkt_Base[0] = (UCHAR)0x1F;
	}
	else 
	{
		ShortPkt_Base[0] = (UCHAR)0x00; /* direct access */
	}

	ShortPkt_Base[0x01] = (UCHAR)0x80; /* RMB */

	memcpy((void*)(ShortPkt_Base+2), (void*)(InquiryData+2), 8-2);
	memcpy((void*)(ShortPkt_Base+8), (void*)(DefaultInfoBlk+24), 8);
	memcpy((void*)(ShortPkt_Base+16), (void*)(DefaultInfoBlk+8), 16);
	memcpy((void*)(ShortPkt_Base+32), (void*)(DefaultInfoBlk+32), 4+8);

	USB_Trig_Send(ucLen);
	Set_Media_OK();


}




void Test_Unit_Ready(void)
{
		
	Set_NO_Media();

}

void Prevent_Allow_Removal()
{
	if (MASS_LUN>=X_MaxCard)
	{
		Set_LUN_Error();
	}
	else if (X_CMDBLOCK[4]&0x01)
	{
		Invalid_Field_In_Command();
	}
	else
	{
		Set_Media_OK();
	}
	
	return;
}



void Read_Format_Capacity(void)
{

	UCHAR ucLen;

	if (X_MASS_XFR_LEN.All<12) 
		ucLen = X_MASS_XFR_LEN.BYTEMODE.BYTE1;
	else 
		ucLen = 12;

	if (MASS_LUN>=X_MaxCard)
	{
		Set_LUN_Error();
	}
	else //if ( (MediaStatus[SEL_MEDIA]&(NO_MEDIA_BIT | STOP_DEVICE_BIT | PASSWORD_LOCK_BIT | INIT_FAIL_BIT)) )
	{
		ShortPkt_Base[2] = ShortPkt_Base[1] = ShortPkt_Base[0] = 0x00; 
		ShortPkt_Base[3] = 0x08;
		ShortPkt_Base[7] = ShortPkt_Base[6] = ShortPkt_Base[5] = ShortPkt_Base[4] = 0xFF;
		ShortPkt_Base[8] = 0x03;
		ShortPkt_Base[9] = 0x00;
		ShortPkt_Base[10] = 0x02;
		ShortPkt_Base[11] = 0x00;
		FifoShortPkt(gcodeFormat_Capacity,ucLen);
		Set_Media_OK();
	}

	return;

}


void Request_Sense()
{
		UCHAR ucLen;
		
		if (X_MASS_XFR_LEN.All>18) 
			ucLen = 18;  // we may need to stall this condition after short packet transfer
		else 
			ucLen = X_MASS_XFR_LEN.BYTEMODE.BYTE1;

		memset(ShortPkt_Base,0x00,18);
		ShortPkt_Base[0] = 0x70;
		ShortPkt_Base[2] = (MASS_LUN>=X_MaxCard) ? 0x05 : X_SenseKey[SEL_MEDIA];
		ShortPkt_Base[7] = 0x0A;
		ShortPkt_Base[12] = (MASS_LUN>=X_MaxCard) ? 0x025 : X_bASC[SEL_MEDIA];
		ShortPkt_Base[13] = (MASS_LUN>=X_MaxCard) ? 0x020 : X_bASCQ[SEL_MEDIA];
 
		if (ucLen)
    {
        FifoShortPkt(ShortPkt_Base,ucLen);
    }
    Set_Media_OK();
    
}


void Mode_Sense6(void)
{
	unsigned char ucLen, ucTmp;

	ShortPkt_Base[0] = 0x03;
	ShortPkt_Base[1] = 0x00;
	ShortPkt_Base[2] = CheckWriteProtect() ? 0x80 :0x00;
	ShortPkt_Base[3] = 0x00;

	ucLen = 4;
	ucTmp = X_CMDBLOCK[2]&0x3f;
	if ( ((ucTmp)==0x05) || ((ucTmp)==0x3f) )
	{
		ucLen = 4+32;
		ShortPkt_Base[0] = 3+32;
		ShortPkt_Base[4] = 0x05;
		ShortPkt_Base[5] = 0x1E;
		ShortPkt_Base[6] = 0xf0;
		ShortPkt_Base[7] = 0x00;
		ShortPkt_Base[8] = 0;//X_maxhd_tk[SEL_MEDIA];  // X_maxsec_hd[SEL_MEDIA];
		ShortPkt_Base[9] = 0;//X_maxsec_hd[SEL_MEDIA]; // X_maxhd_tk[SEL_MEDIA];
		ShortPkt_Base[10] = 0x02;
		ShortPkt_Base[11] = 0x00;
		ShortPkt_Base[12] = 0;//X_maxcyl[SEL_MEDIA]>>8;
		ShortPkt_Base[13] = 0;//X_maxcyl[SEL_MEDIA];
		for (ucTmp=0; ucTmp<22; ucTmp++) ShortPkt_Base[14+ucTmp] = 0x00;
	}
	if (ucLen>X_MASS_XFR_LEN.All) ucLen = X_MASS_XFR_LEN.BYTEMODE.BYTE1;

	FifoShortPkt(ShortPkt_Base,ucLen);
	X_CSWStatus = 0x00;

	return;
}



void Mode_Sense()
{
	unsigned char ucLen, ucTmp;

	ShortPkt_Base[0] = 0x00;
	ShortPkt_Base[1] = 0x06;
	ShortPkt_Base[2] = 0x00;
	ShortPkt_Base[3] = CheckWriteProtect() ? 0x80 :0x00;
	ShortPkt_Base[7] = ShortPkt_Base[6] = ShortPkt_Base[5] = ShortPkt_Base[4] = 0x00;
	ucLen = 8;
	ucTmp = X_CMDBLOCK[2] & 0x3f;

	if ( ((ucTmp)==0x05) || ((ucTmp)==0x3f) )
	{
		ucLen = 8+32;
		ShortPkt_Base[1] = 6+32;
		ShortPkt_Base[8] = 0x05;
		ShortPkt_Base[9] = 0x1E;
		ShortPkt_Base[10] = 0xf0;
		ShortPkt_Base[11] = 0x00;
		ShortPkt_Base[12] = 0;//X_maxhd_tk[SEL_MEDIA];  // X_maxsec_hd[SEL_MEDIA];
		ShortPkt_Base[13] = 0;//X_maxsec_hd[SEL_MEDIA]; // X_maxhd_tk[SEL_MEDIA];
		ShortPkt_Base[14] = 0x02;
		ShortPkt_Base[15] = 0x00;
		ShortPkt_Base[16] = 0;//X_maxcyl[SEL_MEDIA]>>8;
		ShortPkt_Base[17] = 0;//X_maxcyl[SEL_MEDIA];
		for (ucTmp=0; ucTmp<22; ucTmp++) ShortPkt_Base[18+ucTmp] = 0x00;
	}
	else if(ucTmp == 0x01)  
	{
		memcpy(ShortPkt_Base, Mode_Sense_PageCode01, 16);
		if( X_CMDBLOCK[2] & 0x40 )
		ShortPkt_Base[11] = 0xff;
		ucLen = 16;
	}
	else if(ucTmp == 0x1a)  
	{
		memcpy(ShortPkt_Base, Mode_Sense_PageCode1A, 12);
		ucLen = 12;
	}
	else if(ucTmp == 0x2a)  
	{
		memcpy(ShortPkt_Base, Mode_Sense_PageCode2A, 40);
		ucLen = 40;
	}

	if (ucLen>X_MASS_XFR_LEN.All) ucLen = X_MASS_XFR_LEN.BYTEMODE.BYTE1;
	
	FifoShortPkt(ShortPkt_Base,ucLen);
	X_CSWStatus = 0x00;

	return;
}
