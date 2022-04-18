/**
 * @file pba_test.c
 * Copyrihgt 2015 PHISON Electronics Corp.
 *
 * <b>
@verbatim
FILE NAME :  pba_test.c
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

#define __PBA_TEST_C__

/*--------------------------------------------------------------------------*/
/* INCLUDE FILE                                                             */
/*--------------------------------------------------------------------------*/
#include "nes.h"
#ifdef D_NES_CMODEL
#include "8051.h"
#else
#include <reg52.h>
#include <8051.h>
#endif
#include <string.h>
#include "MCU_Reg.h"
#include "Var_def.h"
#include "Buf_DEF.h"
#include "UsbReg.h"
#include "USB_API.h"
#include "UART_API.h"
#include "GPIO_Reg.h"
#include "pba_api.h"
#include "pba_vc_api.h"
#include "pba_test.h"
#include "pba_test_api.h"

#include "DMA_Reg.h"

#if EN_PBA_TEST
/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTION IMPLEMENT                                                 */
/*--------------------------------------------------------------------------*/
static void dummy_fun(void)
{


}

#if EN_REG_WR_TEST
static void reg_wr_rd_test(void)
{
    volatile U8* pbyPBACEReg;
    U8  byCeIdx;
    U16 wRegOff;

    //Check power value of Global Reg
    for (wRegOff = 0; wRegOff < 0x100; wRegOff++) {
        //check power on default value
        if (PBAREG[wRegOff] != _gabyGRegDefaultTbl[wRegOff]) {
            if(wRegOff!=0xF8&&wRegOff!=0xFB&&wRegOff!=0xF4&&wRegOff!=0xF5)
            {  
              _gwPBA_ErrCode = 0x0101;
              //return;
            } 

        }
    }

    //Check power value of of CE Reg
    for (byCeIdx = 0; byCeIdx < 0x08; byCeIdx++) {
        pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;
        for (wRegOff = 0; wRegOff < 0x100; wRegOff++) {
            //check power on default value
            if (pbyPBACEReg[wRegOff] != _gabyCERegDefaultTbl[wRegOff]) {
                if(wRegOff!=0x7D)
                {
                   _gwPBA_ErrCode = 0x0102;
                  // return;
                }
               
            }
        }
    }

    //W/R test of Global Reg
    for (wRegOff = 0; wRegOff < 0x100; wRegOff++) {
        //write read compare
        PBAREG[wRegOff] = 0xFF;
        if (PBAREG[wRegOff] != _gabyGRegCmpTbl[wRegOff]) {
            if(wRegOff!=0x02&&wRegOff!=0xF8&&wRegOff!=0xFB&&wRegOff!=0xFE&&wRegOff!=0xFF&&wRegOff!=0x9F&&wRegOff!=0xF4&&wRegOff!=0xF5)
            {
               _gwPBA_ErrCode = 0x0103;
              // return;
            }
           
        }
        PBAREG[wRegOff] = 0x00;
    }

    //W/R test of of CE Reg
    for (byCeIdx = 0; byCeIdx < 0x08; byCeIdx++) {
        pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;
        for (wRegOff = 0; wRegOff < 0x100; wRegOff++) {
            //write read compare
            pbyPBACEReg[wRegOff] = 0xFF;
            if (pbyPBACEReg[wRegOff] != _gabyCERegCmpTbl[wRegOff]) {
                 if(wRegOff!=0x41&&wRegOff!=0x7D)
                 {
                     _gwPBA_ErrCode = 0x0104;
                   // return;
                 }
               
            }
            pbyPBACEReg[wRegOff] = 0x00;
        }
    }

    dummy_fun();
}
#else
static void set_lun_plane_bit(void)
{
    volatile U8* pbyPBACEReg;
    U8  byCeIdx;
    //U8 abyPlaneBit[3];
    //U8 abyLunBit[3];

    for (byCeIdx = 0; byCeIdx < MAX_CE_CNT; byCeIdx++) {
        pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;

        //Write Cache PA_SEL
        pbyPBACEReg[CC_SEL_PA0] = PLANE_BIT0;
        pbyPBACEReg[CC_SEL_PA1] = PLANE_BIT1;
        pbyPBACEReg[CC_SEL_PA2] = PLANE_BIT2;
        pbyPBACEReg[CC_SEL_PA3] = LUN_BIT0;
        pbyPBACEReg[CC_SEL_PA4] = LUN_BIT1;
        pbyPBACEReg[CC_SEL_PA5] = LUN_BIT2;

        //Read Cache PA_SEL
        pbyPBACEReg[RD_CACHE_SEL_PA0] = PLANE_BIT0;
        pbyPBACEReg[RD_CACHE_SEL_PA1] = PLANE_BIT1;
        pbyPBACEReg[RD_CACHE_SEL_PA2] = PLANE_BIT2;
        pbyPBACEReg[RD_CACHE_SEL_PA3] = LUN_BIT0;
        pbyPBACEReg[RD_CACHE_SEL_PA4] = LUN_BIT1;
        pbyPBACEReg[RD_CACHE_SEL_PA5] = LUN_BIT2;

        //Enahnce 71Cmd STA_SEL
        pbyPBACEReg[STA_SEL_PA0_ENH] = LUN_BIT0_EN71;
        pbyPBACEReg[STA_SEL_PA1_ENH] = LUN_BIT1_EN71;
        pbyPBACEReg[STA_SEL_PA2_ENH] = LUN_BIT2_EN71;

        //Cmd78 STA_SEL
        pbyPBACEReg[STA_SEL_PA0] = PLANE_BIT0;
        pbyPBACEReg[STA_SEL_PA1] = PLANE_BIT1;
        pbyPBACEReg[STA_SEL_PA2] = PLANE_BIT2;
        pbyPBACEReg[STA_SEL_PA3] = LUN_BIT0;
        pbyPBACEReg[STA_SEL_PA4] = LUN_BIT1;
        pbyPBACEReg[STA_SEL_PA5] = LUN_BIT2;
    }
}

static void set_bsy_dly(U8 byLunDlyValue)
{
    volatile U8* pbyPBACEReg;
    U8 byCeIdx;
    U8 i;

    for (byCeIdx = 0; byCeIdx < MAX_CE_CNT; byCeIdx++) {
        pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;
        for (i = 0; i < 8; i++) {
            pbyPBACEReg[RDY_STAT_DLY1_LUN0 + i] = byLunDlyValue;
        }

        for (i = 0; i < (8 * 6); i++) {
            pbyPBACEReg[RDY_STAT_DLY1_LUN1 + i] = byLunDlyValue;
        }

        for (i = 0; i < 8; i++) {
            pbyPBACEReg[RDY_STAT_DLY1_LUN7 + i] = byLunDlyValue;
        }
    }
}

static void pba_test_initial_pba_reg(void)
{
    volatile U8* pbyPBACEReg;
    U8 byCeIdx;

    _gwPBA_ErrCode = 0x0000;

    //U8 byCeIdx, byCeCnt, byLunCnt, byPlaneCnt;

    //IOW F000 80, PBA RST
    PBAREG[CRBYTE0] = 0x80;

#if EN_TOGGLE_MODE_TEST
#if EN_USE_NEW_PBA_DEFINE
    for (byCeIdx = 0; byCeIdx < MAX_CE_CNT; byCeIdx++) {
        pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;
        //set flash type
        pbyPBACEReg[CRBYTE0_CE] &= MASK_MODE;
        pbyPBACEReg[CRBYTE0_CE] |= S_TOGGLE_MODE;
    }
#else
    //set flash type
    PBAREG[CRBYTE0] &= MASK_MODE;
    PBAREG[CRBYTE0] |= S_TOGGLE_MODE;
#endif
#elif EN_ONFI_MODE_TEST
    //set flash type
    PBAREG[CRBYTE0] &= MASK_MODE;
    PBAREG[CRBYTE0] |= S_ONFI_MODE;
#endif

    //Select PBA 8K/16K Mode
    //PBAREG[PAGE_SIZE] = PAGE8K;
    PBAREG[PAGE_SIZE] = PAGE16K;

#if EN_SW_MODE_TEST
    PBAREG[RR_CACHE] &= SET_FW_OR_HWADDR_MODE;
    PBAREG[WR_CACHE] &= SW_WR_CCMODE_SEL;
    PBAREG[RD_CACHE] &= SW_RD_CCMODE_SEL;
    PBAREG[WR_CACHE] = 0x00;
    PBAREG[RD_CACHE] = 0x00;
#elif EN_HW_ADDR_MODE_TEST
    PBAREG[RR_CACHE] &= SET_FW_OR_HWADDR_MODE;
    PBAREG[WR_CACHE] |= HW_WR_CCMODE_SEL;
    //PBAREG[RD_CACHE] |= HW_RD_CCMODE_SEL;
    PBAREG[RD_CACHE] |= (HW_RD_CCMODE_SEL | FORCE_RD_CACHE_MODE_SEL);
    //for test
    PBAREG[WR_CACHE] |= 0x0F;
    PBAREG[RD_CACHE] |= 0x0F;
#elif EN_RR_MODE_TEST
    PBAREG[RR_CACHE] |= SET_RR_MODE;
#endif

#if !EN_CACHE_32SETS_TEST
    PBAREG[RR_CACHE] |= SET_64SETS_CACHE_BUF;
#endif

    //PBAREG[SET_CE_LUN_PLANE] |= (FOURCE_MODE | ONELUN_MODE | ONEPLANE_MODE);
    //PBAREG[SET_CE_LUN_PLANE] |= (ONECE_MODE | EIGHTLUN_MODE | EIGHTPLANE_MODE);
    //PBAREG[SET_CE_LUN_PLANE] |= (ONECE_MODE | ONELUN_MODE | ONEPLANE_MODE);
    PBAREG[SET_CE_LUN_PLANE] |= ((CE_MODE << 4) | (LUN_MODE << 2) | PLANE_MODE);

    PBAREG[EN_ALL_CE_WP_LOCK] &= REF_CE_NUM;

    //default disable WP/LockUnlock function    
    PBAREG[EN_ALL_CE_WP_LOCK] &= ~(EN_LOCK_UNLOCK_FUN | EN_WP_FUN);

    //HW debug mode
    PBAREG[CFDE_DBG_SEL] = 0x12;

    //PBAREG_CE0[RR_RPTR_LD] = 0x41;

    set_lun_plane_bit();

    pba_initial_status_buf();

    set_bsy_dly(0x3F);
}

static void setting_CFDE_unit_FFCmd(void)
{
    // CFDE sel cmd ff
    /*
    IOW F047 00
    IOW F040 00
    IOW F041 08
    IOW F042 03
    IOW F043 fe
    IOW F044 00
    IOW F045 27

    IOW F040 00
    IOW F041 00
    IOW F042 28
    IOW F043 00
    IOW F044 04
    IOW F045 00
    */
    //CmdFF
    PBAREG[CFDE_SEL] = CFDE_IDX_CMD_FF;
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x08;
    PBAREG[CFDE_CMDQ_31_24] = 0x03;
    PBAREG[CFDE_CMDQ_23_16] = 0xfe;
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x27;

    //EOC
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x28;
    PBAREG[CFDE_CMDQ_23_16] = 0x00;
    PBAREG[CFDE_CMDQ_15_8]  = 0x04;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;
}

