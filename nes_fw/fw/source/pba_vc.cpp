/**
 * @file ph_ap.c
 * Copyrihgt 2015 PHISON Electronics Corp.
 *
 * <b>
@verbatim
FILE NAME :  ph_ap.c
PURPOSE   :
AUTHOR    :
PHASE     :
Document  :
History   :
Date        Author          Version  Descrption
----------  --------------  -------  --------------------------------


@endverbatim
 * </b>\n
 *
 * @note
 * @bug
 */

#define __PBA_VC_C__
/*--------------------------------------------------------------------------*/
/* INCLUDE FILE                                                             */
/*--------------------------------------------------------------------------*/
#include "nes.h"
#ifdef D_NES_CMODEL
#include "8051.h"
#include "nes_cmodel.h"
#pragma comment(lib, "nes_cmodel.lib")
#else
#include <reg52.h>
#include <8051.h>
#include <intrins.h>
#endif

#include "Var_Def.h"
#include "MCU_Reg.h"
//#include "I2C_API.h"
#include "Buf_DEF.h"
#include "usb_api.h"
#include "string.h"
#include "SPI_Flash_API.h"
#include "sys_api.h"
#include "GPIO_API.h"
#include "GPIO_Reg.h"
//#include "ph_ap.h"
#include "ph_ap_api.h"
#include "UART_API.h"
#include "USB_String.h"
#include "FW_Info.h"
#include "UsbReg.h"
#include "Interrupt_API.h"
#include "pba_api.h"
#include "pba_vc.h"
#include "pba_vc_api.h"
#include "MCU_Reg.h"
#include "HOST.h"
#include "DMA_Reg.h"
#include "Operator.h"
#include "a.h"



U8 xdata gb_Host_MDLL_Mode;
U8 xdata gb_Host_MDLL_L;
U8 xdata gb_Host_MDLL_H;

extern xdata unsigned char _gMax_Ce_Cnt;
extern xdata unsigned char _gMax_Lun_Cnt;
extern xdata unsigned char _gMax_Plane_Cnt;
extern xdata unsigned char Flash_1V2;
extern xdata unsigned char AIPR_EN;
extern xdata unsigned char drive_config[4];

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTION IMPLEMENT                                                 */
/*--------------------------------------------------------------------------*/


static BOOL chk_flh_cmd_in(void)
{
    volatile U8* pbyPBACEReg;
    U8 i;
    

    //SW mode or HW_ADDR mode, scan all CE INT status
    for (i = 0; i < MAX_CE_CNT; i++)
    {
        pbyPBACEReg = PBAREG_CE0 + i * 0x100;
        if (pbyPBACEReg[CFDE_INT_TYPE] & CFDE_INT)
        {          
            return TRUE;
        }
    }



    //RR mode, scan all cache buffer valid
    for (i = 0; i < 0x08 ; i++)
    {
       
        if (PBAREG[CACHE_VAILD_FLAG_7_0 + i] != 0x00)
        {  
            return TRUE;
        }
        
    }
    
    return FALSE;


    
    /*volatile U8* pbyPBACEReg;
    U8 i;
    BOOL  Cmd_in=FALSE;


    for (i = 0; i < _gMax_Ce_Cnt; i++)
    {
        pbyPBACEReg = PBAREG_CE0 + i * 0x100;
        
        if ((pbyPBACEReg[CFDE_INT_TYPE] & CFDE_INT))
        {    
            if(pbyPBACEReg[BSY_STS_O_LUN7_0]==0xFF)
            {
               pbyPBACEReg[CFDE_CLR_ALL_INTR] |= FLAG_CLEAR_INT;
            }
            else
            {
               Cmd_in=TRUE;
            }
        }
       
    }

    
    
    //RR mode, scan all cache buffer valid
    for (i = 0; i < 0x08 ; i++)
    {
       
        if (PBAREG[CACHE_VAILD_FLAG_7_0 + i] != 0x00)
        {  
            return TRUE;
        }
        
    }

    

    return Cmd_in;*/
}


BYTE memcpy_DMA(UINT8 ab_Sur_Page, UINT8 ab_Des_Page, UINT32 al_len)
{

    //-------------------------------------------------------------------------------------------------
    DMAREG[_IN DMA_SRHB_H] = INT_BYTE2(Convert_Page2BufAddr(ab_Sur_Page));  // Set DMA Source Addr
    DMAREG[_IN DMA_SRHB_M] = INT_BYTE1(Convert_Page2BufAddr(ab_Sur_Page));
    DMAREG[_IN DMA_SRHB_L] = INT_BYTE0(Convert_Page2BufAddr(ab_Sur_Page));

    //-------------------------------------------------------------------------------------------------
    DMAREG[_IN DMA_SRHE_H] = INT_BYTE2(Convert_Page2BufAddr(ab_Des_Page));  // Set DMA Destination Addr
    DMAREG[_IN DMA_SRHE_M] = INT_BYTE1(Convert_Page2BufAddr(ab_Des_Page));
    DMAREG[_IN DMA_SRHE_L] = INT_BYTE0(Convert_Page2BufAddr(ab_Des_Page));

    //-------------------------------------------------------------------------------------------------
    DMAREG[_IN DMA_LEN_H] = INT_BYTE2(al_len);  // Set DMA Length
    DMAREG[_IN DMA_LEN_M] = INT_BYTE1(al_len);
    DMAREG[_IN DMA_LEN_L] = INT_BYTE0(al_len);

    //-------------------------------------------------------------------------------------------------
    DMAREG[_IN DMA_CTRL] = OP_COPY; // Trigger DMA Copy

    //-------------------------------------------------------------------------------------------------

    //while (DMAREG[DMA_CTRL] & 0x07);


    return 0;
}




static void pba_vc_get_CFDE_request_NEW(void)
{
   
    gb_Time0_Cnt=0;
    while (gb_Time0_Cnt< 100)
    {
        if (chk_flh_cmd_in())
        {
    				
    		SYSREG[SYS_PBA_CASEL_CFG0] = SEL_SYS_INFO;
    		memcpy_DMA(FLAG_BASENO_PBA_CACHE,BUF_NUM_TEMP_BUF,(512*SYS_INFO_SIZE));
           
    		SYSREG[SYS_PBA_CASEL_CFG0] = SEL_ORI_MAPPING;
    		USBBulkInSectorFromPageBuffer(BUF_NUM_TEMP_BUF, SYS_INFO_SIZE, 0);
						
            Set_Media_OK();
            return;
        }
    }
 
    SYSREG[SYS_PBA_CASEL_CFG0] = SEL_ORI_MAPPING;
    REQ_BLK_Base[REQ_BLK_HEADER0] = REQ_BLK_HEADER0_VAL;
    REQ_BLK_Base[REQ_BLK_HEADER1] = REQ_BLK_HEADER1_VAL;
    REQ_BLK_Base[0x02] = PBAREG[CFDE_GPIO_DATA_OUTP];
    REQ_BLK_Base[0x0C] = PBAREG[DATABUS_OUT_SRC];// return power state info
    REQ_BLK_Base[3070] = gb_ErrorHappenCnt >> 8;
    REQ_BLK_Base[3071] = gb_ErrorHappenCnt;

    USBBulkInSectorFromPageBuffer(FLAG_BASENO_REQ_BLK, SYS_INFO_SIZE, 0);

   
    Set_Media_OK();
    


}



static void pba_vc_get_CFDE_request(void)
{

    //U16 wCounter;

    //wCounter = 0;
#if CACHE_HD_CPU
    U8 i=0,j=0;
    U16 k=0;
    volatile U8* pbyPBACEReg;
#endif

#if 0
    //need confirm sysinfo size is 16K or not
    for (;;)
    {
        if (chk_flh_cmd_in())
        {
            //Must setting status buffer entry to 0x80, for previous error status clear
            //PBAREG_CE0[STA_BUF_DLY_T_NO] = 0x48;
            //PBAREG_CE0[STA_WR_DLY_T_ENTRY] = 0x80;
            //PBAREG_CE0[STA_BUF_NO] = 0x48;
            //PBAREG_CE0[STA_WR_ENTRY] = 0x80;

            SYSREG[SYS_PBA_CASEL_CFG0] = SEL_SYS_INFO;
            PBAREG[CTL_ADR_BFU_SRAM_ADDR_SEL] = 0x01;
            USBBulkInSectorFromPageBuffer(FLAG_BASENO_PBA_CACHE, SYS_INFO_SIZE, 0);
            SYSREG[SYS_PBA_CASEL_CFG0] = SEL_ORI_MAPPING;
            PBAREG[CTL_ADR_BFU_SRAM_ADDR_SEL] = 0x00;
            Set_Media_OK();
            return;
        }
        else
        {
            //It shall output empty info after 5S
            /*
            if (gb_T1MS_CFDE >= MAX_VAL_CFDE_INT) { //Wait 5S
                break;
            }
            */
            //pba_set_delay(65534);
            //break;
            wCounter++;
            if (wCounter >= 500)
            {
                break;
            }
        }
    }

    /* Output request block(32K,no CFDE INT assert) to USB_Host after waitting 5S */
    SYSREG[SYS_PBA_CASEL_CFG0] = SEL_ORI_MAPPING;
    REQ_BLK_Base[REQ_BLK_HEADER0] = REQ_BLK_HEADER0_VAL;
    REQ_BLK_Base[REQ_BLK_HEADER1] = REQ_BLK_HEADER1_VAL;
    REQ_BLK_Base[0x02] = PBAREG[CFDE_GPIO_DATA_OUTP];
    REQ_BLK_Base[3070] = gb_ErrorHappenCnt >> 8;
    REQ_BLK_Base[3071] = gb_ErrorHappenCnt;
    USBBulkInSectorFromPageBuffer(FLAG_BASENO_REQ_BLK, SYS_INFO_SIZE, 0);

    Set_Media_OK();
#else

    //BRAREG[BRA_FBSAddr] = 0x20;
    //BRAREG[BRA_FBLen]   = 0x20;

    /*SYSREG[SYS_PBA_CASEL_CFG0] = SEL_ORI_MAPPING;
    REQ_BLK_Base[REQ_BLK_HEADER0] = REQ_BLK_HEADER0_VAL;
    REQ_BLK_Base[REQ_BLK_HEADER1] = REQ_BLK_HEADER1_VAL;
    USBBulkInSectorFromPageBuffer(FLAG_BASENO_REQ_BLK, SYS_INFO_SIZE, 0);*/

    gb_Time0_Cnt=0;
    while (gb_Time0_Cnt< 1000)
    {
        if (chk_flh_cmd_in())
        {
            //GPIOREG[GPIO_A_O] = 0xF0;

#if CACHE_HD_CPU
            SYSREG[SYS_PBA_CASEL_CFG0] = SEL_ORI_MAPPING;

            for(i=0; i<64; i++)
            {
                PBAREG[CTL_PBA_CACHE_HEADER_SEL] = i;

                for(j=0; j<8; j++)
                {
                    TEMP_BUF[k]=PBAREG[CTL_PBA_CACHE_HEADER_DAT0+j];
                    k++;
                }

                for(j=0; j<8; j++)
                {
                    TEMP_BUF[k]=PBAREG[CTL_PBA_CACHE_HEADER_DAT8+j];
                    k++;
                }
            }

            for(i=0; i<_gMax_Ce_Cnt; i++)
            {
                pbyPBACEReg = PBAREG_CE0 + i * 0x100;
                for(j=0; j<13; j++)
                {
                    TEMP_BUF[k]=pbyPBACEReg[CFDE_INTV_7_0+j];
                    k++;
                }
            }


            for(i=0; i<_gMax_Ce_Cnt; i++)
            {
                pbyPBACEReg = PBAREG_CE0 + i * 0x100;

                for(j=0; j<13; j++)
                {
                    TEMP_BUF[k]=pbyPBACEReg[CFDE_TAG_99_96+j];
                    k++;
                }
            }

            for(i=0; i<_gMax_Ce_Cnt; i++)
            {
                pbyPBACEReg = PBAREG_CE0 + i * 0x100;

                TEMP_BUF[k]=pbyPBACEReg[RD_TRF_CNT_L];
                k++;
                TEMP_BUF[k]=pbyPBACEReg[RD_TRF_CNT_H];
                k++;
                TEMP_BUF[k]=pbyPBACEReg[WR_TRF_CNT_L];
                k++;
                TEMP_BUF[k]=pbyPBACEReg[WR_TRF_CNT_H];
                k++;
            }


            GPIOREG[GPIO_A_O] = 0xB0;
            USBBulkInSectorFromPageBuffer(BUF_NUM_TEMP_BUF, 0x03, 0);
            GPIOREG[GPIO_A_O] = 0x00;
#else

    SYSREG[SYS_PBA_CASEL_CFG0] = SEL_SYS_INFO;
    //PBAREG[CTL_ADR_BFU_SRAM_ADDR_SEL] = 0x01;
    //GPIOREG[GPIO_A_O] = 0xB0;

    USBBulkInSectorFromPageBuffer(FLAG_BASENO_PBA_CACHE, SYS_INFO_SIZE, 0);
    //GPIOREG[GPIO_A_O] = 0x00;
    SYSREG[SYS_PBA_CASEL_CFG0] = SEL_ORI_MAPPING;
    //PBAREG[CTL_ADR_BFU_SRAM_ADDR_SEL] = 0x00;
#endif

            Set_Media_OK();
            return;
        }

    }


    /* Output request block(32K,no CFDE INT assert) to USB_Host after waitting 5S */
    SYSREG[SYS_PBA_CASEL_CFG0] = SEL_ORI_MAPPING;
    REQ_BLK_Base[REQ_BLK_HEADER0] = REQ_BLK_HEADER0_VAL;
    REQ_BLK_Base[REQ_BLK_HEADER1] = REQ_BLK_HEADER1_VAL;
    REQ_BLK_Base[0x02] = PBAREG[CFDE_GPIO_DATA_OUTP];
    REQ_BLK_Base[3070] = gb_ErrorHappenCnt >> 8;
    REQ_BLK_Base[3071] = gb_ErrorHappenCnt;

#if CACHE_HD_CPU
    USBBulkInSectorFromPageBuffer(FLAG_BASENO_REQ_BLK, 0x03, 0);
#else
    USBBulkInSectorFromPageBuffer(FLAG_BASENO_REQ_BLK, SYS_INFO_SIZE, 0);
#endif
    Set_Media_OK();

#endif


}

