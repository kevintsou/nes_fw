/**
 * @file pba.c
 * Copyrihgt 2015 PHISON Electronics Corp.
 *
 * <b>
@verbatim
FILE NAME :  pba.c
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

#include "nes.h"
#ifdef D_NES_CMODEL
#include "8051.h"
#else
#include <reg52.h>
#include <8051.h>
#endif

#define __PBA_C__
 xdata unsigned char _gMax_Ce_Cnt;
 xdata unsigned char _gMax_Lun_Cnt;
 xdata unsigned char _gMax_Plane_Cnt;
 xdata unsigned char Flash_1V2;
 xdata unsigned char AIPR_EN;
 xdata unsigned char drive_config[4];

/*--------------------------------------------------------------------------*/
/* INCLUDE FILE                                                             */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "MCU_Reg.h"
#include "Var_def.h"
#include "Buf_DEF.h"
#include "UsbReg.h"
#include "USB_API.h"
#include "UART_API.h"
#include "GPIO_Reg.h"
#include "pba.h"
#include "pba_api.h"
#include "pba_vc_api.h"
#include "DMA_Reg.h"
#include "Interrupt_API.h"
#include "HOST.h"
#include "pba_vc.h"
/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTION IMPLEMENT                                                 */
/*--------------------------------------------------------------------------*/
static void pba_set_dpsram_lun_bit(void)
{
    volatile U8* pbyPBACEReg;
    U8  byCeIdx;
    U8  i;

    for (byCeIdx = 0; byCeIdx < _gMax_Ce_Cnt; byCeIdx++) {
        pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;     
        
        for (i = 0; i < 0x03; i++) {
            pbyPBACEReg[DPS_SEL_LUN_BIT0 + i] = INFO_PAGE_Base[IP_DPS_RAM_LUN_SEL_PA0 + i];
        }           
    }
}

static void pba_set_badcolumn_base_ptr(void)
{
    volatile U8* pbyPBACEReg;
    U8  byCeIdx;
    
    for (byCeIdx = 0; byCeIdx < _gMax_Ce_Cnt; byCeIdx++) {
        pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;     
        pbyPBACEReg[BAD_COL_BASE_ADR] = INFO_PAGE_Base[IP_BAD_COLUM_BASE_PTR_CE0 + byCeIdx];                   
    }
}    

static void pba_set_addrbuf_cacheheader_lun_plane_bit(void)
{
    volatile U8* pbyPBACEReg;
    U8  byCeIdx;
    U8  i;

    for (byCeIdx = 0; byCeIdx < _gMax_Ce_Cnt; byCeIdx++) {
        pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;

        for (i = 0; i < 0x04; i++) {
            pbyPBACEReg[PLANE_SEL_PA0 + i] = INFO_PAGE_Base[IP_PLANE_SEL_PA0 + i];
        }
        pbyPBACEReg[LUN_SEL_PA1] = INFO_PAGE_Base[IP_LUN_SEL_PA1];
        pbyPBACEReg[LUN_SEL_PA2] = INFO_PAGE_Base[IP_LUN_SEL_PA2];      
    }
}

static void pba_set_wr_rd_cache_lun_plane_bit(void)
{
    volatile U8* pbyPBACEReg;
    U8  byCeIdx;
    U8  i;

    for (byCeIdx = 0; byCeIdx < _gMax_Ce_Cnt; byCeIdx++) {
        pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;

        //Write Cache PA_SEL
        for (i = 0; i < 0x04; i++) {
            pbyPBACEReg[CC_SEL_PA0 + i] = INFO_PAGE_Base[IP_WR_CACHE_SEL_PA0 + i + byCeIdx * CE_CACHE_SEL_LEN];
        }
        pbyPBACEReg[CC_SEL_PA4] = INFO_PAGE_Base[IP_WR_CACHE_SEL_PA4 + byCeIdx * CE_CACHE_SEL_LEN];
        pbyPBACEReg[CC_SEL_PA5] = INFO_PAGE_Base[IP_WR_CACHE_SEL_PA5 + byCeIdx * CE_CACHE_SEL_LEN];

        //Read Cache PA_SEL
        for (i = 0; i < 0x06; i++)
            pbyPBACEReg[RD_CACHE_SEL_PA0 + i] = INFO_PAGE_Base[IP_RD_CACHE_SEL_PA0 + i + byCeIdx * CE_CACHE_SEL_LEN];
    }
}

static void pba_set_status_buf_lun_plane_bit(void)
{
    volatile U8* pbyPBACEReg;
    U8  byCeIdx;
    U8  i;

    for (byCeIdx = 0; byCeIdx < _gMax_Ce_Cnt; byCeIdx++) {
        pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;

        //Enahnce 71Cmd STA_SEL
        for (i = 0; i < 0x03; i++)
            pbyPBACEReg[STA_SEL_PA0_ENH + i] = INFO_PAGE_Base[IP_EN71CMD_STA_SEL_PA0 + i + byCeIdx * CE_STA_SEL_LEN];

        //Cmd78 STA_SEL
        for (i = 0; i < 0x06; i++)
            pbyPBACEReg[STA_SEL_PA0 + i] = INFO_PAGE_Base[IP_78CMD_STA_SEL_PA0 + i + byCeIdx * CE_STA_SEL_LEN];
    }
}

static void pba_set_rd_trans_size(void)
{
    volatile U8* pbyPBACEReg;
    U8  byCeIdx;

    for (byCeIdx = 0; byCeIdx < _gMax_Ce_Cnt; byCeIdx++) {
        pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;
        pbyPBACEReg[RD_XFER_NO_H] = INFO_PAGE_Base[IP_RD_XFER_NO_H_OFS];
        pbyPBACEReg[RD_XFER_NO_L] = INFO_PAGE_Base[IP_RD_XFER_NO_L_OFS];     
    }
}

U8 pba_convert_value(U8 byValue)
{
    U8 byReturnValue;

    if (byValue == 0x08)
        byReturnValue = 0x03;
    else if (byValue == 0x04)
        byReturnValue = 0x02;
    else if (byValue == 0x02)
        byReturnValue = 0x01;
    else
        byReturnValue = 0x00;

    return byReturnValue;
}

void pba_set_support_ce_lun_plane(void)
{
    U8 byCeType, byLunType, byPlaneType;

    _gMax_Ce_Cnt    = INFO_PAGE_Base[IP_MAX_CE_CNT_OFS];
    _gMax_Lun_Cnt   = INFO_PAGE_Base[IP_MAX_LUN_CNT_OFS];
    _gMax_Plane_Cnt = INFO_PAGE_Base[IP_MAX_PLANE_CNT_OFS];

    byCeType    = pba_convert_value(_gMax_Ce_Cnt);
    byLunType   = pba_convert_value(_gMax_Lun_Cnt);
    byPlaneType = pba_convert_value(_gMax_Plane_Cnt);

    PBAREG[EN_ALL_CE_WP_LOCK] &= REF_CE_NUM;
       
    PBAREG[SET_CE_LUN_PLANE] = 0x00;

    if(AIPR_EN)
        byPlaneType=0x00;
    
    PBAREG[SET_CE_LUN_PLANE] |= ((byCeType << 4) | (byLunType << 2) | byPlaneType);
    
    
    
}