static void setting_CFDE_unit_8010Cmd(void)
{
    //U32 dwPageLen = 16384+64;
    U32 dwPageLen = 16384;
    //U32 dwPageLen = 8190;

    // CFDE sel cmd 80-10
    /*
    IOW F047 01
    IOW F040 00
    IOW F041 00
    IOW F042 03
    IOW F043 00
    IOW F044 40
    IOW F045 00
    */
    //Cmd80
    PBAREG[CFDE_SEL] = CFDE_IDX_CMD_8010;
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x03;
    PBAREG[CFDE_CMDQ_23_16] = 0x00; //CMD80
#if EN_HW_ADDR_MODE_TEST
    PBAREG[CFDE_CMDQ_15_8]  = 0x40 | HW_ADDR;
#else
    PBAREG[CFDE_CMDQ_15_8]  = 0x40;
#endif
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 04
    IOW F043 20
    IOW F044 00
    IOW F045 00
    */
    //adr0
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x04;
    PBAREG[CFDE_CMDQ_23_16] = 0x20;
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 06
    IOW F043 20
    IOW F044 00
    IOW F045 00
    */
    //adr1
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x06;
    PBAREG[CFDE_CMDQ_23_16] = 0x20;
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 08
    IOW F043 20
    IOW F044 00
    IOW F045 00
    */
    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x08;
    PBAREG[CFDE_CMDQ_23_16] = 0x20;
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 0a
    IOW F043 20
    IOW F044 00
    IOW F045 00
    */
    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x0a;
    PBAREG[CFDE_CMDQ_23_16] = 0x20;
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 0c
    IOW F043 20
    IOW F044 40
    IOW F045 00
    */
    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x0c;
    PBAREG[CFDE_CMDQ_23_16] = 0x20;
    PBAREG[CFDE_CMDQ_15_8]  = 0x40;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 1c
    IOW F043 80
    IOW F044 80
    IOW F045 00
    */
    //Data 16384+64
    /*
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x1c;
    PBAREG[CFDE_CMDQ_23_16] = 0x80;
    PBAREG[CFDE_CMDQ_15_8]  = 0x80;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;
    */
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_WDATA_DDR) | DATA_LEN_H(dwPageLen);
    PBAREG[CFDE_CMDQ_23_16] = DATA_LEN_M(dwPageLen);
    PBAREG[CFDE_CMDQ_15_8]  = DATA_LEN_L(dwPageLen);
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 04
    IOW F042 02
    IOW F043 20
    IOW F044 00
    IOW F045 01
    */
    //Cmd10
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_39_32] = 0x05; //bit32 Round_Robin cache wtag
#else
    PBAREG[CFDE_CMDQ_39_32] = 0x04; //BSY1
#endif
    PBAREG[CFDE_CMDQ_31_24] = 0x02;
    PBAREG[CFDE_CMDQ_23_16] = 0x20; //CMD10
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
#if EN_RR_MODE_TEST || EN_HW_ADDR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = 0x15; //bit4 Round_Robin cache ptr inc, bit2 for auto addess end
#else
    PBAREG[CFDE_CMDQ_7_0]   = 0x05; //bit2 for auto addess end
#endif

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 28
    IOW F043 00
    IOW F044 04
    IOW F045 21
    */
    //EOC
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x28;
    PBAREG[CFDE_CMDQ_23_16] = 0x00;
    PBAREG[CFDE_CMDQ_15_8]  = 0x04;
    PBAREG[CFDE_CMDQ_7_0]   = 0x21;
}

static void setting_CFDE_unit_70Cmd(void)
{
    //Cmd70
    PBAREG[CFDE_SEL] =  CFDE_IDX_CMD_70;
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x70) | OP_CODE(OP_CMD_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x70);
    PBAREG[CFDE_CMDQ_15_8]  = RD_DIR | OUT_SEL(STS_REG);
    PBAREG[CFDE_CMDQ_7_0]   = 0x00; //no interrupt for status read due to polling status

    //Status read
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_STS_READ);
    PBAREG[CFDE_CMDQ_23_16] = STS_BUF_NO_H(STS_BUFNO_70CMD);
    PBAREG[CFDE_CMDQ_15_8]  = STS_BUF_NO_L(STS_BUFNO_70CMD);
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //EOC
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_EOC);
    PBAREG[CFDE_CMDQ_23_16] = 0x00;
    PBAREG[CFDE_CMDQ_15_8]  = RST_LOCAL;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;
}

static void setting_CFDE_unit_0030Cmd(void)
{
    // CFDE sel cmd 00-30
    /*
    IOW F047 03
    IOW F040 00
    IOW F041 00
    IOW F042 02
    IOW F043 00
    IOW F044 08
    IOW F045 00
    */
    //Cmd00
    PBAREG[CFDE_SEL] = CFDE_IDX_CMD_0030;
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    //#if EN_RR_MODE_TEST
#if 0
    PBAREG[CFDE_CMDQ_39_32] = 0x02; //bit33 Round_Robin cache rtag
#else
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
#endif
    PBAREG[CFDE_CMDQ_31_24] = 0x02;
    PBAREG[CFDE_CMDQ_23_16] = 0x00;
#if EN_HW_ADDR_MODE_TEST
    PBAREG[CFDE_CMDQ_15_8]  = 0x08 | HW_ADDR;
#else
    PBAREG[CFDE_CMDQ_15_8]  = 0x08;
#endif
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 04
    IOW F043 22
    IOW F044 00
    IOW F045 00
    */
    //adr0
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x04;
    PBAREG[CFDE_CMDQ_23_16] = 0x22;
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 06
    IOW F043 22
    IOW F044 00
    IOW F045 00
    */
    //adr1
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x06;
    PBAREG[CFDE_CMDQ_23_16] = 0x22;
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 08
    IOW F043 22
    IOW F044 00
    IOW F045 00
    */
    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x08;
    PBAREG[CFDE_CMDQ_23_16] = 0x22;
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 0a
    IOW F043 22
    IOW F044 00
    IOW F045 00
    */
    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x0a;
    PBAREG[CFDE_CMDQ_23_16] = 0x22;
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 0c
    IOW F043 22
    IOW F044 00
    IOW F045 00
    */
    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x0c;
    PBAREG[CFDE_CMDQ_23_16] = 0x22;
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 30
    IOW F041 08
    IOW F042 02
    IOW F043 60
    IOW F044 08
    IOW F045 02
    */
    //Cmd30
    //PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_47_40] = 0x30; //enable bit43(Afifo prewrite)/bit44(Afifo RST)
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_39_32] = 0x0a; //bit33 Round_Robin cache rtag
#else
    PBAREG[CFDE_CMDQ_39_32] = 0x08; //BSY2
#endif
    PBAREG[CFDE_CMDQ_31_24] = 0x02;
    PBAREG[CFDE_CMDQ_23_16] = 0x60;
    PBAREG[CFDE_CMDQ_15_8]  = 0x08;
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = 0x16; //bit4 Round_Robin cache ptr inc
#else
    //PBAREG[CFDE_CMDQ_7_0]   = 0x02;
    PBAREG[CFDE_CMDQ_7_0]   = 0x06;
#endif

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 28
    IOW F043 00
    IOW F044 04
    IOW F045 00
    */
    //EOC
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x28;
    PBAREG[CFDE_CMDQ_23_16] = 0x00;
    PBAREG[CFDE_CMDQ_15_8]  = 0x04;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;
}

static void setting_CFDE_unit_31Cmd(void)
{
    // CFDE sel cmd 31
    /*
    IOW F047 0e
    IOW F040 00
    IOW F041 08
    IOW F042 02
    IOW F043 62
    IOW F044 08
    IOW F045 02
    */
    //Cmd31
    PBAREG[CFDE_SEL] = CFDE_IDX_CMD_31;
    //PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_47_40] = 0x30;
    PBAREG[CFDE_CMDQ_39_32] = 0x08;
    PBAREG[CFDE_CMDQ_31_24] = 0x02;
    PBAREG[CFDE_CMDQ_23_16] = 0x62;
    PBAREG[CFDE_CMDQ_15_8]  = 0x08;
    PBAREG[CFDE_CMDQ_7_0]   = 0x02;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 28
    IOW F043 00
    IOW F044 04
    IOW F045 00
    */
    //EOC
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x28;
    PBAREG[CFDE_CMDQ_23_16] = 0x00;
    PBAREG[CFDE_CMDQ_15_8]  = 0x04;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;
}

static void setting_CFDE_unit_3FCmd(void)
{
    // CFDE sel cmd 3F
    /*
    IOW F047 0d
    IOW F040 00
    IOW F041 08
    IOW F042 02
    IOW F043 7e
    IOW F044 08
    IOW F045 02
    */
    //Cmd31
    PBAREG[CFDE_SEL] = CFDE_IDX_CMD_3F;
    //PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_47_40] = 0x30;
    PBAREG[CFDE_CMDQ_39_32] = 0x08;
    PBAREG[CFDE_CMDQ_31_24] = 0x02;
    PBAREG[CFDE_CMDQ_23_16] = 0x7e;
    PBAREG[CFDE_CMDQ_15_8]  = 0x08;
    PBAREG[CFDE_CMDQ_7_0]   = 0x02;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 28
    IOW F043 00
    IOW F044 04
    IOW F045 00
    */
    //EOC
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x28;
    PBAREG[CFDE_CMDQ_23_16] = 0x00;
    PBAREG[CFDE_CMDQ_15_8]  = 0x04;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;
}