static void pba_vc_set_info_page(void)
{
    /* Receive info page(2K) */


    SYSREG[SYS_PBA_CASEL_CFG0] = RAM_DEF;
    USBBulkOutSectorToPageBuffer(FLAG_BASENO_INFO_PAGE, 4, 0);


    if (!pba_set_info_page())
    {
        Set_Media_OK();
    }
    else
    {
        //Set_CSW_Fail();
        Set_Media_NG();
    }
}


static void pba_vc_fill_CFDE_blk(void)
{
    /* Receive CFDE block(1K) */
    SYSREG[SYS_PBA_CASEL_CFG0] = RAM_DEF;
    USBBulkOutSectorToPageBuffer(FLAG_BASENO_CFDE_BLK, 2, 0);

    if (!pba_fill_CFDE_blk())
    {
        Set_Media_OK();
    }
    else
    {
        Set_Media_NG();
    }

}

static void pba_vc_fill_Cache_Header(void)
{
    if (!pba_fill_Cache_Header())
    {
        SYSREG[SYS_PBA_CASEL_CFG0] = RAM_DEF;
        USBBulkInSectorFromPageBuffer(FLAG_BASENO_CFDE_BLK, 0x01, 0);
        Set_Media_OK();
    }
    else
    {
        Set_Media_NG();
    }

}

static void pba_vc_get_dps_ram_data(void)
{

    if (!pba_get_dps_ram_data(X_CMDBLOCK[3], X_CMDBLOCK[4], X_CMDBLOCK[5]))
    {
        SYSREG[SYS_PBA_CASEL_CFG0] = RAM_DEF;
        USBBulkInSectorFromPageBuffer(FLAG_BASENO_CFDE_BLK, 0x01, 0);
        Set_Media_OK();
    }
    else
    {
        Set_Media_NG();
    }
}

static void pba_vc_Set_dps_ram_data(void)
{
    SYSREG[SYS_PBA_CASEL_CFG0] = RAM_DEF;
    USBBulkOutSectorToPageBuffer(FLAG_BASENO_CFDE_BLK, 0x01, 0);
    if (!pba_Set_dps_ram_data(X_CMDBLOCK[3], X_CMDBLOCK[4], X_CMDBLOCK[5],X_CMDBLOCK[6]))
    {

        Set_Media_OK();
    }
    else
    {
        Set_Media_NG();
    }
}

static void pba_vc_get_Tag_Info(void)
{

    if (!pba_get_Tag_Info(X_CMDBLOCK[3]))
    {
        SYSREG[SYS_PBA_CASEL_CFG0] = RAM_DEF;
        USBBulkInSectorFromPageBuffer(FLAG_BASENO_CFDE_BLK, 0x01, 0);
        Set_Media_OK();
    }
    else
    {
        Set_Media_NG();
    }
}



static void pba_vc_fill_SG_buf_data(void)
{
    if (!pba_fill_SG_buf_data(X_CMDBLOCK[3], X_CMDBLOCK[4]))
    {
        SYSREG[SYS_PBA_CASEL_CFG0] = RAM_DEF;
        USBBulkInSectorFromPageBuffer(FLAG_BASENO_CFDE_BLK, 0x01, 0);
        Set_Media_OK();
    }
    else
    {
        Set_Media_NG();
    }

    
}

static void pba_vc_switch_flash_type(void)
{
    if (!pba_switch_flash_type(X_CMDBLOCK[3], X_CMDBLOCK[4]))
    {
        Set_Media_OK();
    }
    else
    {
        Set_Media_NG();
    }
}

static void pba_vc_get_reg(void)
{
    U8 i=0;
    U16 j=0;
    volatile U8* pbyPBACEReg;

    SYSREG[SYS_PBA_CASEL_CFG0] = SEL_ORI_MAPPING;


    for(i=0; i<9; i++)
    {
        for(j=0; j<256; j++)
        {
            if(i==0)
            {
                TEMP_BUF[i*256+j]=PBAREG[j];
            }
            else
            {
                pbyPBACEReg = PBAREG_CE0 + (i-1) * 0x100;
                TEMP_BUF[i*256+j]=pbyPBACEReg[j];
            }
        }

    }



    USBBulkInSectorFromPageBuffer(BUF_NUM_TEMP_BUF, 0x05, 0);
    Set_Media_OK();
}



static void pba_vc_usb_bulk_in(void)
{


    SYSREG[SYS_PBA_CASEL_CFG0] = X_CMDBLOCK[4];

    if(X_CMDBLOCK[5]==LEGACY_MODE)
        PBAREG[PWR_CHG_INT]&=CLR_BIT5;
    else if(X_CMDBLOCK[5]==TOGGLE_MODE)
        PBAREG[PWR_CHG_INT]|=SET_BIT5;

   

    USBBulkInSectorFromPageBuffer(FLAG_BASENO_PBA_CACHE, X_CMDBLOCK[3], 0);

    SYSREG[SYS_PBA_CASEL_CFG0] = SEL_ORI_MAPPING;

    Set_Media_OK();

}
/// bulkin multi plane
static void pba_vc_usb_bulk_in_multi_plane(void)
{
    U8 i=0;

    if(X_CMDBLOCK[4]==LEGACY_MODE)
        PBAREG[PWR_CHG_INT]&=CLR_BIT5;
    else if(X_CMDBLOCK[4]==TOGGLE_MODE)
        PBAREG[PWR_CHG_INT]|=SET_BIT5;

    for(i=0;i<X_CMDBLOCK[5];i++)
    {
        SYSREG[SYS_PBA_CASEL_CFG0] = X_CMDBLOCK[6+i];
        USBBulkInSectorFromPageBuffer(FLAG_BASENO_PBA_CACHE, X_CMDBLOCK[3], 0);
    }
    SYSREG[SYS_PBA_CASEL_CFG0] = SEL_ORI_MAPPING;

    Set_Media_OK();
}

static void pba_vc_usb_bulk_out_rdy_page(void)
{

    SYSREG[SYS_PBA_CASEL_CFG0] = X_CMDBLOCK[4];


    if(X_CMDBLOCK[5]==LEGACY_MODE)
        PBAREG[PWR_CHG_INT]&=CLR_BIT5;
    else if(X_CMDBLOCK[5]==TOGGLE_MODE)
        PBAREG[PWR_CHG_INT]|=SET_BIT5;


    USBBulkOutSectorToPageBuffer(FLAG_BASENO_PBA_CACHE, 36, 0);


    /* Receive Ready Page(1K) */
    SYSREG[SYS_PBA_CASEL_CFG0] = SEL_ORI_MAPPING;


    USBBulkOutSectorToPageBuffer(FLAG_BASENO_SET_RDY, 2, 0);

    /* */
    if (!pba_set_rdy_page())
    {
        //Pass
        result_set_ready = RDY_PG_PASS;
        Set_Media_OK();
    }
    else
    {
        //Fail
        result_set_ready = RDY_PG_FAIL;
        Set_Media_NG();
    }


}



static void pba_vc_usb_bulk_out(void)
{

    SYSREG[SYS_PBA_CASEL_CFG0] = X_CMDBLOCK[4];

    if(X_CMDBLOCK[5]==LEGACY_MODE)
        PBAREG[PWR_CHG_INT]&=CLR_BIT5;
    else if(X_CMDBLOCK[5]==TOGGLE_MODE)
        PBAREG[PWR_CHG_INT]|=SET_BIT5;


    USBBulkOutSectorToPageBuffer(FLAG_BASENO_PBA_CACHE, X_CMDBLOCK[3], 0);

    SYSREG[SYS_PBA_CASEL_CFG0] = SEL_ORI_MAPPING;

    Set_Media_OK();

}