/// reset to default driving setting from info page
void pba_reset_default_drive_setting(void)
{
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

/// add trigger mode and driving strength setting
void pba_set_driving_reg(void)
{
    // reset to system default trigger mode and driving
    SYSREG[PBA_WE_CONOF] = 0x15;
    SYSREG[PBA_REB_CONOF] = 0x15;
    SYSREG[PBA_DQS_CONOF] = 0x15;
    SYSREG[PBA_DAT_CONOF] = 0x15;

    HOST_PBA_IO_REG[PBA_WEB] &= (CLR_BIT2&CLR_BIT3&CLR_BIT4);
    HOST_PBA_IO_REG[PBA_REB] &= (CLR_BIT2&CLR_BIT3);
    HOST_PBA_IO_REG[PBA_REB_CAL_N] &= CLR_BIT6;
    HOST_PBA_IO_REG[PBA_DQS] &= (CLR_BIT2&CLR_BIT3);
    HOST_PBA_IO_REG[PBA_DQS_CAL_N] &= CLR_BIT6;
    HOST_PBA_IO_REG[PBA_HIO] &= (CLR_BIT2&CLR_BIT3&CLR_BIT4);

    // save driving info
    drive_config[DRIVE_CONFIG_RE_TRIGGER] = INFO_PAGE_Base[IP_REB_CFG0];
    drive_config[DRIVE_CONFIG_DQS_STRENGTH] = INFO_PAGE_Base[IP_DQS_CFG1];
    drive_config[DRIVE_CONFIG_DQ_STRENGTH] = INFO_PAGE_Base[IP_DAT_CFG1];

    // set trigger mode cmos trigger if 0x01, schmitt trigger if 0x02
    if(INFO_PAGE_Base[IP_WEB_CFG0] == SET_BIT1) {
        SYSREG[PBA_WE_CONOF] = (SYSREG[PBA_WE_CONOF]&CLR_BIT0)|SET_BIT1;
    }
    if(INFO_PAGE_Base[IP_REB_CFG0] == SET_BIT1) {
        SYSREG[PBA_REB_CONOF] = (SYSREG[PBA_REB_CONOF]&CLR_BIT0)|SET_BIT1;
    }
    // set driving strength
    if(INFO_PAGE_Base[IP_WEB_CFG1] != 0x00) {
        //cfg = INFO_PAGE_Base[IP_WEB_CFG1];
        HOST_PBA_IO_REG[PBA_WEB] |= ((INFO_PAGE_Base[IP_WEB_CFG1]&(SET_BIT0|SET_BIT1|SET_BIT2)) << 2);
    }
    if(INFO_PAGE_Base[IP_REB_CFG1] != 0x00) {
        //cfg = INFO_PAGE_Base[IP_REB_CFG1];
        HOST_PBA_IO_REG[PBA_REB] |= ((INFO_PAGE_Base[IP_REB_CFG1]&(SET_BIT0|SET_BIT1)) << 2);
        HOST_PBA_IO_REG[PBA_REB_CAL_N] |= ((INFO_PAGE_Base[IP_REB_CFG1]&SET_BIT2) << 4);
    }
}

static void pba_initial_pba_reg(void)
{
    
    //IOW F000 80, PBA RST
    PBAREG[CRBYTE0] = 0x80;//0x80;
    //default disable WP/LockUnlock function 
    PBAREG[EN_ALL_CE_WP_LOCK] &= ~(EN_LOCK_UNLOCK_FUN | EN_WP_FUN);


    PBAREG[0x2D] |= SET_BIT4;//0x80;

    //PS8318_JGS_12V 

    if(INFO_PAGE_Base[RDY_PG_OFS_FLASH_IO_VOLTAGE]==0x01)
    {
   
    //PBAREG[0x7AF]=0x08;
    

        HOST_PBA_IO_REG[0x00]|=PMODE_12V;
        HOST_PBA_IO_REG[0x02]=0x26;
        HOST_PBA_IO_REG[0x03]=0x26;

        HOST_PBA_IO_REG[0x08]|=PMODE_12V;
        HOST_PBA_IO_REG[0x0A]=0x26;
        HOST_PBA_IO_REG[0x0B]=0x4D;


        HOST_PBA_IO_REG[PBA_RDY]|=PMODE_12V;
        HOST_PBA_IO_REG[PBA_RDY_CAL_N]= 0x26;
        HOST_PBA_IO_REG[PBA_RDY_CAL_P]= 0x26;  


        HOST_PBA_IO_REG[PBA_DQS]|=PMODE_12V;    
        HOST_PBA_IO_REG[PBA_DQS_CAL_N]= 0x26;
        HOST_PBA_IO_REG[PBA_DQS_CAL_P]= 0x26;   

        
        HOST_PBA_IO_REG[PBA_HIO]|=PMODE_12V;
        HOST_PBA_IO_REG[PBA_HIO_CAL_N]= 0x26;
        HOST_PBA_IO_REG[PBA_HIO_CAL_P]= 0x26;
       
       

        HOST_PBA_IO_REG[PBA_CLE]|=PMODE_12V;
        HOST_PBA_IO_REG[PBA_CLE_CAL_N]= 0x26;
        HOST_PBA_IO_REG[PBA_CLE_CAL_P]= 0x26; 
        

        HOST_PBA_IO_REG[PBA_ALE]|=PMODE_12V;
        HOST_PBA_IO_REG[PBA_ALE_CAL_N]= 0x26;
        HOST_PBA_IO_REG[PBA_ALE_CAL_P]= 0x26; 
        
        
        HOST_PBA_IO_REG[PBA_REB]|=PMODE_12V;
        HOST_PBA_IO_REG[PBA_REB_CAL_N]= 0x26;
        HOST_PBA_IO_REG[PBA_REB_CAL_P]= 0x26; 
       
      
        HOST_PBA_IO_REG[PBA_WEB]|=PMODE_12V;
        HOST_PBA_IO_REG[PBA_WEB_CAL_N]= 0x26;
        HOST_PBA_IO_REG[PBA_WEB_CAL_P]= 0x26; 
        
       
        HOST_PBA_IO_REG[PBA_CE]|=PMODE_12V;
        HOST_PBA_IO_REG[PBA_CE_CAL_N]= 0x26;
        HOST_PBA_IO_REG[PBA_CE_CAL_P]= 0x26; 


    //HOST_PBA_IO_REG[PBA_HIO]|=SET_BIT3;
    //HOST_PBA_IO_REG[PBA_DQS]|=SET_BIT3;
    
        Flash_1V2=0x01;
    
    }
    else
    {
        
        HOST_PBA_IO_REG[0x00]&=PMODE_18V;
        HOST_PBA_IO_REG[0x02]=0x18;
        HOST_PBA_IO_REG[0x03]=0x18;

        HOST_PBA_IO_REG[0x08]&=PMODE_18V;
        HOST_PBA_IO_REG[0x0A]=0x18;
        HOST_PBA_IO_REG[0x0B]=0x31;


        HOST_PBA_IO_REG[PBA_RDY]&=PMODE_18V;
        HOST_PBA_IO_REG[PBA_RDY_CAL_N]= 0x18;
        HOST_PBA_IO_REG[PBA_RDY_CAL_P]= 0x18;  


        HOST_PBA_IO_REG[PBA_DQS]&=PMODE_18V;    
        HOST_PBA_IO_REG[PBA_DQS_CAL_N]= 0x18;
        HOST_PBA_IO_REG[PBA_DQS_CAL_P]= 0x18;   

        
        HOST_PBA_IO_REG[PBA_HIO]&=PMODE_18V;
        HOST_PBA_IO_REG[PBA_HIO_CAL_N]= 0x18;
        HOST_PBA_IO_REG[PBA_HIO_CAL_P]= 0x18;
       
       

        HOST_PBA_IO_REG[PBA_CLE]&=PMODE_18V;
        HOST_PBA_IO_REG[PBA_CLE_CAL_N]= 0x18;
        HOST_PBA_IO_REG[PBA_CLE_CAL_P]= 0x18; 
        

        HOST_PBA_IO_REG[PBA_ALE]&=PMODE_18V;
        HOST_PBA_IO_REG[PBA_ALE_CAL_N]= 0x18;
        HOST_PBA_IO_REG[PBA_ALE_CAL_P]= 0x18; 
        
        
        HOST_PBA_IO_REG[PBA_REB]&=PMODE_18V;
        HOST_PBA_IO_REG[PBA_REB_CAL_N]= 0x18;
        HOST_PBA_IO_REG[PBA_REB_CAL_P]= 0x18; 
       
      
        HOST_PBA_IO_REG[PBA_WEB]&=PMODE_18V;
        HOST_PBA_IO_REG[PBA_WEB_CAL_N]= 0x18;
        HOST_PBA_IO_REG[PBA_WEB_CAL_P]= 0x18; 
        
       
        HOST_PBA_IO_REG[PBA_CE]&=PMODE_18V;
        HOST_PBA_IO_REG[PBA_CE_CAL_N]= 0x18;
        HOST_PBA_IO_REG[PBA_CE_CAL_P]= 0x18; 

        SYSREG[PBA_DAT_CONOF]&=CLR_BIT1;
        SYSREG[PBA_REB_CONOF]&=CLR_BIT1;
        SYSREG[PBA_DQS_CONOF]&=CLR_BIT1;

        HOST_PBA_IO_REG[PBA_HIO]&=CLR_BIT2;
        HOST_PBA_IO_REG[PBA_HIO]&=CLR_BIT3;
        HOST_PBA_IO_REG[PBA_HIO]&=CLR_BIT4;

        HOST_PBA_IO_REG[PBA_DQS]&=CLR_BIT2;
        HOST_PBA_IO_REG[PBA_DQS]&=CLR_BIT3;
        HOST_PBA_IO_REG[PBA_DQS_CAL_N]&=CLR_BIT6;

        Flash_1V2=0x00;
    }

#if FORCE_CALI_MODE
    PBAREG[CRBYTE0]|=PBA_CALI_FORCE_MODE;
#else
    PBAREG[CRBYTE0]|=PBA_CALI_MODE;
#endif

}

static void pba_fill_specified_flash_id(void)
{
    volatile U8* pbyPBACEReg;
    U8 i;


      for (i = 0; i < MAX_CE_CNT; i++) {
        pbyPBACEReg = PBAREG_CE0 + i * 0x100;
      
        pbyPBACEReg[CMD90_ID0_3] = 0x00;
        pbyPBACEReg[CMD90_ID1_3] = 0x00;
        pbyPBACEReg[CMD90_ID2_3] = 0x00;
        pbyPBACEReg[CMD90_ID3_3] = 0x00;
        pbyPBACEReg[CMD90_ID4_3] = 0x00;
        pbyPBACEReg[CMD90_ID5_3] = 0x00;
        pbyPBACEReg[CMD90_ID6_3] = 0x00;
        pbyPBACEReg[CMD90_ID7_3] = 0x00;

   
        pbyPBACEReg[CMD90_ID0_2] = 0x00;
        pbyPBACEReg[CMD90_ID1_2] = 0x00;
        pbyPBACEReg[CMD90_ID2_2] = 0x00;
        pbyPBACEReg[CMD90_ID3_2] = 0x00;
        pbyPBACEReg[CMD90_ID4_2] = 0x00;
        pbyPBACEReg[CMD90_ID5_2] = 0x00;
        pbyPBACEReg[CMD90_ID6_2] = 0x00;
        pbyPBACEReg[CMD90_ID7_2] = 0x00;

    
        pbyPBACEReg[CMD90_ID0] = 0x00;
        pbyPBACEReg[CMD90_ID1] = 0x00;
        pbyPBACEReg[CMD90_ID2] = 0x00;
        pbyPBACEReg[CMD90_ID3] = 0x00;
        pbyPBACEReg[CMD90_ID4] = 0x00;
        pbyPBACEReg[CMD90_ID5] = 0x00;
        pbyPBACEReg[CMD90_ID6] = 0x00;
        pbyPBACEReg[CMD90_ID7] = 0x00;
    
      }
    

    for (i = 0; i < _gMax_Ce_Cnt; i++) {
        pbyPBACEReg = PBAREG_CE0 + i * 0x100;

        /* Fill flash ID for 90_40 command */
        //PBAREG[CFDE_DBG_SEL] = 0x02;
        pbyPBACEReg[CMD90_ID0_3] = INFO_PAGE_Base[IP_FLASH_ID_0x40_OFS];
        pbyPBACEReg[CMD90_ID1_3] = INFO_PAGE_Base[IP_FLASH_ID_0x40_OFS + 1];
        pbyPBACEReg[CMD90_ID2_3] = INFO_PAGE_Base[IP_FLASH_ID_0x40_OFS + 2];
        pbyPBACEReg[CMD90_ID3_3] = INFO_PAGE_Base[IP_FLASH_ID_0x40_OFS + 3];
        pbyPBACEReg[CMD90_ID4_3] = INFO_PAGE_Base[IP_FLASH_ID_0x40_OFS + 4];
        pbyPBACEReg[CMD90_ID5_3] = INFO_PAGE_Base[IP_FLASH_ID_0x40_OFS + 5];
        pbyPBACEReg[CMD90_ID6_3] = INFO_PAGE_Base[IP_FLASH_ID_0x40_OFS + 6];
        pbyPBACEReg[CMD90_ID7_3] = INFO_PAGE_Base[IP_FLASH_ID_0x40_OFS + 7];

        /* Fill flash ID for 90_20 command */
        //PBAREG[CFDE_DBG_SEL] = 0x01;
        pbyPBACEReg[CMD90_ID0_2] = INFO_PAGE_Base[IP_FLASH_ID_0x20_OFS];
        pbyPBACEReg[CMD90_ID1_2] = INFO_PAGE_Base[IP_FLASH_ID_0x20_OFS + 1];
        pbyPBACEReg[CMD90_ID2_2] = INFO_PAGE_Base[IP_FLASH_ID_0x20_OFS + 2];
        pbyPBACEReg[CMD90_ID3_2] = INFO_PAGE_Base[IP_FLASH_ID_0x20_OFS + 3];
        pbyPBACEReg[CMD90_ID4_2] = INFO_PAGE_Base[IP_FLASH_ID_0x20_OFS + 4];
        pbyPBACEReg[CMD90_ID5_2] = INFO_PAGE_Base[IP_FLASH_ID_0x20_OFS + 5];
        pbyPBACEReg[CMD90_ID6_2] = INFO_PAGE_Base[IP_FLASH_ID_0x20_OFS + 6];
        pbyPBACEReg[CMD90_ID7_2] = INFO_PAGE_Base[IP_FLASH_ID_0x20_OFS + 7];

        /* Fill flash ID for 90_00 command */
        //PBAREG[CFDE_DBG_SEL] = 0x00;
        pbyPBACEReg[CMD90_ID0] = INFO_PAGE_Base[IP_FLASH_ID_OFS];
        pbyPBACEReg[CMD90_ID1] = INFO_PAGE_Base[IP_FLASH_ID_OFS + 1];
        pbyPBACEReg[CMD90_ID2] = INFO_PAGE_Base[IP_FLASH_ID_OFS + 2];
        pbyPBACEReg[CMD90_ID3] = INFO_PAGE_Base[IP_FLASH_ID_OFS + 3];
        pbyPBACEReg[CMD90_ID4] = INFO_PAGE_Base[IP_FLASH_ID_OFS + 4];
        pbyPBACEReg[CMD90_ID5] = INFO_PAGE_Base[IP_FLASH_ID_OFS + 5];
        pbyPBACEReg[CMD90_ID6] = INFO_PAGE_Base[IP_FLASH_ID_OFS + 6];
        pbyPBACEReg[CMD90_ID7] = INFO_PAGE_Base[IP_FLASH_ID_OFS + 7];
    }
}


void pba_initial_sys_reg(void)
{

    
    // IOW F121 00, enable IP rst
    CLKREG[CLK_GATE] = 0x00;

    //IOW F124 01, enable IP rst
    CLKREG[CLK_GATE1] = 0x01;
    //CLKREG[CLK_GATE1] |= UHS2_GATE;
 

    //IOW F12F 20, PLL EN
    CLKREG[CLK_WUCNT] = 0x20;
    //CLKREG[CLK_WUCNT] |= PLL_ENB;

    //IOW F180 55, test wr
    //???
    DMAREG[_IN DMA_SRHB_L] = 0x55;


    
    
}


BOOL pba_set_info_page(void)
{
    unsigned int c = 0,i=0;
    U8 byCacheMode;
    U8 byCeIdx;
    volatile U8* pbyPBACEReg;

    AIPR_EN=0;

    /* Check info page mark */
    if ((INFO_PAGE_Base[IP_HEADER0_OFS] != IP_HEADER0_VAL) || (INFO_PAGE_Base[IP_HEADER1_OFS] != IP_HEADER1_VAL)) {
        return FAIL;
    }


    AIPR_EN=INFO_PAGE_Base[IP_AIPR_EN];
   
    
    pba_initial_sys_reg();

    pba_initial_pba_reg();

    // set trigger mode and driving strength from info page
    pba_set_driving_reg();

    /* Get and Set support CE/LUN/PLANE */
    pba_set_support_ce_lun_plane();

    pba_fill_specified_flash_id();

    /* Set flash type and page size */

    for (byCeIdx = 0; byCeIdx <  _gMax_Ce_Cnt; byCeIdx++) {
        pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;
        pbyPBACEReg[CRBYTE0_CE] &= MASK_MODE;
        pbyPBACEReg[CRBYTE0_CE] |=(INFO_PAGE_Base[IP_FLASH_TYPE_OFS] << FLASH_TYPE_SFT_BIT);

        pbyPBACEReg[CFDE_CLR_ALL_INTR] |= FLAG_CLEAR_INT;
        pbyPBACEReg[CFDE_TAG_99_96] |= CLR_CFDE_TAG;
    }


    PBAREG[PAGE_SIZE] = INFO_PAGE_Base[IP_PAGE_SIZE_OFS];

    /* Set which cache mode */
    byCacheMode = INFO_PAGE_Base[IP_CACHE_MODE_OFS];
    if (byCacheMode == CACHE_MODE_RR) {
        PBAREG[RR_CACHE] |= SET_RR_MODE;
    } else if (byCacheMode == CACHE_MODE_HW_ADDR) {
        PBAREG[RR_CACHE] &= SET_FW_OR_HWADDR_MODE;
        PBAREG[WR_CACHE] |= (HW_WR_CCMODE_SEL | FORCE_WR_CACHE_MODE_SEL);
        PBAREG[RD_CACHE] |= (HW_RD_CCMODE_SEL | FORCE_RD_CACHE_MODE_SEL);
        PBAREG[CTL_ADR_BFU_SRAM_ADDR_SEL] = 0x01; 
        /* Select page buffer by PA(Page Address) */
        pba_set_wr_rd_cache_lun_plane_bit();
        pba_set_addrbuf_cacheheader_lun_plane_bit();
        pba_set_dpsram_lun_bit();
        
    } else if (byCacheMode == CACHE_MODE_SW) {
        PBAREG[RR_CACHE] &= SET_FW_OR_HWADDR_MODE;
        PBAREG[WR_CACHE] &= SW_WR_CCMODE_SEL;
        PBAREG[RD_CACHE] &= SW_RD_CCMODE_SEL;
    }

    //initial cache buffer PTR 
    PBAREG[RR_CACHE]|=SET_CTL_RR_CACHE_OFFSET_PTR;

    if (INFO_PAGE_Base[IP_EN_BAD_COL_FUN] == EN_BAD_COL_FUN) {
        pba_set_badcolumn_base_ptr();        
    } 
    //else {
        PBAREG[RR_CACHE] |= SET_64SETS_CACHE_BUF;
   // }

    if (INFO_PAGE_Base[IP_EN_LOCK_UNLOCK_FUN] == 0x01) {
        PBAREG[EN_ALL_CE_WP_LOCK] |= EN_LOCK_UNLOCK_FUN;
    }

    if (INFO_PAGE_Base[IP_EN_WP_FUN] == 0x01) {
        PBAREG[EN_ALL_CE_WP_LOCK] |= EN_WP_FUN;
    }

    if(INFO_PAGE_Base[IP_MASK_RSTN_BSY_EN]==0x01)
    {
        PBAREG[PWR_CHG_INT]&=MASK_RSTN_BSY;
        //PBAREG[PWR_CHG_INT]|=SET_BIT2;
    }
        

    if(INFO_PAGE_Base[IP_COV_EN]==0x01)
    {
        for(c=0;c<0x28;c++)
            COVREG[COV_MODE+c]|=INFO_PAGE_Base[IP_COV_MODE_BYTE0_OFS+c];       
    }

    //HW debug mode
    //PBAREG[CFDE_DBG_SEL] = INFO_PAGE_Base[IP_HW_DBG_MODE];    

    /* Fill default value of status buffer */
    pba_initial_status_buf();

    /* Status buffer select by PA(Page Address) */
    pba_set_status_buf_lun_plane_bit();

    //clear int and set ready for all CE
    pba_clear_allceint_and_setallrdy();

    /* Set Read transaction size */
    pba_set_rd_trans_size();

		
    /* Eanble ANA related reg setting */
    if (INFO_PAGE_Base[IP_BUS_ANA_EN] & 0x01)
    {
      PBAREG[BUS_ANA_EN]     = INFO_PAGE_Base[IP_BUS_ANA_EN];
      PBAREG[BUS_ANA_THRE_L] = INFO_PAGE_Base[IP_BUS_ANA_THRE_L];
      PBAREG[BUS_ANA_THRE_H] = INFO_PAGE_Base[IP_BUS_ANA_THRE_H];
      PBAREG[CRBYTE0]       |= S_H_IO_IN_EN;

      if (INFO_PAGE_Base[IP_BUS_ANA_STACK_MODE_EN] == 0x01) {
          PBAREG[PBA_INT_EN] |= EN_BUS_ANA_STACK_MODE;
      } 

      if(INFO_PAGE_Base[IP_DPS_STACKING_MODE]==0x01)
      {
         for (byCeIdx = 0; byCeIdx <  _gMax_Ce_Cnt; byCeIdx++) 
         {
           pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100; 
           pbyPBACEReg[CFDE_CLR_ALL_INTR]|=EN_DPS_STACK_MODE;
         }  
      }     
    }

    if (INFO_PAGE_Base[IP_PWR_DET_EN] == PWR_DET_EN) {
        PBAREG[PWR_CHG_INT] |= CHG_INT_EN;        
    }

    PBAREG[TOG_READ_ID_TR]&=SET_TOG_READ_ID_TR_CNT;


    //clear temp tag
    PBAREG[RR_CACHE] |= SET_CLR_CFDE_TAG_ALL;
    PBAREG[RR_CACHE] &= FORCE_CLR_CFDE_TAG_ALL_DEFAUL;

    //clear Cache Head Valid info
    for (i = 0; i < 0x08; i++) {
        PBAREG[CLR_CACHE_7_0_HEADER + i] = 0xFF;
    }
    //PBAREG[CFDE_DBG_SEL] = 0x12;

		

    
    //Clear CFDE unit Write count 
    PBAREG[CFDE_EN_7_0]=0x00;
    PBAREG[CFDE_EN_15_8]=0x00;
    PBAREG[CFDE_EN_23_16]=0x00;
    PBAREG[CFDE_EN_31_24]=0x00;
    PBAREG[CFDE_EN_39_32]=0x00;
    PBAREG[CFDE_EN_47_40]=0x00;
    PBAREG[CFDE_EN_55_48]=0x00;
    PBAREG[CFDE_EN_63_56]=0x00;
    PBAREG[CFDE_EN_71_64]=0x00;
    PBAREG[CFDE_EN_79_72]=0x00;
    PBAREG[CFDE_EN_87_80]=0x00;
    PBAREG[CFDE_EN_95_88]=0x00;
    PBAREG[CFDE_EN_99_96]=0x00;


    
   
    return PASS;
}


int pba_fill_CFDE_blk(void)
{

    U8 setno = 0, c = 0,MAX_CFDE_Depth=0;

   

  /* Enable CFDE unit */
    if (CFDE_BLK_Base[CFDE_BLK_UNITNO] >= 0 && CFDE_BLK_Base[CFDE_BLK_UNITNO] <= 7) {
        PBAREG[CFDE_EN_7_0] &= ~((1 << CFDE_BLK_Base[CFDE_BLK_UNITNO])&0xFF);
        PBAREG[CFDE_EN_7_0] |= (1 << CFDE_BLK_Base[CFDE_BLK_UNITNO]);
    }

    if (CFDE_BLK_Base[CFDE_BLK_UNITNO] >= 8 && CFDE_BLK_Base[CFDE_BLK_UNITNO] <= 15) {
        PBAREG[CFDE_EN_15_8] &= ~((1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO]-8))&0xFF);
        PBAREG[CFDE_EN_15_8] |= (1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO] - 8));
    }

    if (CFDE_BLK_Base[CFDE_BLK_UNITNO] >= 16 && CFDE_BLK_Base[CFDE_BLK_UNITNO] <= 23) {
         PBAREG[CFDE_EN_23_16] &= ~((1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO]-16))&0xFF);
        PBAREG[CFDE_EN_23_16] |= (1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO] - 16));
    }

    if (CFDE_BLK_Base[CFDE_BLK_UNITNO] >= 24 && CFDE_BLK_Base[CFDE_BLK_UNITNO] <= 31) {
         PBAREG[CFDE_EN_31_24] &= ~((1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO]-24))&0xFF);
        PBAREG[CFDE_EN_31_24] |= (1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO] - 24));
    }

    if (CFDE_BLK_Base[CFDE_BLK_UNITNO] >= 32 && CFDE_BLK_Base[CFDE_BLK_UNITNO] <= 39) {
         PBAREG[CFDE_EN_39_32] &= ~((1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO]-32))&0xFF);
        PBAREG[CFDE_EN_39_32] |= (1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO] - 32));
    }

    if (CFDE_BLK_Base[CFDE_BLK_UNITNO] >= 40 && CFDE_BLK_Base[CFDE_BLK_UNITNO] <= 47) {
         PBAREG[CFDE_EN_47_40] &= ~((1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO]-40))&0xFF);
        PBAREG[CFDE_EN_47_40] |= (1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO] - 40));
    }

    if (CFDE_BLK_Base[CFDE_BLK_UNITNO] >= 48 && CFDE_BLK_Base[CFDE_BLK_UNITNO] <= 55) {
         PBAREG[CFDE_EN_55_48] &= ~((1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO]-48))&0xFF);
        PBAREG[CFDE_EN_55_48] |= (1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO] - 48));
    }

    if (CFDE_BLK_Base[CFDE_BLK_UNITNO] >= 56 && CFDE_BLK_Base[CFDE_BLK_UNITNO] <= 63) {
         PBAREG[CFDE_EN_63_56] &= ~((1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO]-56))&0xFF);
        PBAREG[CFDE_EN_63_56] |= (1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO] - 56));
    }

    if (CFDE_BLK_Base[CFDE_BLK_UNITNO] >= 64 && CFDE_BLK_Base[CFDE_BLK_UNITNO] <= 71) {
         PBAREG[CFDE_EN_71_64] &= ~((1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO]-64))&0xFF);
        PBAREG[CFDE_EN_71_64] |= (1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO] - 64));
    }

    if (CFDE_BLK_Base[CFDE_BLK_UNITNO] >= 72 && CFDE_BLK_Base[CFDE_BLK_UNITNO] <= 79) {
         PBAREG[CFDE_EN_79_72] &= ~((1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO]-72))&0xFF);
        PBAREG[CFDE_EN_79_72] |= (1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO] - 72));
    }

    if (CFDE_BLK_Base[CFDE_BLK_UNITNO] >= 80 && CFDE_BLK_Base[CFDE_BLK_UNITNO] <= 87) {
         PBAREG[CFDE_EN_87_80] &= ~((1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO]-80))&0xFF);
        PBAREG[CFDE_EN_87_80] |= (1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO] - 80));
    }

    if (CFDE_BLK_Base[CFDE_BLK_UNITNO] >= 88 && CFDE_BLK_Base[CFDE_BLK_UNITNO] <= 95) {
         PBAREG[CFDE_EN_95_88] &= ~((1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO]-88))&0xFF);
        PBAREG[CFDE_EN_95_88] |= (1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO] - 88));
    }

    if (CFDE_BLK_Base[CFDE_BLK_UNITNO] >= 96 && CFDE_BLK_Base[CFDE_BLK_UNITNO] <= 99) {
         PBAREG[CFDE_EN_99_96] &= ~((1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO]-96))&0xFF);
        PBAREG[CFDE_EN_99_96] |= (1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO] - 96));
    }



    /* Check fill CFDE page mark */
    if ((CFDE_BLK_Base[CFDE_BLK_HD_H] != CFDE_BLK_MARKER_H) || (CFDE_BLK_Base[CFDE_BLK_HD_L] != CFDE_BLK_MARKER_L)) {
        return -1;
    }

    /* Select which CFDE */
    PBAREG[CFDE_SEL] = CFDE_BLK_Base[CFDE_BLK_UNITNO];

 
    MAX_CFDE_Depth=32;
    
    for (c = 0; c < MAX_CFDE_Depth; c++)
    {


        PBAREG[CFDE_CMDQ_47_40] = 0x00;
        PBAREG[CFDE_CMDQ_39_32] = 0x00;
        PBAREG[CFDE_CMDQ_31_24] = 0x00;
        PBAREG[CFDE_CMDQ_23_16] = 0x00;
        PBAREG[CFDE_CMDQ_15_8] = 0x00; 
        PBAREG[CFDE_CMDQ_7_0] = 0x00;
     } 
    

    /* Enable CFDE unit */
    if (CFDE_BLK_Base[CFDE_BLK_UNITNO] >= 0 && CFDE_BLK_Base[CFDE_BLK_UNITNO] <= 7) {
        PBAREG[CFDE_EN_7_0] &= ~((1 << CFDE_BLK_Base[CFDE_BLK_UNITNO])&0xFF);
        PBAREG[CFDE_EN_7_0] |= (1 << CFDE_BLK_Base[CFDE_BLK_UNITNO]);
    }

    if (CFDE_BLK_Base[CFDE_BLK_UNITNO] >= 8 && CFDE_BLK_Base[CFDE_BLK_UNITNO] <= 15) {
        PBAREG[CFDE_EN_15_8] &= ~((1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO]-8))&0xFF);
        PBAREG[CFDE_EN_15_8] |= (1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO] - 8));
    }

    if (CFDE_BLK_Base[CFDE_BLK_UNITNO] >= 16 && CFDE_BLK_Base[CFDE_BLK_UNITNO] <= 23) {
         PBAREG[CFDE_EN_23_16] &= ~((1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO]-16))&0xFF);
        PBAREG[CFDE_EN_23_16] |= (1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO] - 16));
    }

    if (CFDE_BLK_Base[CFDE_BLK_UNITNO] >= 24 && CFDE_BLK_Base[CFDE_BLK_UNITNO] <= 31) {
         PBAREG[CFDE_EN_31_24] &= ~((1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO]-24))&0xFF);
        PBAREG[CFDE_EN_31_24] |= (1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO] - 24));
    }

    if (CFDE_BLK_Base[CFDE_BLK_UNITNO] >= 32 && CFDE_BLK_Base[CFDE_BLK_UNITNO] <= 39) {
         PBAREG[CFDE_EN_39_32] &= ~((1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO]-32))&0xFF);
        PBAREG[CFDE_EN_39_32] |= (1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO] - 32));
    }

    if (CFDE_BLK_Base[CFDE_BLK_UNITNO] >= 40 && CFDE_BLK_Base[CFDE_BLK_UNITNO] <= 47) {
         PBAREG[CFDE_EN_47_40] &= ~((1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO]-40))&0xFF);
        PBAREG[CFDE_EN_47_40] |= (1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO] - 40));
    }

    if (CFDE_BLK_Base[CFDE_BLK_UNITNO] >= 48 && CFDE_BLK_Base[CFDE_BLK_UNITNO] <= 55) {
         PBAREG[CFDE_EN_55_48] &= ~((1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO]-48))&0xFF);
        PBAREG[CFDE_EN_55_48] |= (1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO] - 48));
    }

    if (CFDE_BLK_Base[CFDE_BLK_UNITNO] >= 56 && CFDE_BLK_Base[CFDE_BLK_UNITNO] <= 63) {
         PBAREG[CFDE_EN_63_56] &= ~((1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO]-56))&0xFF);
        PBAREG[CFDE_EN_63_56] |= (1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO] - 56));
    }

    if (CFDE_BLK_Base[CFDE_BLK_UNITNO] >= 64 && CFDE_BLK_Base[CFDE_BLK_UNITNO] <= 71) {
         PBAREG[CFDE_EN_71_64] &= ~((1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO]-64))&0xFF);
        PBAREG[CFDE_EN_71_64] |= (1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO] - 64));
    }

    if (CFDE_BLK_Base[CFDE_BLK_UNITNO] >= 72 && CFDE_BLK_Base[CFDE_BLK_UNITNO] <= 79) {
         PBAREG[CFDE_EN_79_72] &= ~((1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO]-72))&0xFF);
        PBAREG[CFDE_EN_79_72] |= (1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO] - 72));
    }

    if (CFDE_BLK_Base[CFDE_BLK_UNITNO] >= 80 && CFDE_BLK_Base[CFDE_BLK_UNITNO] <= 87) {
         PBAREG[CFDE_EN_87_80] &= ~((1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO]-80))&0xFF);
        PBAREG[CFDE_EN_87_80] |= (1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO] - 80));
    }

    if (CFDE_BLK_Base[CFDE_BLK_UNITNO] >= 88 && CFDE_BLK_Base[CFDE_BLK_UNITNO] <= 95) {
         PBAREG[CFDE_EN_95_88] &= ~((1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO]-88))&0xFF);
        PBAREG[CFDE_EN_95_88] |= (1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO] - 88));
    }

    if (CFDE_BLK_Base[CFDE_BLK_UNITNO] >= 96 && CFDE_BLK_Base[CFDE_BLK_UNITNO] <= 99) {
         PBAREG[CFDE_EN_99_96] &= ~((1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO]-96))&0xFF);
        PBAREG[CFDE_EN_99_96] |= (1 << (CFDE_BLK_Base[CFDE_BLK_UNITNO] - 96));
    }




    /* Determine setting CFDE unit count */
    setno = CFDE_BLK_Base[CFDE_BLK_SETNO];



    /* Setting CFDE unit */
    for (c = 0; c < setno; c++) {
        PBAREG[CFDE_CMDQ_47_40] = CFDE_BLK_Base[CFDE_BLK_CMDQ_START + c * CFDE_CMDQ_NO + CFDE_CMDQ_BYTE5];
        PBAREG[CFDE_CMDQ_39_32] = CFDE_BLK_Base[CFDE_BLK_CMDQ_START + c * CFDE_CMDQ_NO + CFDE_CMDQ_BYTE4];
        PBAREG[CFDE_CMDQ_31_24] = CFDE_BLK_Base[CFDE_BLK_CMDQ_START + c * CFDE_CMDQ_NO + CFDE_CMDQ_BYTE3];
        PBAREG[CFDE_CMDQ_23_16] = CFDE_BLK_Base[CFDE_BLK_CMDQ_START + c * CFDE_CMDQ_NO + CFDE_CMDQ_BYTE2];
        PBAREG[CFDE_CMDQ_15_8] = CFDE_BLK_Base[CFDE_BLK_CMDQ_START + c * CFDE_CMDQ_NO + CFDE_CMDQ_BYTE1];
        PBAREG[CFDE_CMDQ_7_0] = CFDE_BLK_Base[CFDE_BLK_CMDQ_START + c * CFDE_CMDQ_NO + CFDE_CMDQ_BYTE0];

    }

    /* Read back to check setting CFDE unit */
    setno = CFDE_BLK_Base[CFDE_BLK_SETNO];
    PBAREG[CFDE_SEL] = CFDE_BLK_Base[CFDE_BLK_UNITNO];
    for (c = 0; c < setno; c++) 
    {
        PBAREG[CFDE_CMDQ_RDPTR] = c;

        if (PBAREG[CFDE_CMDQ_RD_47_40] != CFDE_BLK_Base[CFDE_BLK_CMDQ_START + c * CFDE_CMDQ_NO + CFDE_CMDQ_BYTE5]) {
            return -1;
        }

        if (PBAREG[CFDE_CMDQ_RD_39_32] != CFDE_BLK_Base[CFDE_BLK_CMDQ_START + c * CFDE_CMDQ_NO + CFDE_CMDQ_BYTE4]) {
            return -1;
        }

        if (PBAREG[CFDE_CMDQ_RD_31_24] != CFDE_BLK_Base[CFDE_BLK_CMDQ_START + c * CFDE_CMDQ_NO + CFDE_CMDQ_BYTE3]) {
            return -1;
        }

        if (PBAREG[CFDE_CMDQ_RD_23_16] != CFDE_BLK_Base[CFDE_BLK_CMDQ_START + c * CFDE_CMDQ_NO + CFDE_CMDQ_BYTE2]) {
            return -1;
        }

        if (PBAREG[CFDE_CMDQ_RD_15_8] != CFDE_BLK_Base[CFDE_BLK_CMDQ_START + c * CFDE_CMDQ_NO + CFDE_CMDQ_BYTE1]) {
            return -1;
        }

        if (PBAREG[CFDE_CMDQ_RD_7_0] != CFDE_BLK_Base[CFDE_BLK_CMDQ_START + c * CFDE_CMDQ_NO + CFDE_CMDQ_BYTE0]) {
            return -1;
        }
    }

  

    return 0;

}