static void setting_CFDE_unit_8011Cmd(void)
{
    U32 dwPageLen = 16384;

    // CFDE sel cmd 80-11
    /*
    IOW F047 01
    IOW F040 00
    IOW F041 00
    IOW F042 03
    IOW F043 00
    IOW F044 40
    IOW F045 00
    */
    //Cmd80
    //PBAREG[CFDE_SEL] = CFDE_NUM_MAX;
    PBAREG[CFDE_SEL] = CFDE_IDX_CMD_8011;
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x03;
    PBAREG[CFDE_CMDQ_23_16] = 0x00; //CMD80
#if EN_HW_ADDR_MODE_TEST
    PBAREG[CFDE_CMDQ_15_8]  = 0x40 | HW_ADDR;
#else
    PBAREG[CFDE_CMDQ_15_8]  = 0x40;
#endif
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 04
    IOW F043 20
    IOW F044 00
    IOW F045 00
    */
    //adr0
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x04;
    PBAREG[CFDE_CMDQ_23_16] = 0x20;
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 06
    IOW F043 20
    IOW F044 00
    IOW F045 00
    */
    //adr1
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x06;
    PBAREG[CFDE_CMDQ_23_16] = 0x20;
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 08
    IOW F043 20
    IOW F044 00
    IOW F045 00
    */
    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x08;
    PBAREG[CFDE_CMDQ_23_16] = 0x20;
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 0a
    IOW F043 20
    IOW F044 00
    IOW F045 00
    */
    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x0a;
    PBAREG[CFDE_CMDQ_23_16] = 0x20;
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 0c
    IOW F043 20
    IOW F044 40
    IOW F045 00
    */
    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x0c;
    PBAREG[CFDE_CMDQ_23_16] = 0x20;
    PBAREG[CFDE_CMDQ_15_8]  = 0x40;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 1c
    IOW F043 80
    IOW F044 80
    IOW F045 00
    */
    //Data 16384+64
    /*
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x1c;
    PBAREG[CFDE_CMDQ_23_16] = 0x80;
    PBAREG[CFDE_CMDQ_15_8]  = 0x80;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;
    */
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_WDATA_DDR) | DATA_LEN_H(dwPageLen);
    PBAREG[CFDE_CMDQ_23_16] = DATA_LEN_M(dwPageLen);
    PBAREG[CFDE_CMDQ_15_8]  = DATA_LEN_L(dwPageLen);
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 04
    IOW F042 02
    IOW F043 20
    IOW F044 00
    IOW F045 01
    */
    //Cmd11
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_39_32] = 0x05; //bit32 Round_Robin cache wtag
#else
    PBAREG[CFDE_CMDQ_39_32] = 0x04; //BSY1
#endif
    PBAREG[CFDE_CMDQ_31_24] = 0x02;
    PBAREG[CFDE_CMDQ_23_16] = 0x22; //CMD11
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
#if EN_RR_MODE_TEST || EN_HW_ADDR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = 0x15; //bit4 Round_Robin cache ptr inc, bit2 for auto addess end
#else
    PBAREG[CFDE_CMDQ_7_0]   = 0x05; //bit2 for auto addess end
#endif

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 28
    IOW F043 00
    IOW F044 04
    IOW F045 21
    */
    //EOC
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x28;
    PBAREG[CFDE_CMDQ_23_16] = 0x00;
    PBAREG[CFDE_CMDQ_15_8]  = 0x04;
    PBAREG[CFDE_CMDQ_7_0]   = 0x21;
}

static void setting_CFDE_unit_8111Cmd(void)
{
    U32 dwPageLen = 16384;

    // CFDE sel cmd 81-11
    /*
    IOW F047 01
    IOW F040 00
    IOW F041 00
    IOW F042 03
    IOW F043 00
    IOW F044 40
    IOW F045 00
    */
    //Cmd81
    //PBAREG[CFDE_SEL] = (CFDE_NUM_MAX - 1);
    PBAREG[CFDE_SEL] = CFDE_IDX_CMD_8111;
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x03;
    PBAREG[CFDE_CMDQ_23_16] = 0x02; //CMD81
#if EN_HW_ADDR_MODE_TEST
    PBAREG[CFDE_CMDQ_15_8]  = 0x40 | HW_ADDR;
#else
    PBAREG[CFDE_CMDQ_15_8]  = 0x40;
#endif
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 04
    IOW F043 20
    IOW F044 00
    IOW F045 00
    */
    //adr0
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x04;
    PBAREG[CFDE_CMDQ_23_16] = 0x20;
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 06
    IOW F043 20
    IOW F044 00
    IOW F045 00
    */
    //adr1
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x06;
    PBAREG[CFDE_CMDQ_23_16] = 0x20;
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 08
    IOW F043 20
    IOW F044 00
    IOW F045 00
    */
    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x08;
    PBAREG[CFDE_CMDQ_23_16] = 0x20;
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 0a
    IOW F043 20
    IOW F044 00
    IOW F045 00
    */
    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x0a;
    PBAREG[CFDE_CMDQ_23_16] = 0x20;
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 0c
    IOW F043 20
    IOW F044 40
    IOW F045 00
    */
    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x0c;
    PBAREG[CFDE_CMDQ_23_16] = 0x20;
    PBAREG[CFDE_CMDQ_15_8]  = 0x40;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 1c
    IOW F043 80
    IOW F044 80
    IOW F045 00
    */
    //Data 16384+64
    /*
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x1c;
    PBAREG[CFDE_CMDQ_23_16] = 0x80;
    PBAREG[CFDE_CMDQ_15_8]  = 0x80;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;
    */
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_WDATA_DDR) | DATA_LEN_H(dwPageLen);
    PBAREG[CFDE_CMDQ_23_16] = DATA_LEN_M(dwPageLen);
    PBAREG[CFDE_CMDQ_15_8]  = DATA_LEN_L(dwPageLen);
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 04
    IOW F042 02
    IOW F043 20
    IOW F044 00
    IOW F045 01
    */
    //Cmd11
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_39_32] = 0x05; //bit32 Round_Robin cache wtag
#else
    PBAREG[CFDE_CMDQ_39_32] = 0x04; //BSY1
#endif
    PBAREG[CFDE_CMDQ_31_24] = 0x02;
    PBAREG[CFDE_CMDQ_23_16] = 0x22; //CMD11
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
#if EN_RR_MODE_TEST || EN_HW_ADDR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = 0x15; //bit4 Round_Robin cache ptr inc, bit2 for auto addess end
#else
    PBAREG[CFDE_CMDQ_7_0]   = 0x05; //bit2 for auto addess end
#endif

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 28
    IOW F043 00
    IOW F044 04
    IOW F045 21
    */
    //EOC
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x28;
    PBAREG[CFDE_CMDQ_23_16] = 0x00;
    PBAREG[CFDE_CMDQ_15_8]  = 0x04;
    PBAREG[CFDE_CMDQ_7_0]   = 0x21;
}

static void setting_CFDE_unit_8110Cmd(void)
{
    U32 dwPageLen = 16384;

    // CFDE sel cmd 81-10
    /*
    IOW F047 01
    IOW F040 00
    IOW F041 00
    IOW F042 03
    IOW F043 00
    IOW F044 40
    IOW F045 00
    */
    //Cmd81
    //PBAREG[CFDE_SEL] = (CFDE_NUM_MAX - 2);
    PBAREG[CFDE_SEL] = CFDE_IDX_CMD_8110;
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x03;
    PBAREG[CFDE_CMDQ_23_16] = 0x02; //CMD81
#if EN_HW_ADDR_MODE_TEST
    PBAREG[CFDE_CMDQ_15_8]  = 0x40 | HW_ADDR;
#else
    PBAREG[CFDE_CMDQ_15_8]  = 0x40;
#endif
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 04
    IOW F043 20
    IOW F044 00
    IOW F045 00
    */
    //adr0
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x04;
    PBAREG[CFDE_CMDQ_23_16] = 0x20;
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 06
    IOW F043 20
    IOW F044 00
    IOW F045 00
    */
    //adr1
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x06;
    PBAREG[CFDE_CMDQ_23_16] = 0x20;
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 08
    IOW F043 20
    IOW F044 00
    IOW F045 00
    */
    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x08;
    PBAREG[CFDE_CMDQ_23_16] = 0x20;
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 0a
    IOW F043 20
    IOW F044 00
    IOW F045 00
    */
    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x0a;
    PBAREG[CFDE_CMDQ_23_16] = 0x20;
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 0c
    IOW F043 20
    IOW F044 40
    IOW F045 00
    */
    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x0c;
    PBAREG[CFDE_CMDQ_23_16] = 0x20;
    PBAREG[CFDE_CMDQ_15_8]  = 0x40;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 1c
    IOW F043 80
    IOW F044 80
    IOW F045 00
    */
    //Data 16384+64
    /*
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x1c;
    PBAREG[CFDE_CMDQ_23_16] = 0x80;
    PBAREG[CFDE_CMDQ_15_8]  = 0x80;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;
    */
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_WDATA_DDR) | DATA_LEN_H(dwPageLen);
    PBAREG[CFDE_CMDQ_23_16] = DATA_LEN_M(dwPageLen);
    PBAREG[CFDE_CMDQ_15_8]  = DATA_LEN_L(dwPageLen);
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 04
    IOW F042 02
    IOW F043 20
    IOW F044 00
    IOW F045 01
    */
    //Cmd10
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_39_32] = 0x05; //bit32 Round_Robin cache wtag
#else
    PBAREG[CFDE_CMDQ_39_32] = 0x04; //BSY1
#endif
    PBAREG[CFDE_CMDQ_31_24] = 0x02;
    PBAREG[CFDE_CMDQ_23_16] = 0x20; //CMD10
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
#if EN_RR_MODE_TEST || EN_HW_ADDR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = 0x15; //bit4 Round_Robin cache ptr inc, bit2 for auto addess end
#else
    PBAREG[CFDE_CMDQ_7_0]   = 0x05; //bit2 for auto addess end
#endif

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 28
    IOW F043 00
    IOW F044 04
    IOW F045 21
    */
    //EOC
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x28;
    PBAREG[CFDE_CMDQ_23_16] = 0x00;
    PBAREG[CFDE_CMDQ_15_8]  = 0x04;
    PBAREG[CFDE_CMDQ_7_0]   = 0x21;
}

static void setting_CFDE_unit_0032Cmd(void)
{
    // CFDE sel cmd 00-32
    /*
    IOW F047 03
    IOW F040 00
    IOW F041 00
    IOW F042 02
    IOW F043 00
    IOW F044 08
    IOW F045 00
    */
    //Cmd00
    //PBAREG[CFDE_SEL] = (CFDE_NUM_MAX - 3);
    PBAREG[CFDE_SEL] = CFDE_IDX_CMD_0032;
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    //#if EN_RR_MODE_TEST
#if 0
    PBAREG[CFDE_CMDQ_39_32] = 0x02; //bit33 Round_Robin cache rtag
#else
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
#endif
    PBAREG[CFDE_CMDQ_31_24] = 0x02;
    PBAREG[CFDE_CMDQ_23_16] = 0x00; //CMD00
#if EN_HW_ADDR_MODE_TEST
    PBAREG[CFDE_CMDQ_15_8]  = 0x08 | HW_ADDR;
#else
    PBAREG[CFDE_CMDQ_15_8]  = 0x08;
#endif
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 04
    IOW F043 22
    IOW F044 00
    IOW F045 00
    */
    //adr0
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x04;
    PBAREG[CFDE_CMDQ_23_16] = 0x22;
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 06
    IOW F043 22
    IOW F044 00
    IOW F045 00
    */
    //adr1
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x06;
    PBAREG[CFDE_CMDQ_23_16] = 0x22;
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 08
    IOW F043 22
    IOW F044 00
    IOW F045 00
    */
    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x08;
    PBAREG[CFDE_CMDQ_23_16] = 0x22;
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 0a
    IOW F043 22
    IOW F044 00
    IOW F045 00
    */
    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x0a;
    PBAREG[CFDE_CMDQ_23_16] = 0x22;
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 0c
    IOW F043 22
    IOW F044 00
    IOW F045 00
    */
    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x0c;
    PBAREG[CFDE_CMDQ_23_16] = 0x22;
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    /*
    IOW F040 30
    IOW F041 08
    IOW F042 02
    IOW F043 60
    IOW F044 08
    IOW F045 02
    */
    //Cmd32
    //PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_47_40] = 0x30; //enable bit43(Afifo prewrite)/bit44(Afifo RST)
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_39_32] = 0x0a; //bit33 Round_Robin cache rtag
#else
    PBAREG[CFDE_CMDQ_39_32] = 0x08; //BSY2
