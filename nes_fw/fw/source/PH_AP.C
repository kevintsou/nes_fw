#include "8051.h"
#include "Var_Def.h"
#include "MCU_Reg.h"
#include "Buf_Def.h"
#include "USB_API.h"
#include "string.h"
#include "MSPI_API.h"
#include "M_SPI_Reg.h"
#include "SPI_Flash_API.h"
#include "SPI_Flash_Reg.h"
#include "sys_api.h"
#include "GPIO_API.h"
#include "PH_AP_API.h"
#include "USB_String.h"
#include "UsbReg.h"
#include "FW_Info.h"
#include "UART_Api.h"
#include "Interrupt_API.h"
#include "pba_vc.h"
#include "MCU_Reg.h"
#include "DMA_Reg.h"
#include "WDT_Reg.h"
#include "GPIO_Reg.h"
#include "pba_vc_api.h"
#include "Basetype.h"
#include "a.h"


//==================================================================================================================

void SPIISP(void)
{
    BYTE *rb_ptr;
    //Print_Debug_Message(__MSG_LEVEL_FUN_NAME__,"[==>Fun: SPIISP()]");

    rb_ptr = 0x20000;

    if (!gi_SPIFlash_Initialed) {
        SPIFlash_Initial();
    }

    if (!gi_SPIFlash_IDRead) {
        SPIFlash_ReadID();
    }

    switch (X_CMDBLOCK[2]) {
        case 0x00:  //Read STS
            SPIFlash_ReadStatus();
            break;

        case 0x01:  //Read ID
            *rb_ptr = gb_Manufacturer_ID;
            USBBulkInSectorFromPageBuffer(0x80, 1, 0);
            break;

        case 0x02:  //All chip erase
            SPIFlash_Chip_Erase();
            break;

        case 0x03:
            SPIFlash_Read(0, 1, 1); //shift read
            break;

        case 0x04:
            SPIFlash_Write(1);  //shift write
            break;

        case 0x05:
            SPIResetMCU();
            break;

        case 0x06:
            PRAM_Write();
            break;

        case 0x07:
            PRAM_Read();
            break;

        case 0x08:
            JUMP_PRAM();
            break;

        case 0x09:
            SPIFlash_Read(0, 0, 1); //No shift read
            break;

        case 0x0A:
            SPIFlash_Write(0);  //No shift write
            break;

        case 0x0B:
            SPIFlash_Sector_Erase();
            break;

        case 0x0C:
            SPIFlash_Write_Header();
            break;

				case 0x0E:
						SPIFlash_Check();
						break;

        default:
            break;
    };
}

//============================================================================
//  FUNCTION DESCRIPTION:
//      Test GPSPI, through CBW to check GPSPI HW function.
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
//============================================================================
void SPITest(void)
{
    //Print_Debug_Message(__MSG_LEVEL_FUN_NAME__,"[==>Fun: SPITest()]");
    switch (X_CMDBLOCK[2]) {
        case 0x02:
            MSPI_RDID();
            break;

        case 0x03:
            MSPI_RDSR();
            break;

        default:
            break;
    };
}

#define AD5245 0x2C //Socket Power
#define AD5245_2 0x2D   //IO Power
#define PCA9534 0x20