/// bulkout multi plane
static void pba_vc_usb_bulk_out_multi_plane(void)
{
    U8 i=0;

    if(X_CMDBLOCK[4]==LEGACY_MODE)
        PBAREG[PWR_CHG_INT]&=CLR_BIT5;
    else if(X_CMDBLOCK[4]==TOGGLE_MODE)
        PBAREG[PWR_CHG_INT]|=SET_BIT5;

    for(i=0;i<X_CMDBLOCK[5];i++)
    {
        SYSREG[SYS_PBA_CASEL_CFG0] = X_CMDBLOCK[6+i];
        USBBulkOutSectorToPageBuffer(FLAG_BASENO_PBA_CACHE, X_CMDBLOCK[3], 0);
    }
    SYSREG[SYS_PBA_CASEL_CFG0] = SEL_ORI_MAPPING;

    Set_Media_OK();
}

static void pba_vc_set_rdy_page(void)
{

    /* Receive Ready Page(1K) */
    SYSREG[SYS_PBA_CASEL_CFG0] = SEL_ORI_MAPPING;


    USBBulkOutSectorToPageBuffer(FLAG_BASENO_SET_RDY, 2, 0);

    /* */
    if (!pba_set_rdy_page())
    {
        //Pass
        result_set_ready = RDY_PG_PASS;
        Set_Media_OK();
    }
    else
    {
        //Fail
        result_set_ready = RDY_PG_FAIL;
        Set_Media_NG();
    }
}


static void Pba_vc_GPIO_EN_DIS(void)
{
    SYSREG[SYS_PBA_CASEL_CFG0] = SEL_ORI_MAPPING;
    GPIOREG[GPIO_A_OE] = X_CMDBLOCK[3];
    GPIOREG[GPIO_B_OE] = X_CMDBLOCK[4];
    GPIOREG[GPIO_C_OE] = X_CMDBLOCK[5];
    GPIOREG[GPIO_D_OE] = X_CMDBLOCK[6];
}


static void Pba_vc_Set_GPIO_Value(void)
{
    SYSREG[SYS_PBA_CASEL_CFG0] = SEL_ORI_MAPPING;
    GPIOREG[GPIO_A_O] = X_CMDBLOCK[3];
    GPIOREG[GPIO_B_O] = X_CMDBLOCK[4];
    GPIOREG[GPIO_C_O] = X_CMDBLOCK[5];
    GPIOREG[GPIO_D_O] = X_CMDBLOCK[6];
}



static void pba_vc_get_bus_ana_int_request(void)
{
    //U16 i, j;
    U16 wCounter;

    wCounter = 0;

    for (;;)
    {
        /* bulk in 1 sector vaild bus record info(GCR and CE0 CR) during polling 5S */
        if (PBAREG[BUS_ANA_INT_STS]&MASK_INT)
        {
            SYSREG[SYS_PBA_CASEL_CFG0] = SEL_SYS_INFO;
            USBBulkInSectorFromPageBuffer(FLAG_BASENO_PBA_CACHE, 0x01, 0);
            SYSREG[SYS_PBA_CASEL_CFG0] = SEL_ORI_MAPPING;
            Set_Media_OK();
            return;
        }
        else
        {
            //It shall output empty info after 5S
            /*
            if (gb_T1MS_CFDE >= MAX_VAL_CFDE_INT) { //Wait 5S
                break;
            }
            */
            //pba_set_delay(65534);
            //break;
            wCounter++;
            if (wCounter >= 500)
            {
                break;
            }
        }
    }

    /* bulk in 1 sector empty info(GCR and CE0 CR) after waitting 5S */
    SYSREG[SYS_PBA_CASEL_CFG0] = SEL_ORI_MAPPING;
    REQ_BLK_Base[REQ_BLK_HEADER0] = REQ_BLK_HEADER0_VAL;
    REQ_BLK_Base[REQ_BLK_HEADER1] = REQ_BLK_HEADER1_VAL;
    REQ_BLK_Base[0x02] = PBAREG[CFDE_GPIO_DATA_OUTP];
    REQ_BLK_Base[3070] = gb_ErrorHappenCnt >> 8;
    REQ_BLK_Base[3071] = gb_ErrorHappenCnt;
    USBBulkInSectorFromPageBuffer(FLAG_BASENO_REQ_BLK, 0x01, 0);

    Set_Media_OK();
}

static void pba_vc_Convertion(void)
{
    SYSREG[SYS_PBA_CASEL_CFG0] =0x01 ;

    COVREG[COV_PAGE_L]=X_CMDBLOCK[3];
    COVREG[COV_PAGE_H]=X_CMDBLOCK[4];


    COVREG[COV_START]|=SET_BIT0;

    while(1)
    {
        if(COVREG[COV_START]==0x00)
            break;
    }

    SYSREG[SYS_PBA_CASEL_CFG0] = SEL_ORI_MAPPING;

}

static void VC_Set_Differential_DQS_IO_REG(void)
{

    //20200925 Add Close Differential
    if(X_CMDBLOCK[3])
    {
       SYSREG[PBA_DAT_CONOF]=SONOF|CONOF;
       SYSREG[PBA_REB_CONOF]=SONOF|CONOF;
       SYSREG[PBA_DQS_CONOF]=SONOF|CONOF;


        if(Flash_1V2==0x01)
        {
            // 1v2 add differential driving DQS
            if(drive_config[DRIVE_CONFIG_DQS_STRENGTH] != 0x00) {
                HOST_PBA_IO_REG[PBA_DQS] |= ((drive_config[1]&(SET_BIT0|SET_BIT1)) << 2);
                HOST_PBA_IO_REG[PBA_DQS_CAL_N] |= ((drive_config[1]&SET_BIT2) << 4);
            }else
                HOST_PBA_IO_REG[PBA_DQS]|=SET_BIT3;

            // add differential driving DQ
            if(drive_config[DRIVE_CONFIG_DQ_STRENGTH] != 0x00) {
                HOST_PBA_IO_REG[PBA_HIO] |= ((drive_config[2]&(SET_BIT0|SET_BIT1|SET_BIT2)) << 2);
            }else
                HOST_PBA_IO_REG[PBA_HIO]|=SET_BIT4;//(SET_BIT3|SET_BIT2);
        }
        else
        {
            // 1v8 add differential driving DQS
            if(drive_config[DRIVE_CONFIG_DQS_STRENGTH] != 0x00) {
                HOST_PBA_IO_REG[PBA_DQS] |= ((drive_config[1]&(SET_BIT0|SET_BIT1)) << 2);
                HOST_PBA_IO_REG[PBA_DQS_CAL_N] |= ((drive_config[1]&SET_BIT2) << 4);
            }else
                HOST_PBA_IO_REG[PBA_DQS_CAL_N]|=SET_BIT6;

            // add differential driving DQ
            if(drive_config[DRIVE_CONFIG_DQ_STRENGTH] != 0x00) {
                HOST_PBA_IO_REG[PBA_HIO] |= ((drive_config[2]&(SET_BIT0|SET_BIT1|SET_BIT2)) << 2);
            }else
                HOST_PBA_IO_REG[PBA_HIO]|=SET_BIT4;
        }
    }
    else
    {
        // reset to system default driving
        if(drive_config[DRIVE_CONFIG_RE_TRIGGER] == SET_BIT1)
            SYSREG[PBA_REB_CONOF] = (SYSREG[PBA_REB_CONOF]&CLR_BIT0)|SET_BIT1;
        else
            SYSREG[PBA_REB_CONOF] = (SYSREG[PBA_REB_CONOF]&CLR_BIT1)|SET_BIT0;
        SYSREG[PBA_DQS_CONOF] = (SYSREG[PBA_DQS_CONOF]&CLR_BIT1)|SET_BIT0;
        SYSREG[PBA_DAT_CONOF] = (SYSREG[PBA_DAT_CONOF]&CLR_BIT1)|SET_BIT0;

        HOST_PBA_IO_REG[PBA_DQS] &= (CLR_BIT2&CLR_BIT3);
        HOST_PBA_IO_REG[PBA_DQS_CAL_N] &= CLR_BIT6;
        HOST_PBA_IO_REG[PBA_HIO] &= (CLR_BIT2&CLR_BIT3&CLR_BIT4);
    }

}