#endif
    PBAREG[CFDE_CMDQ_31_24] = 0x02;
    PBAREG[CFDE_CMDQ_23_16] = 0x64; //CMD32
    PBAREG[CFDE_CMDQ_15_8]  = 0x08;
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = 0x16; //bit4 Round_Robin cache ptr inc
#else
    PBAREG[CFDE_CMDQ_7_0]   = 0x02;
#endif

    /*
    IOW F040 00
    IOW F041 00
    IOW F042 28
    IOW F043 00
    IOW F044 04
    IOW F045 00
    */
    //EOC
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x28;
    PBAREG[CFDE_CMDQ_23_16] = 0x00;
    PBAREG[CFDE_CMDQ_15_8]  = 0x04;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;
}

static void setting_CFDE_unit_71Cmd(void)
{
    //Cmd71
    PBAREG[CFDE_SEL] =  CFDE_IDX_CMD_71;
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x71) | OP_CODE(OP_CMD_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x71);
    PBAREG[CFDE_CMDQ_15_8]  = RD_DIR | OUT_SEL(STS_REG);
    PBAREG[CFDE_CMDQ_7_0]   = 0x00; //no interrupt for status read due to polling status

    //Status read
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_STS_READ);
    PBAREG[CFDE_CMDQ_23_16] = STS_BUF_NO_H(STS_BUFNO_71CMD);
    PBAREG[CFDE_CMDQ_15_8]  = STS_BUF_NO_L(STS_BUFNO_71CMD);
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //EOC
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_EOC);
    PBAREG[CFDE_CMDQ_23_16] = 0x00;
    PBAREG[CFDE_CMDQ_15_8]  = RST_LOCAL;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;
}

static void setting_CFDE_unit_73Cmd(void)
{
    //Cmd70
    PBAREG[CFDE_SEL] =  CFDE_IDX_CMD_73;
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x73) | OP_CODE(OP_CMD_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x73);
    PBAREG[CFDE_CMDQ_15_8]  = RD_DIR | OUT_SEL(STS_REG);
    PBAREG[CFDE_CMDQ_7_0]   = 0x00; //no interrupt for status read due to polling status

    //Status read
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_STS_READ);
    PBAREG[CFDE_CMDQ_23_16] = STS_BUF_NO_H(STS_BUFNO_73CMD);
    PBAREG[CFDE_CMDQ_15_8]  = STS_BUF_NO_L(STS_BUFNO_73CMD);
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //EOC
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_EOC);
    PBAREG[CFDE_CMDQ_23_16] = 0x00;
    PBAREG[CFDE_CMDQ_15_8]  = RST_LOCAL;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;
}

static void setting_CFDE_unit_F1Cmd(void)
{
    //CmdF1
    PBAREG[CFDE_SEL] = CFDE_IDX_CMD_F1;
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0xF1) | OP_CODE(OP_CMD_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0xF1);
    PBAREG[CFDE_CMDQ_15_8]  = RD_DIR | OUT_SEL(STS_REG);
    PBAREG[CFDE_CMDQ_7_0]   = 0x00; //no interrupt for status read due to polling status

    //Status read
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_STS_READ);
    PBAREG[CFDE_CMDQ_23_16] = STS_BUF_NO_H(STS_BUFNO_F1CMD);
    PBAREG[CFDE_CMDQ_15_8]  = STS_BUF_NO_L(STS_BUFNO_F1CMD);
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //EOC
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_EOC);
    PBAREG[CFDE_CMDQ_23_16] = 0x00;
    PBAREG[CFDE_CMDQ_15_8]  = RST_LOCAL;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;
}

static void setting_CFDE_unit_F2Cmd(void)
{
    //CmdF2
    PBAREG[CFDE_SEL] = CFDE_IDX_CMD_F2;
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0xF2) | OP_CODE(OP_CMD_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0xF2);
    PBAREG[CFDE_CMDQ_15_8]  = RD_DIR | OUT_SEL(STS_REG);
    PBAREG[CFDE_CMDQ_7_0]   = 0x00; //no interrupt for status read due to polling status

    //Status read
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_STS_READ);
    PBAREG[CFDE_CMDQ_23_16] = STS_BUF_NO_H(STS_BUFNO_F2CMD);
    PBAREG[CFDE_CMDQ_15_8]  = STS_BUF_NO_L(STS_BUFNO_F2CMD);
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //EOC
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_EOC);
    PBAREG[CFDE_CMDQ_23_16] = 0x00;
    PBAREG[CFDE_CMDQ_15_8]  = RST_LOCAL;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;
}

static void setting_CFDE_unit_78Cmd(void)
{
    //Cmd78
    PBAREG[CFDE_SEL] = CFDE_IDX_CMD_78;
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x78) | OP_CODE(OP_CMD_LATCH);;
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x78);;
    PBAREG[CFDE_CMDQ_15_8]  = RD_DIR | OUT_SEL(STS_REG_HW);
    PBAREG[CFDE_CMDQ_7_0]   = 0x00; //no interrupt for status read due to polling status

    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //Status read
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_STS_READ);
    PBAREG[CFDE_CMDQ_23_16] = STS_BUF_NO_H(STS_BUFNO_ENH_78CMD);
    PBAREG[CFDE_CMDQ_15_8]  = STS_BUF_NO_L(STS_BUFNO_ENH_78CMD);
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //EOC
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_EOC);
    PBAREG[CFDE_CMDQ_23_16] = 0x00;
    PBAREG[CFDE_CMDQ_15_8]  = RST_LOCAL;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;
}

static void setting_CFDE_unit_enhance71Cmd(void)
{
    //Cmd71
    PBAREG[CFDE_SEL] = CFDE_IDX_CMD_EN71;
    PBAREG[CFDE_CMDQ_47_40] = STA_ENH_EN;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x71) | OP_CODE(OP_CMD_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x71);
    PBAREG[CFDE_CMDQ_15_8]  = RD_DIR | OUT_SEL(STS_REG_HW);
    PBAREG[CFDE_CMDQ_7_0]   = 0x00; //no interrupt for status read due to polling status

    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //Status read
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_STS_READ);
    PBAREG[CFDE_CMDQ_23_16] = STS_BUF_NO_H(STS_BUFNO_ENH_71CMD);
    PBAREG[CFDE_CMDQ_15_8]  = STS_BUF_NO_L(STS_BUFNO_ENH_71CMD);
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //EOC
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_EOC);
    PBAREG[CFDE_CMDQ_23_16] = 0x00;
    PBAREG[CFDE_CMDQ_15_8]  = RST_LOCAL;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;
}

#if !(EN_INFO_BIT_TEST || EN_BUSY_WINDOW_TEST)
static void setting_CFDE_unit_60D0Cmd(void)
{
    //Cmd60
    PBAREG[CFDE_SEL] =  CFDE_IDX_CMD_60D0;
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x60) | OP_CODE(OP_CMD_LATCH);;
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x60);;
    PBAREG[CFDE_CMDQ_15_8]  = WR_DIR | OUT_SEL(TRANS_DATA);
    PBAREG[CFDE_CMDQ_7_0]   = 0x00; //no interrupt for status read due to polling status

    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //CmdD0
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_39_32] = CACHE_WRITE_TAG | CFDE_SET_BSY1; //bit32 Round_Robin cache wtag
#else
    PBAREG[CFDE_CMDQ_39_32] = CFDE_SET_BSY1;
#endif

    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0xD0) | OP_CODE(OP_CMD_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0xD0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = ERASE_INT_ASSERT | CACHE_PTR_INC | LATCH_ADDRESS_END;
#else
    PBAREG[CFDE_CMDQ_7_0]   = ERASE_INT_ASSERT | LATCH_ADDRESS_END;
#endif

    //EOC
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_EOC);
    PBAREG[CFDE_CMDQ_23_16] = 0x00;
    PBAREG[CFDE_CMDQ_15_8]  = RST_ALL;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;
}

static void setting_CFDE_unit_6060D0Cmd(void)
{
    //Cmd60
    PBAREG[CFDE_SEL] =  CFDE_IDX_CMD_6060D0;
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x60) | OP_CODE(OP_CMD_LATCH);;
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x60);;
    PBAREG[CFDE_CMDQ_15_8]  = WR_DIR | OUT_SEL(TRANS_DATA);
    PBAREG[CFDE_CMDQ_7_0]   = 0x00; //no interrupt for status read due to polling status

    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //Cmd60
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x60) | OP_CODE(OP_CMD_LATCH);;
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x60);;
    PBAREG[CFDE_CMDQ_15_8]  = WR_DIR | OUT_SEL(TRANS_DATA);
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = CACHE_PTR_INC | LATCH_ADDRESS_END;
#else
    PBAREG[CFDE_CMDQ_7_0]   = LATCH_ADDRESS_END;
#endif

    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //CmdD0
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_39_32] = CACHE_WRITE_TAG | CFDE_SET_BSY1; //bit32 Round_Robin cache wtag
#else
    PBAREG[CFDE_CMDQ_39_32] = CFDE_SET_BSY1;
#endif

    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0xD0) | OP_CODE(OP_CMD_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0xD0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = ERASE_INT_ASSERT | CACHE_PTR_INC | LATCH_ADDRESS_END;
#else
    PBAREG[CFDE_CMDQ_7_0]   = ERASE_INT_ASSERT | LATCH_ADDRESS_END;
#endif

    //EOC
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_EOC);
    PBAREG[CFDE_CMDQ_23_16] = 0x00;
    PBAREG[CFDE_CMDQ_15_8]  = RST_ALL;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;
}

static void setting_CFDE_unit_FourPlaneEraseCmd(void)
{
    //plane0
    //Cmd60
    PBAREG[CFDE_SEL] =  CFDE_IDX_CMD_FOUR_PLANE_ERASE;
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x60) | OP_CODE(OP_CMD_LATCH);;
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x60);;
    PBAREG[CFDE_CMDQ_15_8]  = WR_DIR | OUT_SEL(TRANS_DATA);
    PBAREG[CFDE_CMDQ_7_0]   = 0x00; //no interrupt for status read due to polling status

    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //plane1
    //Cmd60
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x60) | OP_CODE(OP_CMD_LATCH);;
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x60);;
    PBAREG[CFDE_CMDQ_15_8]  = WR_DIR | OUT_SEL(TRANS_DATA);
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = CACHE_PTR_INC | LATCH_ADDRESS_END;
#else
    PBAREG[CFDE_CMDQ_7_0]   = LATCH_ADDRESS_END;