int pba_fill_Cache_Header(void)
{
    U8 byHeaderIdx;
    U8 i;

    for (byHeaderIdx = 0; byHeaderIdx < 0x40; byHeaderIdx++) {
        PBAREG[CTL_PBA_CACHE_HEADER_SEL] = byHeaderIdx;
        //memcpy(CFDE_BLK_Base[0x00 + byHeaderIdx * 0x08], &PBAREG[CTL_PBA_CACHE_HEADER_DAT0], 0x08;
        for (i = 0; i < 0x08; i++) {
            CFDE_BLK_Base[byHeaderIdx * 0x08 + i] = PBAREG[CTL_PBA_CACHE_HEADER_DAT0 + i];
        }
    }

    return 0;
}

int pba_get_dps_ram_data(U8 byflashType, U8 byCeIdx, U8 byLunIdx)
{
    volatile U8* pbyPBACEReg;
    U16 wI;
    U8 i=0;

    pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;

    byLunIdx=((byLunIdx+1)%8);

    //Set Busy
    if (byLunIdx == 0) {
       pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN0] = SET_ALL_BSY;
       //pbyPBACEReg[SET_BSY8_5_BUSY_RDY_LUN0] = SET_ALL_BSY; 
    } 
    else if(byLunIdx > 0 && byLunIdx < 8) 
    {
       pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN1 + (byLunIdx - 1) * 2] = SET_ALL_BSY;
       //pbyPBACEReg[SET_BSY8_5_BUSY_RDY_LUN1 + (byLunIdx - 1) * 2] = SET_ALL_BSY;
    }   
    
    
    
    //Get SG Data Buf
    if (byflashType == VC_LEAGCY_MODE) {
        pbyPBACEReg[SIG_DAT_LEG_SRAM_LUN_SEL] = byLunIdx;
        for (wI = 0; wI < 0x100; wI++) {
            pbyPBACEReg[SIG_DAT_LEG_SRAM_ADDR_SEL] = wI;
            CFDE_BLK_Base[wI] = pbyPBACEReg[SIG_DAT_LEG_SRAM_RW];
        }

    } else if (byflashType == VC_TOGGLE_MODE ) {
        pbyPBACEReg[SIG_DAT_TOG_SRAM_LUN_SEL] = byLunIdx;
        for (wI = 0; wI < 0x100; wI++) {
            pbyPBACEReg[SIG_DAT_TOG_SRAM_ADDR_SEL] = wI;
            CFDE_BLK_Base[wI] = pbyPBACEReg[SIG_DAT_TOG_SRAM_RW];
        }   
    }


    if(byLunIdx > 0 && byLunIdx < 8) 
    {
       pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN1 + (byLunIdx - 1) * 2] = SET_ALL_RDY;
    }   
    
    return 0;

}