//============================================================================
//  FUNCTION DESCRIPTION:
//      Load PS2802's info block into the buffer.
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
//============================================================================
/*
void Load_Info()
{
    BYTE *rb_ptr = 0x20000;
    char code rb_Date[11] = __DATE__;   //MMM DD YYYY
    char code rb_Time[] = __TIME__;


    memset(rb_ptr, 0x00, 0x200);

    memcpy(&rb_ptr[0] , gucInquiryData, 44);

    rb_ptr[0x30] = FW_YEAR;
    rb_ptr[0x31] = FW_MON;
    rb_ptr[0x32] = FW_DAY;
    rb_ptr[0x33] = FW_VER_H;
    rb_ptr[0x34] = FW_VER_L;
    rb_ptr[0x35] = FW_VER_MINOR;
    
    rb_ptr[0x36] = FW_VER_MINOR_1;
    rb_ptr[0x37] = FW_VER_MINOR_2;
    rb_ptr[0x38] = ' ';
    rb_ptr[0x39] = FW_MOD_1;
    rb_ptr[0x3A] = FW_MOD_2;
    rb_ptr[0x3B] = FW_MOD_3;
    rb_ptr[0x3C] = FW_MOD_4;

    rb_ptr[0x40] = rb_Date[7];
    rb_ptr[0x41] = rb_Date[8];
    rb_ptr[0x42] = rb_Date[9];
    rb_ptr[0x43] = rb_Date[10];
    rb_ptr[0x44] = rb_Date[6];
    rb_ptr[0x45] = rb_Date[0];
    rb_ptr[0x46] = rb_Date[1];
    rb_ptr[0x47] = rb_Date[2];
    rb_ptr[0x48] = rb_Date[3];
    rb_ptr[0x49] = rb_Date[4];
    rb_ptr[0x4A] = rb_Date[5];

    rb_ptr[0x50] = rb_Time[0];
    rb_ptr[0x51] = rb_Time[1];
    rb_ptr[0x52] = rb_Time[2];
    rb_ptr[0x53] = rb_Time[3];
    rb_ptr[0x54] = rb_Time[4];
    rb_ptr[0x55] = rb_Time[5];
    rb_ptr[0x56] = rb_Time[6];
    rb_ptr[0x57] = rb_Time[7];

    rb_ptr[0x60] = 'U';
    rb_ptr[0x61] = 'S';
    rb_ptr[0x62] = 'B';

    if (gucUsbSpeed == 3) {
        rb_ptr[0x63] = '3';
    } else if (gucUsbSpeed == 2) {
        rb_ptr[0x63] = '2';
    } else {
        rb_ptr[0x63] = '?';
    }

    rb_ptr[0x64] = '.';
    rb_ptr[0x65] = '0';

    //Print_Debug_Message(__MSG_LEVEL_FUN_NAME__,"[==>Fun: Load_Info()]");
    //Load_Description();

    //PS2803 without information block, maybe create a USB PHY information
    //memcpy(INFO_Base,X_CopyBuf,512);
}
*/
//============================================================================
//  FUNCTION DESCRIPTION:
//      After load info block, send that to host.
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
//============================================================================
/*
void Send_Info()
{
    xdata BYTE r_Buf[16];
    BYTE *rb_ptr = 0x20000;

    //Print_Debug_Message(__MSG_LEVEL_FUN_NAME__,"[==>Fun: Send_Info()]");
    Load_Info();

    if (gucUsbSpeed == 3) {
        FifoShortPkt(rb_ptr, 528);
    } else {
        USBBulkInSectorFromPageBuffer(0x80, 1, 0);

        //memcpy(ShortPkt_Buf, 0xFF, 16);
        memset(r_Buf, 0xFF, 16);

        FifoShortPkt(r_Buf, 16);
    }

    Set_Media_OK();
}
*/
/**************************************************************************************
// description: read/write register
// input:
//  R_W_SEL: 0=read (sector unit),  1=write (byte unit)
// output: none
**************************************************************************************/

#define SDRAM_          0x2F290UL
void Access_Reg(UINT8 R_W_SEL, UINT8 Item_SEL, UINT8 Addr, UINT8 Value)
{
    UINT8 xdata *tmp_Addr = NULL;
    //Debug_FUN_NAME_Print("[==>Fun: Access_Reg()]");

    switch (Item_SEL) {
            //case  0: tmp_Addr = _TEMP_;   break;  // MMC/SD/UHS2
        case  1:
            tmp_Addr = CLKREG;
            break;  // CLK

        case  2:
            tmp_Addr = SYSREG;
            break;  // SYS

        case  3:
            tmp_Addr = DMAREG;
            break;  // DMA

        case  4:
            tmp_Addr = BRAREG;
            break;  // RA

            //case  5: tmp_Addr = _TEMP_;     break;    // AES
            //case  6: tmp_Addr = _TEMP_;     break;    // SHA
        case  7:
            tmp_Addr = SDRAM_;
            break;  // SDRAM

              //case  9: tmp_Addr = _TEMP_;       break;  // OTP
        case 10:
            tmp_Addr = WDTREG;
            break;  // WDT

            // case 11: tmp_Addr = I2CREG;     break;    // I2C
        case 12:
            tmp_Addr = GPIOREG;
            break;  // GPIO

        case 13:
            tmp_Addr = SPIREG;
            break;  // SPI

        case 14:
            tmp_Addr = M_SPIREG;
            break;  // RSPI

        default:
            Invalid_Cmd();
            break;
    }

    if (R_W_SEL) {
        // 1-Write
        tmp_Addr[Addr] = Value;
    } else {
        // 0-Read
        memcpy(PAGE2MEM_CHAR(MEM_SDMMC_DATA_PAGE), tmp_Addr, 512);
        USBBulkInSectorFromPageBuffer(MEM_SDMMC_DATA_PAGE, 1, 0);
    }
}


//============================================================================
//  FUNCTION DESCRIPTION:
//      Through CBW to send back the EA ram data to host.
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
//============================================================================
void Dump_Ram_EA()
{
    BYTE rb_Loop;
    //Print_Debug_Message(__MSG_LEVEL_FUN_NAME__,"[==>Fun: Dump_Ram_EA()]");

    for (rb_Loop = 0x00; rb_Loop < 0x8; rb_Loop++) {
        USBBulkInSectorFromPageBuffer((0xE0 + rb_Loop + (X_CMDBLOCK[2] * 0x8)), 1, 0);
    }
}

//============================================================================
//  FUNCTION DESCRIPTION:
//      Through CBW dump xdata to host.
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
//============================================================================
void Ram_Dump()
{
    //Print_Debug_Message(__MSG_LEVEL_FUN_NAME__,"[==>Fun: Ram_Dump()]");
    USBBulkInSectorFromPageBuffer(X_CMDBLOCK[2], 1, 0);
}