#endif

    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //plane2
    //Cmd60
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x60) | OP_CODE(OP_CMD_LATCH);;
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x60);;
    PBAREG[CFDE_CMDQ_15_8]  = WR_DIR | OUT_SEL(TRANS_DATA);
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = CACHE_PTR_INC | LATCH_ADDRESS_END;
#else
    PBAREG[CFDE_CMDQ_7_0]   = LATCH_ADDRESS_END;
#endif

    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //plane3
    //Cmd60
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x60) | OP_CODE(OP_CMD_LATCH);;
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x60);;
    PBAREG[CFDE_CMDQ_15_8]  = WR_DIR | OUT_SEL(TRANS_DATA);
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = CACHE_PTR_INC | LATCH_ADDRESS_END;
#else
    PBAREG[CFDE_CMDQ_7_0]   = LATCH_ADDRESS_END;
#endif

    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //CmdD0
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_39_32] = CACHE_WRITE_TAG | CFDE_SET_BSY1; //bit32 Round_Robin cache wtag
#else
    PBAREG[CFDE_CMDQ_39_32] = CFDE_SET_BSY1;
#endif

    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0xD0) | OP_CODE(OP_CMD_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0xD0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = ERASE_INT_ASSERT | CACHE_PTR_INC | LATCH_ADDRESS_END;
#else
    PBAREG[CFDE_CMDQ_7_0]   = ERASE_INT_ASSERT | LATCH_ADDRESS_END;
#endif

    //EOC
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_EOC);
    PBAREG[CFDE_CMDQ_23_16] = 0x00;
    PBAREG[CFDE_CMDQ_15_8]  = RST_ALL;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;
}

static void setting_CFDE_unit_EightPlaneEraseCmd(void)
{
    //plane0
    //Cmd60
    PBAREG[CFDE_SEL] =  CFDE_IDX_CMD_EIGHT_PLANE_ERASE;
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x60) | OP_CODE(OP_CMD_LATCH);;
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x60);;
    PBAREG[CFDE_CMDQ_15_8]  = WR_DIR | OUT_SEL(TRANS_DATA);
    PBAREG[CFDE_CMDQ_7_0]   = 0x00; //no interrupt for status read due to polling status

    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //plane1
    //Cmd60
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x60) | OP_CODE(OP_CMD_LATCH);;
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x60);;
    PBAREG[CFDE_CMDQ_15_8]  = WR_DIR | OUT_SEL(TRANS_DATA);
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = CACHE_PTR_INC | LATCH_ADDRESS_END;
#else
    PBAREG[CFDE_CMDQ_7_0]   = LATCH_ADDRESS_END;
#endif

    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //plane2
    //Cmd60
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x60) | OP_CODE(OP_CMD_LATCH);;
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x60);;
    PBAREG[CFDE_CMDQ_15_8]  = WR_DIR | OUT_SEL(TRANS_DATA);
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = CACHE_PTR_INC | LATCH_ADDRESS_END;
#else
    PBAREG[CFDE_CMDQ_7_0]   = LATCH_ADDRESS_END;
#endif

    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //plane3
    //Cmd60
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x60) | OP_CODE(OP_CMD_LATCH);;
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x60);;
    PBAREG[CFDE_CMDQ_15_8]  = WR_DIR | OUT_SEL(TRANS_DATA);
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = CACHE_PTR_INC | LATCH_ADDRESS_END;
#else
    PBAREG[CFDE_CMDQ_7_0]   = LATCH_ADDRESS_END;
#endif

    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //plane4
    //Cmd60
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x60) | OP_CODE(OP_CMD_LATCH);;
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x60);;
    PBAREG[CFDE_CMDQ_15_8]  = WR_DIR | OUT_SEL(TRANS_DATA);
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = CACHE_PTR_INC | LATCH_ADDRESS_END;
#else
    PBAREG[CFDE_CMDQ_7_0]   = LATCH_ADDRESS_END;
#endif

    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //plane5
    //Cmd60
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x60) | OP_CODE(OP_CMD_LATCH);;
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x60);;
    PBAREG[CFDE_CMDQ_15_8]  = WR_DIR | OUT_SEL(TRANS_DATA);
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = CACHE_PTR_INC | LATCH_ADDRESS_END;
#else
    PBAREG[CFDE_CMDQ_7_0]   = LATCH_ADDRESS_END;
#endif

    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //plane6
    //Cmd60
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x60) | OP_CODE(OP_CMD_LATCH);;
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x60);;
    PBAREG[CFDE_CMDQ_15_8]  = WR_DIR | OUT_SEL(TRANS_DATA);
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = CACHE_PTR_INC | LATCH_ADDRESS_END;
#else
    PBAREG[CFDE_CMDQ_7_0]   = LATCH_ADDRESS_END;
#endif

    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //plane7
    //Cmd60
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x60) | OP_CODE(OP_CMD_LATCH);;
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x60);;
    PBAREG[CFDE_CMDQ_15_8]  = WR_DIR | OUT_SEL(TRANS_DATA);
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = CACHE_PTR_INC | LATCH_ADDRESS_END;
#else
    PBAREG[CFDE_CMDQ_7_0]   = LATCH_ADDRESS_END;
#endif

    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //CmdD0
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_39_32] = CACHE_WRITE_TAG | CFDE_SET_BSY1; //bit32 Round_Robin cache wtag
#else
    PBAREG[CFDE_CMDQ_39_32] = CFDE_SET_BSY1;
#endif

    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0xD0) | OP_CODE(OP_CMD_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0xD0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = ERASE_INT_ASSERT | CACHE_PTR_INC | LATCH_ADDRESS_END;
#else
    PBAREG[CFDE_CMDQ_7_0]   = ERASE_INT_ASSERT | LATCH_ADDRESS_END;
#endif

    //EOC
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_EOC);
    PBAREG[CFDE_CMDQ_23_16] = 0x00;
    PBAREG[CFDE_CMDQ_15_8]  = RST_ALL;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;
}

static void setting_CFDE_unit_606030Cmd(void)
{
    //Cmd60
    PBAREG[CFDE_SEL] =  CFDE_IDX_CMD_606030;
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x60) | OP_CODE(OP_CMD_LATCH);;
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x60);;
    PBAREG[CFDE_CMDQ_15_8]  = RD_DIR | OUT_SEL(TRANS_DATA);
    PBAREG[CFDE_CMDQ_7_0]   = 0x00; //no interrupt for status read due to polling status

    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //Cmd60
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x60) | OP_CODE(OP_CMD_LATCH);;
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x60);;
    PBAREG[CFDE_CMDQ_15_8]  = WR_DIR | OUT_SEL(TRANS_DATA);
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = CACHE_PTR_INC | LATCH_ADDRESS_END;
#else
    PBAREG[CFDE_CMDQ_7_0]   = LATCH_ADDRESS_END;
#endif

    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //Cmd30
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_39_32] = CACHE_WRITE_TAG | CFDE_SET_BSY1; //bit32 Round_Robin cache wtag
#else
    PBAREG[CFDE_CMDQ_39_32] = CFDE_SET_BSY1;
#endif

    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x30) | OP_CODE(OP_CMD_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x30);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = READ_INT_ASSERT | CACHE_PTR_INC | LATCH_ADDRESS_END;
#else
    PBAREG[CFDE_CMDQ_7_0]   = READ_INT_ASSERT | LATCH_ADDRESS_END;
#endif

    //EOC
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_EOC);
    PBAREG[CFDE_CMDQ_23_16] = 0x00;
    PBAREG[CFDE_CMDQ_15_8]  = RST_ALL;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;
}

static void setting_CFDE_unit_FourPlaneReadCmd(void)
{
    //plane0
    //Cmd60
    PBAREG[CFDE_SEL] =  CFDE_IDX_CMD_FOUR_PLANE_READ;
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x60) | OP_CODE(OP_CMD_LATCH);;
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x60);;
    PBAREG[CFDE_CMDQ_15_8]  = RD_DIR | OUT_SEL(TRANS_DATA);
    PBAREG[CFDE_CMDQ_7_0]   = 0x00; //no interrupt for status read due to polling status

    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //plane1
    //Cmd60
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x60) | OP_CODE(OP_CMD_LATCH);;
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x60);;
    PBAREG[CFDE_CMDQ_15_8]  = WR_DIR | OUT_SEL(TRANS_DATA);
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = CACHE_PTR_INC | LATCH_ADDRESS_END;
#else
    PBAREG[CFDE_CMDQ_7_0]   = LATCH_ADDRESS_END;
#endif

    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //plane2
    //Cmd60
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x60) | OP_CODE(OP_CMD_LATCH);;
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x60);;
    PBAREG[CFDE_CMDQ_15_8]  = WR_DIR | OUT_SEL(TRANS_DATA);
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = CACHE_PTR_INC | LATCH_ADDRESS_END;
#else
    PBAREG[CFDE_CMDQ_7_0]   = LATCH_ADDRESS_END;
#endif

    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //plane3
    //Cmd60
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x60) | OP_CODE(OP_CMD_LATCH);;
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x60);;
    PBAREG[CFDE_CMDQ_15_8]  = WR_DIR | OUT_SEL(TRANS_DATA);
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = CACHE_PTR_INC | LATCH_ADDRESS_END;
#else
    PBAREG[CFDE_CMDQ_7_0]   = LATCH_ADDRESS_END;
#endif

    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //Cmd30
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_39_32] = CACHE_WRITE_TAG | CFDE_SET_BSY1; //bit32 Round_Robin cache wtag
#else
    PBAREG[CFDE_CMDQ_39_32] = CFDE_SET_BSY1;
#endif

    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x30) | OP_CODE(OP_CMD_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x30);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = READ_INT_ASSERT | CACHE_PTR_INC | LATCH_ADDRESS_END;
#else
    PBAREG[CFDE_CMDQ_7_0]   = READ_INT_ASSERT | LATCH_ADDRESS_END;
#endif

    //EOC
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_EOC);
    PBAREG[CFDE_CMDQ_23_16] = 0x00;
    PBAREG[CFDE_CMDQ_15_8]  = RST_ALL;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;
}

static void setting_CFDE_unit_EightlaneReadCmd(void)
{
    //plane0
    //Cmd60
    PBAREG[CFDE_SEL] =  CFDE_IDX_CMD_EIGHT_PLANE_READ;
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x60) | OP_CODE(OP_CMD_LATCH);;
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x60);;
    PBAREG[CFDE_CMDQ_15_8]  = RD_DIR | OUT_SEL(TRANS_DATA);
    PBAREG[CFDE_CMDQ_7_0]   = 0x00; //no interrupt for status read due to polling status

    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //plane1
    //Cmd60
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x60) | OP_CODE(OP_CMD_LATCH);;
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x60);;
    PBAREG[CFDE_CMDQ_15_8]  = WR_DIR | OUT_SEL(TRANS_DATA);
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = CACHE_PTR_INC | LATCH_ADDRESS_END;
#else
    PBAREG[CFDE_CMDQ_7_0]   = LATCH_ADDRESS_END;