int pba_Set_dps_ram_data(U8 byflashType, U8 byCeIdx, U8 byLunIdx,U8 Set_Ready)
{
    volatile U8* pbyPBACEReg;
    U16 wI;
  
    

    pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;

    byLunIdx=((byLunIdx+1)%8);

    //Set Busy
    if (byLunIdx == 0) {
       pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN0] = SET_ALL_BSY;
       //pbyPBACEReg[SET_BSY8_5_BUSY_RDY_LUN0] = SET_ALL_BSY; 
    } 
    else if(byLunIdx > 0 && byLunIdx < 8) 
    {
       pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN1 + (byLunIdx - 1) * 2] = SET_ALL_BSY;
       //pbyPBACEReg[SET_BSY8_5_BUSY_RDY_LUN1 + (byLunIdx - 1) * 2] = SET_ALL_BSY;
    }   

        
     //Set SG Data Buf
    if (byflashType == VC_LEAGCY_MODE) {
        pbyPBACEReg[SIG_DAT_LEG_SRAM_LUN_SEL] = byLunIdx;
        for (wI = 0; wI < 0x100; wI++) {
            pbyPBACEReg[SIG_DAT_LEG_SRAM_ADDR_SEL] = wI;
            pbyPBACEReg[SIG_DAT_LEG_SRAM_RW] = CFDE_BLK_Base[wI];
        }

    } else if (byflashType == VC_TOGGLE_MODE ) {
        pbyPBACEReg[SIG_DAT_TOG_SRAM_LUN_SEL] = byLunIdx;
        for (wI = 0; wI < 0x100; wI++) {
            pbyPBACEReg[SIG_DAT_TOG_SRAM_ADDR_SEL] = wI;
            pbyPBACEReg[SIG_DAT_TOG_SRAM_RW] = CFDE_BLK_Base[wI];
        }   
    }

    if(Set_Ready)
    {
        //Set Ready
        if (byLunIdx == 0) {
           pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN0] = SET_ALL_RDY;
           //pbyPBACEReg[SET_BSY8_5_BUSY_RDY_LUN0] = SET_ALL_RDY; 
        } 
        else if(byLunIdx > 0 && byLunIdx < 8) 
        {
           pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN1 + (byLunIdx - 1) * 2] = SET_ALL_RDY;
           //pbyPBACEReg[SET_BSY8_5_BUSY_RDY_LUN1 + (byLunIdx - 1) * 2] = SET_ALL_RDY;
        }   
    }
    return 0;
}



int pba_get_Tag_Info(U8 byCeIdx)
{
    volatile U8* pbyPBACEReg;
    U8 i=0;

    pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;

    for(i=0;i<MAX_TAG_GP;i++)
    {
      if(i == MAX_TAG_GP-1)
         CFDE_BLK_Base[i] = ((pbyPBACEReg[CFDE_TAG_99_96]>>1) & 0x0F);
      else
         CFDE_BLK_Base[i] = pbyPBACEReg[CFDE_TAG_7_0+i];
    }
    
    
   return 0; 
}
int pba_fill_SG_buf_data(U8 byflashType, U8 byCeIdx)
{
    volatile U8* pbyPBACEReg;
    U8 i,j;

    pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;

    //Get SG Data Buf
    if (byflashType == VC_LEAGCY_MODE) {
        for (i = 0; i < 0x80; i++) {
            pbyPBACEReg[DATA_BUF_PTR] = i;
            CFDE_BLK_Base[i] = pbyPBACEReg[DATA_BUF];
        }

    } else if (byflashType == VC_TOGGLE_MODE ) {
        for (i = 0; i < 0x80; i++) {
            pbyPBACEReg[DAT_BUF_PTR_TOG] = i;
            CFDE_BLK_Base[i] = pbyPBACEReg[DAT_BUF_TOG];
        }       
    }

    //Get SG addr Buf 
    for(j=0;j<2;j++)
    {
       pbyPBACEReg[ADDR_BUF_NO_SG_DATA] = j;
        for (i = 0; i < 0x07; i++) {
            CFDE_BLK_Base[0x80 + i+(j*7)] = pbyPBACEReg[CYCLE6 + i];
        }
    }

    return 0;
}

void pba_switch_flash_type_CE(U8 byflashType, U8 byCeIdx)
{
    volatile U8* pbyPBACEReg;
  
    pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;

    pbyPBACEReg[CRBYTE0_CE] &= MASK_MODE;

    if (byflashType == VC_LEAGCY_MODE) 
    {
        // reset to system default driving
        pba_reset_default_drive_setting();

    } else if (byflashType == VC_TOGGLE_MODE) {
        pbyPBACEReg[CRBYTE0_CE] |= S_TOGGLE_MODE;      
    }     
}  

int pba_switch_flash_type(U8 byflashType, U8 byCeIdx)
{
    U8 i;

    if (byCeIdx == 0x08) {
        for (i = 0; i < _gMax_Ce_Cnt; i++) {
            pba_switch_flash_type_CE(byflashType, i);           
        }    
          
    } else {
        pba_switch_flash_type_CE(byflashType, byCeIdx);       
    }

    return 0;
}

int pba_Set_Ready_Busy(BOOL RDY,U8 byCeIdx,U8 byLunIdx)
{
    volatile U8* pbyPBACEReg;
    U8 i,j;
    U8 Start_CE,End_CE;
    U8 Start_Lun,End_Lun;
    
    if(byCeIdx==RDY_ALL_CE)
    {
        Start_CE = 0;
        End_CE = _gMax_Ce_Cnt;
    }
    else
    {
        Start_CE = byCeIdx;
        End_CE = Start_CE++;
    }
    
    if(byLunIdx==RDY_ALL_LUN)
    {
        Start_Lun= 0;
        End_Lun = _gMax_Lun_Cnt;
    }
    else
    {
        Start_Lun = byLunIdx;
        End_Lun = Start_Lun++;
    }

  
    for(i=Start_CE;i<End_CE;i++)
    {
       pbyPBACEReg = PBAREG_CE0 + i * 0x100;

       for(j=Start_Lun;j<End_Lun;j++)
       {
            if(j==0)//Lun0
            {
                if(RDY==SET_READY)
                {       
                   pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN0] = SET_ALL_RDY;
                   pbyPBACEReg[SET_BSY8_5_BUSY_RDY_LUN0] = SET_ALL_RDY;  
                }
                else if(RDY==SET_BUSY)
                {
                   pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN0] = SET_ALL_BSY;
                   pbyPBACEReg[SET_BSY8_5_BUSY_RDY_LUN0] = SET_ALL_BSY;   
                }
              
            }
            else//Lun1~7
            {
                if(RDY==SET_READY)
                {    
                   pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN1 + (j-1)*2] = SET_ALL_RDY;
                   pbyPBACEReg[SET_BSY8_5_BUSY_RDY_LUN1 + (j-1)*2] = SET_ALL_RDY;
                }
                else if(RDY==SET_BUSY)
                {
                   pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN1 + (j-1)*2] = SET_ALL_BSY;
                   pbyPBACEReg[SET_BSY8_5_BUSY_RDY_LUN1 + (j-1)*2] = SET_ALL_BSY;  
                }
               
            }
                
        }

    }     
  
 
    return 0;
}




