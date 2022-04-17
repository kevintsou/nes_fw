#include "nes.h"
#ifdef D_NES_CMODEL
#include "8051.h"
#else
#include <reg52.h>
#include <8051.h>
#endif

#include <string.h>
#include <buf_def.h>
#include "Var_Def.h"
#include "MCU_Reg.h"
#include "USBReg.h"
#include "SPI_Flash_Reg.h"
#include "USB_API.h"
#include "GPIO_Reg.h"
#include "IO_Macro.h"
#include "USB_Api.h"
#include "PRAM_Def.h"
#include "a.h"
//==================================================================================================================

BYTE xdata gb_Manufacturer_ID;
bit gi_SPIFlash_IDRead = 0;
bit gi_SPIFlash_Initialed = 0;
//==================================================================================================================

//==================================================================================================================
//  FUNCTION DESCRIPTION:
//      Initial SPI flash.
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
void SPIFlash_Initial()
{
    //Check High Speed
    if (IO_SPI_SPD)
    {
        SPIREG[SPI_CFG] = 0x08;    //High Speed
    }
    else
    {
        SPIREG[SPI_CFG] = 0x00;    //Low  Speed
    }


    //Check CLK Idel State
    if (IO_SPI_IDEL_STATE == 0)
    {
        SPIREG[SPI_CFG] |= SET_BIT2;    //CLK Default High
    }
    else
    {
        SPIREG[SPI_CFG] &= ~(SET_BIT2);    //CLK Default Low
    }

    CLKREG[CLK_SPI] = 0x04;  //125 / 6


		//here for transfer 16-kByte data @ 30M clk, the spent time = 4.26 ms
		//Set Driven
		SYSREG[SYS_SPI_CFG] = (SPI_DRN_E8 + SPI_CONOF); //SPI driven

		gi_SPIFlash_Initialed = 1;

		SPIREG[SPI_MST_TRIG] |= 0x80;
}

//==================================================================================================================
//  FUNCTION DESCRIPTION:
//      Read SPI flash status register.
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
BYTE SPIFlash_RDSR()
{
    SPIREG[SPI_CMD_DATA] = 0x05;            // [0xf410] SPI Command Data Register
    SPIREG[SPI_CMD_BCNT] = 1 - 1;           // [0xf40e] SPI Command Byte Count Register
    SPIREG[SPI_RSP_BCNT] = 1 - 1;           // [0xf40f] SPI Response Byte Count Register

    SPIREG[SPI_CS_N_CTRL] = 0x01;           // [0xf402] SPI_CS_N output enable, output value = 0

    SPIREG[SPI_MST_TRIG] = BIT3;            // [0xf401] SPI Master Trigger Register, trigger spi command

    while (SPIREG[SPI_MST_TRIG] & BIT3 );   // [0xf401] polling spi command finish

    SPIREG[SPI_MST_TRIG] = BIT4;            // [0xf401] SPI Master Trigger Register, trigger spi response

    while ( SPIREG[SPI_MST_TRIG] & BIT4 );  // [0xf401] polling spi response finish

    SPIREG[SPI_CS_N_CTRL] = 0x03;           // [0xf402] SPI_CS_N output enable, output value = 1

    return SPIREG[SPI_RSP_DATA];            // [0xf418] SPI Response Data Register
}

//==================================================================================================================
//  FUNCTION DESCRIPTION:
//      SPI flash set write enable.
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
void SPIFlash_WREN()
{
    SPIREG[SPI_CMD_BCNT] = 1 - 1;
    SPIREG[SPI_CS_N_CTRL] = 1;  //0xF402, SPI CS_N Control, ouput value=0, enable
    SPIREG[SPI_CMD_DATA] = 0x06;
    SPIREG[SPI_MST_TRIG] = BIT3;        //0xF401, trigger spi command

    while (SPIREG[SPI_MST_TRIG] & BIT3); //wait finish

    SPIREG[SPI_CS_N_CTRL] = 0x03;   //0xF402, SPI CS_N Control, ouput value=0, enable
}

//==================================================================================================================
//  FUNCTION DESCRIPTION:
//      SPI flash set write disable.
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
void SPIFlash_WRDI()
{
    SPIREG[SPI_CMD_BCNT] = 1 - 1;
    SPIREG[SPI_CS_N_CTRL] = 1;  //0xF402, SPI CS_N Control, ouput value=0, enable
    SPIREG[SPI_CMD_DATA] = 0x04;
    SPIREG[SPI_MST_TRIG] = BIT3;        //0xF401, trigger spi command

    while (SPIREG[SPI_MST_TRIG] & BIT3); //wait finish

    SPIREG[SPI_CS_N_CTRL] = 0x03;   //0xF402, SPI CS_N Control, ouput value=0, enable
}