#endif

    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //plane2
    //Cmd60
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x60) | OP_CODE(OP_CMD_LATCH);;
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x60);;
    PBAREG[CFDE_CMDQ_15_8]  = WR_DIR | OUT_SEL(TRANS_DATA);
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = CACHE_PTR_INC | LATCH_ADDRESS_END;
#else
    PBAREG[CFDE_CMDQ_7_0]   = LATCH_ADDRESS_END;
#endif

    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //plane3
    //Cmd60
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x60) | OP_CODE(OP_CMD_LATCH);;
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x60);;
    PBAREG[CFDE_CMDQ_15_8]  = WR_DIR | OUT_SEL(TRANS_DATA);
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = CACHE_PTR_INC | LATCH_ADDRESS_END;
#else
    PBAREG[CFDE_CMDQ_7_0]   = LATCH_ADDRESS_END;
#endif

    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //plane4
    //Cmd60
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x60) | OP_CODE(OP_CMD_LATCH);;
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x60);;
    PBAREG[CFDE_CMDQ_15_8]  = WR_DIR | OUT_SEL(TRANS_DATA);
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = CACHE_PTR_INC | LATCH_ADDRESS_END;
#else
    PBAREG[CFDE_CMDQ_7_0]   = LATCH_ADDRESS_END;
#endif

    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //plane5
    //Cmd60
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x60) | OP_CODE(OP_CMD_LATCH);;
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x60);;
    PBAREG[CFDE_CMDQ_15_8]  = WR_DIR | OUT_SEL(TRANS_DATA);
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = CACHE_PTR_INC | LATCH_ADDRESS_END;
#else
    PBAREG[CFDE_CMDQ_7_0]   = LATCH_ADDRESS_END;
#endif

    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //plane6
    //Cmd60
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x60) | OP_CODE(OP_CMD_LATCH);;
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x60);;
    PBAREG[CFDE_CMDQ_15_8]  = WR_DIR | OUT_SEL(TRANS_DATA);
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = CACHE_PTR_INC | LATCH_ADDRESS_END;
#else
    PBAREG[CFDE_CMDQ_7_0]   = LATCH_ADDRESS_END;
#endif

    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //plane7
    //Cmd60
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x60) | OP_CODE(OP_CMD_LATCH);;
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x60);;
    PBAREG[CFDE_CMDQ_15_8]  = WR_DIR | OUT_SEL(TRANS_DATA);
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = CACHE_PTR_INC | LATCH_ADDRESS_END;
#else
    PBAREG[CFDE_CMDQ_7_0]   = LATCH_ADDRESS_END;
#endif

    //adr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //Cmd30
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_39_32] = CACHE_WRITE_TAG | CFDE_SET_BSY1; //bit32 Round_Robin cache wtag
#else
    PBAREG[CFDE_CMDQ_39_32] = CFDE_SET_BSY1;
#endif

    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x30) | OP_CODE(OP_CMD_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x30);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = READ_INT_ASSERT | CACHE_PTR_INC | LATCH_ADDRESS_END;
#else
    PBAREG[CFDE_CMDQ_7_0]   = READ_INT_ASSERT | LATCH_ADDRESS_END;
#endif

    //EOC
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_EOC);
    PBAREG[CFDE_CMDQ_23_16] = 0x00;
    PBAREG[CFDE_CMDQ_15_8]  = RST_ALL;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;
}


static void setting_CFDE_unit_00_cmd(void)
{
    PBAREG[CFDE_SEL] = CFDE_IDX_CMD_00;

    //Cmd 00
    PBAREG[CFDE_CMDQ_47_40] = AFIFO_PREWRITE | AFIFO_RST;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x00) | OP_CODE(OP_CMD_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x00);
    PBAREG[CFDE_CMDQ_15_8]  = RD_DIR;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //EOC
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_EOC) | MASK_LATCH_ADDR_EN;
    PBAREG[CFDE_CMDQ_23_16] = 0x00;
    PBAREG[CFDE_CMDQ_15_8]  = RST_LOCAL;
    PBAREG[CFDE_CMDQ_7_0]   = HW_RST;
}


static void setting_CFDE_unit_05E0_cmd(void)
{

    PBAREG[CFDE_SEL] = CFDE_IDX_CMD_05E0;

    //Cmd 05
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x05) | OP_CODE(OP_CMD_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x05);
    PBAREG[CFDE_CMDQ_15_8]  = RD_DIR;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //Addr0
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR0_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //Addr1
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR1_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //Cmd E0
    PBAREG[CFDE_CMDQ_47_40] = AFIFO_PREWRITE | AFIFO_RST;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0xE0) | OP_CODE(OP_CMD_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0xE0);
    PBAREG[CFDE_CMDQ_15_8]  = RD_CHG_COL | RD_DIR;
    PBAREG[CFDE_CMDQ_7_0]   = BYPASS_HWADDR;

    //EOC
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_EOC);
    PBAREG[CFDE_CMDQ_23_16] = 0x00;
    PBAREG[CFDE_CMDQ_15_8]  = RST_ALL;
    PBAREG[CFDE_CMDQ_7_0]   = HW_RST;
}

static void setting_CFDE_unit_0005E0_cmd(void)
{

    PBAREG[CFDE_SEL] = CFDE_IDX_CMD_0005E0;

    //Cmd 00
    PBAREG[CFDE_CMDQ_47_40] = AFIFO_PREWRITE | AFIFO_RST;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x00) | OP_CODE(OP_CMD_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x00);
    PBAREG[CFDE_CMDQ_15_8]  = RD_DIR;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //Addr0
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR0_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //Addr1
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR1_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;


    //Addr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //Addr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;


    //Addr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;


    //Cmd 05
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x05) | OP_CODE(OP_CMD_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x05);
    PBAREG[CFDE_CMDQ_15_8]  = RD_DIR;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //Addr0
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR0_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //Addr1
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR1_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //Cmd E0
    PBAREG[CFDE_CMDQ_47_40] = AFIFO_PREWRITE | AFIFO_RST;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0xE0) | OP_CODE(OP_CMD_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0xE0);
    PBAREG[CFDE_CMDQ_15_8]  = HW_ADDR | RD_CHG_COL | RD_DIR;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //EOC
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_EOC);
    PBAREG[CFDE_CMDQ_23_16] = 0x00;
    PBAREG[CFDE_CMDQ_15_8]  = RST_ALL;
    PBAREG[CFDE_CMDQ_7_0]   = HW_RST;
}

static void setting_CFDE_unit_ChangeWriteColCmd(void)
{

    PBAREG[CFDE_SEL] = CFDE_IDX_CMD_CHANGE_WRITE_COLUMN;

    //Cmd 85
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x85) | OP_CODE(OP_CMD_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x85);
    PBAREG[CFDE_CMDQ_15_8]  = WR_DIR | OUT_SEL(TRANS_DATA);
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //Addr0
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR0_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //Addr1
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR1_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = WR_CHG_COL;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //EOC
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_EOC);
    PBAREG[CFDE_CMDQ_23_16] = 0x00;
    PBAREG[CFDE_CMDQ_15_8]  = RST_LOCAL;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

}

static void setting_CFDE_unit_ChangeWriteRowCmd(void)
{

    PBAREG[CFDE_SEL] = CFDE_IDX_CMD_CHANGE_WRITE_ROW;

    //Cmd 85
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(0x85) | OP_CODE(OP_CMD_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(0x85);
    PBAREG[CFDE_CMDQ_15_8]  = WR_DIR | OUT_SEL(TRANS_DATA);
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //Addr0
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR0_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //Addr1
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR1_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //Addr2
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //Addr3
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //Addr4
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = WR_CHG_COL;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //EOC
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_EOC);
    PBAREG[CFDE_CMDQ_23_16] = 0x00;
    PBAREG[CFDE_CMDQ_15_8]  = RST_LOCAL;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

}
#endif

#if EN_STATUS_BUF_BSY_TEST || EN_BUSY_WINDOW_TEST
static void setting_CFDE_unit_ProgCmd(U8 byCmdVal_0, U8 byCmdVal_1, U8 byCFDEIdx, U8 bySubBSYType)
{
    // CFDE sel cmd Cmd0-Cmd1

    //U32 dwPageLen = 16384 + 64;
    U32 dwPageLen = 16384;

    //Cmd0
    PBAREG[CFDE_SEL] = byCFDEIdx;
    /*
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x03;
    PBAREG[CFDE_CMDQ_23_16] = 0x00; //CMD80
    PBAREG[CFDE_CMDQ_15_8]  = 0x40;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;
    */
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(byCmdVal_0) | OP_CODE(OP_CMD_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(byCmdVal_0);
#if EN_HW_ADDR_MODE_TEST
    PBAREG[CFDE_CMDQ_15_8]  = WR_DIR | OUT_SEL(TRANS_DATA) | HW_ADDR;
#else
    PBAREG[CFDE_CMDQ_15_8]  = WR_DIR | OUT_SEL(TRANS_DATA);
#endif
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr0
    /*
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x04;
    PBAREG[CFDE_CMDQ_23_16] = 0x20;
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;
    */
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR0_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr1
    /*
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x06;
    PBAREG[CFDE_CMDQ_23_16] = 0x20;
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;
    */
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR1_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr2
    /*
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x08;
    PBAREG[CFDE_CMDQ_23_16] = 0x20;
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;
    */
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR2_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr3
    /*
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x0a;
    PBAREG[CFDE_CMDQ_23_16] = 0x20;
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;
    */
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR3_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //adr4
    /*
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x0c;
    PBAREG[CFDE_CMDQ_23_16] = 0x20;
    PBAREG[CFDE_CMDQ_15_8]  = 0x40;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;
    */
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_ADR4_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = LATCH_ADDR_EN | ADR_BUF_NO(ADR_BUFNO_0);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //Data 16384+64
    /*
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x1c;
    PBAREG[CFDE_CMDQ_23_16] = 0x80;
    PBAREG[CFDE_CMDQ_15_8]  = 0x80;
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;
    */
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_WDATA_DDR) | DATA_LEN_H(dwPageLen);
    PBAREG[CFDE_CMDQ_23_16] = DATA_LEN_M(dwPageLen);
    PBAREG[CFDE_CMDQ_15_8]  = DATA_LEN_L(dwPageLen);
    PBAREG[CFDE_CMDQ_7_0]   = 0x00;

    //Cmd1
    /*
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_39_32] = 0x05; //bit32 Round_Robin cache wtag
#else
    PBAREG[CFDE_CMDQ_39_32] = 0x04; //BSY1
#endif
    PBAREG[CFDE_CMDQ_31_24] = 0x02;
    PBAREG[CFDE_CMDQ_23_16] = 0x20; //CMD10
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
#if EN_RR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = 0x11; //bit4 Round_Robin cache ptr inc
#else
    PBAREG[CFDE_CMDQ_7_0]   = 0x01;
#endif
    */
    if (bySubBSYType >= TYPE_SUB_BSY7) {
        PBAREG[CFDE_CMDQ_47_40] = 1 << (bySubBSYType - 7);
#if EN_RR_MODE_TEST
        PBAREG[CFDE_CMDQ_39_32] = CACHE_WRITE_TAG; //bit32 Round_Robin cache wtag
#else
        PBAREG[CFDE_CMDQ_39_32] = 0x00;
#endif
    } else {
        PBAREG[CFDE_CMDQ_47_40] = 0x00;
#if EN_RR_MODE_TEST
        PBAREG[CFDE_CMDQ_39_32] = (1 << (bySubBSYType + 1)) | CACHE_WRITE_TAG; //bit32 Round_Robin cache wtag
#else
        PBAREG[CFDE_CMDQ_39_32] = 1 << (bySubBSYType + 1);
#endif
    }

    PBAREG[CFDE_CMDQ_31_24] = CMD_SEQ_WORD_H(byCmdVal_1) | OP_CODE(OP_CMD_LATCH);
    PBAREG[CFDE_CMDQ_23_16] = CMD_SEQ_WORD_L(byCmdVal_1);
    PBAREG[CFDE_CMDQ_15_8]  = 0x00;
#if EN_RR_MODE_TEST || EN_HW_ADDR_MODE_TEST
    PBAREG[CFDE_CMDQ_7_0]   = PRG_INT_ASSERT | CACHE_PTR_INC | LATCH_ADDRESS_END;
#else
    PBAREG[CFDE_CMDQ_7_0]   = PRG_INT_ASSERT;
#endif

    //EOC
    /*
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = 0x28;
    PBAREG[CFDE_CMDQ_23_16] = 0x00;
    PBAREG[CFDE_CMDQ_15_8]  = 0x04;
    PBAREG[CFDE_CMDQ_7_0]   = 0x21;
    */
    PBAREG[CFDE_CMDQ_47_40] = 0x00;
    PBAREG[CFDE_CMDQ_39_32] = 0x00;
    PBAREG[CFDE_CMDQ_31_24] = OP_CODE(OP_EOC);
    PBAREG[CFDE_CMDQ_23_16] = 0x00;
    PBAREG[CFDE_CMDQ_15_8]  = RST_ALL;
    PBAREG[CFDE_CMDQ_7_0]   = HW_RST;
}
#endif