void pba_update_AIPR_DLY_Status78_Bit5_Mode(U8 byCeIdx ,U8 byLunIdx)
{
     volatile U8* pbyPBACEReg;
     
     pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;
     
     pbyPBACEReg[STA_BUF_NO] = (STS_BUFNO_ENH_78CMD+(byLunIdx*LUN_CNT_CE));
     pbyPBACEReg[STA_BUF_DLY_T_NO] = (STS_BUFNO_ENH_78CMD+(byLunIdx*LUN_CNT_CE));

     if(byLunIdx==0)
       pbyPBACEReg[STA0_BIT5]=SET_RDY_Base[RDY_PG_OFS_STA_BIT5_MODE];//IB_BSY8_DLY_AND_BSY1_DLY_LUN0; 
     else if(byLunIdx==1)
       pbyPBACEReg[STA0_BIT5]=SET_RDY_Base[RDY_PG_OFS_STA_BIT5_MODE];//IB_BSY8_DLY_AND_BSY1_DLY_LUN1; 
     else if(byLunIdx==2)
       pbyPBACEReg[STA0_BIT5]=SET_RDY_Base[RDY_PG_OFS_STA_BIT5_MODE];//IB_BSY8_DLY_AND_BSY1_DLY_LUN2;
     else if(byLunIdx==3)
       pbyPBACEReg[STA0_BIT5]=SET_RDY_Base[RDY_PG_OFS_STA_BIT5_MODE];//IB_BSY8_DLY_AND_BSY1_DLY_LUN3;  
     else if(byLunIdx==4)
       pbyPBACEReg[STA0_BIT5]=SET_RDY_Base[RDY_PG_OFS_STA_BIT5_MODE];//IB_BSY8_DLY_AND_BSY1_DLY_LUN4; 
     else if(byLunIdx==5)
       pbyPBACEReg[STA0_BIT5]=SET_RDY_Base[RDY_PG_OFS_STA_BIT5_MODE];//IB_BSY8_DLY_AND_BSY1_DLY_LUN5; 
     else if(byLunIdx==6)
       pbyPBACEReg[STA0_BIT5]=SET_RDY_Base[RDY_PG_OFS_STA_BIT5_MODE];//IB_BSY8_DLY_AND_BSY1_DLY_LUN6;
     else if(byLunIdx==7)
       pbyPBACEReg[STA0_BIT5]=SET_RDY_Base[RDY_PG_OFS_STA_BIT5_MODE];//IB_BSY8_DLY_AND_BSY1_DLY_LUN7;  
      
    
    if (byLunIdx == 0) {
        pbyPBACEReg[RDY_STAT_DLY1_LUN0] = 0x50;
    } else if ((byLunIdx >= 0x01) && (byLunIdx <= 0x06)) {
        pbyPBACEReg[RDY_STAT_DLY1_LUN1 + (byLunIdx - 1) * SUB_BSY_COUNT_LUN ] = 0x50;
    } else {
        pbyPBACEReg[RDY_STAT_DLY1_LUN7] = 0x50;
    }   
}
        

static void pba_set_bsy_dly(U8 byCeIdx, U8 byLunIdx, U8 bySubBSYIdx, U8 byDlyValue)
{
    volatile U8* pbyPBACEReg;
    U8 i=0;

    pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;


    
    //Cmd 70 process
    pbyPBACEReg[STA_BUF_NO] = STS_BUFNO_70CMD;
    pbyPBACEReg[STA_BUF_DLY_T_NO]=  STS_BUFNO_70CMD;
    pbyPBACEReg[STA0_BIT5]=SET_RDY_Base[RDY_PG_OFS_STA_BIT5_MODE];
    pbyPBACEReg[STA0_BIT6]=SET_RDY_Base[RDY_PG_OFS_STA_BIT6_MODE];

     //Cmd 71 process
    pbyPBACEReg[STA_BUF_NO] = STS_BUFNO_71CMD;
    pbyPBACEReg[STA_BUF_DLY_T_NO]=  STS_BUFNO_71CMD;
    pbyPBACEReg[STA0_BIT5]=SET_RDY_Base[RDY_PG_OFS_STA_BIT5_MODE];

     //Cmd 72 process
    pbyPBACEReg[STA_BUF_NO] = STS_BUFNO_72CMD;
    pbyPBACEReg[STA_BUF_DLY_T_NO]=  STS_BUFNO_72CMD;
    pbyPBACEReg[STA0_BIT5]=SET_RDY_Base[RDY_PG_OFS_STA_BIT5_MODE];

     //Cmd 73 process
    pbyPBACEReg[STA_BUF_NO] = STS_BUFNO_73CMD;
    pbyPBACEReg[STA_BUF_DLY_T_NO]=  STS_BUFNO_73CMD;
    pbyPBACEReg[STA0_BIT5]=SET_RDY_Base[RDY_PG_OFS_STA_BIT5_MODE];


     //Cmd 74 process
    pbyPBACEReg[STA_BUF_NO] = STS_BUFNO_74CMD;
    pbyPBACEReg[STA_BUF_DLY_T_NO]=  STS_BUFNO_74CMD;
    pbyPBACEReg[STA0_BIT5]=SET_RDY_Base[RDY_PG_OFS_STA_BIT5_MODE];

    
    //Cmd Fx process
    pbyPBACEReg[STA_BUF_NO] = SET_RDY_Base[RDY_PG_OFS_STS_BUF_BY_LUN];
    pbyPBACEReg[STA_BUF_DLY_T_NO]= SET_RDY_Base[RDY_PG_OFS_STS_BUF_BY_LUN];
    pbyPBACEReg[STA0_BIT5]=SET_RDY_Base[RDY_PG_OFS_STS_BUF_LUN_DLY_OP]; 


    
    if(AIPR_EN)
    {
        for(i=0;i<SET_RDY_Base[RDY_PG_OFS_SET_RDY_MAX_LUN_NO];i++)
        {
           byLunIdx= SET_RDY_Base[RDY_PG_OFS_SET_RDY_LUN_NO_0+i];
           pba_update_AIPR_DLY_Status78_Bit5_Mode(byCeIdx,byLunIdx);
        }
    }
    else
    {
        //Status 78 for 8x 15
        for(i=0;i<_gMax_Plane_Cnt;i++)
        {

             pbyPBACEReg[STA_BUF_NO] = (STS_BUFNO_ENH_78CMD+(byLunIdx*LUN_CNT_CE)+i);
             pbyPBACEReg[STA_BUF_DLY_T_NO] = (STS_BUFNO_ENH_78CMD+(byLunIdx*LUN_CNT_CE)+i);
             pbyPBACEReg[STA0_BIT5] = SET_RDY_Base[RDY_PG_OFS_STA_BIT5_MODE];
             /*if(byLunIdx==0)
               pbyPBACEReg[STA0_BIT5]=IB_BSY8_DLY_AND_BSY1_DLY_LUN0; 
             else if(byLunIdx==1)
               pbyPBACEReg[STA0_BIT5]=IB_BSY8_DLY_AND_BSY1_DLY_LUN1; 
             else if(byLunIdx==2)
               pbyPBACEReg[STA0_BIT5]=IB_BSY8_DLY_AND_BSY1_DLY_LUN2;
             else if(byLunIdx==3)
               pbyPBACEReg[STA0_BIT5]=IB_BSY8_DLY_AND_BSY1_DLY_LUN3;*/
             
        }
    
    
        if (byLunIdx == 0) {
            pbyPBACEReg[RDY_STAT_DLY1_LUN0 + bySubBSYIdx] = byDlyValue;
        } else if ((byLunIdx >= 0x01) && (byLunIdx <= 0x06)) {
            pbyPBACEReg[RDY_STAT_DLY1_LUN1 + (byLunIdx - 1) * SUB_BSY_COUNT_LUN + bySubBSYIdx] = byDlyValue;
        } else {
            pbyPBACEReg[RDY_STAT_DLY1_LUN7 + bySubBSYIdx] = byDlyValue;
        }   
        
    }
      
}



static void pba_update_status_Bit5_Mode(U8 byCeIdx,U8 byLunIdx,U8 Status_Buf_No)
{
    volatile U8* pbyPBACEReg;
    pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;
    pbyPBACEReg[STA_BUF_NO] = Status_Buf_No;
    pbyPBACEReg[STA_BUF_DLY_T_NO]=  Status_Buf_No;


    if(SET_RDY_Base[RDY_PG_OFS_STA_BIT5_MODE]==IB_STABUF_ENTERY)
    {
    
        if((SET_RDY_Base[RDY_PG_OFS_STS_BUF1_VALUE_70h]>>5)&SET_BIT0)
        {
             if(byLunIdx==0)
                 pbyPBACEReg[STA0_BIT5] = IB_BSY8_AND_BSY1_LUN0;
             else
                 pbyPBACEReg[STA0_BIT5] = IB_BSY8_AND_BSY1_LUN1+(byLunIdx-1)*11;
       
        }
        else
            pbyPBACEReg[STA0_BIT5]=IB_NO_SRC_TIE_L;
    }
    else
    {
       pbyPBACEReg[STA0_BIT6]=SET_RDY_Base[RDY_PG_OFS_STA_BIT5_MODE]; 
       pbyPBACEReg[STA0_BIT5]=SET_RDY_Base[RDY_PG_OFS_STA_BIT5_MODE];    
    }
    
}    

static void pba_update_Fn_status_Bit5_Mode(U8 byCeIdx,U8 byLunIdx,U8 Status_Buf_No)
{
    volatile U8* pbyPBACEReg;
    pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;
    pbyPBACEReg[STA_BUF_NO] = Status_Buf_No;
    pbyPBACEReg[STA_BUF_DLY_T_NO]=  Status_Buf_No;


    if(SET_RDY_Base[RDY_PG_OFS_STA_BIT5_MODE]==IB_STABUF_ENTERY)
    {
    
        if((SET_RDY_Base[RDY_PG_OFS_STS_BUF1_VALUE_70h]>>5)&SET_BIT0)
        {
             if(byLunIdx==0)
                 pbyPBACEReg[STA0_BIT5] = IB_BSY8_AND_BSY1_LUN0;
             else
                 pbyPBACEReg[STA0_BIT5] = IB_BSY8_AND_BSY1_LUN1+(byLunIdx-1)*11;
       
        }
        else
            pbyPBACEReg[STA0_BIT5]=IB_NO_SRC_TIE_L;
    }
    else
    {
        pbyPBACEReg[STA0_BIT5] = SET_RDY_Base[RDY_PG_OFS_STA_BIT5_MODE];
        /*if(byLunIdx==0)
            pbyPBACEReg[STA0_BIT5] = IB_BSY8_AND_BSY1_LUN0;
        else
            pbyPBACEReg[STA0_BIT5] = IB_BSY8_AND_BSY1_LUN1+(byLunIdx-1)*11;*/
    }
    
}    


void pba_update_AIPR_NO_DLY_Status78_Bit5_Mode(U8 byCeIdx,U8 byLunIdx)
{
     volatile U8* pbyPBACEReg;

     pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;
     
     pbyPBACEReg[STA_BUF_NO] = (STS_BUFNO_ENH_78CMD+(byLunIdx*LUN_CNT_CE));
     pbyPBACEReg[STA_BUF_DLY_T_NO] = (STS_BUFNO_ENH_78CMD+(byLunIdx*LUN_CNT_CE));
     

     if(SET_RDY_Base[RDY_PG_OFS_STA_BIT5_MODE]==IB_STABUF_ENTERY)
     {
        if((SET_RDY_Base[(RDY_PG_OFS_STS_BUF1_VALUE_ENH_78+(byLunIdx*LUN_CNT_CE))]>>5)&SET_BIT0)
        {
             if(byLunIdx==0)
                 pbyPBACEReg[STA0_BIT5] = IB_BSY8_AND_BSY1_LUN0;
             else
                 pbyPBACEReg[STA0_BIT5] = IB_BSY8_AND_BSY1_LUN1+(byLunIdx-1)*11;
       
        }
        else
            pbyPBACEReg[STA0_BIT5]=IB_NO_SRC_TIE_L;
     }       
     else
     {
        /*if(byLunIdx==0)
           pbyPBACEReg[STA0_BIT5] = IB_BSY8_AND_BSY1_LUN0;
        else
           pbyPBACEReg[STA0_BIT5] = IB_BSY8_AND_BSY1_LUN1+(byLunIdx-1)*11;*/   

        pbyPBACEReg[STA0_BIT5]=SET_RDY_Base[RDY_PG_OFS_STA_BIT5_MODE];
     }
     
    
    if (byLunIdx == 0) {
        pbyPBACEReg[RDY_STAT_DLY1_LUN0 ] = 0x00;
    } else if ((byLunIdx >= 0x01) && (byLunIdx <= 0x06)) {
        pbyPBACEReg[RDY_STAT_DLY1_LUN1 + (byLunIdx - 1) * SUB_BSY_COUNT_LUN ] = 0x00;
    } else {
        pbyPBACEReg[RDY_STAT_DLY1_LUN7 ] = 0x00;
    }   
}