/*
//==================================================================================================================
//  FUNCTION DESCRIPTION:
//      SPI flash set write status register.
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
void SPIFlash_WRSR(BYTE status)
{
    SPIREG[SPI_CMD_BCNT]=2-1;
    SPIREG[SPI_CS_N_CTRL] = 1;  //0xF402, SPI CS_N Control, ouput value=0, enable
    SPIREG[SPI_CMD_DATA]=0x01;
    SPIREG[SPI_CMD_DATA+1]=status;
    SPIREG[SPI_MST_TRIG] = BIT3;        //0xF401, trigger spi command
    while(SPIREG[SPI_MST_TRIG] & BIT3); //wait finish
    SPIREG[SPI_CS_N_CTRL] = 0x03;   //0xF402, SPI CS_N Control, ouput value=0, enable
    //while(SPIFlash_RDSR()&0x01) ;
}*/

//==================================================================================================================
//  FUNCTION DESCRIPTION:
//      SPI flash earse all memory.
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
void SPIFlash_Chip_Erase()
{
    SPIREG[SPI_MST_TRIG] |= 0x80;

    SPIFlash_WREN();

    /*if(Manufacturer_ID==0xBF) //EWSR
    {
        SPIREG[SPI_CMD_BCNT]=1-1;
        SPIREG[SPI_CS_N_CTRL] = 1;  //0xF402, SPI CS_N Control, ouput value=0, enable
        SPIREG[SPI_CMD_DATA]=0x50;
        SPIREG[SPI_MST_TRIG] = BIT3;        //0xF401, trigger spi command
        while(SPIREG[SPI_MST_TRIG] & BIT3); //wait finish
        SPIREG[SPI_CS_N_CTRL] = 0x03;   //0xF402, SPI CS_N Control, ouput value=0, enable
    }

    SPIFlash_WRSR(0x00);*/

    SPIREG[SPI_CS_N_CTRL] = 1;  //0xF402, SPI CS_N Control, ouput value=0, enable
    SPIREG[SPI_CMD_DATA] = 0xC7;        //0xF410, Instruction
    SPIREG[SPI_CMD_BCNT] = (1 - 1); //0xF40F, SPI Command Byte Count
    //Command Trigger
    SPIREG[SPI_MST_TRIG] = BIT3;    //0xF401, trigger spi command

    while (SPIREG[SPI_MST_TRIG] & BIT3); //wait finish

    SPIREG[SPI_CS_N_CTRL] = 0x03; //0xF402, SPI CS_N Control, ouput value=0, disable

    while (SPIFlash_RDSR()&BIT0);

    SPIFlash_WRDI();
}

//==================================================================================================================
//  FUNCTION DESCRIPTION:
//      SPI flash earse single sector(4K).
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
void SPIFlash_Sector_Erase()
{
    SPIREG[SPI_MST_TRIG] |= 0x80;

    SPIFlash_WREN();

    SPIREG[SPI_CS_N_CTRL] = 1;  //0xF402, SPI CS_N Control, ouput value=0, enable
    SPIREG[SPI_CMD_DATA] = 0x20;        //0xF410, Instruction
    SPIREG[SPI_CMD_DATA + 1] = X_CMDBLOCK[3];    //0xF411, Address H
    SPIREG[SPI_CMD_DATA + 2] = X_CMDBLOCK[4];    //0xF412, Address M
    SPIREG[SPI_CMD_DATA + 3] = X_CMDBLOCK[5];    //0xF413, Address L

    SPIREG[SPI_CMD_BCNT] = (4 - 1); //0xF40F, SPI Command Byte Count
    //Command Trigger
    SPIREG[SPI_MST_TRIG] = BIT3;    //0xF401, trigger spi command

    while (SPIREG[SPI_MST_TRIG] & BIT3); //wait finish

    SPIREG[SPI_CS_N_CTRL] = 0x03; //0xF402, SPI CS_N Control, ouput value=0, disable

    while (SPIFlash_RDSR() & BIT0);

    SPIFlash_WRDI();
}