static void en_CFDE_unit(U8 byEnCFDECnt)
{
    U8 byCFDEIdx;
    U8 byCnt;
    U8 byRemain;
    U8 i;

    byCnt    = byEnCFDECnt >> 3;
    byRemain = byEnCFDECnt % 8;

    for (byCFDEIdx = 0; byCFDEIdx < byCnt; byCFDEIdx++) {
        PBAREG[CFDE_EN_7_0 + byCnt] = 0xFF;
    }

    if (byRemain != 0) {
        for (i = 0; i < byRemain; i++) {
            PBAREG[CFDE_EN_7_0 + byCnt * 8] =  1 << byRemain ;
        }
    }
}

static void setting_all_CFDE_unit(void)
{
    U8 i;

    setting_CFDE_unit_FFCmd();

    setting_CFDE_unit_8010Cmd();

    setting_CFDE_unit_70Cmd();

    setting_CFDE_unit_0030Cmd();


    setting_CFDE_unit_31Cmd();

    setting_CFDE_unit_3FCmd();


    setting_CFDE_unit_8011Cmd();

    setting_CFDE_unit_8111Cmd();

    setting_CFDE_unit_8110Cmd();

    setting_CFDE_unit_0032Cmd();


    setting_CFDE_unit_F1Cmd();

    setting_CFDE_unit_F2Cmd();

    setting_CFDE_unit_78Cmd();

    setting_CFDE_unit_enhance71Cmd();

#if EN_STATUS_BUF_BSY_TEST
    setting_CFDE_unit_71Cmd();

    setting_CFDE_unit_73Cmd();
#endif

#if EN_STATUS_BUF_BSY_TEST || EN_BUSY_WINDOW_TEST
    setting_CFDE_unit_ProgCmd(0x80, 0x15, CFDE_IDX_CMD_8015, TYPE_SUB_BSY1);
    setting_CFDE_unit_ProgCmd(0x80, 0x1A, CFDE_IDX_CMD_801A, TYPE_SUB_BSY2);
    setting_CFDE_unit_ProgCmd(0x81, 0x15, CFDE_IDX_CMD_8115, TYPE_SUB_BSY3);
    setting_CFDE_unit_ProgCmd(0x81, 0x1A, CFDE_IDX_CMD_811A, TYPE_SUB_BSY4);
    setting_CFDE_unit_ProgCmd(0x85, 0x10, CFDE_IDX_CMD_8510, TYPE_SUB_BSY5);
    setting_CFDE_unit_ProgCmd(0x85, 0x11, CFDE_IDX_CMD_8511, TYPE_SUB_BSY6);
    setting_CFDE_unit_ProgCmd(0x85, 0x15, CFDE_IDX_CMD_8515, TYPE_SUB_BSY7);
    setting_CFDE_unit_ProgCmd(0x85, 0x1A, CFDE_IDX_CMD_851A, TYPE_SUB_BSY8);
#endif

#if !(EN_INFO_BIT_TEST || EN_BUSY_WINDOW_TEST || EN_STATUS_BUF_BSY_TEST)
    setting_CFDE_unit_60D0Cmd();
    setting_CFDE_unit_6060D0Cmd();
    setting_CFDE_unit_606030Cmd();

    setting_CFDE_unit_FourPlaneEraseCmd();
    setting_CFDE_unit_EightPlaneEraseCmd();
    setting_CFDE_unit_FourPlaneReadCmd();
    setting_CFDE_unit_EightlaneReadCmd();

    setting_CFDE_unit_05E0_cmd();
    setting_CFDE_unit_0005E0_cmd();
    setting_CFDE_unit_ChangeWriteColCmd();
    setting_CFDE_unit_ChangeWriteRowCmd();
#endif

    /* Enable all CFDE unit */
    /*
    for (i = 0; i < 0x0D; i++) {
        PBAREG[CFDE_EN_7_0 + i] = 0xFF;
    }
    */
    PBAREG[CFDE_EN_7_0]  = 0xFF;
    PBAREG[CFDE_EN_15_8] = 0x03;
    //PBAREG[CFDE_EN_7_0]  = 0x0F;

    //IOW F0A1 64 // bus_ana_thre_l
    //IOW F0A0 1d // bus_ana_err_int_en
    //PBAREG[BUS_ANA_THRE_L] = 0x64;
    //PBAREG[BUS_ANA_EN]     = 0x1d;
    PBAREG[BUS_ANA_THRE_L] = 0x64;
    PBAREG[BUS_ANA_EN]     = 0x1C;

    //IOW F006 03 // WR_CACHE_MODE
    //IOW F00A 03 // RD_CACHE_MODE
    //PBAREG[WR_CACHE] = 0x03;
    //PBAREG[WR_CACHE] = 0x03;
    //SYSREG[SYS_PBA_CASEL_CFG0] = 0x0F;
    //PBAREG[WR_CACHE] = 0x0E;
    //PBAREG[RD_CACHE] = 0x0E;
    //PBAREG[WR_CACHE] = 0x0F;
    //PBAREG[RD_CACHE] = 0x0F;

    //SYSREG[SYS_PBA_CASEL_CFG0] = 0x20;
    //PBAREG[WR_CACHE] = 0x3F;
    //PBAREG[RD_CACHE] = 0x3F;
    //PBAREG[WR_CACHE] = 0x00;
    //PBAREG[RD_CACHE] = 0x00;

    //PBAREG[WR_CACHE] = 0x00;
    //PBAREG[RD_CACHE] = 0x00;

    //clear int and set ready for all CE
    pba_clear_allceint_and_setallrdy();
}

#if !EN_STATUS_BUF_BSY_TEST
static void process_cmd_for_ce(U8 byCeIdx)
{
#if 0
    volatile U8* pbyPBACEReg;
    U8 i;

    pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;

#if 0
    /**
      WAIT_RDCHK E870 01 01           // wait FF
    */
    //while (!(PBAREG_CE0[CFDE_INTV_7_0] & BIT0));
    while (pbyPBACEReg[CFDE_INTV_7_0] != (1 << CFDE_IDX_CMD_FF));

    clear_int_and_setrdy(byCeIdx);
#endif

    //for (i = 0; i < 0x3F; i++) {
    for (i = 0; i < 0x01; i++) {
        /**
          WAIT_RDCHK E870 02 02           // wait 80-10
        */
        //while (!(PBAREG[CFDE_INTV_7_0] & BIT1));
        while (pbyPBACEReg[CFDE_INTV_7_0] != (1 << CFDE_IDX_CMD_8010));

        //check_addr_buffer();

        pba_clear_int_and_setrdy(byCeIdx);

        dummy_fun();
    }

#if 0
    /**
      WAIT_RDCHK F070 08 08           // wait 00-30
    */
    //while (!(PBAREG[CFDE_INTV_7_0] & BIT3));
    while (pbyPBACEReg[CFDE_INTV_7_0] != (1 << CFDE_IDX_CMD_0030));

    //check_addr_buffer();

    pba_clear_int_and_setrdy(byCeIdx);

    dummy_fun();
#endif
#endif
}

static void process_read_write_erase_Cmd(void)
{
    U8 i;

    for (i = 0; i < MAX_CE_CNT; i++) {
        process_cmd_for_ce(i);
        //process_cmd_for_ce(0x01);
    }
}
#endif

static void check_addr_buffer(void)
{
#if 0
    xdata U8 abyRcvAddBuf[10];
    U8 byIdx;

    PBAREG[CTL_ADR_BFU_SRAM_ADDR_SEL] = 0x00;

    PBAREG[CTL_ADR_BUF_SRAM_ADDR_9_8] = 0x00;
    PBAREG[CTL_ADR_BUF_SRAM_ADDR_7_0] = 0x00;

    for (byIdx = 0; byIdx < 10; byIdx++) {
        abyRcvAddBuf[byIdx] = PBAREG[ADR0_BUF0 - byIdx];
        SET_RDY_Base[byIdx] = PBAREG[ADR0_BUF0 - byIdx];
    }
#endif

    U16 wAdd_SRAM_Idx;
    U8 byIdx;

    PBAREG[CTL_ADR_BFU_SRAM_ADDR_SEL] = 0x00;

    for (wAdd_SRAM_Idx = 0; wAdd_SRAM_Idx < 1024; wAdd_SRAM_Idx++) {
        PBAREG[CTL_ADR_BUF_SRAM_ADDR_9_8] = wAdd_SRAM_Idx >> 8;
        PBAREG[CTL_ADR_BUF_SRAM_ADDR_7_0] = wAdd_SRAM_Idx;
        for (byIdx = 0; byIdx < 10; byIdx++) {
            SET_RDY_Base[wAdd_SRAM_Idx * 10 + byIdx] = PBAREG[ADR0_BUF0 - byIdx];
            if (SET_RDY_Base[wAdd_SRAM_Idx * 10 + byIdx] != 0x00) {
                goto EXIT;
            }
        }
    }

EXIT:
    dummy_fun();

}