static void pba_set_bsy_No_dly(U8 byCeIdx,U8 byLunIdx,U8 bySubBSYIdx)
{
    volatile U8* pbyPBACEReg;
    U8 Buf_No=0,i=0;
    
    pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;

    //Status 70
    pba_update_status_Bit5_Mode(byCeIdx,byLunIdx,STS_BUFNO_70CMD);
    //Status 71
    pba_update_status_Bit5_Mode(byCeIdx,byLunIdx,STS_BUFNO_71CMD);
    //Status 72
    pba_update_status_Bit5_Mode(byCeIdx,byLunIdx,STS_BUFNO_72CMD);
    //Status 73
    pba_update_status_Bit5_Mode(byCeIdx,byLunIdx,STS_BUFNO_73CMD);
    //Status 74
    pba_update_status_Bit5_Mode(byCeIdx,byLunIdx,STS_BUFNO_74CMD);
    //Status Fx
    pba_update_Fn_status_Bit5_Mode(byCeIdx,byLunIdx,SET_RDY_Base[RDY_PG_OFS_STS_BUF_BY_LUN]);
 

    
    if(SET_RDY_Base[RDY_PG_OFS_UPDATA_STS_BY_LUN_78h]==0x01)
    {
       
          if(AIPR_EN)
          {
             for(i=0;i<SET_RDY_Base[RDY_PG_OFS_SET_RDY_MAX_LUN_NO];i++)
             {
                byLunIdx=SET_RDY_Base[RDY_PG_OFS_SET_RDY_LUN_NO_0+i];
                pba_update_AIPR_NO_DLY_Status78_Bit5_Mode(byCeIdx, byLunIdx);
             }
          }
          else
          {
            for(i=0;i<_gMax_Plane_Cnt;i++)
            {
                pbyPBACEReg[STA_BUF_NO] = (STS_BUFNO_ENH_78CMD+(byLunIdx*LUN_CNT_CE)+i);
                pbyPBACEReg[STA_BUF_DLY_T_NO] = (STS_BUFNO_ENH_78CMD+(byLunIdx*LUN_CNT_CE)+i);

                if(SET_RDY_Base[RDY_PG_OFS_STA_BIT5_MODE]==IB_STABUF_ENTERY)
                {
                    if((SET_RDY_Base[(RDY_PG_OFS_STS_BUF1_VALUE_ENH_78+(byLunIdx*LUN_CNT_CE+i))]>>5)&SET_BIT0)
                    {
                        if(byLunIdx==0)
                            pbyPBACEReg[STA0_BIT5] = IB_BSY8_AND_BSY1_LUN0;
                        else
                            pbyPBACEReg[STA0_BIT5] = IB_BSY8_AND_BSY1_LUN1+(byLunIdx-1)*11;
                    }
                    else
                        pbyPBACEReg[STA0_BIT5]=IB_NO_SRC_TIE_L;
                }
                else
                {
                    pbyPBACEReg[STA0_BIT5] = SET_RDY_Base[RDY_PG_OFS_STA_BIT5_MODE];
                    /*if(byLunIdx==0)
                        pbyPBACEReg[STA0_BIT5] = IB_BSY8_AND_BSY1_LUN0;
                    else
                        pbyPBACEReg[STA0_BIT5] = IB_BSY8_AND_BSY1_LUN1+(byLunIdx-1)*11;*/
                }
            }


            if (byLunIdx == 0) {
                pbyPBACEReg[RDY_STAT_DLY1_LUN0 + bySubBSYIdx] = 0x00;
            } else if ((byLunIdx >= 0x01) && (byLunIdx <= 0x06)) {
                pbyPBACEReg[RDY_STAT_DLY1_LUN1 + (byLunIdx - 1) * SUB_BSY_COUNT_LUN + bySubBSYIdx] = 0x00;
            } else {
                pbyPBACEReg[RDY_STAT_DLY1_LUN7 + bySubBSYIdx] = 0x00;
            }
          }       
    }
    else
    {
        //Cmd 78 process
        Buf_No=SET_RDY_Base[RDY_PG_OFS_STS_BUF_ENH_78];
        pbyPBACEReg[STA_BUF_NO] = Buf_No;
        pbyPBACEReg[STA_BUF_DLY_T_NO]= Buf_No;
      
        if(SET_RDY_Base[RDY_PG_OFS_STA_BIT5_MODE]==IB_STABUF_ENTERY)
        {
            if((SET_RDY_Base[RDY_PG_OFS_STS_BUF1_VALUE_ENH_78 + Buf_No]>>5)&SET_BIT0)
            {
                 if(byLunIdx==0)
                     pbyPBACEReg[STA0_BIT5] = IB_BSY8_AND_BSY1_LUN0;
                 else
                     pbyPBACEReg[STA0_BIT5] = IB_BSY8_AND_BSY1_LUN1+(byLunIdx-1)*11;
           
            }
            else
                pbyPBACEReg[STA0_BIT5]=IB_NO_SRC_TIE_L;
        }       
        else
        {
           pbyPBACEReg[STA0_BIT5]=SET_RDY_Base[RDY_PG_OFS_STA_BIT5_MODE];
        }


        if (byLunIdx == 0) {
            pbyPBACEReg[RDY_STAT_DLY1_LUN0 + bySubBSYIdx] = 0x00;
        } else if ((byLunIdx >= 0x01) && (byLunIdx <= 0x06)) {
            pbyPBACEReg[RDY_STAT_DLY1_LUN1 + (byLunIdx - 1) * SUB_BSY_COUNT_LUN + bySubBSYIdx] = 0x00;
        } else {
            pbyPBACEReg[RDY_STAT_DLY1_LUN7 + bySubBSYIdx] = 0x00;
    }
        
    
    }


    /*if (byLunIdx == 0) {
        pbyPBACEReg[RDY_STAT_DLY1_LUN0 + bySubBSYIdx] = 0x00;
    } else if ((byLunIdx >= 0x01) && (byLunIdx <= 0x06)) {
        pbyPBACEReg[RDY_STAT_DLY1_LUN1 + (byLunIdx - 1) * SUB_BSY_COUNT_LUN + bySubBSYIdx] = 0x00;
    } else {
        pbyPBACEReg[RDY_STAT_DLY1_LUN7 + bySubBSYIdx] = 0x00;
    }*/   
    
}
   



void pba_update_statusbuf_entry_value(void)
{

    volatile U8* pbyPBACEReg;
 
	U8 byCeIdx=0, byLunIdx=0,i=0,Buf_No=0;
   
    //update specified CE/Lun status entry(Bit4~0)        
    byCeIdx = SET_RDY_Base[RDY_PG_OFS_CE_IDX];
    byLunIdx = SET_RDY_Base[RDY_PG_OFS_LUN_IDX];
    pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100; 

    if(SET_RDY_Base[RDY_PG_OFS_2G_CLR_ALL_STS_BUF]==0x01)
    {
        for(i=0;i<MAX_STS_BUF_NO;i++)
        {
            pbyPBACEReg[STA_BUF_DLY_T_NO] = i;
            pbyPBACEReg[STA_WR_DLY_T_ENTRY] = 0xE0;
           
   
            pbyPBACEReg[STA_BUF_NO] = i;
            pbyPBACEReg[STA_WR_ENTRY] = 0xE0; 
         
           
        }         
    }
    else
    {
        //Cmd78
        if(SET_RDY_Base[RDY_PG_OFS_UPDATA_STS_BY_LUN_78h]==0x01)
        {
            
          if(AIPR_EN)
          {
               /*for(i=0;i<SET_RDY_Base[RDY_PG_OFS_SET_RDY_MAX_LUN_NO];i++)
               {
                    byLunIdx= SET_RDY_Base[RDY_PG_OFS_SET_RDY_LUN_NO_0+i];     

                    pbyPBACEReg[STA_BUF_DLY_T_NO] = (STS_BUFNO_ENH_78CMD+(byLunIdx*LUN_CNT_CE));            
                    pbyPBACEReg[STA_WR_DLY_T_ENTRY] = SET_RDY_Base[(RDY_PG_OFS_STS_BUF2_VALUE_ENH_78+(byLunIdx*LUN_CNT_CE))]&0x1F;
                     
                    pbyPBACEReg[STA_BUF_NO] = (STS_BUFNO_ENH_78CMD+(byLunIdx*LUN_CNT_CE));    
                    pbyPBACEReg[STA_WR_ENTRY] = SET_RDY_Base[(RDY_PG_OFS_STS_BUF1_VALUE_ENH_78+(byLunIdx*LUN_CNT_CE))]&0x1F;

               }*/


              for(i=0;i<_gMax_Plane_Cnt;i++)
              {
                 pbyPBACEReg[STA_BUF_DLY_T_NO] = (STS_BUFNO_ENH_78CMD+(i*LUN_CNT_CE));            
                 pbyPBACEReg[STA_WR_DLY_T_ENTRY] = SET_RDY_Base[(RDY_PG_OFS_STS_BUF2_VALUE_ENH_78+(i*LUN_CNT_CE))]&0x1F;
                 
                 pbyPBACEReg[STA_BUF_NO] = (STS_BUFNO_ENH_78CMD+(i*LUN_CNT_CE));    
                 pbyPBACEReg[STA_WR_ENTRY] = SET_RDY_Base[(RDY_PG_OFS_STS_BUF1_VALUE_ENH_78+(i*LUN_CNT_CE))]&0x1F;
                        
              }
          }
          else
          {
              for(i=0;i<_gMax_Plane_Cnt;i++)
              {
                 pbyPBACEReg[STA_BUF_DLY_T_NO] = (STS_BUFNO_ENH_78CMD+(byLunIdx*LUN_CNT_CE)+i);            
                 pbyPBACEReg[STA_WR_DLY_T_ENTRY] = SET_RDY_Base[(RDY_PG_OFS_STS_BUF2_VALUE_ENH_78+(byLunIdx*LUN_CNT_CE+i))]&0x1F;
                 
            
                 pbyPBACEReg[STA_BUF_NO] = (STS_BUFNO_ENH_78CMD+(byLunIdx*LUN_CNT_CE)+i);    
                 pbyPBACEReg[STA_WR_ENTRY] = SET_RDY_Base[(RDY_PG_OFS_STS_BUF1_VALUE_ENH_78+(byLunIdx*LUN_CNT_CE+i))]&0x1F;
                
                      
              }
          }
        }
        else
        {
             Buf_No = SET_RDY_Base[RDY_PG_OFS_STS_BUF_ENH_78];
             pbyPBACEReg[STA_BUF_DLY_T_NO] = Buf_No;       
             pbyPBACEReg[STA_WR_DLY_T_ENTRY] = SET_RDY_Base[RDY_PG_OFS_STS_BUF2_VALUE_ENH_78+Buf_No]&0x1F;
              
             pbyPBACEReg[STA_BUF_NO] = Buf_No;  
             pbyPBACEReg[STA_WR_ENTRY] = SET_RDY_Base[RDY_PG_OFS_STS_BUF1_VALUE_ENH_78+Buf_No]&0x1F; 
            
                
        }
       
         //cmd70
         pbyPBACEReg[STA_BUF_DLY_T_NO] = SET_RDY_Base[RDY_PG_OFS_STS_BUF_70h];
         pbyPBACEReg[STA_WR_DLY_T_ENTRY] = SET_RDY_Base[RDY_PG_OFS_STS_BUF2_VALUE_70h]&0x1F;

         pbyPBACEReg[STA_BUF_NO] = SET_RDY_Base[RDY_PG_OFS_STS_BUF_70h];
         pbyPBACEReg[STA_WR_ENTRY] = SET_RDY_Base[RDY_PG_OFS_STS_BUF1_VALUE_70h]&0x1F;
         
      
       
            
        //enhance Cmd71
         pbyPBACEReg[STA_BUF_DLY_T_NO] = (STS_BUFNO_ENH_71CMD+byLunIdx);
         pbyPBACEReg[STA_WR_DLY_T_ENTRY] = SET_RDY_Base[RDY_PG_OFS_STS_BUF2_VALUE_ENH_71+byLunIdx]&0x1F;
        
         pbyPBACEReg[STA_BUF_NO] = (STS_BUFNO_ENH_71CMD+byLunIdx); 
         pbyPBACEReg[STA_WR_ENTRY] = SET_RDY_Base[RDY_PG_OFS_STS_BUF1_VALUE_ENH_71+byLunIdx]&0x1F;
  
        

        //cmd71
         pbyPBACEReg[STA_BUF_DLY_T_NO] = SET_RDY_Base[RDY_PG_OFS_STS_BUF_71h];
         pbyPBACEReg[STA_WR_DLY_T_ENTRY] = SET_RDY_Base[RDY_PG_OFS_STS_BUF2_VALUE_71h]&0x1F;
               
         pbyPBACEReg[STA_BUF_NO] = SET_RDY_Base[RDY_PG_OFS_STS_BUF_71h];
         pbyPBACEReg[STA_WR_ENTRY] = SET_RDY_Base[RDY_PG_OFS_STS_BUF1_VALUE_71h]&0x1F;
    
        

        //cmd72
         pbyPBACEReg[STA_BUF_DLY_T_NO] = SET_RDY_Base[RDY_PG_OFS_STS_BUF_72h];   
         pbyPBACEReg[STA_WR_DLY_T_ENTRY] = SET_RDY_Base[RDY_PG_OFS_STS_BUF2_VALUE_72h]&0x1F;
        
        
         pbyPBACEReg[STA_BUF_NO] = SET_RDY_Base[RDY_PG_OFS_STS_BUF_72h];           
         pbyPBACEReg[STA_WR_ENTRY] = SET_RDY_Base[RDY_PG_OFS_STS_BUF1_VALUE_72h]&0x1F;
       

        //cmd73
         pbyPBACEReg[STA_BUF_DLY_T_NO] = SET_RDY_Base[RDY_PG_OFS_STS_BUF_73h];   
         pbyPBACEReg[STA_WR_DLY_T_ENTRY] = SET_RDY_Base[RDY_PG_OFS_STS_BUF2_VALUE_73h]&0x1F; 
 
            
         pbyPBACEReg[STA_BUF_NO] = SET_RDY_Base[RDY_PG_OFS_STS_BUF_73h];
         pbyPBACEReg[STA_WR_ENTRY] = SET_RDY_Base[RDY_PG_OFS_STS_BUF1_VALUE_73h]&0x1F;
     
        


        //cmd74
         pbyPBACEReg[STA_BUF_DLY_T_NO] = SET_RDY_Base[RDY_PG_OFS_STS_BUF_74h];  
         pbyPBACEReg[STA_WR_DLY_T_ENTRY] = SET_RDY_Base[RDY_PG_OFS_STS_BUF2_VALUE_74h]&0x1F;
   
         pbyPBACEReg[STA_BUF_NO] = SET_RDY_Base[RDY_PG_OFS_STS_BUF_74h];         
         pbyPBACEReg[STA_WR_ENTRY] = SET_RDY_Base[RDY_PG_OFS_STS_BUF1_VALUE_74h]&0x1F;
    
        
      

        /*
        //cmd75
        pbyPBACEReg[STA_BUF_DLY_T_NO] = SET_RDY_Base[RDY_PG_OFS_STS_BUF_75h];
        pbyPBACEReg[STA_WR_DLY_T_ENTRY] = SET_RDY_Base[RDY_PG_OFS_STS_BUF2_VALUE_75h];
        pbyPBACEReg[STA_BUF_NO] = SET_RDY_Base[RDY_PG_OFS_STS_BUF_75h];
        pbyPBACEReg[STA_WR_ENTRY] = SET_RDY_Base[RDY_PG_OFS_STS_BUF1_VALUE_75h];
        */
        
        //cmd76
         pbyPBACEReg[STA_BUF_DLY_T_NO] = SET_RDY_Base[RDY_PG_OFS_STS_BUF_76h];
         pbyPBACEReg[STA_WR_DLY_T_ENTRY] = SET_RDY_Base[RDY_PG_OFS_STS_BUF2_VALUE_76h]&0xFF;

         pbyPBACEReg[STA_BUF_NO] = SET_RDY_Base[RDY_PG_OFS_STS_BUF_76h];
         pbyPBACEReg[STA_WR_ENTRY] = SET_RDY_Base[RDY_PG_OFS_STS_BUF1_VALUE_76h]&0xFF;

         //cmd6C
         pbyPBACEReg[STA_BUF_DLY_T_NO] = SET_RDY_Base[RDY_PG_OFS_STS_BUF_6Ch];
         pbyPBACEReg[STA_WR_DLY_T_ENTRY] = SET_RDY_Base[RDY_PG_OFS_STS_BUF2_VALUE_6Ch]&0xFF;

         pbyPBACEReg[STA_BUF_NO] = SET_RDY_Base[RDY_PG_OFS_STS_BUF_6Ch];
         pbyPBACEReg[STA_WR_ENTRY] = SET_RDY_Base[RDY_PG_OFS_STS_BUF1_VALUE_6Ch]&0xFF;

         //cmd6B
         pbyPBACEReg[STA_BUF_DLY_T_NO] = SET_RDY_Base[RDY_PG_OFS_STS_BUF_6Bh];
         pbyPBACEReg[STA_WR_DLY_T_ENTRY] = SET_RDY_Base[RDY_PG_OFS_STS_BUF2_VALUE_6Bh]&0xFF;

         pbyPBACEReg[STA_BUF_NO] = SET_RDY_Base[RDY_PG_OFS_STS_BUF_6Bh];
         pbyPBACEReg[STA_WR_ENTRY] = SET_RDY_Base[RDY_PG_OFS_STS_BUF1_VALUE_6Bh]&0xFF;

         //Cmd 7C
         pbyPBACEReg[STA_BUF_DLY_T_NO] = STS_BUFNO_7C_CMD;
         pbyPBACEReg[STA_WR_DLY_T_ENTRY] = SET_RDY_Base[RDY_PG_OFS_2G_FLASH_TEMP]; 
    
         pbyPBACEReg[STA_BUF_NO] = STS_BUFNO_7C_CMD;           
         pbyPBACEReg[STA_WR_ENTRY] = SET_RDY_Base[RDY_PG_OFS_2G_FLASH_TEMP];    
         

        /*
         //cmd77
        pbyPBACEReg[STA_BUF_DLY_T_NO] = SET_RDY_Base[RDY_PG_OFS_STS_BUF_77h];
        pbyPBACEReg[STA_WR_DLY_T_ENTRY] = SET_RDY_Base[RDY_PG_OFS_STS_BUF2_VALUE_77h];
        pbyPBACEReg[STA_BUF_NO] = SET_RDY_Base[RDY_PG_OFS_STS_BUF_77h];
        pbyPBACEReg[STA_WR_ENTRY] = SET_RDY_Base[RDY_PG_OFS_STS_BUF1_VALUE_77h];
        */


      
        //cmdFx
        pbyPBACEReg[STA_BUF_DLY_T_NO] = SET_RDY_Base[RDY_PG_OFS_STS_BUF_BY_LUN];
        pbyPBACEReg[STA_WR_DLY_T_ENTRY] = SET_RDY_Base[RDY_PG_OFS_STS_BUF2_VAL]&0x1F; 
 
        pbyPBACEReg[STA_BUF_NO] = SET_RDY_Base[RDY_PG_OFS_STS_BUF_BY_LUN];
        pbyPBACEReg[STA_WR_ENTRY] = SET_RDY_Base[RDY_PG_OFS_STS_BUF1_VAL]&0x1F; 
   
        
     
    }
    
}