//==================================================================================================================
//  FUNCTION DESCRIPTION:
//      SPI flash read status.
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
void SPIFlash_ReadStatus()
{
    BYTE *rb_ptr;

    SPIREG[SPI_MST_TRIG] |= 0x80;
    rb_ptr = 0x20000;
    *rb_ptr = SPIFlash_RDSR();

    USBBulkInSectorFromPageBuffer(0x80, 1, 0);
}

//==================================================================================================================
//  FUNCTION DESCRIPTION:
//      SPI flash read device ID.
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
void SPIFlash_ReadID()
{
    SPIREG[SPI_CMD_DATA] = 0x90;  // [0xf410] SPI Command Data Register
    SPIREG[SPI_CMD_DATA + 1] = 0x00;  // [0xf410] SPI Command Data Register
    SPIREG[SPI_CMD_DATA + 2] = 0x00;  // [0xf410] SPI Command Data Register
    SPIREG[SPI_CMD_DATA + 3] = 0x00;  // [0xf410] SPI Command Data Register
    SPIREG[SPI_CMD_BCNT] = 4 - 1; // [0xf40e] SPI Command Byte Count Register
    SPIREG[SPI_RSP_BCNT] = 2 - 1; // [0xf40f] SPI Response Byte Count Register

    SPIREG[SPI_CS_N_CTRL] = 0x01;  // [0xf402] SPI_CS_N output enable, output value = 0

    SPIREG[SPI_MST_TRIG] = BIT3;  // [0xf401] SPI Master Trigger Register, trigger spi command

    while ( SPIREG[SPI_MST_TRIG] & BIT3 ); // [0xf401] polling spi command finish

    SPIREG[SPI_MST_TRIG] = BIT4;  // [0xf401] SPI Master Trigger Register, trigger spi response

    while ( SPIREG[SPI_MST_TRIG] & BIT4 ); // [0xf401] polling spi response finish

    SPIREG[SPI_CS_N_CTRL] = 0x03;  // [0xf402] SPI_CS_N output enable, output value = 1

    gb_Manufacturer_ID = SPIREG[SPI_RSP_DATA];  // [0xf418] SPI Response Data Register

    if (gb_Manufacturer_ID != 0xBF)
    {
        SPIREG[SPI_CMD_DATA] = 0x9F;  // [0xf410] SPI Command Data Register
        SPIREG[SPI_CMD_BCNT] = 1 - 1; // [0xf40e] SPI Command Byte Count Register
        SPIREG[SPI_RSP_BCNT] = 1 - 1; // [0xf40f] SPI Response Byte Count Register

        SPIREG[SPI_CS_N_CTRL] = 0x01;  // [0xf402] SPI_CS_N output enable, output value = 0

        SPIREG[SPI_MST_TRIG] = BIT3;  // [0xf401] SPI Master Trigger Register, trigger spi command

        while (SPIREG[SPI_MST_TRIG] & BIT3 ); // [0xf401] polling spi command finish

        SPIREG[SPI_MST_TRIG] = BIT4;  // [0xf401] SPI Master Trigger Register, trigger spi response

        while (SPIREG[SPI_MST_TRIG] & BIT4 ); // [0xf401] polling spi response finish

        SPIREG[SPI_CS_N_CTRL] = 0x03;  // [0xf402] SPI_CS_N output enable, output value = 1

        gb_Manufacturer_ID = SPIREG[SPI_RSP_DATA];  // [0xf418] SPI Response Data Register
    }

    gi_SPIFlash_IDRead = 1;
}