static void VC_Set_Io_Driving(void)
{

    SYSREG[SYS_PBA_CASEL_CFG0] = SEL_ORI_MAPPING;

    USBBulkOutSectorToPageBuffer(BUF_NUM_TEMP_BUF, 0x05, 0);

    /*HOST_PBA_IO_REG[PBA_DQS]|=PMODE_12V;
    HOST_PBA_IO_REG[PBA_DQS]|=SET_BIT3;//iolh

    HOST_PBA_IO_REG[PBA_HIO]|=PMODE_12V;
    HOST_PBA_IO_REG[PBA_HIO]|=SET_BIT3;//iolh

    HOST_PBA_IO_REG[PBA_CLE]|=PMODE_12V;
    HOST_PBA_IO_REG[PBA_CLE]|=SET_BIT3;//iolh

    HOST_PBA_IO_REG[PBA_ALE]|=PMODE_12V;
    HOST_PBA_IO_REG[PBA_ALE]|=SET_BIT3;//iolh

    HOST_PBA_IO_REG[PBA_REB]|=PMODE_12V;
    HOST_PBA_IO_REG[PBA_REB]|=SET_BIT3;//iolh

    HOST_PBA_IO_REG[PBA_WEB]|=PMODE_12V;
    HOST_PBA_IO_REG[PBA_WEB]|=SET_BIT3;//iolh



    HOST_PBA_IO_REG[PBA_CE]|=PMODE_12V;
    HOST_PBA_IO_REG[PBA_CE]|=SET_BIT3;//iolh

    HOST_PBA_IO_REG[HOST_LOCK]|=PMODE_12V;
    HOST_PBA_IO_REG[HOST_LOCK]|=SET_BIT3;//iolh

    HOST_PBA_IO_REG[HOST_WP]|=PMODE_12V;
    HOST_PBA_IO_REG[HOST_WP]|=SET_BIT3;//iolh

    HOST_PBA_IO_REG[HOST_RDY]|=PMODE_12V;
    HOST_PBA_IO_REG[HOST_RDY]|=SET_BIT3;//iolh

    HOST_PBA_IO_REG[HOST_DQS]|=PMODE_12V;
    HOST_PBA_IO_REG[HOST_DQS]|=SET_BIT3;//iolh

    HOST_PBA_IO_REG[HOST_HIO]|=PMODE_12V;
    HOST_PBA_IO_REG[HOST_HIO]|=SET_BIT3;//iolh

    HOST_PBA_IO_REG[HOST_CLE]|=PMODE_12V;
    HOST_PBA_IO_REG[HOST_CLE]|=SET_BIT3;//iolh

    HOST_PBA_IO_REG[HOST_ALE]|=PMODE_12V;
    HOST_PBA_IO_REG[HOST_ALE]|=SET_BIT3;//iolh

    HOST_PBA_IO_REG[HOST_REB]|=PMODE_12V;
    HOST_PBA_IO_REG[HOST_REB]|=SET_BIT3;//iolh

    HOST_PBA_IO_REG[HOST_WEB]|=PMODE_12V;
    HOST_PBA_IO_REG[HOST_WEB]|=SET_BIT3;//iolh

    HOST_PBA_IO_REG[HOST_CE]|=PMODE_12V;
    HOST_PBA_IO_REG[HOST_CE]|=SET_BIT3;//iolh*/







    if(X_CMDBLOCK[3]==0x00)
    {

        if(X_CMDBLOCK[4]==0x01)
        {

            SYSREG[Host_DQ_CONOF_SONOF]=TEMP_BUF[Host_DQ_CONOF_SONOF+F140_OFS];
            HOST_PBA_IO_REG[Host_DQ_SR_DTY]=TEMP_BUF[Host_DQ_SR_DTY];
            HOST_PBA_IO_REG[Host_DQ_PMODE_ODTEN_IOLH]=TEMP_BUF[Host_DQ_PMODE_ODTEN_IOLH];
            HOST_PBA_IO_REG[Host_DQ_CAL_N]=TEMP_BUF[Host_DQ_CAL_N];
            HOST_PBA_IO_REG[Host_DQ_CAL_P]=TEMP_BUF[Host_DQ_CAL_P];
            HOST_PBA_IO_REG[Host_A0_D0_VREF_REN]=TEMP_BUF[Host_A0_D0_VREF_REN];
            HOST_PBA_IO_REG[Host_DQ_TX_RX_DCTRL_D0]=TEMP_BUF[Host_DQ_TX_RX_DCTRL_D0];
            HOST_PBA_IO_REG[Host_DQ_TX_RX_DCTRL_D1]=TEMP_BUF[Host_DQ_TX_RX_DCTRL_D1];
            HOST_PBA_IO_REG[Host_DQ_TX_RX_DCTRL_D2]=TEMP_BUF[Host_DQ_TX_RX_DCTRL_D2];
            HOST_PBA_IO_REG[Host_DQ_TX_RX_DCTRL_D3]=TEMP_BUF[Host_DQ_TX_RX_DCTRL_D3];
            HOST_PBA_IO_REG[Host_DQ_TX_RX_DCTRL_D4]=TEMP_BUF[Host_DQ_TX_RX_DCTRL_D4];
            HOST_PBA_IO_REG[Host_DQ_TX_RX_DCTRL_D5]=TEMP_BUF[Host_DQ_TX_RX_DCTRL_D5];
            HOST_PBA_IO_REG[Host_DQ_TX_RX_DCTRL_D6]=TEMP_BUF[Host_DQ_TX_RX_DCTRL_D6];
            HOST_PBA_IO_REG[Host_DQ_TX_RX_DCTRL_D7]=TEMP_BUF[Host_DQ_TX_RX_DCTRL_D7];
        }

        if(X_CMDBLOCK[5]==0x01)
        {
            SYSREG[Host_RE_CONOF_SONOF]=TEMP_BUF[Host_RE_CONOF_SONOF+F140_OFS];
            HOST_PBA_IO_REG[Host_RE_SR_CALN]=TEMP_BUF[Host_RE_SR_CALN];
            HOST_PBA_IO_REG[Host_RE_SR_SRIOB_DTY]=TEMP_BUF[Host_RE_SR_SRIOB_DTY];
            HOST_PBA_IO_REG[Host_RE_SRIOB_IOLH_PMODE_ODTEN]=TEMP_BUF[Host_RE_SRIOB_IOLH_PMODE_ODTEN];
            HOST_PBA_IO_REG[Host_RE_OutputEN_CALP]=TEMP_BUF[Host_RE_OutputEN_CALP];
            HOST_PBA_IO_REG[Host_RE_TX_RX_DCTRL]=TEMP_BUF[Host_RE_TX_RX_DCTRL];

        }

        if(X_CMDBLOCK[6]==0x01)
        {
            SYSREG[Host_DQS_CONOF_SONOF]=TEMP_BUF[Host_DQS_CONOF_SONOF+F140_OFS];
            HOST_PBA_IO_REG[Host_DQS_SR_CALN]=TEMP_BUF[Host_DQS_SR_CALN];
            HOST_PBA_IO_REG[Host_DQS_SR_SRIOB_DTY]=TEMP_BUF[Host_DQS_SR_SRIOB_DTY];
            HOST_PBA_IO_REG[Host_DQS_SRIOB_IOLH_PMODE_ODTEN]=TEMP_BUF[Host_DQS_SRIOB_IOLH_PMODE_ODTEN];
            HOST_PBA_IO_REG[Host_DQS_OutputEN_CALP]=TEMP_BUF[Host_DQS_OutputEN_CALP];
            HOST_PBA_IO_REG[Host_DQS_TX_RX_DCTRL]=TEMP_BUF[Host_DQS_TX_RX_DCTRL];
        }

        if(X_CMDBLOCK[7]==0x01)
        {
            gb_Host_MDLL_Mode = 0x01;

            gb_Host_MDLL_L = TEMP_BUF[DLL_SEL_VAL_L];
            gb_Host_MDLL_H = TEMP_BUF[DLL_SEL_VAL_H];


        }
        else
        {
            gb_Host_MDLL_Mode=0x00;
        }



        if(X_CMDBLOCK[8]==0x01)
        {

            SYSREG[Host_WE_CONOF_SONOF]=TEMP_BUF[Host_WE_CONOF_SONOF+F140_OFS];
            HOST_PBA_IO_REG[Host_WE_PMODE_ODTEN_IOLH]=TEMP_BUF[Host_WE_PMODE_ODTEN_IOLH];
            HOST_PBA_IO_REG[Host_WE_SR_DTY]=TEMP_BUF[Host_WE_SR_DTY];
            HOST_PBA_IO_REG[Host_WE_CAL_N]=TEMP_BUF[Host_WE_CAL_N];
            HOST_PBA_IO_REG[Host_WE_CAL_P]=TEMP_BUF[Host_WE_CAL_P];

        }


        if(X_CMDBLOCK[9]==0x01)
        {
            HOST_PBA_IO_REG[GPIO_IOLH_ODTEN_PMODE]=TEMP_BUF[GPIO_IOLH_ODTEN_PMODE];
            HOST_PBA_IO_REG[GPIO_SR_DTY]=TEMP_BUF[GPIO_SR_DTY];
            HOST_PBA_IO_REG[GPIO_CAL_N]=TEMP_BUF[GPIO_CAL_N];
            HOST_PBA_IO_REG[GPIO_CAL_P]=TEMP_BUF[GPIO_CAL_P];
        }




    }
    else if(X_CMDBLOCK[3]==0x01)
    {



        if(X_CMDBLOCK[4]==0x01)
        {

            SYSREG[PBA_DQ_CONOF_SONOF]=TEMP_BUF[PBA_DQ_CONOF_SONOF+F140_OFS];
            HOST_PBA_IO_REG[PBA_DQ_SR_DTY]=TEMP_BUF[PBA_DQ_SR_DTY];
            HOST_PBA_IO_REG[PBA_DQ_PMODE_ODTEN_IOLH]=TEMP_BUF[PBA_DQ_PMODE_ODTEN_IOLH];
            HOST_PBA_IO_REG[PBA_DQ_CAL_N]=TEMP_BUF[PBA_DQ_CAL_N];
            HOST_PBA_IO_REG[PBA_DQ_CAL_P]=TEMP_BUF[PBA_DQ_CAL_P];
            HOST_PBA_IO_REG[PBA_DQ_TX_RX_DCTRL_D0]=TEMP_BUF[PBA_DQ_TX_RX_DCTRL_D0];
            HOST_PBA_IO_REG[PBA_DQ_TX_RX_DCTRL_D1]=TEMP_BUF[PBA_DQ_TX_RX_DCTRL_D1];
            HOST_PBA_IO_REG[PBA_DQ_TX_RX_DCTRL_D2]=TEMP_BUF[PBA_DQ_TX_RX_DCTRL_D2];
            HOST_PBA_IO_REG[PBA_DQ_TX_RX_DCTRL_D3]=TEMP_BUF[PBA_DQ_TX_RX_DCTRL_D3];
            HOST_PBA_IO_REG[PBA_DQ_TX_RX_DCTRL_D4]=TEMP_BUF[PBA_DQ_TX_RX_DCTRL_D4];
            HOST_PBA_IO_REG[PBA_DQ_TX_RX_DCTRL_D5]=TEMP_BUF[PBA_DQ_TX_RX_DCTRL_D5];
            HOST_PBA_IO_REG[PBA_DQ_TX_RX_DCTRL_D6]=TEMP_BUF[PBA_DQ_TX_RX_DCTRL_D6];
            HOST_PBA_IO_REG[PBA_DQ_TX_RX_DCTRL_D7]=TEMP_BUF[PBA_DQ_TX_RX_DCTRL_D7];
            HOST_PBA_IO_REG[PBA_DQ_ODT_CAL_N]=TEMP_BUF[PBA_DQ_ODT_CAL_N];
            HOST_PBA_IO_REG[PBA_DQ_ODT_CAL_P]=TEMP_BUF[PBA_DQ_ODT_CAL_P];


        }

        if(X_CMDBLOCK[5]==0x01)
        {
            SYSREG[PBA_RE_CONOF_SONOF]=TEMP_BUF[PBA_RE_CONOF_SONOF+F140_OFS];
            HOST_PBA_IO_REG[PBA_RE_SR_DTY]=TEMP_BUF[PBA_RE_SR_DTY];
            HOST_PBA_IO_REG[PBA_RE_SRIOB_IOLH_PMODE_ODTEN]=TEMP_BUF[PBA_RE_SRIOB_IOLH_PMODE_ODTEN];
            HOST_PBA_IO_REG[PBA_RE_IOLH_CALN]=TEMP_BUF[PBA_RE_IOLH_CALN];
            HOST_PBA_IO_REG[PBA_RE_CAL_P]=TEMP_BUF[PBA_RE_CAL_P];
            HOST_PBA_IO_REG[PBA_RE_TX_RX_DCTRL]=TEMP_BUF[PBA_RE_TX_RX_DCTRL];
        }

        if(X_CMDBLOCK[6]==0x01)
        {
            SYSREG[PBA_DQS_CONOF_SONOF]=TEMP_BUF[PBA_DQS_CONOF_SONOF+F140_OFS];
            HOST_PBA_IO_REG[PBA_DQS_SR_DTY]=TEMP_BUF[PBA_DQS_SR_DTY];
            HOST_PBA_IO_REG[PBA_DQS_SRIOB_IOLH_PMODE_ODTEN]=TEMP_BUF[PBA_DQS_SRIOB_IOLH_PMODE_ODTEN];
            HOST_PBA_IO_REG[PBA_DQS_IOLH_CALN]=TEMP_BUF[PBA_DQS_IOLH_CALN];
            HOST_PBA_IO_REG[PBA_DQS_CAL_P]=TEMP_BUF[PBA_DQS_CAL_P];
            HOST_PBA_IO_REG[PBA_DQS_TX_RX_DCTRL]=TEMP_BUF[PBA_DQS_TX_RX_DCTRL];
            HOST_PBA_IO_REG[PBA_DQS_ODT_CAL_P]=TEMP_BUF[PBA_DQS_ODT_CAL_P];
            HOST_PBA_IO_REG[PBA_DQS_ODT_CAL_N]=TEMP_BUF[PBA_DQS_ODT_CAL_N];

        }

        if(X_CMDBLOCK[9]==0x01)
        {
            HOST_PBA_IO_REG[GPIO_IOLH_ODTEN_PMODE]=TEMP_BUF[GPIO_IOLH_ODTEN_PMODE];
            HOST_PBA_IO_REG[GPIO_SR_DTY]=TEMP_BUF[GPIO_SR_DTY];
            HOST_PBA_IO_REG[GPIO_CAL_N]=TEMP_BUF[GPIO_CAL_N];
            HOST_PBA_IO_REG[GPIO_CAL_P]=TEMP_BUF[GPIO_CAL_P];
        }
    }

}