void pba_clear_CFDE_int_tag(void)
{
    volatile U8* pbyPBACEReg;
   
    //clear specified CE INT and TAG
    pbyPBACEReg = PBAREG_CE0 + SET_RDY_Base[RDY_PG_OFS_CE_IDX] * 0x100;
    pbyPBACEReg[CFDE_CLR_ALL_INTR] |= FLAG_CLEAR_INT;
    pbyPBACEReg[CFDE_TAG_99_96] |= CLR_CFDE_TAG;
    
    
    //clear temp tag
    if(SET_RDY_Base[RDY_PG_OFS_EN_Clear_Temp_Tag])  
       PBAREG[RR_CACHE] |= SET_CLR_CFDE_TAG_ALL;
    
    PBAREG[RR_CACHE] &= FORCE_CLR_CFDE_TAG_ALL_DEFAUL;
}

void pba_clear_cache_header_info(void)
{
    U8 i;
	
	volatile U8* pbyPBACEReg;

    //clear specified CE TAG
    pbyPBACEReg = PBAREG_CE0 + SET_RDY_Base[RDY_PG_OFS_CE_IDX] * 0x100;
    //pbyPBACEReg[CFDE_TAG_99_96] |= CLR_CFDE_TAG;
    
    for (i = 0; i < 0x08; i++) {
        PBAREG[CLR_CACHE_7_0_HEADER + i] = SET_RDY_Base[RDY_PG_OFS_CLR_CACHE_7_0_HEADER + i];
    }

}


void pba_Set_Ready_Busy_for_Lun(U8 byLunIdx)
{
   volatile U8* pbyPBACEReg;
   pbyPBACEReg = PBAREG_CE0 + SET_RDY_Base[RDY_PG_OFS_CE_IDX] * 0x100;

   
   //Set Ready
    if (byLunIdx == 0) {
        pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN0] = SET_ALL_RDY;
        //pbyPBACEReg[SET_BSY8_5_BUSY_RDY_LUN0] = SET_ALL_RDY; 
    } 
    else if(byLunIdx > 0 && byLunIdx < 8) 
    {
        pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN1 + (byLunIdx - 1) * 2] = SET_ALL_RDY;
        //pbyPBACEReg[SET_BSY8_5_BUSY_RDY_LUN1 + (byLunIdx - 1) * 2] = SET_ALL_RDY;
    }   
}

void pba_set_rdy_busy_for_ce_lun(void)
{
    volatile U8* pbyPBACEReg;
    U8 byCEIdx, byLunIdx,i;

    byCEIdx  = SET_RDY_Base[RDY_PG_OFS_CE_IDX]; 
    byLunIdx = SET_RDY_Base[RDY_PG_OFS_LUN_IDX]; 

    pbyPBACEReg = PBAREG_CE0 + byCEIdx * 0x100;


    if(SET_RDY_Base[RDY_PG_OFS_VENDOR_CMD_SET_ALL_READY]==0x01)
    { //Flash Vendoe Mode Set All Ready  
        if(AIPR_EN)
        {
            pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN0] = SET_ALL_BSY;
            pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN1] = SET_ALL_BSY;
            pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN2] = SET_ALL_BSY;
            pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN3] = SET_ALL_BSY;
            pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN4] = SET_ALL_BSY;
            pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN5] = SET_ALL_BSY;
            pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN6] = SET_ALL_BSY;
            pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN7] = SET_ALL_BSY;
        }
        pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN0] = SET_ALL_RDY;
        //pbyPBACEReg[SET_BSY8_5_BUSY_RDY_LUN0] = SET_ALL_RDY;

        for (i = 0x00; i < 0x0E; i++) {
            pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN1 + i] = SET_ALL_RDY;
            
        }    
    }
    else
    {

        if(AIPR_EN)
        {
            
            for(i=0;i<SET_RDY_Base[RDY_PG_OFS_SET_RDY_MAX_LUN_NO];i++)
            {
               byLunIdx=SET_RDY_Base[RDY_PG_OFS_SET_RDY_LUN_NO_0+i];
               pba_Set_Ready_Busy_for_Lun(byLunIdx);
            }
        }
        else
        {
        //Set Ready
            if (byLunIdx == 0) {
                pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN0] = SET_ALL_RDY;
                //pbyPBACEReg[SET_BSY8_5_BUSY_RDY_LUN0] = SET_ALL_RDY; 
            } 
            else if(byLunIdx > 0 && byLunIdx < 8) 
            {
                pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN1 + (byLunIdx - 1) * 2] = SET_ALL_RDY;
               // pbyPBACEReg[SET_BSY8_5_BUSY_RDY_LUN1 + (byLunIdx - 1) * 2] = SET_ALL_RDY;
            }     
        }   
    }
           
}

void pba_set_ce_all_rdy(void)
{
    volatile U8* pbyPBACEReg;

    pbyPBACEReg = PBAREG_CE0 + SET_RDY_Base[RDY_PG_OFS_CE_IDX] * 0x100;


    if(AIPR_EN)
    {
        pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN0] = SET_ALL_BSY;
        pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN1] = SET_ALL_BSY;
        pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN2] = SET_ALL_BSY; 
        pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN3] = SET_ALL_BSY;  
        pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN4] = SET_ALL_BSY;
        pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN5] = SET_ALL_BSY;
        pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN6] = SET_ALL_BSY; 
        pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN7] = SET_ALL_BSY;

    }                                   


    pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN0] = SET_ALL_RDY;
    pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN1] = SET_ALL_RDY;
    pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN2] = SET_ALL_RDY; 
    pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN3] = SET_ALL_RDY;
    pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN4] = SET_ALL_RDY;
    pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN5] = SET_ALL_RDY;
    pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN6] = SET_ALL_RDY; 
    pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN7] = SET_ALL_RDY;

}


void Shift_Lun_Plane_Bit()
{

   volatile U8* pbyPBACEReg;
   U8 i=0;
    
   pbyPBACEReg = PBAREG_CE0 + SET_RDY_Base[RDY_PG_OFS_CE_IDX] * 0x100;

   
   
    //Write Cache PA_SEL
    for (i = 0; i < 0x04; i++)
    {
       pbyPBACEReg[CC_SEL_PA0 + i] = SET_RDY_Base[RDY_PG_OFS_Dynamic_WR_CACHE_SEL_PA0+i];
    }
       pbyPBACEReg[CC_SEL_PA4] = SET_RDY_Base[RDY_PG_OFS_Dynamic_WR_CACHE_SEL_PA4];
       pbyPBACEReg[CC_SEL_PA5] = SET_RDY_Base[RDY_PG_OFS_Dynamic_WR_CACHE_SEL_PA5];

    //Read Cache PA_SEL
    for (i = 0; i < 0x06; i++)
       pbyPBACEReg[RD_CACHE_SEL_PA0 + i] = SET_RDY_Base[RDY_PG_OFS_Dynamic_WR_CACHE_SEL_PA0+i];

    //Cache Head Lun Plane Bit
    for (i = 0; i < 0x03; i++) 
    {
       pbyPBACEReg[PLANE_SEL_PA0 + i] = SET_RDY_Base[RDY_PG_OFS_Dynamic_CH_PLANE_SEL_PA0 + i];
    }
       pbyPBACEReg[LUN_SEL_PA0] = SET_RDY_Base[RDY_PG_OFS_Dynamic_CH_LUN_SEL_PA0];
       pbyPBACEReg[LUN_SEL_PA1] = SET_RDY_Base[RDY_PG_OFS_Dynamic_CH_LUN_SEL_PA1];
       pbyPBACEReg[LUN_SEL_PA2] = SET_RDY_Base[RDY_PG_OFS_Dynamic_CH_LUN_SEL_PA2];      
 
    //Cmd78 STA_SEL
    for (i = 0; i < 0x06; i++)
       pbyPBACEReg[STA_SEL_PA0 + i] = SET_RDY_Base[RDY_PG_OFS_Dynamic_STA78_PLANE_SEL_PA0 + i ];

 
   
   
        
}