void SPIFlash_Check()
{
		BYTE rb_Loop,total_loop;
		TDWORD rl_LBA_Offset;
		unsigned int Start_Offest,x;
		unsigned long checksum_data;
		

		SPIREG[SPI_MST_TRIG] |= 0x80;


		SPIREG[SPI_DMA_RAM_ADDR_L] = 0x00;  //0xF428
		SPIREG[SPI_DMA_RAM_ADDR_M] = 0x00;  //0xF429
		SPIREG[SPI_DMA_RAM_ADDR_H] = 0x01;  //0xF42A, Address Mode, movx 0x0000

    //0x00:Write, 0x01:Read
    SPIREG[SPI_DMA_CTRL]    = 0x01; //0xF403, DMA Read/Write

    //Byte count=0x010000 -1=0x00ffff(64KB)
    SPIREG[SPI_DMA_BCNT_L] = 0xff;  //0xF420
    SPIREG[SPI_DMA_BCNT_M] = 0x01;  //0xF421
    SPIREG[SPI_DMA_BCNT_H] = 0x00;  //0xF422

    //Cache count = 256-1= 0xff
    SPIREG[SPI_DMA_CCNT_L] = 0xff;  //0xF424
    SPIREG[SPI_DMA_CCNT_M] = 0x00;  //0xF425
    SPIREG[SPI_DMA_CCNT_H] = 0x00;  //0xF426
    //SPIREG[SPI_CMD_DATA]=0x03;    //0xF410, Instruction

		checksum_data=0;
		
		for(total_loop=0 ; total_loop<96 ; total_loop++)
		{
			Start_Offest = 512*total_loop;
			
	    rl_LBA_Offset.BYTEMODE.LL = Start_Offest;
	    rl_LBA_Offset.BYTEMODE.LH = Start_Offest>>8;
	    rl_LBA_Offset.BYTEMODE.HL = 0x00;
	    rl_LBA_Offset.BYTEMODE.HH = 0x00;

	   
		  //Shift LBA
		  rl_LBA_Offset.DWord += SHIFT_ADDR;

	    SPIREG[SPI_CMD_DATA + 1] = rl_LBA_Offset.BYTEMODE.HL;   //0xF411, Address H
	    SPIREG[SPI_CMD_DATA + 2] = rl_LBA_Offset.BYTEMODE.LH;   //0xF412, Address M
	    SPIREG[SPI_CMD_DATA + 3] = rl_LBA_Offset.BYTEMODE.LL;   //0xF413, Address L

	   
	    for (rb_Loop = 0; rb_Loop < 2; rb_Loop++)
	    {
	        SPIREG[SPI_CS_N_CTRL] = 1;  //0xF402, SPI CS_N Control, ouput value=0, enable
	        SPIREG[SPI_CMD_DATA] = 0x0B;    //0xF410, Instruction

	        //Add LBA
	        rl_LBA_Offset.DWord += (0x100 * rb_Loop);
	        SPIREG[SPI_CMD_DATA + 1] = rl_LBA_Offset.BYTEMODE.HL;   //0xF411, Address H
	        SPIREG[SPI_CMD_DATA + 2] = rl_LBA_Offset.BYTEMODE.LH;   //0xF412, Address M
	        SPIREG[SPI_CMD_DATA + 3] = rl_LBA_Offset.BYTEMODE.LL;   //0xF413, Address L

	        SPIREG[SPI_CMD_BCNT] = (4); //0xF40F, SPI Command Byte Count

	        //Command Trigger
	        SPIREG[SPI_MST_TRIG] = 0x08;    //0xF401, trigger spi command
	        while (SPIREG[SPI_MST_TRIG] & BIT3); //wait finish

	        //Data Trigger
	        SPIREG[SPI_MST_TRIG] |= BIT0;   //0xF401, trigger spi dma_di bus
	        while (SPIREG[SPI_MST_TRIG] & BIT0); //wait finish
	        
	        SPIREG[SPI_CS_N_CTRL] = 0x03;   //0xF402, SPI CS_N Control, ouput value=0, enable

	        //Status
	        SPIFlash_RDSR();
	    }

			if(total_loop == 8)
				FAT_MBR_Base[118]=0x20; // Tester ID;

			if(total_loop < 95) // checkSum 512*95 , l-ast 512 byte no CheckSum
			{
				for(x=0 ; x<512 ; x++)
				{
					checksum_data+=FAT_MBR_Base[x];
				}
			}
		}

		checksum_data&=0xFFFFFFFF;


		memset(TEMP_BUF,0x00,512);
		TEMP_BUF[0]=checksum_data>>24;
		TEMP_BUF[1]=checksum_data>>16;
		TEMP_BUF[2]=checksum_data>>8;
		TEMP_BUF[3]=checksum_data;

		TEMP_BUF[0x10]=FAT_MBR_Base[506];
		TEMP_BUF[0x11]=FAT_MBR_Base[507];
		TEMP_BUF[0x12]=FAT_MBR_Base[508];
		TEMP_BUF[0x13]=FAT_MBR_Base[509];
				
		USBBulkInSectorFromPageBuffer(BUF_NUM_TEMP_BUF, 0x01, 0);
		
	 
}