static void VC_Trig_Host_Write(void)
{


    U32 dly_cnt=0;

#if FORCE_CALI_MODE
    HOST_PBA_IO_REG[HOST_CALI_CTL]|=HOST_CALI_FORCE_MODE;
#endif

    HSTREG[HOST_PLL_CTRL+1]=HSTREG[HOST_PLL_CTRL+1] | 0x10;

    CLKREG[CLK_GATE] &= ~(MMC_GATE);	//enable MMCR SYS clock


    // chris add CALI clock divide into kHz
    P1 = 0x21;
    HSTREG[FLH_PHI_CTRL3+2] = 0xE0;                            // Set CALI_CLK_DIV[7:0]   = 16'h01E0
    HSTREG[FLH_PHI_CTRL3+3] = 0x01;                            // Set CALI_CLK_DIV[7:0]   = 16'h01E0, set CALI_CLK into 540K
    HSTREG[HOST_CALI_CTRL+3]= HSTREG[HOST_CALI_CTRL+3] | 0x02; // Set CALI_CLK_DIV_LOADEN = 1




    P1 = 0x22;
//

    HSTREG[HOST_CALI_CTRL+0]= HSTREG[HOST_CALI_CTRL+0] | 0xC0;


//select cache to cache 0 instead of MCU ext rame
    //SYSREG[0x29] = 0x0f; // PBA used Cache-14.  //Low part SRAM.

    HSTREG[HOST_PLL_CFG0_0+0]= 0x64;
    HSTREG[HOST_PLL_CFG0_0+1]= 0x09;
    HSTREG[HOST_PLL_CFG0_0+2]= 0x33;
    HSTREG[HOST_PLL_CFG0_0+3]= 0xFB;
    HSTREG[HOST_PLL_CFG0_1+0]= 0x0C;

    HSTREG[HOST_PLL_CFG1_0+0]= 0x20;
    HSTREG[HOST_PLL_CFG1_0+1]= 0x09;
    HSTREG[HOST_PLL_CFG1_0+2]= 0x33;
    HSTREG[HOST_PLL_CFG1_0+3]= 0xFB;
    HSTREG[HOST_PLL_CFG1_1+0]= 0x0C;


    //HSTREG[HOST_PLL_DIV0+0]= 0x40;
    HSTREG[HOST_PLL_DIV0+0]= 0x40;
    HSTREG[HOST_PLL_DIV0+1]= 0x02;
    HSTREG[HOST_PLL_DIV0+2]= 0x10;




    //HSTREG[HOST_PLL_DIV1+0]= 0x40;
    //HSTREG[HOST_PLL_DIV1+0]= 0x08;//X_CMDBLOCK[3];//0x50;
    //HSTREG[HOST_PLL_DIV1+1]= 0x02;
    //HSTREG[HOST_PLL_DIV1+2]= 0x10;


    P1 = 0x23;

//drive pll	0
    HSTREG[HOST_PLL_CTRL+1]=HSTREG[HOST_PLL_CTRL+1] | 0x10;
    //HSTREG[0x9]=HSTREG[0x9] | 0x10;
    //pd11_ext V-> pll_dis0 V (larger than 1us) 1T = 8ns
    dly_cnt= 0;
    while(dly_cnt < 125)
    {
        dly_cnt = dly_cnt + 1;
    }
    P1 = 0x24;
    HSTREG[HOST_PLL_CTRL]=HSTREG[HOST_PLL_CTRL] & 0xFE;
    //pll_dis0 V -> pll_rst0(larger than 20us) 1T = 8ns
    //pll_dis0 V -> chpmp_dis0(larger than 20us) 1T = 8ns
    //pll_dis0 V -> envcmid0(larger than 20us) 1T = 8ns
    dly_cnt= 0;
    while(dly_cnt < 2500)
    {
        dly_cnt = dly_cnt + 1;
    }



    P1 = 0x25;
    HSTREG[HOST_PLL_CTRL+1]=HSTREG[HOST_PLL_CTRL+1] | 0x20;
    //poll pll_lock0
    while(! ((HSTREG[HOST_PLL_STAT]&0x04)==0x04));

    HSTREG[RCT_CTRL]= 0x1;

    P1 = 0x26;
    while(! ((HSTREG[HOST_PLL_STAT]&0x04)==0x04));
    HSTREG[RCT_CTRL]= 0xC1;

    P1 = 0x27;
    while(! ((HSTREG[HOST_PLL_STAT]&0x04)!=0x04));
    HSTREG[RCT_CTRL]= 0x31;
    P1 = 0x28;
    while(! ((HSTREG[HOST_PLL_STAT]&0x04)==0x04));



    if(X_CMDBLOCK[5]==0)
    {
        //HSTREG[HOST_PLL_DIV0+0]= 0x40;
        HSTREG[HOST_PLL_DIV0+0]= 0xF0;//0x50;
        HSTREG[HOST_PLL_DIV0+1]= 0x02;//X_CMDBLOCK[3];
        HSTREG[HOST_PLL_DIV0+2]= 0x40;//0x10;


    }
    else
    {
        if(X_CMDBLOCK[3]==0)
        {
            //HSTREG[HOST_PLL_DIV0+0]= 0x40;
            HSTREG[HOST_PLL_DIV0+0]= 0x10;
            HSTREG[HOST_PLL_DIV0+1]= 0x0F;
            HSTREG[HOST_PLL_DIV0+2]= 0x10;
        }
        else
        {
            //HSTREG[HOST_PLL_DIV0+0]= 0x40;
            HSTREG[HOST_PLL_DIV0+0]= X_CMDBLOCK[3];//0x50;
            HSTREG[HOST_PLL_DIV0+1]= 0x02;
            HSTREG[HOST_PLL_DIV0+2]= 0x10;
        }



    }





    //MDLL tracking

    SYSREG[HW_UART_OFS]|=MDLL_SWITCH;

    P1 = 0x37;
    HSTREG[DLL_LOAD_CTL+3]= HSTREG[DLL_LOAD_CTL+3] | 0x10;
    HSTREG[DLL_DLL_CTL+0]= 0xE0;
    HSTREG[DLL_DLL_CTL+0]= 0x14;
    P1 = 0x38;
    while(! ((HSTREG[DLL_MDLL_STA]&0x01)==0x01));
    while(! ((HSTREG[DLL_MDLL_STA]&0x01)==0x01));

    HSTREG[HOST_CALI_CTRL+3]= HSTREG[HOST_CALI_CTRL+3] | 0x01;
    P1 = 0x39;

#if !FORCE_CALI_MODE
    HSTREG[HOST_CALI_CTRL]|=RSTJ_CALI;
    HSTREG[HOST_CALI_CTRL]|=CALI_EN;
    HSTREG[HOST_CALI_LOAD]|=CALI_LOAD;
#endif







    if(X_CMDBLOCK[4]==0)
    {
        if(gb_Host_MDLL_Mode==0x01)
        {
            HSTREG[DLL_DLL_CTL]|=SET_BIT1;
            HSTREG[DLL_SEL_VAL_L]=gb_Host_MDLL_L;
            HSTREG[DLL_SEL_VAL_H]=gb_Host_MDLL_H;
        }
        HSTREG[CRC_CTRL_INT]&=CLR_BIT1;
        HSTREG[CRC_CTRL_INT]|=CRC_CTRL_64_EN;
        HSTREG[PIO_POL_MASK]=0xE0;
        HSTREG[PIO_POL_EXP]=0xE0;


        HSTREG[HOST_DMA_LEN_L]=0xA0;
        HSTREG[HOST_DMA_LEN_H]=0x48;



        //set  program command + address + dma Seq Pattern
        HSTREG[FLH_PHI_CTRL1+3]|= SET_BIT2;//CE tie0

        while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));

        if(X_CMDBLOCK[5]==0)
        {
            HSTREG[FLH_PHI_CTRL1+1]= 0x80;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
            HSTREG[FLH_PHI_CTRL1+0]= 0x13;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));

            /*HSTREG[FLH_PHI_CTRL1+1]= 0x00;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
            HSTREG[FLH_PHI_CTRL1+0]= 0x33;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));


            HSTREG[FLH_PHI_CTRL1+1]= 0x00;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
            HSTREG[FLH_PHI_CTRL1+0]= 0x33;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));


            HSTREG[FLH_PHI_CTRL1+1]= 0x00;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
            HSTREG[FLH_PHI_CTRL1+0]= 0x33;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));


            HSTREG[FLH_PHI_CTRL1+1]= 0x00;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
            HSTREG[FLH_PHI_CTRL1+0]= 0x33;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));

            HSTREG[FLH_PHI_CTRL1+1]= 0x00;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
            HSTREG[FLH_PHI_CTRL1+0]= 0x33;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));*/


            HSTREG[FLH_PHI_CTRL1+1]= 0x00;
            HSTREG[FLH_PHI_CTRL1+0]= 0xF3;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));


            HSTREG[FLH_PHI_CTRL1+1]= 0x10;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
            HSTREG[FLH_PHI_CTRL1+0]= 0x13;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));

            HSTREG[FLH_PHI_CTRL1+1]= 0x70;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
            HSTREG[FLH_PHI_CTRL1+0]= 0x13;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));

            HSTREG[HOST_PLL_DIV0+0]= 0x10;//0x50;
            HSTREG[HOST_PLL_DIV0+1]= 0x04;//X_CMDBLOCK[3];
            HSTREG[HOST_PLL_DIV0+2]= 0x20;//0x10;


            dly_cnt= 0;
            while(dly_cnt < 50)
            {
                dly_cnt = dly_cnt + 1;
            }


            HSTREG[FLH_PHI_CTRL1+0]= 0xB1;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));



        }
        else
        {
            HSTREG[FLH_PHI_CTRL1+0]= 0x0A;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
            HSTREG[FLH_PHI_CTRL1+0]= 0x0B;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
        }



        //HSTREG[FLH_PHI_CTRL1+3]|= SET_BIT3;
        //while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));//CE tie1

    }
    else if(X_CMDBLOCK[4]==1)
    {

        if(gb_Host_MDLL_Mode==0x01)
        {
            HSTREG[DLL_SEL_VAL_L]=gb_Host_MDLL_L;
            HSTREG[DLL_SEL_VAL_H]=gb_Host_MDLL_H;
            HSTREG[DLL_DLL_CTL]|=SET_BIT1;
        }

        //set  program command + address + dma  Worst Pattern
        HSTREG[FLH_PHI_CTRL1+3]|= SET_BIT2;//CE tie0
        while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
        if(X_CMDBLOCK[5]==0)
        {
            HSTREG[FLH_PHI_CTRL1+1]= 0x80;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
            HSTREG[FLH_PHI_CTRL1+0]= 0x13;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));

            /*HSTREG[FLH_PHI_CTRL1+1]= 0x00;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
            HSTREG[FLH_PHI_CTRL1+0]= 0x33;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));


            HSTREG[FLH_PHI_CTRL1+1]= 0x00;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
            HSTREG[FLH_PHI_CTRL1+0]= 0x33;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));


            HSTREG[FLH_PHI_CTRL1+1]= 0x00;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
            HSTREG[FLH_PHI_CTRL1+0]= 0x33;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));


            HSTREG[FLH_PHI_CTRL1+1]= 0x00;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
            HSTREG[FLH_PHI_CTRL1+0]= 0x33;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));

            HSTREG[FLH_PHI_CTRL1+1]= 0x00;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
            HSTREG[FLH_PHI_CTRL1+0]= 0x33;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));*/


            HSTREG[FLH_PHI_CTRL1+3]|= SET_BIT5;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
            HSTREG[FLH_PHI_CTRL1+0]= 0xF3;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));




        }
        else
        {

            HSTREG[FLH_PHI_CTRL1+3]|= SET_BIT5;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
            HSTREG[FLH_PHI_CTRL1+0]= 0x0A;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
            HSTREG[FLH_PHI_CTRL1+0]= 0x0B;
        }


    }
    else if(X_CMDBLOCK[4]==2)
    {
        if(gb_Host_MDLL_Mode==0x01)
        {
            HSTREG[DLL_DLL_CTL]|=SET_BIT1;
            HSTREG[DLL_SEL_VAL_L]=gb_Host_MDLL_L;
            HSTREG[DLL_SEL_VAL_H]=gb_Host_MDLL_H;
        }
        HSTREG[CRC_CTRL_INT]&=CLR_BIT1;
        HSTREG[CRC_CTRL_INT]|=CRC_CTRL_64_EN;
        HSTREG[PIO_POL_MASK]=0xE0;
        HSTREG[PIO_POL_EXP]=0xE0;

        HSTREG[HOST_DMA_LEN_L]=0xA0;
        HSTREG[HOST_DMA_LEN_H]=0x48;


        HSTREG[FLH_PHI_CTRL1+3]|= SET_BIT2;//CE tie0
        while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));

        if(X_CMDBLOCK[5]==0)
        {
            HSTREG[FLH_PHI_CTRL1+1]= 0x80;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
            HSTREG[FLH_PHI_CTRL1+0]= 0x13;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
            /*
            HSTREG[FLH_PHI_CTRL1+1]= 0x00;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
            HSTREG[FLH_PHI_CTRL1+0]= 0x33;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));


            HSTREG[FLH_PHI_CTRL1+1]= 0x00;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
            HSTREG[FLH_PHI_CTRL1+0]= 0x33;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));


            HSTREG[FLH_PHI_CTRL1+1]= 0x00;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
            HSTREG[FLH_PHI_CTRL1+0]= 0x33;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));


            HSTREG[FLH_PHI_CTRL1+1]= 0x00;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
            HSTREG[FLH_PHI_CTRL1+0]= 0x33;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));

            HSTREG[FLH_PHI_CTRL1+1]= 0x00;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
            HSTREG[FLH_PHI_CTRL1+0]= 0x33;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
            */
            HSTREG[FLH_PHI_CTRL1+3]|= SET_BIT4;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
            HSTREG[FLH_PHI_CTRL1+0]= 0xF3;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));


            HSTREG[FLH_PHI_CTRL1+1]= 0x10;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
            HSTREG[FLH_PHI_CTRL1+0]= 0x13;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));


            HSTREG[FLH_PHI_CTRL1+1]= 0x70;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
            HSTREG[FLH_PHI_CTRL1+0]= 0x13;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));


            HSTREG[HOST_PLL_DIV0+0]= 0x10;//0x50;
            HSTREG[HOST_PLL_DIV0+1]= 0x04;//X_CMDBLOCK[3];
            HSTREG[HOST_PLL_DIV0+2]= 0x20;//0x10;


            dly_cnt= 0;
            while(dly_cnt < 50)
            {
                dly_cnt = dly_cnt + 1;
            }

            HSTREG[FLH_PHI_CTRL1+0]= 0xB1;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
        }
        else
        {
            //set  program command + address + dma  0x55 0xAA
            HSTREG[FLH_PHI_CTRL1+3]|= SET_BIT4;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
            HSTREG[FLH_PHI_CTRL1+0]= 0x0A;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
            HSTREG[FLH_PHI_CTRL1+0]= 0x0B;
            while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));

        }

    }


}