BOOL pba_set_rdy_page(void)
{
    volatile U8* pbyPBACEReg;

    pbyPBACEReg = PBAREG_CE0 + SET_RDY_Base[RDY_PG_OFS_CE_IDX] * 0x100;
    

    /* Check ready page mark */
    if (SET_RDY_Base[RDY_PG_OFS_HD0] != RDY_PG_VAL_HD0 || SET_RDY_Base[RDY_PG_OFS_HD1] != RDY_PG_VAL_HD1)
    {
        return FAIL;
    }

        
    if(SET_RDY_Base[RDY_PG_OFS_EN_CE_LEVEL_MODE]||SET_RDY_Base[RDY_PG_OFS_EN_LUN_LEVEL_MODE])
    {
        //CE level mode clear specified CE CFDE INT,TAG,temp tag   
        pba_clear_CFDE_int_tag();
    }
    else
    {
      //non CE level mode clear cache header info
        pba_clear_cache_header_info();
    }
   
   

    /* clear bus analyzer status */
    if (SET_RDY_Base[RDY_PG_OFS_BUS_ANA_EN] == 0x01)
    {
        PBAREG[BUS_ANA_STS] = SET_RDY_Base[RDY_VALUE_BUS_ANA_STS];
    }

    if (SET_RDY_Base[RDY_PG_OFS_CLR_PWR_CHG_EVENT_EN] == CLR_PWR_CHG_EVENT_EN)
    {
       PBAREG[PWR_CHG_INT] |= CHG_INT_CLR;
        // reset to system default driving
        pba_reset_default_drive_setting();
    }

    if (SET_RDY_Base[RDY_PG_OFS_EN_CLR_CE_TEMP_ADDR] == 0x01)
    {  
        pbyPBACEReg[CFDE_CLR_ALL_INTR] |= CLR_CUR_TEMP_ADDR_BUF; 
    }
    /* Enable AFIFO setting */
    if (SET_RDY_Base[RDY_PG_OFS_EN_AFIFO_SETTING] == 0x01)
    {
        PBAREG[CRBYTE0] |= EN_AFIFO_RST;
        PBAREG[CRBYTE0] |= EN_PRELOAD_AFIFO_DATA;
    }

    //dynamic Shift Lun plane Bit 
    if(SET_RDY_Base[RDY_PG_OFS_Shift_Lun_Plane_Bit] == 0x01)
    {
       Shift_Lun_Plane_Bit();
    }
        
    
    //clear CE write counter
    if (SET_RDY_Base[RDY_PG_OFS_EN_CLR_WR_CNT_CE] == 0x01)
    {
        pbyPBACEReg[CRBYTE0_CE] |= CLR_WR_CNT; 
    }

    //clear CE read counter
    if (SET_RDY_Base[RDY_PG_OFS_EN_CLR_RD_CNT_CE] == 0x01)
    {
        pbyPBACEReg[CRBYTE0_CE] |= CLR_RD_CNT;
    }

    //clear CE single addr queue
    if (SET_RDY_Base[RDY_PG_OFS_EN_CLR_SG_ADDR_QUEUE_CE] == 0x01)
    {
        pbyPBACEReg[CRBYTE0_CE] |= CLR_SG_ADDR_QUEUE;
    }



    
    if (SET_RDY_Base[RDY_PG_OFS_STATUS_SW_MODE_EN] == 0x01)
    {
        pba_update_statusbuf_entry_value();       
    }
    
   
    
    if (SET_RDY_Base[RDY_PG_OFS_SET_DEALY_WINDOW_EN] == 0x01)
    {
        pba_set_bsy_dly(SET_RDY_Base[RDY_PG_OFS_CE_IDX], SET_RDY_Base[RDY_PG_OFS_LUN_IDX], SET_RDY_Base[RDY_PG_OFS_SUBBSY_IDX], SET_RDY_Base[RDY_PG_OFS_DEALY_WINDOW_VAL]);
    } 
    else
        pba_set_bsy_No_dly(SET_RDY_Base[RDY_PG_OFS_CE_IDX],SET_RDY_Base[RDY_PG_OFS_LUN_IDX], SET_RDY_Base[RDY_PG_OFS_SUBBSY_IDX]);
    
        
 
    /* Set Ready Busy for which CE and LUN */
    if (SET_RDY_Base[RDY_PG_OFS_EN_CE_LEVEL_MODE] == 0x01) {
        pba_set_ce_all_rdy();
    } else {
        pba_set_rdy_busy_for_ce_lun();
    }


    
    return PASS;

}


void pba_fill_entryvalue_and_infobit(volatile U8* pbyPBACEReg, U8 byStatusBufIdx, U16 wStartPos)
{
    //volatile U8* pbyPBACEReg;
    U8 byStaBitOff;

    pbyPBACEReg[STA_BUF_DLY_T_NO] = byStatusBufIdx;
    pbyPBACEReg[STA_WR_DLY_T_ENTRY] = 0xE0;

    pbyPBACEReg[STA_BUF_NO] = byStatusBufIdx;
    pbyPBACEReg[STA_WR_ENTRY] = 0x80;

    //Bit0~4 use default SW mode
    for (byStaBitOff = 0; byStaBitOff < 0x05; byStaBitOff++) {
        pbyPBACEReg[STA0_BIT0 + byStaBitOff] = IB_STABUF_ENTERY;
    }

    //Bit5~7 determine by info page parameter
    pbyPBACEReg[STA0_BIT5] = INFO_PAGE_Base[wStartPos];
    pbyPBACEReg[STA0_BIT6] = INFO_PAGE_Base[wStartPos + 1];
    pbyPBACEReg[STA0_BIT7] = INFO_PAGE_Base[wStartPos + 2];
}

void Pba_fill_tempture(volatile U8* pbyPBACEReg, U8 byStatusBufIdx, U8 temperature)
{
    U8 byStaBitOff;
    
    pbyPBACEReg[STA_BUF_DLY_T_NO] = byStatusBufIdx;
    pbyPBACEReg[STA_WR_DLY_T_ENTRY] = temperature;

    pbyPBACEReg[STA_BUF_NO] = byStatusBufIdx;
    pbyPBACEReg[STA_WR_ENTRY] = temperature;

    //Bit0~4 use default SW mode
    for (byStaBitOff = 0; byStaBitOff < 0x05; byStaBitOff++) {
        pbyPBACEReg[STA0_BIT0 + byStaBitOff] = IB_STABUF_ENTERY;
    }

    //Bit5~7 determine by info page parameter
    pbyPBACEReg[STA0_BIT5] = IB_STABUF_ENTERY;
    pbyPBACEReg[STA0_BIT6] = IB_STABUF_ENTERY;
    pbyPBACEReg[STA0_BIT7] = IB_STABUF_ENTERY;
}



void pba_initial_status_buf(void)
{
    volatile U8* pbyPBACEReg;
    U8 byLunIdx, byPlaneIdx;
    //U8 byStaBitOff;
    U8 byCeIdx,byBSYIdx;
    //U8 byStatusBufIdx;

    for (byCeIdx = 0; byCeIdx < _gMax_Ce_Cnt; byCeIdx++) {
        pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;

        //fill cmd78
        for (byLunIdx = 0; byLunIdx < LUN_CNT_CE; byLunIdx++) {
            for (byPlaneIdx = 0; byPlaneIdx < PLANE_CNT_LUN; byPlaneIdx++) {
                pba_fill_entryvalue_and_infobit(pbyPBACEReg, (STS_BUFNO_ENH_78CMD + byLunIdx * PLANE_CNT_LUN + byPlaneIdx),  (IP_CMD78_LUN0_STA0_BIT5_OFS + byLunIdx * 0x03));
            }
        }

        //fill enhance cmd71
        for (byLunIdx = 0; byLunIdx < LUN_CNT_CE; byLunIdx++) {
            pba_fill_entryvalue_and_infobit(pbyPBACEReg, STS_BUFNO_ENH_71CMD + byLunIdx, (IP_ENCMD71_LUN0_STA0_BIT5_OFS + byLunIdx * 0x03));
        }

        //fill other status cmd(Cmd70/71/72/73/74/F1,F2,F3,F4,F5,F6,F7,F8)
        pba_fill_entryvalue_and_infobit(pbyPBACEReg, STS_BUFNO_70CMD, IP_70_LUN0_STA0_BIT5_OFS);

        pba_fill_entryvalue_and_infobit(pbyPBACEReg, STS_BUFNO_71CMD, IP_71_STA_BIT5_OFS);

        pba_fill_entryvalue_and_infobit(pbyPBACEReg, STS_BUFNO_72CMD, IP_72_STA_BIT5_OFS);

        pba_fill_entryvalue_and_infobit(pbyPBACEReg, STS_BUFNO_73CMD, IP_73_STA_BIT5_OFS);

        pba_fill_entryvalue_and_infobit(pbyPBACEReg, STS_BUFNO_74CMD, IP_74_STA_BIT5_OFS);

        pba_fill_entryvalue_and_infobit(pbyPBACEReg, STS_BUFNO_76CMD, IP_76_STA_BIT5_OFS);


        pba_fill_entryvalue_and_infobit(pbyPBACEReg, STS_BUFNO_F1CMD, IP_F1_LUN0_STA0_BIT5_OFS);

        pba_fill_entryvalue_and_infobit(pbyPBACEReg, STS_BUFNO_F2CMD, IP_F2_LUN1_STA0_BIT5_OFS);

        pba_fill_entryvalue_and_infobit(pbyPBACEReg, STS_BUFNO_F3CMD, IP_F3_LUN2_STA0_BIT5_OFS);
        
        pba_fill_entryvalue_and_infobit(pbyPBACEReg, STS_BUFNO_F4CMD, IP_F4_LUN3_STA0_BIT5_OFS);

        pba_fill_entryvalue_and_infobit(pbyPBACEReg, STS_BUFNO_F5CMD, IP_F5_LUN4_STA0_BIT5_OFS);
        
        pba_fill_entryvalue_and_infobit(pbyPBACEReg, STS_BUFNO_F6CMD, IP_F6_LUN5_STA0_BIT5_OFS);

        pba_fill_entryvalue_and_infobit(pbyPBACEReg, STS_BUFNO_F7CMD, IP_F7_LUN6_STA0_BIT5_OFS);
        
        pba_fill_entryvalue_and_infobit(pbyPBACEReg, STS_BUFNO_F8CMD, IP_F8_LUN7_STA0_BIT5_OFS);

        Pba_fill_tempture(pbyPBACEReg,STS_BUFNO_7C_CMD,0x5C);

        pba_fill_entryvalue_and_infobit(pbyPBACEReg, STS_BUFNO_6CCMD, IP_6C_STA_BIT5_OFS);

        pba_fill_entryvalue_and_infobit(pbyPBACEReg, STS_BUFNO_6BCMD, IP_6B_STA_BIT5_OFS);

       //Fill LUN0~LUN8 BYS1~BSY8 Delay time
       for (byBSYIdx=0;byBSYIdx<8;byBSYIdx++)
       {
          pbyPBACEReg[RDY_STAT_DLY1_LUN0+byBSYIdx]=INFO_PAGE_Base[IP_STA_LUN0_BSY1_BSY8_DLY];
          pbyPBACEReg[RDY_STAT_DLY1_LUN1+byBSYIdx]=INFO_PAGE_Base[IP_STA_LUN1_BSY1_BSY8_DLY];
          pbyPBACEReg[RDY_STAT_DLY1_LUN2+byBSYIdx]=INFO_PAGE_Base[IP_STA_LUN2_BSY1_BSY8_DLY];
          pbyPBACEReg[RDY_STAT_DLY1_LUN3+byBSYIdx]=INFO_PAGE_Base[IP_STA_LUN3_BSY1_BSY8_DLY];
          pbyPBACEReg[RDY_STAT_DLY1_LUN4+byBSYIdx]=INFO_PAGE_Base[IP_STA_LUN4_BSY1_BSY8_DLY];
          pbyPBACEReg[RDY_STAT_DLY1_LUN5+byBSYIdx]=INFO_PAGE_Base[IP_STA_LUN5_BSY1_BSY8_DLY];
          pbyPBACEReg[RDY_STAT_DLY1_LUN6+byBSYIdx]=INFO_PAGE_Base[IP_STA_LUN6_BSY1_BSY8_DLY];
          pbyPBACEReg[RDY_STAT_DLY1_LUN7+byBSYIdx]=INFO_PAGE_Base[IP_STA_LUN7_BSY1_BSY8_DLY];
       }
    }
}


void pba_clear_allceint_and_setallrdy(void)
{
    U8 byCeIdx;

    for (byCeIdx = 0; byCeIdx < 0x08; byCeIdx++) {
        pba_clear_int_and_setrdy(byCeIdx);
    }
}

void pba_clear_int_and_setrdy(U8 byCeIdx)
{
    U8 j;
    volatile U8* pbyPBACEReg;

    pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;

    pbyPBACEReg[CFDE_CLR_ALL_INTR] = 0x01;
    pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN0] = 0x55;
    pbyPBACEReg[SET_BSY8_5_BUSY_RDY_LUN0] = 0x55;

    for (j = 0x00; j < 0x0E; j++) {
        pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN1 + j] = 0x55;
    }

}