//==================================================================================================================
//  FUNCTION DESCRIPTION:
//      SPI flash read memory, R/W addr base on X_CMDBLOCK(USB CBW).
//
//  INPUT:
//      ab_PRAM: Data will read to PRAM, 1: yes, 0: no
//      ab_Shift: Shift data fro USB
//      ab_USB: Send back to USB
//
//  OUTPUT:
//      None
//
//MODIFIED GLOBAL VARIABLE:
//      None
//
//==================================================================================================================
void SPIFlash_Read(BYTE ab_PRAM, BYTE ab_Shift, BYTE ab_USB)
{
    BYTE rb_Loop;
    TDWORD rl_LBA_Offset;

    SPIREG[SPI_MST_TRIG] |= 0x80;

    //RAM address=0x000000, Physical
    if (ab_PRAM)
    {
        SPIREG[SPI_DMA_RAM_ADDR_L] = X_CMDBLOCK[8];  //0xF428
        SPIREG[SPI_DMA_RAM_ADDR_M] = X_CMDBLOCK[7];  //0xF429
        SPIREG[SPI_DMA_RAM_ADDR_H] = X_CMDBLOCK[6];  //0xF42A, Address Mode
    }
    else
    {
        SPIREG[SPI_DMA_RAM_ADDR_L] = 0x00;  //0xF428
        SPIREG[SPI_DMA_RAM_ADDR_M] = 0x00;  //0xF429
        SPIREG[SPI_DMA_RAM_ADDR_H] = 0x01;  //0xF42A, Address Mode, movx 0x0000
    }

    //0x00:Write, 0x01:Read
    SPIREG[SPI_DMA_CTRL]    = 0x01; //0xF403, DMA Read/Write

    //Byte count=0x010000 -1=0x00ffff(64KB)
    SPIREG[SPI_DMA_BCNT_L] = 0xff;  //0xF420
    SPIREG[SPI_DMA_BCNT_M] = 0x01;  //0xF421
    SPIREG[SPI_DMA_BCNT_H] = 0x00;  //0xF422

    //Cache count = 256-1= 0xff
    SPIREG[SPI_DMA_CCNT_L] = 0xff;  //0xF424
    SPIREG[SPI_DMA_CCNT_M] = 0x00;  //0xF425
    SPIREG[SPI_DMA_CCNT_H] = 0x00;  //0xF426
    //SPIREG[SPI_CMD_DATA]=0x03;    //0xF410, Instruction

    rl_LBA_Offset.BYTEMODE.LL = X_CMDBLOCK[5];
    rl_LBA_Offset.BYTEMODE.LH = X_CMDBLOCK[4];
    rl_LBA_Offset.BYTEMODE.HL = X_CMDBLOCK[3];
    rl_LBA_Offset.BYTEMODE.HH = 0x00;

    if (ab_Shift)
    {
        /*rl_LBA_Offset.BYTEMODE.LL = X_CMDBLOCK[5];
        rl_LBA_Offset.BYTEMODE.LH = X_CMDBLOCK[4];
        rl_LBA_Offset.BYTEMODE.HL = X_CMDBLOCK[3];
        rl_LBA_Offset.BYTEMODE.HH = 0x00;*/

        //Shift LBA
        rl_LBA_Offset.DWord += SHIFT_ADDR;

        /*X_CMDBLOCK[5] = rl_LBA_Offset.BYTEMODE.LL;
        X_CMDBLOCK[4] = rl_LBA_Offset.BYTEMODE.LH;
        X_CMDBLOCK[3] = rl_LBA_Offset.BYTEMODE.HL;*/
    }

    /*SPIREG[SPI_CMD_DATA + 1] = X_CMDBLOCK[3];  //0xF411, Address H
    SPIREG[SPI_CMD_DATA + 2] = X_CMDBLOCK[4];    //0xF412, Address M
    SPIREG[SPI_CMD_DATA + 3] = X_CMDBLOCK[5];    //0xF413, Address L*/
    SPIREG[SPI_CMD_DATA + 1] = rl_LBA_Offset.BYTEMODE.HL;   //0xF411, Address H
    SPIREG[SPI_CMD_DATA + 2] = rl_LBA_Offset.BYTEMODE.LH;   //0xF412, Address M
    SPIREG[SPI_CMD_DATA + 3] = rl_LBA_Offset.BYTEMODE.LL;   //0xF413, Address L

    //SPIREG[SPI_CMD_DATA + 1] = 0x00;  //0xF411, Address   H
    //SPIREG[SPI_CMD_DATA + 2] = X_CMDBLOCK[3];  //0xF412, Address   M
    //SPIREG[SPI_CMD_DATA + 3] = X_CMDBLOCK[4];  //0xF413, Address   L

    for (rb_Loop = 0; rb_Loop < 2; rb_Loop++)
    {
        SPIREG[SPI_CS_N_CTRL] = 1;  //0xF402, SPI CS_N Control, ouput value=0, enable

        //SPIREG[SPI_CMD_DATA]=0x03;        //0xF410, Instruction
        SPIREG[SPI_CMD_DATA] = 0x0B;    //0xF410, Instruction
        //SPIREG[SPI_CMD_DATA + 2] =X_CMDBLOCK[4] + rb_Loop; //0xF411, Address

        //Add LBA
        rl_LBA_Offset.DWord += (0x100 * rb_Loop);
        SPIREG[SPI_CMD_DATA + 1] = rl_LBA_Offset.BYTEMODE.HL;   //0xF411, Address H
        SPIREG[SPI_CMD_DATA + 2] = rl_LBA_Offset.BYTEMODE.LH;   //0xF412, Address M
        SPIREG[SPI_CMD_DATA + 3] = rl_LBA_Offset.BYTEMODE.LL;   //0xF413, Address L

        //SPIREG[SPI_CMD_BCNT] = (4-1); //0xF40F, SPI Command Byte Count
        SPIREG[SPI_CMD_BCNT] = (4); //0xF40F, SPI Command Byte Count

        //Command Trigger
        SPIREG[SPI_MST_TRIG] = 0x08;    //0xF401, trigger spi command

        while (SPIREG[SPI_MST_TRIG] & BIT3); //wait finish

        //Data Trigger
        SPIREG[SPI_MST_TRIG] |= BIT0;   //0xF401, trigger spi dma_di bus

        //SPIREG[SPI_DMA_RAM_ADDR_M] = j;   //0xF429
        while (SPIREG[SPI_MST_TRIG] & BIT0); //wait finish

        SPIREG[SPI_CS_N_CTRL] = 0x03;   //0xF402, SPI CS_N Control, ouput value=0, enable

        //Status
        SPIFlash_RDSR();
    }

    if (!ab_PRAM && ab_USB)
    {
        USBBulkInSectorFromPageBuffer(0x80, 1, 0);
    }
}