//============================================================================
//  FUNCTION DESCRIPTION:
//      It's summary function for phison vendor command.
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
//============================================================================
void Vendor_Phison_Cmd(void)
{
    switch (X_CMDBLOCK[1]) {
        //case 0x03 :
        //case 0x05 :
        //    Send_Info();    //512B
        //    break;




        case 0xA0:  // Register Setting.
            //  [2] : 0-Read 1-Write
            //  [3] : Select Item
            //  [4] : Address
            //  [5] : Value
            Access_Reg(X_CMDBLOCK[2], X_CMDBLOCK[3], X_CMDBLOCK[4], X_CMDBLOCK[5]);
            Set_Media_OK();
            break;

        case 0xA1:  // like case 0x42/0x43

            //  [2] : 0-Bulk_Out  1-Bulk_IN
            //  [3] : Buffer Page
            //  [4] : Transfer Sector
            if (X_CMDBLOCK[2]) {
                USBBulkInSectorFromPageBuffer(X_CMDBLOCK[3], X_CMDBLOCK[4], 0);
            } else {
                USBBulkOutSectorToPageBuffer(X_CMDBLOCK[3], X_CMDBLOCK[4], 0);
            }

            break;

        case 0xF1 :
            pba_vc_cmd_dispatch();
            break;

        case VC_ISP_FW:
        case VC_VERIFY_FW:
        case VC_JUMP_PRAM:
            pba_vendor_Cmd_isp();
            break;

        case 0xF7 :
            //  Noise_Gen_ToSRAM();
            Set_Media_OK();
            break;

        default:
            Invalid_Cmd();
            break;
    }
}


void CardReader_Cmd(void)
{

    //Print_Debug_Message(__MSG_LEVEL_FUN_NAME__,"[==>Fun: CardReader_Cmd()]");
    switch (X_CMDBLOCK[1]) {
            // UHS2 cmd
        case 0x20:  //
            Set_Media_OK();
            break;

        case 0x21:  // UHS2 cmd trigger
            break;
            /*
            case 0x22:  // UHS2 initial
                Set_Media_OK();
                break;
            */
            // UHS2 cmd end

            // UFS CMD
        case 0x22:
            Set_Media_OK();
            break;

        case 0x23:
            Set_Media_OK();
            break;

        case 0x24:
            Set_Media_OK();
            break;

        case 0x25:  // environment setting
            Set_Media_OK();
            break;

        case 0x30:
            //OtpTest();
            Set_Media_OK();
            break;

        case 0x31:
            //SHATest();
            Set_Media_OK();
            break;

        case 0x32:
            //AESTest(X_CMDBLOCK[2]);
            Set_Media_OK();
            break;

        case 0x35:
            //if (X_CMDBLOCK[6] != 'p' || X_CMDBLOCK[7] != 'H' || X_CMDBLOCK[8] != 'i' ||
            //        X_CMDBLOCK[9] != 'S' || X_CMDBLOCK[10] != 'o' || X_CMDBLOCK[11] != 'N') {
            //    Invalid_Cmd();
            //} else {
                SPIISP();
                Set_Media_OK();
            //}

            break;

        case 0x36:
            SPITest();
            Set_Media_OK();
            break;

        case 0x37: // I2C
            //SW_R(X_CMDBLOCK[3], 0);
            //SW_R(X_CMDBLOCK[3], 1);
            Set_Media_OK();
            break;


        case 0x39:
            //SW_Vol(X_CMDBLOCK[3], 0);
            Set_Media_OK();
            break;


        case 0x3A:
            //SDCPRMTest();
            Set_Media_OK();
            break;

  
        case 0x3D:
            Set_Media_OK();
            break;

        case 0x88:  //Ram Dump
            Ram_Dump();
            Set_Media_OK();
            break;

        case 0x89:  //Dump_Ram_EA
            Dump_Ram_EA();
            Set_Media_OK();
            break;

        case 0x90:  //Dump_Reg
            //Dump_Reg();
            Set_Media_OK();
            break;

        case 0xA0:  //DMA Test
            //DMA_Test();
            Set_Media_OK();
            break;

        case 0xB0:  //GPIO test
            //GPIO_Test(X_CMDBLOCK[2], &X_CMDBLOCK[3]);
            Set_Media_OK();
            break;

            /*case 0xB1:    //SD GPIO test
                SD_GPIO_Test(X_CMDBLOCK[2], X_CMDBLOCK[3], &X_CMDBLOCK[4]);
            break;*/

        case 0xB2: //Responce Link error count
            Send_Link_Err_Count();
            Set_Media_OK();

        case 0xB7:
            //Noise_Gen_ToSRAM();
            Set_Media_OK();
            break;

        case 0xB8:
            Set_Media_OK();
            break;

        default:
            Invalid_Cmd();
            break;
    }
}