static void VC_Trig_Host_Read(void)
{
    U32 dly_cnt=0;

#if FORCE_CALI_MODE
    HOST_PBA_IO_REG[HOST_CALI_CTL]|=HOST_CALI_FORCE_MODE;
#endif

    HSTREG[HOST_PLL_CTRL+1]=HSTREG[HOST_PLL_CTRL+1] | 0x10;

    CLKREG[CLK_GATE] &= ~(MMC_GATE);	//enable MMCR SYS clock


    // chris add CALI clock divide into kHz
    P1 = 0x21;
    HSTREG[FLH_PHI_CTRL3+2] = 0xE0;                            // Set CALI_CLK_DIV[7:0]   = 16'h01E0
    HSTREG[FLH_PHI_CTRL3+3] = 0x01;                            // Set CALI_CLK_DIV[7:0]   = 16'h01E0, set CALI_CLK into 540K
    HSTREG[HOST_CALI_CTRL+3]= HSTREG[HOST_CALI_CTRL+3] | 0x02; // Set CALI_CLK_DIV_LOADEN = 1




    P1 = 0x22;
//

    HSTREG[HOST_CALI_CTRL+0]= HSTREG[HOST_CALI_CTRL+0] | 0xC0;


//select cache to cache 0 instead of MCU ext rame
    //SYSREG[0x29] = 0x0f; // PBA used Cache-14.  //Low part SRAM.

    HSTREG[HOST_PLL_CFG0_0+0]= 0x64;
    HSTREG[HOST_PLL_CFG0_0+1]= 0x09;
    HSTREG[HOST_PLL_CFG0_0+2]= 0x33;
    HSTREG[HOST_PLL_CFG0_0+3]= 0xFB;
    HSTREG[HOST_PLL_CFG0_1+0]= 0x0C;

    HSTREG[HOST_PLL_CFG1_0+0]= 0x20;
    HSTREG[HOST_PLL_CFG1_0+1]= 0x09;
    HSTREG[HOST_PLL_CFG1_0+2]= 0x33;
    HSTREG[HOST_PLL_CFG1_0+3]= 0xFB;
    HSTREG[HOST_PLL_CFG1_1+0]= 0x0C;

    if(X_CMDBLOCK[4]==0)
    {
        //HSTREG[HOST_PLL_DIV0+0]= 0x40;
        HSTREG[HOST_PLL_DIV0+0]= 0x10;//0x50;
        HSTREG[HOST_PLL_DIV0+1]= 0x04;//X_CMDBLOCK[3];
        HSTREG[HOST_PLL_DIV0+2]= 0x20;//0x10;


    }
    else
    {
        //HSTREG[HOST_PLL_DIV0+0]= 0x40;
        HSTREG[HOST_PLL_DIV0+0]= X_CMDBLOCK[3];//0x50;
        HSTREG[HOST_PLL_DIV0+1]= 0x02;
        HSTREG[HOST_PLL_DIV0+2]= 0x10;

    }





    P1 = 0x23;

//drive pll	0
    HSTREG[HOST_PLL_CTRL+1]=HSTREG[HOST_PLL_CTRL+1] | 0x10;
    //HSTREG[0x9]=HSTREG[0x9] | 0x10;
    //pd11_ext V-> pll_dis0 V (larger than 1us) 1T = 8ns
    dly_cnt= 0;
    while(dly_cnt < 125)
    {
        dly_cnt = dly_cnt + 1;
    }
    P1 = 0x24;
    HSTREG[HOST_PLL_CTRL]=HSTREG[HOST_PLL_CTRL] & 0xFE;
    //pll_dis0 V -> pll_rst0(larger than 20us) 1T = 8ns
    //pll_dis0 V -> chpmp_dis0(larger than 20us) 1T = 8ns
    //pll_dis0 V -> envcmid0(larger than 20us) 1T = 8ns
    dly_cnt= 0;
    while(dly_cnt < 2500)
    {
        dly_cnt = dly_cnt + 1;
    }



    P1 = 0x25;
    HSTREG[HOST_PLL_CTRL+1]=HSTREG[HOST_PLL_CTRL+1] | 0x20;
    //poll pll_lock0
    while(! ((HSTREG[HOST_PLL_STAT]&0x04)==0x04));

    HSTREG[RCT_CTRL]= 0x1;

    P1 = 0x26;
    while(! ((HSTREG[HOST_PLL_STAT]&0x04)==0x04));
    HSTREG[RCT_CTRL]= 0xC1;

    P1 = 0x27;
    while(! ((HSTREG[HOST_PLL_STAT]&0x04)!=0x04));
    HSTREG[RCT_CTRL]= 0x31;
    P1 = 0x28;
    while(! ((HSTREG[HOST_PLL_STAT]&0x04)==0x04));

    //MDLL tracking

    SYSREG[HW_UART_OFS]|=MDLL_SWITCH;

    P1 = 0x37;
    HSTREG[DLL_LOAD_CTL+3]= HSTREG[DLL_LOAD_CTL+3] | 0x10;
    HSTREG[DLL_DLL_CTL+0]= 0xE0;
    HSTREG[DLL_DLL_CTL+0]= 0x14;
    P1 = 0x38;
    while(! ((HSTREG[DLL_MDLL_STA]&0x01)==0x01));
    while(! ((HSTREG[DLL_MDLL_STA]&0x01)==0x01));

    HSTREG[HOST_CALI_CTRL+3]= HSTREG[HOST_CALI_CTRL+3] | 0x01;
    P1 = 0x39;


    HSTREG[CRC_CTRL_INT]&=CLR_BIT1;
    HSTREG[CRC_CTRL_INT]|=CRC_CTRL_64_EN;
    HSTREG[PIO_POL_MASK]=0xE0;
    HSTREG[PIO_POL_EXP]=0xE0;

    HSTREG[HOST_DMA_LEN_L]=0x00;
    HSTREG[HOST_DMA_LEN_H]=0x80;

    //HSTREG[CRC_CTRL1]= HSTREG[CRC_CTRL1] & 0xFE;

#if !FORCE_CALI_MODE
    HSTREG[HOST_CALI_CTRL]|=RSTJ_CALI;
    HSTREG[HOST_CALI_CTRL]|=CALI_EN;
    HSTREG[HOST_CALI_LOAD]|=CALI_LOAD;
#endif
    if(gb_Host_MDLL_Mode==0x01)
    {
        HSTREG[DLL_DLL_CTL]|=SET_BIT1;
    }

    //set  Read command + address + dma Seq Pattern
    HSTREG[FLH_PHI_CTRL1+3]|= SET_BIT2;//CE tie0
    while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));

    if(X_CMDBLOCK[4]==0)
    {
        HSTREG[FLH_PHI_CTRL1+1]= 0x00;
        while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
        HSTREG[FLH_PHI_CTRL1+0]= 0x13;
        while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));

        HSTREG[FLH_PHI_CTRL1+1]= 0x00;
        while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
        HSTREG[FLH_PHI_CTRL1+0]= 0x33;
        while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));


        HSTREG[FLH_PHI_CTRL1+1]= 0x00;
        while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
        HSTREG[FLH_PHI_CTRL1+0]= 0x33;
        while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));


        HSTREG[FLH_PHI_CTRL1+1]= 0x00;
        while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
        HSTREG[FLH_PHI_CTRL1+0]= 0x33;
        while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));


        HSTREG[FLH_PHI_CTRL1+1]= 0x00;
        while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
        HSTREG[FLH_PHI_CTRL1+0]= 0x33;
        while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));

        HSTREG[FLH_PHI_CTRL1+1]= 0x00;
        while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
        HSTREG[FLH_PHI_CTRL1+3]= 0x33;
        while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));


        HSTREG[FLH_PHI_CTRL1+1]= 0x30;
        while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
        HSTREG[FLH_PHI_CTRL1+0]= 0x13;
        while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));


        HSTREG[FLH_PHI_CTRL1+1]= 0x70;
        while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
        HSTREG[FLH_PHI_CTRL1+0]= 0x13;
        while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));


        HSTREG[FLH_PHI_CTRL1+0]= 0xB1;
        while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));

        HSTREG[FLH_PHI_CTRL1+1]= 0x00;
        while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
        HSTREG[FLH_PHI_CTRL1+0]= 0x13;
        while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x0));

        HSTREG[FLH_PHI_CTRL1+0]= 0xD1;
        while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x0));


    }
    else
    {
        HSTREG[FLH_PHI_CTRL1+0]= 0x08;
        while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
        HSTREG[FLH_PHI_CTRL1+0]= 0x09;
        while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));
    }



    //HSTREG[FLH_PHI_CTRL1+3]|= SET_BIT3;
    //while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x00));//CE tie1

    /*HSTREG[FLH_PHI_CTRL1+3]|= SET_BIT2;
    while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x0));//CE tie0
       //execute toggle read dma
    HSTREG[FLH_PHI_CTRL1+0]= HSTREG[FLH_PHI_CTRL1+0] & 0xFD;

    HSTREG[FLH_PHI_CTRL1+3]= HSTREG[FLH_PHI_CTRL1+3] | 0x04;

    HSTREG[FLH_PHI_CTRL1+0]= HSTREG[FLH_PHI_CTRL1+0] | 0x01;
    P1 = 0x42;
    while(! ((HSTREG[FLH_PHI_CTRL1+3]&0x80)==0x0));*/

    /*
    P1 = 0x43;
    	if(HSTREG[CRC_CTRL3]!=HSTREG[CRC_CTRL4]){
    	    HSTREG[HOST_DBG+1]= HSTREG[HOST_DBG+1] | 0x1;
    P1 = 0x44;
    	}
    	if(HSTREG[CRC_CTRL3+1]!=HSTREG[CRC_CTRL4+1]){
    	    HSTREG[HOST_DBG+1]= HSTREG[HOST_DBG+1] | 0x1;
    P1 = 0x45;
    	}
    	if(HSTREG[CRC_CTRL3+2]!=HSTREG[CRC_CTRL4+2]){
    	    HSTREG[HOST_DBG+1]= HSTREG[HOST_DBG+1] | 0x1;
    P1 = 0x46;
    	}
    	if(HSTREG[CRC_CTRL3+3]!=HSTREG[CRC_CTRL4+3]){
    	    HSTREG[HOST_DBG+1]= HSTREG[HOST_DBG+1] | 0x1;
    P1 = 0x47;
    	}
        HSTREG[HOST_DBG+1]= HSTREG[HOST_DBG+1] | 0x2;
    P1 = 0x48;
    */

}