//==================================================================================================================
//  FUNCTION DESCRIPTION:
//      Program data to SPI
//
//  INPUT:
//      ab_Shift: Shift data fro USB
//
//  OUTPUT:
//      None
//
//MODIFIED GLOBAL VARIABLE:
//      None
//
//==================================================================================================================
void SPIFLASH_PROGRAM(BYTE ab_Shift)
{
    SWORD rb_Loop;
    BYTE *rb_ptr = 0x24000;
    TDWORD rl_LBA_Offset;

    SPIREG[SPI_MST_TRIG] |= 0x80;
    //RAM address=0x0C0000, Physical
    SPIREG[SPI_DMA_RAM_ADDR_L] = 0x00;  //0xF428
    SPIREG[SPI_DMA_RAM_ADDR_M] = 0x00;    //0xF429
    SPIREG[SPI_DMA_RAM_ADDR_H] = 0x01;  //0xF42A, Address Mode

    //0x00:Write, 0x01:Read
    SPIREG[SPI_DMA_CTRL]    = 0x00; //0xF403, DMA Read/Write

    //Byte count=0x0001FF -1=0x00ffff(64KB)
    SPIREG[SPI_DMA_BCNT_L] = 0xff;  //0xF420
    SPIREG[SPI_DMA_BCNT_M] = 0x01;  //0xF421
    SPIREG[SPI_DMA_BCNT_H] = 0x00;  //0xF422

    //Cache count = 256-1= 0xff
    SPIREG[SPI_DMA_CCNT_L] = 0xff;  //0xF424
    SPIREG[SPI_DMA_CCNT_M] = 0x00;  //0xF425
    SPIREG[SPI_DMA_CCNT_H] = 0x00;  //0xF426

    //SPIREG[SPI_CMD_DATA]=0x02;    //0xF410, Instruction

    rl_LBA_Offset.BYTEMODE.LL = X_CMDBLOCK[5];
    rl_LBA_Offset.BYTEMODE.LH = X_CMDBLOCK[4];
    rl_LBA_Offset.BYTEMODE.HL = X_CMDBLOCK[3];
    rl_LBA_Offset.BYTEMODE.HH = 0x00;

    if (ab_Shift)
    {
        /*rl_LBA_Offset.BYTEMODE.LL = X_CMDBLOCK[5];
        rl_LBA_Offset.BYTEMODE.LH = X_CMDBLOCK[4];
        rl_LBA_Offset.BYTEMODE.HL = X_CMDBLOCK[3];
        rl_LBA_Offset.BYTEMODE.HH = 0x00;*/

        //Shift LBA
        rl_LBA_Offset.DWord += SHIFT_ADDR;

        /*X_CMDBLOCK[5] = rl_LBA_Offset.BYTEMODE.LL;
        X_CMDBLOCK[4] = rl_LBA_Offset.BYTEMODE.LH;
        X_CMDBLOCK[3] = rl_LBA_Offset.BYTEMODE.HL;*/
    }

    /*SPIREG[SPI_CMD_DATA + 1] = X_CMDBLOCK[3];  //0xF411, Address H
    SPIREG[SPI_CMD_DATA + 2] = X_CMDBLOCK[4];    //0xF412, Address M
    SPIREG[SPI_CMD_DATA + 3] = X_CMDBLOCK[5];    //0xF413, Address L*/

    SPIREG[SPI_CMD_DATA + 1] = rl_LBA_Offset.BYTEMODE.HL;   //0xF411, Address H
    SPIREG[SPI_CMD_DATA + 2] = rl_LBA_Offset.BYTEMODE.LH;   //0xF412, Address M
    SPIREG[SPI_CMD_DATA + 3] = rl_LBA_Offset.BYTEMODE.LL;   //0xF413, Address L

    //SPIREG[SPI_CMD_DATA + 1]=0x00;    //0xF411, Address   H
    //SPIREG[SPI_CMD_DATA + 2]=X_CMDBLOCK[3];    //0xF412, Address   M
    //SPIREG[SPI_CMD_DATA + 3]=X_CMDBLOCK[4];    //0xF413, Address   L

    //SPIFlash_WREN();
    /*if(Manufacturer_ID==0xBF) //EWSR
    {
        SPIREG[SPI_CMD_BCNT]=1-1;
        SPIREG[SPI_CS_N_CTRL] = 1;  //0xF402, SPI CS_N Control, ouput value=0, enable
        SPIREG[SPI_CMD_DATA]=0x50;
        SPIREG[SPI_MST_TRIG] = BIT3;        //0xF401, trigger spi command
        while(SPIREG[SPI_MST_TRIG] & BIT3); //wait finish
        SPIREG[SPI_CS_N_CTRL] = 0x03;   //0xF402, SPI CS_N Control, ouput value=0, enable
    }
    SPIFlash_WRSR(0x00);*/
    //0xAF-Addr2-Addr1-Addr0-Data0-0xAF-Data1-0xAF-Data2-......-Data511
    if (gb_Manufacturer_ID == 0xBF)
    {

        SPIFlash_WREN();
        SPIREG[SPI_CMD_DATA] = 0xAF;        //0xF410, Instruction
        /*SPIREG[SPI_CMD_DATA + 1] = X_CMDBLOCK[3];  //0xF411, Address H
        SPIREG[SPI_CMD_DATA + 2] = X_CMDBLOCK[4];    //0xF412, Address M
        SPIREG[SPI_CMD_DATA + 3] = X_CMDBLOCK[5];    //0xF413, Address L*/

        SPIREG[SPI_CMD_DATA + 1] = rl_LBA_Offset.BYTEMODE.HL;   //0xF411, Address H
        SPIREG[SPI_CMD_DATA + 2] = rl_LBA_Offset.BYTEMODE.LH;   //0xF412, Address M
        SPIREG[SPI_CMD_DATA + 3] = rl_LBA_Offset.BYTEMODE.LL;   //0xF413, Address L

        SPIREG[SPI_CMD_DATA + 4] = *rb_ptr++;   //0xF413, Address
        SPIREG[SPI_CS_N_CTRL] = 1;  //0xF402, SPI CS_N Control, ouput value=0, enable
        SPIREG[SPI_CMD_BCNT] = (5 - 1); //0xF40F, SPI Command Byte Count
        SPIREG[SPI_MST_TRIG] = BIT3;    //0xF401, trigger spi command

        while (SPIREG[SPI_MST_TRIG] & BIT3); //wait finish

        while (SPIFlash_RDSR() & BIT0);

        for (rb_Loop = 0; rb_Loop < 511; rb_Loop++)
        {
            SPIREG[SPI_CMD_DATA] = 0xAF;        //0xF410, Instruction
            SPIREG[SPI_CMD_DATA + 1] = *rb_ptr++;   //0xF411, Address
            SPIREG[SPI_CMD_BCNT] = (2 - 1); //0xF40F, SPI Command Byte Count
            SPIREG[SPI_MST_TRIG] = BIT3;    //0xF401, trigger spi command

            while (SPIREG[SPI_MST_TRIG] & BIT3); //wait finish

            while (SPIFlash_RDSR() & BIT0);
        }

        SPIREG[SPI_CS_N_CTRL] = 0x03; //0xF402, SPI CS_N Control, ouput value=0, disable
        SPIFlash_WRDI();
    }
    else
    {
        for (rb_Loop = 0; rb_Loop < 2; rb_Loop++)
        {
            do
            {
                SPIFlash_WREN();
            }
            while (!SPIFlash_RDSR() & BIT1);

            SPIREG[SPI_CS_N_CTRL] = 1;  //0xF402, SPI CS_N Control, ouput value=0, enable
            SPIREG[SPI_CMD_DATA] = 0x02;        //0xF410, Instruction
            //SPIREG[SPI_CMD_DATA + 2] +=j; //0xF411, Address
            //SPIREG[SPI_CMD_DATA + 2] = X_CMDBLOCK[4] + rb_Loop;    //0xF411, Address

            //Add LBA
            rl_LBA_Offset.DWord += (0x100 * rb_Loop);
            SPIREG[SPI_CMD_DATA + 1] = rl_LBA_Offset.BYTEMODE.HL;   //0xF411, Address H
            SPIREG[SPI_CMD_DATA + 2] = rl_LBA_Offset.BYTEMODE.LH;   //0xF412, Address M
            SPIREG[SPI_CMD_DATA + 3] = rl_LBA_Offset.BYTEMODE.LL;   //0xF413, Address L

            SPIREG[SPI_CMD_BCNT] = (4 - 1); //0xF40F, SPI Command Byte Count

            //Command Trigger
            SPIREG[SPI_MST_TRIG] = BIT3;    //0xF401, trigger spi command

            while (SPIREG[SPI_MST_TRIG] & BIT3); //wait finish

            //Data Trigger
            SPIREG[SPI_MST_TRIG] = BIT1;    //0xF401, trigger spi dma_dio bus

            //SPIREG[SPI_DMA_RAM_ADDR_M] = j;   //0xF429
            while (SPIREG[SPI_MST_TRIG] & BIT1); //wait finish

            SPIREG[SPI_CS_N_CTRL] = 0x03; //0xF402, SPI CS_N Control, ouput value=0, disable

            while (SPIFlash_RDSR() & BIT0);

            //SPIFlash_WRDI();
        }
    }
}