#if EN_STATUS_BUF_BSY_TEST
static void check_status_buffer(void)
{
    //fill status buffre entry

}

static void check_internal_bsy(void)
{
    volatile U8* pbyPBACEReg;
    U8  byCeIdx;
    U8 i;

#if 1
    //Wait all CFDE INT finish
    for (byCeIdx = 0; byCeIdx < MAX_CE_CNT; byCeIdx++) {
        if (byCeIdx == 0x02 || byCeIdx == 0x03 || byCeIdx == 0x07)
            continue;
        pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;
        while ((pbyPBACEReg[CFDE_INTV_15_8] != 0xC0) ||
                (pbyPBACEReg[CFDE_INTV_23_16] != 0x3F));
    }

    //Case 1
    //Check the BSY status by HW set busy
    for (byCeIdx = 0; byCeIdx < MAX_CE_CNT; byCeIdx++) {
        if (byCeIdx == 0x02 || byCeIdx == 0x03 || byCeIdx == 0x07)
            continue;
        pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;
        if (pbyPBACEReg[BSY8_1_FLAG_LUN0] != 0x00) {
            _gwPBA_ErrCode = 0x0001;
            return;
        }

        // HW bug so skip test
        for (i = 0; i < 0x07; i++) {
            if (pbyPBACEReg[BSY8_1_FLAG_LUN1 + i] != 0x00) {
                _gwPBA_ErrCode = 0x0002;
                return;
            }
        }

        if (pbyPBACEReg[BSY_STS_O_LUN7_0] != 0x00) {
            //if (pbyPBACEReg[BSY_STS_O_LUN7_0] != 0xFE) {
            _gwPBA_ErrCode = 0x0003;
            return;
        }
    }

    //if (PBAREG[BSY_O_CE7_0] != 0x00) {
    if (PBAREG[BSY_O_CE7_0] != 0x8C) {
        _gwPBA_ErrCode = 0x0004;
        return;
    }

    if ((PBAREG[BSY_O_ALL ]&CHK_BIT0) != 0x00) {
        _gwPBA_ErrCode = 0x0005;
        return;
    }

    //FW set ready and check status
    for (byCeIdx = 0; byCeIdx < MAX_CE_CNT; byCeIdx++) {
        pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;
        pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN0] |= (SET_RDY1 | SET_RDY2 | SET_RDY3 | SET_RDY4);
        pbyPBACEReg[SET_BSY8_5_BUSY_RDY_LUN0] |= (SET_RDY5 | SET_RDY6 | SET_RDY7 | SET_RDY8);

        for (i = 0; i < 0x0E; i = (i + 2)) {
            pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN1 + i] |= (SET_RDY1 | SET_RDY2 | SET_RDY3 | SET_RDY4);
            pbyPBACEReg[SET_BSY8_5_BUSY_RDY_LUN1 + i] |= (SET_RDY5 | SET_RDY6 | SET_RDY7 | SET_RDY8);
        }
    }

    for (byCeIdx = 0; byCeIdx < MAX_CE_CNT; byCeIdx++) {
        pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;
        if (pbyPBACEReg[BSY8_1_FLAG_LUN0] != 0xFF) {
            _gwPBA_ErrCode = 0x0006;
            return;
        }

        for (i = 0; i < 0x07; i++) {
            if (pbyPBACEReg[BSY8_1_FLAG_LUN1 + i] != 0xFF) {
                _gwPBA_ErrCode = 0x0007;
                return;
            }
        }

        if (pbyPBACEReg[BSY_STS_O_LUN7_0] != 0xFF) {
            _gwPBA_ErrCode = 0x0008;
            return;
        }
    }

    if (PBAREG[BSY_O_CE7_0] != 0xFF) {
        _gwPBA_ErrCode = 0x0009;
        return;
    }

    if ((PBAREG[BSY_O_ALL ]&CHK_BIT0) != 0x01) {
        _gwPBA_ErrCode = 0x000A;
        return;
    }
#endif

    //Case 2
    //FW set busy and check status
    for (byCeIdx = 0; byCeIdx < MAX_CE_CNT; byCeIdx++) {
        pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;
        pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN0] |= (SET_BSY1 | SET_BSY2 | SET_BSY3 | SET_BSY4);
        pbyPBACEReg[SET_BSY8_5_BUSY_RDY_LUN0] |= (SET_BSY5 | SET_BSY6 | SET_BSY7 | SET_BSY8);

        for (i = 0; i < 0x0E; i = (i + 2)) {
            pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN1 + i] |= (SET_BSY1 | SET_BSY2 | SET_BSY3 | SET_BSY4);
            pbyPBACEReg[SET_BSY8_5_BUSY_RDY_LUN1 + i] |= (SET_BSY5 | SET_BSY6 | SET_BSY7 | SET_BSY8);
        }
    }

    for (byCeIdx = 0; byCeIdx < MAX_CE_CNT; byCeIdx++) {
        pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;
        if (pbyPBACEReg[BSY8_1_FLAG_LUN0] != 0x00) {
            _gwPBA_ErrCode = 0x0010;
            return;
        }

        for (i = 0; i < 0x07; i++) {
            if (pbyPBACEReg[BSY8_1_FLAG_LUN1 + i] != 0x00) {
                _gwPBA_ErrCode = 0x0011;
                return;
            }
        }

        if (pbyPBACEReg[BSY_STS_O_LUN7_0] != 0x00) {
            _gwPBA_ErrCode = 0x0012;
            return;
        }
    }

    if (PBAREG[BSY_O_CE7_0] != 0x00) {
        _gwPBA_ErrCode = 0x0013;
        return;
    }

    if ((PBAREG[BSY_O_ALL ]&CHK_BIT0) != 0x00) {
        _gwPBA_ErrCode = 0x0014;
        return;
    }

    //FW set reafy and check status
    for (byCeIdx = 0; byCeIdx < MAX_CE_CNT; byCeIdx++) {
        pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;
        pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN0] |= (SET_RDY1 | SET_RDY2 | SET_RDY3 | SET_RDY4);
        pbyPBACEReg[SET_BSY8_5_BUSY_RDY_LUN0] |= (SET_RDY5 | SET_RDY6 | SET_RDY7 | SET_RDY8);

        for (i = 0; i < 0x0E; i = (i + 2)) {
            pbyPBACEReg[SET_BSY4_1_BUSY_RDY_LUN1 + i] |= (SET_RDY1 | SET_RDY2 | SET_RDY3 | SET_RDY4);
            pbyPBACEReg[SET_BSY8_5_BUSY_RDY_LUN1 + i] |= (SET_RDY5 | SET_RDY6 | SET_RDY7 | SET_RDY8);
        }
    }

    for (byCeIdx = 0; byCeIdx < MAX_CE_CNT; byCeIdx++) {
        pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;
        if (pbyPBACEReg[BSY8_1_FLAG_LUN0] != 0xFF) {
            _gwPBA_ErrCode = 0x0015;
            return;
        }

        for (i = 0; i < 0x07; i++) {
            if (pbyPBACEReg[BSY8_1_FLAG_LUN1 + i] != 0xFF) {
                _gwPBA_ErrCode = 0x0016;
                return;
            }
        }

        if (pbyPBACEReg[BSY_STS_O_LUN7_0] != 0xFF) {
            _gwPBA_ErrCode = 0x0017;
            return;
        }
    }

    if (PBAREG[BSY_O_CE7_0] != 0xFF) {
        _gwPBA_ErrCode = 0x0018;
        return;
    }

    if ((PBAREG[BSY_O_ALL ]&CHK_BIT0) != 0x01) {
        _gwPBA_ErrCode = 0x0019;
        return;
    }
}
#endif

#if EN_BUSY_WINDOW_TEST
void process_busy_window(void)
{
    volatile U8* pbyPBACEReg;
    U8 byCeIdx;

    for (byCeIdx = 0; byCeIdx < MAX_CE_CNT; byCeIdx++) {
        pbyPBACEReg = PBAREG_CE0 + byCeIdx * 0x100;
        //while (pbyPBACEReg[CFDE_INTV_7_0] != (1 << CFDE_IDX_CMD_8010));
        while (pbyPBACEReg[CFDE_INTV_15_8] != (1 << (CFDE_IDX_CMD_8015 - 0x08)));

        //Must setting status buffer entry to 0x80, for previous error status clear
        pbyPBACEReg[STA_BUF_DLY_T_NO] = 0x48;
        //pbyPBACEReg[STA_WR_DLY_T_ENTRY] = 0x80;
        pbyPBACEReg[STA_WR_DLY_T_ENTRY] = 0xFF;
        pbyPBACEReg[STA_BUF_NO] = 0x48;
        //pbyPBACEReg[STA_WR_ENTRY] = 0x80;
        pbyPBACEReg[STA_WR_ENTRY] = 0xFF;

        //Fill status buffer1/2 for busy window
        //pbyPBACEReg[STA_BUF_DLY_T_NO] = 0x48;
        //pbyPBACEReg[STA_WR_DLY_T_ENTRY] = 0xE0;
        //pbyPBACEReg[STA_BUF_NO] = 0x48;
        //pbyPBACEReg[STA_WR_ENTRY] = 0xE0;
        set_bsy_dly(0xFF);

        clear_int_and_setrdy(byCeIdx);
    }
}
#endif //EN_REG_WR_TEST
#endif

#if 0 
static void pba_test_test_pba_cache(void)
{
    U16 wI;  

    SYSREG[SYS_PBA_CASEL_CFG0] = SEL_ORI_MAPPING;
    for (wI = 0; wI < 16384; wI++) {
        //PBA_Cache_Buf[wI] = wI % 256;
        PBA_Cache_Buf[wI] = 0xAA;
    }  

    SYSREG[SYS_PBA_CASEL_CFG0] = SEL_CACHE0;
    for (wI = 0; wI < 16384; wI++) {
        //PBA_Cache_Buf[wI] = 0xFF - (wI % 256);
        PBA_Cache_Buf[wI] = 0xBB;
    }  

    SYSREG[SYS_PBA_CASEL_CFG0] = SEL_ORI_MAPPING;

}
#endif

/*--------------------------------------------------------------------------*/
/* GLOBAL FUNCTIONS IMPLEMENT                                               */
/*--------------------------------------------------------------------------*/
void pba_test(void)
{
    
/*
#if EN_REG_WR_TEST
    reg_wr_rd_test();
#else
    pba_test_initial_pba_reg();

    setting_all_CFDE_unit();

#if EN_ADDR_BUF_TEST
    check_addr_buffer();
#elif EN_STATUS_BUF_BSY_TEST
    check_status_buffer();
    check_internal_bsy();
#elif EN_BUSY_WINDOW_TEST
    process_busy_window();
#else
    process_read_write_erase_Cmd();
#endif
#endif //#if EN_REG_WR_TEST

#if 0 
    pba_test_test_pba_cache();
#endif
*/

}
#endif

#if 0
void auto_handler(void)
{

}
#endif