static void VC_GET_PBA_REG_By_Cache(void)
{
    SYSREG[SYS_PBA_CASEL_CFG0] = SEL_SYS_INFO;
    USBBulkInSectorFromPageBuffer(FLAG_BASENO_PBA_CACHE, 0x05, 0);
    SYSREG[SYS_PBA_CASEL_CFG0] = SEL_ORI_MAPPING;
    Set_Media_OK();
}






static void VC_Get_Host_REG(void)
{

    U16 j=0;


    SYSREG[SYS_PBA_CASEL_CFG0] = SEL_ORI_MAPPING;

    for(j=0; j<2048; j++)
        TEMP_BUF[j]=HSTREG[j];

    USBBulkInSectorFromPageBuffer(BUF_NUM_TEMP_BUF, 0x04, 0);

}



static void VC_Get_SYSTEM_REG(void)
{

    U16 j=0;


    SYSREG[SYS_PBA_CASEL_CFG0] = SEL_ORI_MAPPING;

    for(j=0; j<2048; j++)
        TEMP_BUF[j]=SYSREG[j];

    USBBulkInSectorFromPageBuffer(BUF_NUM_TEMP_BUF, 0x04, 0);

}

static void VC_Write_SYSTEM_REG(void)
{
    SYSREG[SYS_PBA_CASEL_CFG0] = SEL_ORI_MAPPING;
    USBBulkOutSectorToPageBuffer(BUF_NUM_TEMP_BUF, 1, 0);

    SYSREG[DQ_PU5P5_PU75_PD]=TEMP_BUF[DQ_PU5P5_PU75_PD];
    SYSREG[RE_RSEL_PU_PD]=TEMP_BUF[RE_RSEL_PU_PD];
    SYSREG[RE_RSEL_PU_PD]=TEMP_BUF[RE_RSEL_PU_PD];

}


static void VC_GET_HOST_PBA_IO_REG(void)
{
    U16 j=0;

    SYSREG[SYS_PBA_CASEL_CFG0] = SEL_ORI_MAPPING;

    for(j=0; j<256; j++)
        TEMP_BUF[j]=HOST_PBA_IO_REG[j];

    USBBulkInSectorFromPageBuffer(BUF_NUM_TEMP_BUF, 0x01, 0);
}
static void pba_vc_Set_Ready_Busy(void)
{
    if (!pba_Set_Ready_Busy(X_CMDBLOCK[3], X_CMDBLOCK[4],X_CMDBLOCK[5]))
    {
        Set_Media_OK();
    }
    else
    {
        Set_Media_NG();
    }
}

static void VC_Initial_Emulator(void)
{
     
  
     Set_Media_OK();

    USBREG[CONN_SPD] &= 0x7F;

    //Delay 2 sec for USB host cut off link
    //RTC_Delay((SWORD)2000);
    //Delay 5 sec for USB host cut off link
    RTC_Delay(5000);

  
     CLKREG[CLK_SYSRST] = 0x85;
     
  
     
     

}

static void VC_Get_Cache_Head_REG(void)
{

    U8 i=0,j=0;
    U16 k=0;




    SYSREG[SYS_PBA_CASEL_CFG0] = SEL_ORI_MAPPING;

    for(i=0; i<64; i++)
    {
        PBAREG[CTL_PBA_CACHE_HEADER_SEL] = i;

        for(j=0; j<8; j++)
        {
            TEMP_BUF[k]=PBAREG[CTL_PBA_CACHE_HEADER_DAT0+j];
            k++;
        }

        for(j=0; j<8; j++)
        {
            TEMP_BUF[k]=PBAREG[CTL_PBA_CACHE_HEADER_DAT8+j];
            k++;
        }
    }


    USBBulkInSectorFromPageBuffer(BUF_NUM_TEMP_BUF, 0x02, 0);
}

/// get cache head info by index range
static void VC_Get_Cache_Head_REG_ByRange(void)
{
    U8 i=0,j=0;
    U16 k=0;
    U8 start, end;
    start = X_CMDBLOCK[3];
    end = X_CMDBLOCK[4];

    if((start <= end) && (start < 64) && (end < 64)) {
        SYSREG[SYS_PBA_CASEL_CFG0] = SEL_ORI_MAPPING;
        for(i=start;i<=end;i++) {
            PBAREG[CTL_PBA_CACHE_HEADER_SEL] = i;

            k = (i<<0x04);
            for(j=0; j<8; j++)
            {
                TEMP_BUF[k]=PBAREG[CTL_PBA_CACHE_HEADER_DAT0+j];
                k++;
            }
            for(j=0; j<8; j++)
            {
                TEMP_BUF[k]=PBAREG[CTL_PBA_CACHE_HEADER_DAT8+j];
                k++;
            }
        }
    }

    USBBulkInSectorFromPageBuffer(BUF_NUM_TEMP_BUF, 0x02, 0);
}

static void VC_Get_Add_Info(void)
{

    U8 i=0;
    GPIOREG[GPIO_A_O] = 0xF1;
    SYSREG[SYS_PBA_CASEL_CFG0] = SEL_ORI_MAPPING;

    PBAREG[CTL_ADR_BFU_SRAM_ADDR_SEL] = 0x00;
    PBAREG[CTL_ADR_BUF_SRAM_ADDR_7_0]=X_CMDBLOCK[3];
    PBAREG[CTL_ADR_BUF_SRAM_ADDR_9_8]=X_CMDBLOCK[4];


    for(i=0 ; i<5 ; i++)
        TEMP_BUF[i] = PBAREG[ADR0_BUF0-i];

    GPIOREG[GPIO_A_O] = 0xB1;
    USBBulkInSectorFromPageBuffer(BUF_NUM_TEMP_BUF, 0x01, 0);
    GPIOREG[GPIO_A_O] = 0x00;
}