//==================================================================================================================
//  FUNCTION DESCRIPTION:
//      SPI flash write header, data from USB.
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
void SPIFlash_Write_Header()
{
    TDWORD rl_LBA_Offset;

    USBBulkOutSectorToPageBuffer(0x80, 1, 0);   //Recv 512B data form USB, from 0x19600

    rl_LBA_Offset.BYTEMODE.LL = X_CMDBLOCK[5];
    rl_LBA_Offset.BYTEMODE.LH = X_CMDBLOCK[4];
    rl_LBA_Offset.BYTEMODE.HL = X_CMDBLOCK[3];
    rl_LBA_Offset.BYTEMODE.HH = 0x00;

    if (rl_LBA_Offset.DWord <= HEAD_LAST_ADDR)
    {
        SPIFLASH_PROGRAM(0);    //Prevent overwrite FW
    }
}

//==================================================================================================================
//  FUNCTION DESCRIPTION:
//      SPI flash write memory, R/W addr base on X_CMDBLOCK(USB CBW).
//
//  INPUT:
//      ab_Shift: Shift data fro USB
//
//  OUTPUT:
//      None
//
//MODIFIED GLOBAL VARIABLE:
//      None
//
//==================================================================================================================
void SPIFlash_Write(BYTE ab_Shift)
{
#if 0

    if (ab_Shift)
    {
        /*if(X_CMDBLOCK[3] == 0 && X_CMDBLOCK[4] == 0 && X_CMDBLOCK[5] == 0)   //first write
        {
            Save_Info();
        }*/

        USBBulkOutSectorToPageBuffer(0x80, 1, 0);   //Recv 512B data form USB, from 0x19600
    }

#endif

    USBBulkOutSectorToPageBuffer(0x80, 1, 0);   //Recv 512B data form USB, from 0x19600
    SPIFLASH_PROGRAM(ab_Shift);
}