void pba_vendor_Cmd_isp(void)
{
#if 1
    U8 abyCmdBlk[16];

    memcpy(abyCmdBlk, &X_CMDBLOCK[0], 16);

    switch (abyCmdBlk[1])
    {
        case VC_ISP_FW:
            USBBulkOutSectorToPageBuffer(abyCmdBlk[5], abyCmdBlk[6], 0);   //Recv 512B*SecCnt(abyCmdBlk[6]) data form USB to PRAM(0x00)(abyCmdBlk[5])
            Set_Media_OK();
            NE_VC_Proc = 1;
            gi_DoCSW = 1;
            break;

            //ISP_READ  //Cmd[5]=0x00 Cmd[6]=SecCnt
        case VC_VERIFY_FW:
            USBBulkInSectorFromPageBuffer(abyCmdBlk[5], abyCmdBlk[6], 0);  //Recv 512B*SecCnt(abyCmdBlk[6]) data form USB to PRAM(0x00)(abyCmdBlk[5])
            Set_Media_OK();
            NE_VC_Proc = 1;
            gi_DoCSW = 1;
            break;

        case VC_JUMP_PRAM:
            JUMP_PRAM();
            Set_Media_OK();
            NE_VC_Proc = 1;
            gi_DoCSW = 1;
            break;
    }

#endif
}


void VC_Set_ReWrite_Flash_ID(void)
{
    
   U8 i=0;
   volatile U8* pbyPBACEReg;

   for(i=0;i<_gMax_Ce_Cnt;i++)
   {
      pbyPBACEReg = PBAREG_CE0 + i * 0x100;
      
       pbyPBACEReg[CMD90_ID0] = X_CMDBLOCK[3];
       pbyPBACEReg[CMD90_ID1] = X_CMDBLOCK[4];
       pbyPBACEReg[CMD90_ID2] = X_CMDBLOCK[5];
       pbyPBACEReg[CMD90_ID3] = X_CMDBLOCK[6];
       pbyPBACEReg[CMD90_ID4] = X_CMDBLOCK[7];
       pbyPBACEReg[CMD90_ID5] = X_CMDBLOCK[8];
       pbyPBACEReg[CMD90_ID6] = X_CMDBLOCK[9];
       pbyPBACEReg[CMD90_ID7] = X_CMDBLOCK[10];
   }

}

void VC_Set_Clear_INT_CMD(void)
{
    
   U8 CE=0;
   volatile U8* pbyPBACEReg;

   CE=X_CMDBLOCK[3];
   
   pbyPBACEReg = PBAREG_CE0 + CE * 0x100;
   pbyPBACEReg[CFDE_CLR_ALL_INTR] |= FLAG_CLEAR_INT;

}

/// update status buffer
void pba_set_status_buff(void)
{
    /*
        X_CMDBLOCK[3]: CE
        X_CMDBLOCK[4]: BUF_NO
        X_CMDBLOCK[5]: BUF_VAL
        X_CMDBLOCK[6]: BIT0_MODE
        ...
        X_CMDBLOCK[13]: BIT7_MODE
    */
    unsigned char i=0;
    volatile U8* pbyPBACEReg = PBAREG_CE0 + (X_CMDBLOCK[3] << 0x09u);

    pbyPBACEReg[STA_BUF_DLY_T_NO] = X_CMDBLOCK[4];
    pbyPBACEReg[STA_WR_DLY_T_ENTRY] = X_CMDBLOCK[5];

    pbyPBACEReg[STA_BUF_NO] = X_CMDBLOCK[4];
    pbyPBACEReg[STA_WR_ENTRY] = X_CMDBLOCK[5];
    for(i=0;i<8;i++) {
        pbyPBACEReg[STA0_BIT0+i] = X_CMDBLOCK[6+i];
    }
}

void pba_vc_cmd_dispatch(void)
{
    //U8 abyCmdBlk[16];
    //U16 c;
    U8 byTemp;
    U8 i;

    //memcpy(abyCmdBlk, &X_CMDBLOCK[0], 16);

    switch (X_CMDBLOCK[2]/*abyCmdBlk[2]*/)
    {
		case 0x88:
			pba_vc_get_CFDE_request_NEW();
			break;
						
        case VC_GET_CFDE_REQUEST:
            pba_vc_get_CFDE_request();
            break;

        case VC_USB_BULK_IN:
            pba_vc_usb_bulk_in();
            break;

        case VC_USB_BULK_IN_MULTIPLANE:
            //TODO
            pba_vc_usb_bulk_in_multi_plane();
            break;

        case VC_GET_BUS_ANA_INT_REQUEST:
            pba_vc_get_bus_ana_int_request();
            break;

        case 0x99:
            pba_vc_usb_bulk_out_rdy_page();
            break;

        case VC_SET_RDY_PAGE:
            pba_vc_set_rdy_page();
            break;

        case VC_USB_BULK_OUT:
            pba_vc_usb_bulk_out();
            break;

        case VC_USB_BULK_OUT_MULTIPLANE:
            //TODO
            pba_vc_usb_bulk_out_multi_plane();
            break;

        case VC_EN_DIS_CFDE_UNIT:
            //TODO
            Set_Media_OK();
            break;

        case VC_SET_GPIO_EN_DIS:
            Pba_vc_GPIO_EN_DIS();
            Set_Media_OK();
            break;

        case VC_SET_GPIO_VALUE:
            Pba_vc_Set_GPIO_Value();
            Set_Media_OK();
            break;

        case 0x03:
            //TODO
            Set_Media_OK();
            break;

        case VC_PBA_CACHE_WRITE_TEST:
            //TODO
            Set_Media_OK();
            break;

        case VC_PBA_CACHE_READ_TEST:
            //TODO
            Set_Media_OK();
            break;

        case VC_PBA_DBG_SEL:
            SYSREG[SYS_PBA_CASEL_CFG0] = RAM_DEF;
            PBAREG[CFDE_GPIO_DATA_OUTP] = 0x03;
            PBAREG[CFDE_DBG_SEL] = X_CMDBLOCK[3];
            Set_Media_OK();
            break;

        case VC_GET_TAG_INFO:
            pba_vc_get_Tag_Info();
            break;

        case VC_FILL_CFDE_BLK:
            pba_vc_fill_CFDE_blk();
            break;

        case VC_SET_INFO_PAGE:
            pba_vc_set_info_page();
            break;

        case VC_READ_REQUEST_DATA:
            //TODO
            Set_Media_OK();
            break;

        case VC_SET_FLH_ID:
            //TODO
            Set_Media_OK();
            break;

        case VC_DMA_MEMSET_BUF:
            //TODO
            Set_Media_OK();
            break;

        case VC_DUMP_CFDE_BLK:
            //TODO
            Set_Media_OK();
            break;

        case VC_WR_GN_BUF: //Write general buffer with value
            //TODO
            Set_Media_OK();
            break;

        case VC_RD_GN_BUF: //Read general buffer with value
            //TODO
            Set_Media_OK();
            break;
        case VC_GET_DPS_RAM_DATA:
            pba_vc_get_dps_ram_data();
            Set_Media_OK();
            break;

        case VC_SET_DPS_RAM_DATA:
            pba_vc_Set_dps_ram_data();
            Set_Media_OK();
            break;


        case VC_DMA_MEMCPY_BUF:
            //TODO
            Set_Media_OK();
            break;

        case VC_GET_RDY_PAGE_DATA:
            //TODO
            Set_Media_OK();
            break;

        case VC_GET_SINGLE_BUF_DATA:
            pba_vc_fill_SG_buf_data();
            Set_Media_OK();
            break;

        case VC_GET_PBA_REG:
            pba_vc_get_reg();
            Set_Media_OK();
            break;

        case VC_FORCE_H_IO_STATE:
            //TODO
            Set_Media_OK();
            break;

        case VC_SET_STATUS_BUF:
            //TODO
            pba_set_status_buff();
            Set_Media_OK();
            break;

        case VC_SWITCH_FLH_MODE:
            pba_vc_switch_flash_type();
            Set_Media_OK();
            break;

        case VC_CLR_BUS_ANA_STATUS:
            SYSREG[SYS_PBA_CASEL_CFG0] = RAM_DEF;
            //PBAREG[BUS_ANA_STS] = X_CMDBLOCK[3];
            byTemp = X_CMDBLOCK[3];
            for (i = 0; i < 0x03; i++)
            {
                if ((byTemp >> i) & 0x01)
                {
                    PBAREG[BUS_ANA_STS] &= ~(DONE0 << i);
                }
            }

            //Clear Bus Ana interrupt
            PBAREG[CLEAR_BUS_ANA_INT] |= FLAG_CLEAR_BUS_ANA;
            Set_Media_OK();
            break;

        case VC_CLR_PWR_CHG_INT:
            //TODO
            Set_Media_OK();
            break;

        case VC_GET_SYS_REG_INFO:
            //TODO
            Set_Media_OK();
            break;

        case VC_SET_SYS_REG_INFO:
            //TODO
            Set_Media_OK();
            break;

        case VC_GET_BUS_ANA_CC19_TEST:
            //TODO
            Set_Media_OK();
            break;

        case VC_GET_INVAILD_CFDE_REQ:
            //TODO
            Set_Media_OK();
            break;
        case VC_GET_CACHE_HEADER_INF0:
            pba_vc_fill_Cache_Header();
            Set_Media_OK();
            break;

        case VC_SET_READY_BUSY:
            pba_vc_Set_Ready_Busy();
            Set_Media_OK();
            break;
        case VC_Covertion:
            pba_vc_Convertion();
            Set_Media_OK();
            break;
        case VC_SET_IO_DRVING:
            VC_Set_Io_Driving();
            Set_Media_OK();
            break;
        case VC_TRIG_HOST_WRITE:

            VC_Trig_Host_Write();

            break;
        case VC_GET_PBA_REG_Cache:
            VC_GET_PBA_REG_By_Cache();
            break;

        case VC_GET_HOST_REG:
            VC_Get_Host_REG();
            break;
        case VC_TRIG_HOST_READ:
            VC_Trig_Host_Read();
            Set_Media_OK();
            break;

        case VC_GET_SYS_REG:
            VC_Get_SYSTEM_REG();
            Set_Media_OK();
            break;

        case VC_WR_SYS_REG:
            VC_Write_SYSTEM_REG();
            Set_Media_OK();
            break;
        case VC_GET_IO_REG:
            VC_GET_HOST_PBA_IO_REG();
            Set_Media_OK();
            break;
        case VC_Set_Differential_DQS:
            VC_Set_Differential_DQS_IO_REG();
            Set_Media_OK();
            break;
        case VC_Reset_NE_2G:
            VC_Initial_Emulator();
            break;
        case VC_GET_CACHE_HEAD:
            VC_Get_Cache_Head_REG();
            Set_Media_OK();
            break;
        case VC_Get_Add_From_SRAM:
            VC_Get_Add_Info();
            Set_Media_OK();
            break;
        case VC_SET_REWRITE_FLASH_ID:
            VC_Set_ReWrite_Flash_ID();
            Set_Media_OK();
            break;

        case VC_SET_CLEAR_INT_CMD:
            VC_Set_Clear_INT_CMD();
            Set_Media_OK();
            break;
        case VC_GET_CH_INFO_BY_RANGE:
            VC_Get_Cache_Head_REG_ByRange();
            Set_Media_OK();
            break;

    }
}