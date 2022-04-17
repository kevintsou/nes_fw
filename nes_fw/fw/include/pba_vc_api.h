/**
 * @file ph_ap_api.h
 * Copyrihgt 2015 PHISON Electronics Corp.
 *
 * <b>
@verbatim
FILE NAME :  ph_ap_api.h
PURPOSE   :  Header file for ph_ap.c
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

#ifndef __PBA_VC_API_H__
#define __PBA_VC_API_H__

#ifdef __PBA_VC_C__
#define EXTERN
#else
#define EXTERN extern
#endif

/*--------------------------------------------------------------------------*/
/* PREDEFINED DIRECTIVE                                                     */
/*--------------------------------------------------------------------------*/
#define EN_POLLING_CFDE                (0)
#define EN_SKIP_HW_CACHE_BUF_ISSUE     (0)
#define EN_SKIP_HW_CLR_CFDEINT_ISSUE   (0)
#define CACHE_HD_CPU  (0)
/*--------------------------------------------------------------------------*/
/* INCLUDE FILE                                                             */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/* CONSTANT DECLARATION                                                     */
/*--------------------------------------------------------------------------*/
//set info page format
#define IP_HEADER0_VAL              0x49 //'I'
#define IP_HEADER1_VAL              0x50 //'P'

//Global level(0x00~0x1FF)
#define IP_HEADER0_OFS              0x00
#define IP_HEADER1_OFS              0x01
#define IP_FLASH_TYPE_OFS           0x02
#define FLASH_TYPE_SFT_BIT              0x05
#define IP_FLASH_ID_OFS             0x03 //0x03~0x0A
#define IP_PAGE_SIZE_OFS            0x0B
/*
#define IP_TT_DEVS_OFS              0x0C
#define IP_TT_DIES_OFS              0x0D
#define IP_TT_BLKS_OFS              0x0E
#define IP_TT_PAGES_OFS             0x10
#define IP_TT_BYTES_OFS             0x12
*/
//#define IP_SET_CE_LUN_PLANE_OFS     0x0C

#define IP_CACHE_MODE_OFS           0x0D
#define CACHE_MODE_SW                  (0x00)
#define CACHE_MODE_HW_ADDR             (0x01)
#define CACHE_MODE_RR                  (0x02)

#define IP_MAX_CE_CNT_OFS           0x0E
#define IP_MAX_LUN_CNT_OFS          0x0F
#define IP_MAX_PLANE_CNT_OFS        0x10

#define IP_DQS_SR_DTY                0x11


#define IP_RD_XFER_NO_H_OFS         0x1B
#define IP_RD_XFER_NO_L_OFS         0x1C

#define IP_EN_BAD_COL_FUN           0x1D
    #define EN_BAD_COL_FUN          0x01
    #define DIS_BAD_COL_FUN         0x00
#define IP_EN_LOCK_UNLOCK_FUN       0x1E
#define IP_EN_WP_FUN                0x1F

#define IP_HW_DBG_MODE              0x20

#define IP_BUS_ANA_EN               0x24
#define IP_BUS_ANA_THRE_L           0x25
#define IP_BUS_ANA_THRE_H           0x26
#define IP_BUS_ANA_STACK_MODE_EN    0x27

#define IP_PWR_DET_EN               0x28
  #define PWR_DET_EN                0x01
  #define PWR_DET_DIS               0x00

#define IP_DPS_STACKING_MODE        0x29  

#define IP_MASK_RSTN_BSY_EN        0x2A
	


#define IP_FLASH_ID_0x20_OFS        0x2F //0x2F~0x36
#define IP_FLASH_ID_0x40_OFS        0x37 //0x37~0x3E

#define IP_F8_VALUE_OFS           0x3F
#define IP_H_IO_IE_OFS          0x40
#define IP_H_IO_OE_OFS          0x41
#define IP_SET_RDYBSY_OFS       0x42
#define IP_LA_SEL_OFS               0x43
#define IP_WEB_CFG1                 0x44
#define IP_CEB_CFG0                 0x45
#define IP_CEB_CFG1                 0x46
#define IP_RDY_CFG0                 0x47
#define IP_RDY_CFG1                 0x48
#define IP_REB_CFG0                 0x49
#define IP_REB_CFG1                 0x4A
#define IP_DQS_CFG0                 0x4B
#define IP_DQS_CFG1                 0x4C

#define IP_DELAYTIME_H              0x4F
#define IP_DELAYTIME_L              0x50
#define IP_DELAYTIME2_H             0x51
#define IP_DELAYTIME2_L             0x52
#define IP_DELAYTIME3_H             0x53
#define IP_DELAYTIME3_L             0x54
#define IP_RDY_POS                  0x55
#define IP_PWR_CFG                  0x56
#define IP_DAT_CFG0                 0x57
#define IP_DAT_CFG1                 0x58
#define IP_CLE_CFG0                 0x59
#define IP_CLE_CFG1                 0x5A
#define IP_ALE_CFG0                 0x5B
#define IP_ALE_CFG1                 0x5C
#define IP_WEB_CFG0                 0x5D

#define IP_AIPR_EN                  0x5E


//Plane/Lun Bit for Address Buffer and Cache Header reference(0x100~0x105)
#define IP_PLANE_SEL_PA0           0x100
#define IP_PLANE_SEL_PA1           0x101
#define IP_PLANE_SEL_PA2           0x102
#define IP_LUN_SEL_PA0             0x103
#define IP_LUN_SEL_PA1             0x104
#define IP_LUN_SEL_PA2             0x105

//Lun Bit for DPS RAM reference(0x106~0x108)
//F8=>Lun0
//F1=>Lun1, F7=>Lun7
#define IP_DPS_RAM_LUN_SEL_PA0     0x106
#define IP_DPS_RAM_LUN_SEL_PA1     0x107
#define IP_DPS_RAM_LUN_SEL_PA2     0x108

//Bad Column base pointer(HW default+32) for CE(0x109~0x110)
#define IP_BAD_COLUM_BASE_PTR_CE0  0x109
#define IP_BAD_COLUM_BASE_PTR_CE7  0x110


//CE level(0x200~0x3FF)
//WR/RD cache lun/plane bit for cache buffer(0x200~0x260)
#define CE_CACHE_SEL_LEN        (12)
#define IP_WR_CACHE_SEL_PA0     0x200
#define IP_WR_CACHE_SEL_PA1     0x201
#define IP_WR_CACHE_SEL_PA2     0x202
#define IP_WR_CACHE_SEL_PA3     0x203
#define IP_WR_CACHE_SEL_PA4     0x204
#define IP_WR_CACHE_SEL_PA5     0x205

#define IP_RD_CACHE_SEL_PA0     0x206
#define IP_RD_CACHE_SEL_PA1     0x207
#define IP_RD_CACHE_SEL_PA2     0x208
#define IP_RD_CACHE_SEL_PA3     0x209
#define IP_RD_CACHE_SEL_PA4     0x20A
#define IP_RD_CACHE_SEL_PA5     0x20B

//Enhance 71 Cmd/78 cmd lun/plane bit(0x261~2A8)
#define CE_STA_SEL_LEN           (9)
#define IP_EN71CMD_STA_SEL_PA0  0x261
#define IP_EN71CMD_STA_SEL_PA1  0x262
#define IP_EN71CMD_STA_SEL_PA2  0x263

#define IP_78CMD_STA_SEL_PA0    0x264
#define IP_78CMD_STA_SEL_PA1    0x265
#define IP_78CMD_STA_SEL_PA2    0x266
#define IP_78CMD_STA_SEL_PA3    0x267
#define IP_78CMD_STA_SEL_PA4    0x268
#define IP_78CMD_STA_SEL_PA5    0x269

/*
#define IP_STA0_BIT76_OFS       0x200
#define IP_STA0_BIT54_OFS       0x201
#define IP_STA0_BIT32_OFS       0x202
#define IP_STA0_BIT10_OFS       0x203
#define IP_STA0_BUF_VAL_OFS     0x400
#define IP_STA1_BUF_VAL_OFS     0x401
#define IP_STA2_BUF_VAL_OFS     0x402
#define IP_STA3_BUF_VAL_OFS     0x403
#define IP_STA_BUF_BYTENO       0x04
*/

//information bit
//Cmd78(0x2A9~2C0)
//LUN0~7
#define IP_CMD78_LUN0_STA0_BIT5_OFS   0x2A9
#define IP_CMD78_LUN0_STA0_BIT6_OFS   0x2AA
#define IP_CMD78_LUN0_STA0_BIT7_OFS   0x2AB

//Enhance Cmd71(0x2C1~2D8)
//LUN0~7
#define IP_ENCMD71_LUN0_STA0_BIT5_OFS 0x2C1
#define IP_ENCMD71_LUN0_STA0_BIT6_OFS 0x2C2
#define IP_ENCMD71_LUN0_STA0_BIT7_OFS 0x2C3

//7X/Fx Cmd
//CMD70
#define IP_70_LUN0_STA0_BIT5_OFS      0x2D9
#define IP_70_LUN0_STA0_BIT6_OFS      0x2DA
#define IP_70_LUN0_STA0_BIT7_OFS      0x2DB

//F1CMD
#define IP_F1_LUN0_STA0_BIT5_OFS      0x2DC
#define IP_F1_LUN0_STA0_BIT6_OFS      0x2DD
#define IP_F1_LUN0_STA0_BIT7_OFS      0x2DE

//F2CMD
#define IP_F2_LUN1_STA0_BIT5_OFS      0x2DF
#define IP_F2_LUN1_STA0_BIT6_OFS      0x2E0
#define IP_F2_LUN1_STA0_BIT7_OFS      0x2E1

//F3CMD
#define IP_F3_LUN2_STA0_BIT5_OFS      0x2E2
#define IP_F3_LUN2_STA0_BIT6_OFS      0x2E3
#define IP_F3_LUN2_STA0_BIT7_OFS      0x2E4

//F4CMD
#define IP_F4_LUN3_STA0_BIT5_OFS      0x2E5
#define IP_F4_LUN3_STA0_BIT6_OFS      0x2E6
#define IP_F4_LUN3_STA0_BIT7_OFS      0x2E7

//F5CMD
#define IP_F5_LUN4_STA0_BIT5_OFS      0x2E8
#define IP_F5_LUN4_STA0_BIT6_OFS      0x2E9
#define IP_F5_LUN4_STA0_BIT7_OFS      0x2EA

//F6CMD
#define IP_F6_LUN5_STA0_BIT5_OFS      0x2EB
#define IP_F6_LUN5_STA0_BIT6_OFS      0x2EC
#define IP_F6_LUN5_STA0_BIT7_OFS      0x2ED

//F7CMD
#define IP_F7_LUN6_STA0_BIT5_OFS      0x2EE
#define IP_F7_LUN6_STA0_BIT6_OFS      0x2EF
#define IP_F7_LUN6_STA0_BIT7_OFS      0x2F0

//F8CMD
#define IP_F8_LUN7_STA0_BIT5_OFS      0x2F1
#define IP_F8_LUN7_STA0_BIT6_OFS      0x2F2
#define IP_F8_LUN7_STA0_BIT7_OFS      0x2F3

//Fill Status delay time 
#define IP_STA_LUN0_BSY1_BSY8_DLY   0x2F4
#define IP_STA_LUN1_BSY1_BSY8_DLY   0x2F5
#define IP_STA_LUN2_BSY1_BSY8_DLY   0x2F6
#define IP_STA_LUN3_BSY1_BSY8_DLY   0x2F7
#define IP_STA_LUN4_BSY1_BSY8_DLY   0x2F8
#define IP_STA_LUN5_BSY1_BSY8_DLY   0x2F9
#define IP_STA_LUN6_BSY1_BSY8_DLY   0x2FA
#define IP_STA_LUN7_BSY1_BSY8_DLY   0x2FB

//71CMD
#define IP_71_STA_BIT5_OFS      0x2FC
#define IP_71_STA_BIT6_OFS      0x2FD
#define IP_71_STA_BIT7_OFS      0x2FE

//72CMD
#define IP_72_STA_BIT5_OFS      0x2FF
#define IP_72_STA_BIT6_OFS      0x300
#define IP_72_STA_BIT7_OFS      0x301


//73CMD
#define IP_73_STA_BIT5_OFS      0x302
#define IP_73_STA_BIT6_OFS      0x303
#define IP_73_STA_BIT7_OFS      0x304

//74CMD
#define IP_74_STA_BIT5_OFS      0x305
#define IP_74_STA_BIT6_OFS      0x306
#define IP_74_STA_BIT7_OFS      0x307



//Covertion 0x308~0x334
#define IP_COV_EN               0x308
#define IP_COV_START_OFS        0x309  
#define IP_COV_MODE_BYTE0_OFS   0x30D


//74CMD
#define IP_76_STA_BIT5_OFS      0x335
#define IP_76_STA_BIT6_OFS      0x336
#define IP_76_STA_BIT7_OFS      0x337

//6BCMD
#define IP_6B_STA_BIT5_OFS      0x338
#define IP_6B_STA_BIT6_OFS      0x339
#define IP_6B_STA_BIT7_OFS      0x33A
//6CCMD
#define IP_6C_STA_BIT5_OFS      0x33B
#define IP_6C_STA_BIT6_OFS      0x33C
#define IP_6C_STA_BIT7_OFS      0x33D


//fill CFDE Block format
#define CFDE_BLK_MARKER_H           0xCF
#define CFDE_BLK_MARKER_L           0xDE

#define CFDE_BLK_HD_H                   0x00
#define CFDE_BLK_HD_L                   0x01
#define CFDE_BLK_UNITNO                 0x02
#define CFDE_BLK_SETNO                  0x03

#define CFDE_BLK_CMDQ_START             0x06

/*
#define CFDE_CMDQ_NO                    0x04
#define CFDE_CMDQ_HH                    0x00
#define CFDE_CMDQ_HL                    0x01
#define CFDE_CMDQ_LH                    0x02
#define CFDE_CMDQ_LL                    0x03
*/
#define CFDE_CMDQ_NO                    0x06
#define CFDE_CMDQ_BYTE5                 0x00
#define CFDE_CMDQ_BYTE4                 0x01
#define CFDE_CMDQ_BYTE3                 0x02
#define CFDE_CMDQ_BYTE2                 0x03
#define CFDE_CMDQ_BYTE1                 0x04
#define CFDE_CMDQ_BYTE0                 0x05

//request data block definition
#define ADR_CNT_CDB_NO                  0x03
#define STS_CNT_CDB_NO                  0x04

#define REQ_ADR_BUF_NO                  0x01 //use 16 sets of address buffers


#define REQ_BLK_HEADER0_VAL         0x52 //'R'
#define REQ_BLK_HEADER1_VAL         0x51 //'Q'

#define REQ_BLK_HEADER0                 0x00
#define REQ_BLK_HEADER1                 0x01
#define REQ_BLK_CFDE_INT_0          0x02
#define REQ_BLK_CFDE_INT_1          0x03
#define REQ_BLK_CFDE_INT_2          0x04
#define REQ_BLK_CFDE_INT_3          0x05
#define REQ_BLK_CFDE_INT_4          0x06
#define REQ_BLK_CFDE_INT_5          0x07
#define REQ_BLK_CFDE_INT_6          0x08
#define REQ_BLK_CFDE_INT_7          0x09
#define REQ_BLK_CFDE_INT_8          0x0A
#define REQ_BLK_CFDE_INT_9          0x0B
#define REQ_BLK_CFDE_INT_10         0x0C
#define REQ_BLK_CFDE_INT_11         0x0D
#define REQ_BLK_CFDE_INT_12         0x0E
#define REQ_BLK_CFDE_INT_TYPE       0x0F
#define REQ_BLK_CFDE_FLAG               0x10
#define REQ_BLK_BUF0_ADR0               0x11
#define REQ_BLK_BUF0_ADR1               0x12
#define REQ_BLK_BUF0_ADR2               0x13
#define REQ_BLK_BUF0_ADR3               0x14
#define REQ_BLK_BUF0_ADR4               0x15
#define REQ_BLK_BUF0_ADR5               0x16
#define REQ_BLK_BUF0_ADR6               0x17
#define REQ_BLK_BUF0_ADR7               0x18
#define REQ_BLK_BUF0_ADR8               0x19
#define REQ_BLK_BUF0_ADR9               0x1A
#define REQ_BLK_BUF1_ADR0               0x1B
#define REQ_BLK_BUF1_ADR1               0x1C
#define REQ_BLK_BUF1_ADR2               0x1D
#define REQ_BLK_BUF1_ADR3               0x1E
#define REQ_BLK_BUF1_ADR4               0x1F
#define REQ_BLK_BUF1_ADR5               0x20
#define REQ_BLK_BUF1_ADR6               0x21
#define REQ_BLK_BUF1_ADR7               0x22
#define REQ_BLK_BUF1_ADR8               0x23
#define REQ_BLK_BUF1_ADR9               0x24
#define REQ_BLK_BUF2_ADR0               0x25
#define REQ_BLK_BUF2_ADR1               0x26
#define REQ_BLK_BUF2_ADR2               0x27
#define REQ_BLK_BUF2_ADR3               0x28
#define REQ_BLK_BUF2_ADR4               0x29
#define REQ_BLK_BUF2_ADR5               0x2A
#define REQ_BLK_BUF2_ADR6               0x2B
#define REQ_BLK_BUF2_ADR7               0x2C
#define REQ_BLK_BUF2_ADR8               0x2D
#define REQ_BLK_BUF2_ADR9               0x2E
#define REQ_BLK_BUF3_ADR0               0x2F
#define REQ_BLK_BUF3_ADR1               0x30
#define REQ_BLK_BUF3_ADR2               0x31
#define REQ_BLK_BUF3_ADR3               0x32
#define REQ_BLK_BUF3_ADR4               0x33
#define REQ_BLK_BUF3_ADR5               0x34
#define REQ_BLK_BUF3_ADR6               0x35
#define REQ_BLK_BUF3_ADR7               0x36
#define REQ_BLK_BUF3_ADR8               0x37
#define REQ_BLK_BUF3_ADR9               0x38
#define REQ_BLK_ADR_AMOUNT          0x0A
#define REQ_BLK_RD_TRF_CNT_H        0xB1
#define REQ_BLK_RD_TRF_CNT_L        0xB2
#define REQ_BLK_WR_TRF_CNT_H        0xB3
#define REQ_BLK_WR_TRF_CNT_L        0xB4
#define REQ_BLK_CUR_CMD                 0xB5
#define REQ_BLK_PRV_CMD1                0xB6
#define REQ_BLK_PRV_CMD2                0xB7
#define REQ_BLK_PRV_CMD3                0xB8
#define REQ_BLK_PRV_CMD4                0xB9
#define REQ_BLK_PRV_CMD5                0xBA
#define REQ_BLK_PRV_CMD6                0xBB
#define REQ_BLK_PRV_CMD7                0xBC
#define REQ_BLK_PRV_CMD8                0xBD
#define REQ_BLK_PRV_CMD9                0xBE
#define REQ_BLK_PRV_CMD10               0xBF
#define REQ_BLK_PRV_CMD11               0xC0
#define REQ_BLK_PRV_CMD12               0xC1
#define REQ_BLK_PRV_CMD13               0xC2
#define REQ_BLK_PRV_CMD14               0xC3
#define REQ_BLK_PRV_CMD15               0xC4

#define REQ_CFDE_TAG_7_0                0xC9
#define REQ_CFDE_TAG_15_8               0xCA
#define REQ_CFDE_TAG_23_16          0xCB
#define REQ_CFDE_TAG_31_24          0xCC
#define REQ_CFDE_TAG_39_32          0xCD
#define REQ_CFDE_TAG_47_40          0xCE
#define REQ_CFDE_TAG_55_48          0xCF
#define REQ_CFDE_TAG_63_56          0xD0
#define REQ_CFDE_TAG_71_64          0xD1
#define REQ_CFDE_TAG_79_72          0xD2
#define REQ_CFDE_TAG_87_80          0xD3
#define REQ_CFDE_TAG_95_88          0xD4
#define REQ_CFDE_TAG_99_96          0xD5
#define REQ_BUS_ANA_INT_STS         0xD6
#define REQ_BUS_ANA_INT_EN          0xD7
#define REQ_BUS_ANA_THRE_L          0xD8
#define REQ_BUS_ANA_THRE_H          0xD9
#define REQ_BUS_ANA_CC_STS          0xDA
#define REQ_BUS_ANA_NOW_THRE_L  0xDB
#define REQ_BUS_ANA_NOW_THRE_H  0xDC
#define REQ_BUS_ANA_STS                 0xDD
#define REQ_CFDE_STS_FLAG               0xDE

//set ready vendor command format
#define RDY_PG_VAL_HD0                  0x52 //'R'
#define RDY_PG_VAL_HD1                  0x44 //'D'

//Global level(0x00~0x1FF)
#define RDY_PG_GLOBAL_BASE             (0x00)

#define RDY_PG_OFS_HD0                 (0x00)
#define RDY_PG_OFS_HD1                 (0x01)

#define RDY_PG_OFS_CE_IDX              (0x02)
#define RDY_PG_OFS_LUN_IDX             (0x03)
#define RDY_PG_OFS_SUBBSY_IDX          (0x04)

#define RDY_PG_OFS_EN_CLR_CE_TEMP_ADDR (0x05)

//#define RDY_PG_OFS_EN_SET_CE_ALL_RDY   (0x06)
#define RDY_PG_OFS_EN_CE_LEVEL_MODE    (0x06)
#define RDY_PG_OFS_2G_CLR_ALL_STS_BUF  (0x07)
#define RDY_PG_OFS_EN_LUN_LEVEL_MODE   (0x08)
#define RDY_PG_OFS_EN_Clear_Temp_Tag   (0x09)

//#define RDY_PG_OFS_EN_CLR_ALL_INT_CE   (0x07)
//#define RDY_PG_OFS_EN_CLR_ALL_TAG_CE   (0x08)

#define RDY_PG_OFS_CLR_CACHE_7_0_HEADER    (0x10)
#define RDY_PG_OFS_CLR_CACHE_15_8_HEADER   (0x11)
#define RDY_PG_OFS_CLR_CACHE_23_16_HEADER  (0x12)
#define RDY_PG_OFS_CLR_CACHE_31_24_HEADER  (0x13)
#define RDY_PG_OFS_CLR_CACHE_39_32_HEADER  (0x14)
#define RDY_PG_OFS_CLR_CACHE_47_40_HEADER  (0x15)
#define RDY_PG_OFS_CLR_CACHE_55_48_HEADER  (0x16)
#define RDY_PG_OFS_CLR_CACHE_63_56_HEADER  (0x17)

#define RDY_PG_OFS_EN_AFIFO_SETTING        (0x18)

#define RDY_PG_OFS_EN_CLR_WR_CNT_CE        (0x19)

#define RDY_PG_OFS_EN_CLR_RD_CNT_CE        (0x1A)

#define RDY_PG_OFS_EN_CLR_SG_ADDR_QUEUE_CE (0x1B)

#define RDY_VALUE_BUS_ANA_STS              (0x26)
#define RDY_PG_OFS_BUS_ANA_EN              (0x30)

#define RDY_PG_OFS_STATUS_SW_MODE_EN       (0x31)

#define RDY_PG_OFS_SET_DEALY_WINDOW_EN     (0x32)
#define RDY_PG_OFS_DEALY_WINDOW_VAL        (0x33)

#define RDY_PG_OFS_CLR_PWR_CHG_EVENT_EN    (0x34)
  #define CLR_PWR_CHG_EVENT_EN             (0x01)    
  #define CLR_PWR_CHG_EVENT_DIS            (0x00)

#define  RDY_PG_OFS_STA_BIT5_MODE          (0x35)


#define RDY_PG_OFS_Shift_Lun_Plane_Bit      (0x36)

#define RDY_PG_OFS_Shift_Lun_Bit_Value      (0x37)

#define RDY_PG_OFS_Shift_Plane_Bit_Value    (0x38)

#define RDY_PG_OFS_STA_BIT6_MODE           (0x39)

#define RDY_PG_OFS_SET_RDY_MAX_LUN_NO         0x40
#define RDY_PG_OFS_SET_RDY_LUN_NO_0           0x41
#define RDY_PG_OFS_SET_RDY_LUN_NO_1           0x42
#define RDY_PG_OFS_SET_RDY_LUN_NO_2           0x43
#define RDY_PG_OFS_SET_RDY_LUN_NO_3           0x44
#define RDY_PG_OFS_SET_RDY_LUN_NO_4           0x45
#define RDY_PG_OFS_SET_RDY_LUN_NO_5           0x46
#define RDY_PG_OFS_SET_RDY_LUN_NO_6           0x47
#define RDY_PG_OFS_SET_RDY_LUN_NO_7           0x48



//CE level(0x200~0x3FF)

#define RDY_PG_OFS_STS_BUF_BASE          (0x200)

#define RDY_PG_OFS_STS_BUF1_VALUE_BASE    (0x280)

#define RDY_PG_OFS_STS_BUF2_VALUE_BASE    (0x300)

//Status Buf No OSF
#define RDY_PG_OFS_STS_BUF_70h       (RDY_PG_OFS_STS_BUF_BASE+STS_BUFNO_70CMD)  
#define RDY_PG_OFS_STS_BUF_71h       (RDY_PG_OFS_STS_BUF_BASE+STS_BUFNO_71CMD) 
#define RDY_PG_OFS_STS_BUF_72h       (RDY_PG_OFS_STS_BUF_BASE+STS_BUFNO_72CMD) 
#define RDY_PG_OFS_STS_BUF_73h       (RDY_PG_OFS_STS_BUF_BASE+STS_BUFNO_73CMD) 
#define RDY_PG_OFS_STS_BUF_74h       (RDY_PG_OFS_STS_BUF_BASE+STS_BUFNO_74CMD) 
#define RDY_PG_OFS_STS_BUF_75h       (RDY_PG_OFS_STS_BUF_BASE+STS_BUFNO_75CMD) 
#define RDY_PG_OFS_STS_BUF_76h       (RDY_PG_OFS_STS_BUF_BASE+STS_BUFNO_76CMD)
#define RDY_PG_OFS_STS_BUF_77h       (RDY_PG_OFS_STS_BUF_BASE+STS_BUFNO_77CMD) 

#define RDY_PG_OFS_STS_BUF_ENH_78    (RDY_PG_OFS_STS_BUF_BASE+STS_BUFNO_ENH_78CMD) 
#define RDY_PG_OFS_STS_BUF_ENH_71    (RDY_PG_OFS_STS_BUF_BASE+STS_BUFNO_ENH_71CMD) 


#define RDY_PG_OFS_STS_BUF_F1h       (RDY_PG_OFS_STS_BUF_BASE+STS_BUFNO_F1CMD)  
#define RDY_PG_OFS_STS_BUF_F2h       (RDY_PG_OFS_STS_BUF_BASE+STS_BUFNO_F2CMD) 
#define RDY_PG_OFS_STS_BUF_F3h       (RDY_PG_OFS_STS_BUF_BASE+STS_BUFNO_F3CMD) 
#define RDY_PG_OFS_STS_BUF_F4h       (RDY_PG_OFS_STS_BUF_BASE+STS_BUFNO_F4CMD) 
#define RDY_PG_OFS_STS_BUF_F5h       (RDY_PG_OFS_STS_BUF_BASE+STS_BUFNO_F5CMD) 
#define RDY_PG_OFS_STS_BUF_F6h       (RDY_PG_OFS_STS_BUF_BASE+STS_BUFNO_F6CMD) 
#define RDY_PG_OFS_STS_BUF_F7h       (RDY_PG_OFS_STS_BUF_BASE+STS_BUFNO_F7CMD) 
#define RDY_PG_OFS_STS_BUF_F8h       (RDY_PG_OFS_STS_BUF_BASE+STS_BUFNO_F8CMD) 
#define RDY_PG_OFS_STS_BUF_6Ch       (RDY_PG_OFS_STS_BUF_BASE+STS_BUFNO_6CCMD)
#define RDY_PG_OFS_STS_BUF_6Bh       (RDY_PG_OFS_STS_BUF_BASE+STS_BUFNO_6BCMD)

//Status Buf1 Value OFS
#define RDY_PG_OFS_STS_BUF1_VALUE_70h              (RDY_PG_OFS_STS_BUF1_VALUE_BASE+STS_BUFNO_70CMD)  
#define RDY_PG_OFS_STS_BUF1_VALUE_71h              (RDY_PG_OFS_STS_BUF1_VALUE_BASE+STS_BUFNO_71CMD) 
#define RDY_PG_OFS_STS_BUF1_VALUE_72h              (RDY_PG_OFS_STS_BUF1_VALUE_BASE+STS_BUFNO_72CMD) 
#define RDY_PG_OFS_STS_BUF1_VALUE_73h              (RDY_PG_OFS_STS_BUF1_VALUE_BASE+STS_BUFNO_73CMD)
#define RDY_PG_OFS_STS_BUF1_VALUE_74h              (RDY_PG_OFS_STS_BUF1_VALUE_BASE+STS_BUFNO_74CMD)
#define RDY_PG_OFS_STS_BUF1_VALUE_75h              (RDY_PG_OFS_STS_BUF1_VALUE_BASE+STS_BUFNO_75CMD) 
#define RDY_PG_OFS_STS_BUF1_VALUE_76h              (RDY_PG_OFS_STS_BUF1_VALUE_BASE+STS_BUFNO_76CMD)
#define RDY_PG_OFS_STS_BUF1_VALUE_77h              (RDY_PG_OFS_STS_BUF1_VALUE_BASE+STS_BUFNO_77CMD) 

#define RDY_PG_OFS_STS_BUF1_VALUE_ENH_78           (RDY_PG_OFS_STS_BUF1_VALUE_BASE+STS_BUFNO_ENH_78CMD) 
#define RDY_PG_OFS_STS_BUF1_VALUE_ENH_71           (RDY_PG_OFS_STS_BUF1_VALUE_BASE+STS_BUFNO_ENH_71CMD) 


#define RDY_PG_OFS_STS_BUF1_VALUE_F1h              (RDY_PG_OFS_STS_BUF1_VALUE_BASE+STS_BUFNO_F1CMD)  
#define RDY_PG_OFS_STS_BUF1_VALUE_F2h              (RDY_PG_OFS_STS_BUF1_VALUE_BASE+STS_BUFNO_F2CMD) 
#define RDY_PG_OFS_STS_BUF1_VALUE_F3h              (RDY_PG_OFS_STS_BUF1_VALUE_BASE+STS_BUFNO_F3CMD) 
#define RDY_PG_OFS_STS_BUF1_VALUE_F4h              (RDY_PG_OFS_STS_BUF1_VALUE_BASE+STS_BUFNO_F4CMD) 
#define RDY_PG_OFS_STS_BUF1_VALUE_F5h              (RDY_PG_OFS_STS_BUF1_VALUE_BASE+STS_BUFNO_F5CMD) 
#define RDY_PG_OFS_STS_BUF1_VALUE_F6h              (RDY_PG_OFS_STS_BUF1_VALUE_BASE+STS_BUFNO_F6CMD) 
#define RDY_PG_OFS_STS_BUF1_VALUE_F7h              (RDY_PG_OFS_STS_BUF1_VALUE_BASE+STS_BUFNO_F7CMD) 
#define RDY_PG_OFS_STS_BUF1_VALUE_F8h              (RDY_PG_OFS_STS_BUF1_VALUE_BASE+STS_BUFNO_F8CMD)
#define RDY_PG_OFS_STS_BUF1_VALUE_6Ch              (RDY_PG_OFS_STS_BUF1_VALUE_BASE+STS_BUFNO_6CCMD)
#define RDY_PG_OFS_STS_BUF1_VALUE_6Bh              (RDY_PG_OFS_STS_BUF1_VALUE_BASE+STS_BUFNO_6BCMD)



//Status Buf2 Value OFS
#define RDY_PG_OFS_STS_BUF2_VALUE_70h              (RDY_PG_OFS_STS_BUF2_VALUE_BASE+STS_BUFNO_70CMD)  
#define RDY_PG_OFS_STS_BUF2_VALUE_71h              (RDY_PG_OFS_STS_BUF2_VALUE_BASE+STS_BUFNO_71CMD) 
#define RDY_PG_OFS_STS_BUF2_VALUE_72h              (RDY_PG_OFS_STS_BUF2_VALUE_BASE+STS_BUFNO_72CMD) 
#define RDY_PG_OFS_STS_BUF2_VALUE_73h              (RDY_PG_OFS_STS_BUF2_VALUE_BASE+STS_BUFNO_73CMD) 
#define RDY_PG_OFS_STS_BUF2_VALUE_74h              (RDY_PG_OFS_STS_BUF2_VALUE_BASE+STS_BUFNO_74CMD)
#define RDY_PG_OFS_STS_BUF2_VALUE_75h              (RDY_PG_OFS_STS_BUF2_VALUE_BASE+STS_BUFNO_75CMD) 
#define RDY_PG_OFS_STS_BUF2_VALUE_76h              (RDY_PG_OFS_STS_BUF2_VALUE_BASE+STS_BUFNO_76CMD)
#define RDY_PG_OFS_STS_BUF2_VALUE_77h              (RDY_PG_OFS_STS_BUF2_VALUE_BASE+STS_BUFNO_77CMD) 

#define RDY_PG_OFS_STS_BUF2_VALUE_ENH_78           (RDY_PG_OFS_STS_BUF2_VALUE_BASE+STS_BUFNO_ENH_78CMD) 
#define RDY_PG_OFS_STS_BUF2_VALUE_ENH_71           (RDY_PG_OFS_STS_BUF2_VALUE_BASE+STS_BUFNO_ENH_71CMD) 


#define RDY_PG_OFS_STS_BUF2_VALUE_F1h              (RDY_PG_OFS_STS_BUF2_VALUE_BASE+STS_BUFNO_F1CMD)  
#define RDY_PG_OFS_STS_BUF2_VALUE_F2h              (RDY_PG_OFS_STS_BUF2_VALUE_BASE+STS_BUFNO_F2CMD) 
#define RDY_PG_OFS_STS_BUF2_VALUE_F3h              (RDY_PG_OFS_STS_BUF2_VALUE_BASE+STS_BUFNO_F3CMD) 
#define RDY_PG_OFS_STS_BUF2_VALUE_F4h              (RDY_PG_OFS_STS_BUF2_VALUE_BASE+STS_BUFNO_F4CMD) 
#define RDY_PG_OFS_STS_BUF2_VALUE_F5h              (RDY_PG_OFS_STS_BUF2_VALUE_BASE+STS_BUFNO_F5CMD) 
#define RDY_PG_OFS_STS_BUF2_VALUE_F6h              (RDY_PG_OFS_STS_BUF2_VALUE_BASE+STS_BUFNO_F6CMD) 
#define RDY_PG_OFS_STS_BUF2_VALUE_F7h              (RDY_PG_OFS_STS_BUF2_VALUE_BASE+STS_BUFNO_F7CMD) 
#define RDY_PG_OFS_STS_BUF2_VALUE_F8h              (RDY_PG_OFS_STS_BUF2_VALUE_BASE+STS_BUFNO_F8CMD)
#define RDY_PG_OFS_STS_BUF2_VALUE_6Ch              (RDY_PG_OFS_STS_BUF2_VALUE_BASE+STS_BUFNO_6CCMD)
#define RDY_PG_OFS_STS_BUF2_VALUE_6Bh              (RDY_PG_OFS_STS_BUF2_VALUE_BASE+STS_BUFNO_6BCMD)


#define RDY_PG_OFS_STS_BUF_BY_LUN      0x380          
#define RDY_PG_OFS_STS_BUF1_VAL        0x381
#define RDY_PG_OFS_STS_BUF2_VAL        0x382
#define RDY_PG_OFS_STS_BUF_LUN_DLY_OP         0x383       
#define RDY_PG_OFS_UPDATA_STS_BY_LUN_78h      0x384

#define RDY_PG_OFS_Dynamic_WR_CACHE_SEL_PA0   0x385   
#define RDY_PG_OFS_Dynamic_WR_CACHE_SEL_PA1   0x386
#define RDY_PG_OFS_Dynamic_WR_CACHE_SEL_PA2   0x387
#define RDY_PG_OFS_Dynamic_WR_CACHE_SEL_PA3   0x388
#define RDY_PG_OFS_Dynamic_WR_CACHE_SEL_PA4   0x389
#define RDY_PG_OFS_Dynamic_WR_CACHE_SEL_PA5   0x38A
 
#define RDY_PG_OFS_Dynamic_CH_LUN_SEL_PA0     0x38B
#define RDY_PG_OFS_Dynamic_CH_LUN_SEL_PA1     0x38C
#define RDY_PG_OFS_Dynamic_CH_LUN_SEL_PA2     0x38D


#define RDY_PG_OFS_Dynamic_CH_PLANE_SEL_PA0   0x38E
#define RDY_PG_OFS_Dynamic_CH_PLANE_SEL_PA1   0x38F
#define RDY_PG_OFS_Dynamic_CH_PLANE_SEL_PA2   0x390


#define RDY_PG_OFS_Dynamic_STA78_PLANE_SEL_PA0   0x391
#define RDY_PG_OFS_Dynamic_STA78_PLANE_SEL_PA1   0x392
#define RDY_PG_OFS_Dynamic_STA78_PLANE_SEL_PA2   0x393
#define RDY_PG_OFS_Dynamic_STA78_PLANE_SEL_PA3   0x394
#define RDY_PG_OFS_Dynamic_STA78_PLANE_SEL_PA4   0x395
#define RDY_PG_OFS_Dynamic_STA78_PLANE_SEL_PA5   0x396


#define RDY_PG_OFS_VENDOR_CMD_SET_ALL_READY      0x397


#define RDY_PG_OFS_FLASH_IO_VOLTAGE              0x398

#define RDY_PG_OFS_2G_FLASH_TEMP                 0x399


#define CE_CNT           (0x08)
#define LUN_CNT_PERCE    (0x08)

#define RDY_PG_PASS                     0x00
#define RDY_PG_FAIL                     0x01

#define PA_SELX_RESERVED    (0x7F)


/*
#define RDY_PG_OFS_CC_SWHW              0x04
#define RDY_PG_OFS_CC_PTR               0x05
#define RDY_PG_OFS_CC_PA0               0x06
#define RDY_PG_OFS_CC_PA1               0x07
#define RDY_PG_OFS_CC_PA2               0x08
#define RDY_PG_OFS_CC_PA3               0x09
#define RDY_CUR_CMD                         0x0A
#define RDY_PRV_CMD1                        0x0B
#define RDY_PRV_CMD2                        0x0C
#define RDY_PRV_CMD3                        0x0D
#define RDY_PRV_CMD4                        0x0E
#define RDY_WR_XFER_CNT_H               0x0F
#define RDY_WR_XFER_CNT_L               0x10
#define RDY_RD_XFER_CNT_H               0x11
#define RDY_RD_XFER_CNT_L               0x12
#define RDY_VALUE_STS_BUF0          0x13
#define RDY_VALUE_STS_BUF1          0x14
#define RDY_VALUE_STS_BUF2          0x15
#define RDY_VALUE_STS_BUF3          0x16
#define RDY_VALUE_STS_BUF4          0x17
#define RDY_VALUE_STS_BUF5          0x18
#define RDY_VALUE_STS_BUF6          0x19
#define RDY_VALUE_STS_BUF7          0x1A
#define RDY_VALUE_STS_BUF8          0x1B
#define RDY_VALUE_STS_BUF9          0x1C
#define RDY_VALUE_STS_BUF10         0x1D
#define RDY_VALUE_STS_BUF11         0x1E
#define RDY_VALUE_STS_BUF12         0x1F
#define RDY_VALUE_STS_BUF13         0x20
#define RDY_VALUE_STS_BUF14         0x21
#define RDY_VALUE_STS_BUF15         0x22
#define RDY_VALUE_SET_STS_EN        0x23
#define RDY_VALUE_BUSY_STS          0x24 //bulk in
#define RDY_VALUE_RESETCMD          0x25
#define RDY_VALUE_BUS_ANA_STS       0x26
#define RDY_VALUE_SET_BSY_RDY       0x27 //bulk in
#define RDY_VALUE_BADCOL_BASE_ADR   0x28

#define RDY_PG_OFS_G_BAD_COL_EN 0x2F
#define RDY_PG_OFS_BUS_ANA_EN       0x30
#define RDY_SW_LT_MODE_EN               0x31
#define RDY_SW_LT_MODE                  0x32
#define SW_LG_MODE                          0x00
#define SW_TG_MODE                          0x01
#define SW_ONFI_MODE                        0x02
#define RDY_IS_PRG_CMD                  0x33
#define RDY_RESET_PBA                       0x34


#define RDY_ERROR_STATE                 0x40 //bulk in
#define RDY_TRUE_RDY_GAP_H          0x41
#define RDY_TRUE_RDY_GAP_L          0x42

#define RDY_PG_OFS_STS_BUF_GET_FEA_0    0x4B
#define RDY_PG_OFS_STS_VAL_GET_FEA_0    0x4C
#define RDY_PG_OFS_STS_BUF_GET_FEA_1    0x4D
#define RDY_PG_OFS_STS_VAL_GET_FEA_1    0x4E
#define RDY_PG_OFS_STS_BUF_GET_FEA_2    0x4F
#define RDY_PG_OFS_STS_VAL_GET_FEA_2    0x50
#define RDY_PG_OFS_STS_BUF_GET_FEA_3    0x51
#define RDY_PG_OFS_STS_VAL_GET_FEA_3    0x52
#define RDY_PG_ONCE_SET_STS_PRPT            0x53
#define RDY_PG_RPTR_LD                              0x54
#define RDY_GET_FEATURE_BYTE1                   0x55
#define RDY_GET_FEATURE_BYTE2                   0x56
#define RDY_GET_FEATURE_BYTE3                   0x57
#define RDY_PG_OFS_STS_BUF_WP                   0x58
#define RDY_PG_OFS_STS_VAL_WP                   0x59
#define WP_FLAG_VALUE                   SET_BIT0
#define RDY_RESULT                                      0x1FF
#define RDY_PG_PASS                     0x00
#define RDY_PG_FAIL                     0x01

#define STS_BUFFER_RDY                  BIT5
*/

//==================================================================================================================

//Set Flash ID vendor command
#define FLASH_ID_HD0                        0x47 //'I'
#define FLASH_ID_HD1                        0x44 //'D'
#define FLASH_ID_OFS_HD0                0x00
#define FLASH_ID_OFS_HD1                0x01
#define FLASH_ID_OFS_ID0                0x02
#define FLASH_ID_OFS_ID1                0x03
#define FLASH_ID_OFS_ID2                0x04
#define FLASH_ID_OFS_ID3                0x05
#define FLASH_ID_OFS_ID4                0x06
#define FLASH_ID_OFS_ID5                0x07
#define FLASH_ID_OFS_ID6                0x08
#define FLASH_ID_OFS_ID7                0x09


#define MAX_STS_BUF_NO                  0x80

#define CFDE_31_23_EOC                  0x28

#define MAX_INSTRUCTION_CNT            (128)
//#define NAX_INSTRUCTION_NUM            (MAX_INSTRUCTION_CNT - 1)

#define VC_LEAGCY_MODE                 (0x00)
#define VC_TOGGLE_MODE                 (0x01)

//==================================================================================================================

#define MAX_BYTE_NO_PBA_CACHE_RAM       0x8000

//==================================================================================================================

/*--------------------------------------------------------------------------*/
/* MACRO DEFINITION                                                         */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/* TYPE DEFINITION                                                          */
/*--------------------------------------------------------------------------*/
enum VC_TYPE_ISP {
    VC_ISP_FW                  = 0xB1,
    VC_VERIFY_FW               = 0xB2,
    VC_JUMP_PRAM               = 0xB3
};


/*--------------------------------------------------------------------------*/
/* GLOBAL VARIABLES DEFINITION                                              */
/*--------------------------------------------------------------------------*/
EXTERN volatile unsigned char gi_DoCSW, NE_VC_Proc;
EXTERN volatile unsigned char result_set_ready;
//EXTERN U8 set_rdy_pos = 0, blk_out0_in1 = 0;
EXTERN U8 set_rdy_pos, blk_out0_in1;
EXTERN volatile unsigned char Polling_Cmd, show_CFDE, B6_End, EP2_R_PTR;
EXTERN volatile unsigned char B6_Cmd, B9_Cmd, B6_Proc, B9_Proc;
EXTERN volatile unsigned char once_set_sts;
//EXTERN xdata int gb_ErrorHappenCnt = 0;
EXTERN xdata int gb_ErrorHappenCnt;
EXTERN volatile unsigned int gw_truerdy_gap;

//EXTERN volatile unsigned int gb_T1MS_CFDE; //must recheck jeff shu
EXTERN xdata unsigned int gb_T1MS_CFDE; //must recheck jeff shu

/*--------------------------------------------------------------------------*/
/* GLOBAL FUNCTION DECLARATION                                              */
/*--------------------------------------------------------------------------*/
EXTERN void pba_spi_isp(void);
EXTERN void pba_vendor_Cmd_isp(void);
//EXTERN void pba_vendor_Cmd(void);
EXTERN void pba_vc_cmd_dispatch(void);
/*
EXTERN void VendorCmd_Tester_NE_BlkOut(void);
EXTERN void VendorCmd_Tester_NE_BlkIn(void);
EXTERN void VendorCmd_Tester_NE_BlkIn2(void);
EXTERN void VendorCmd_Tester_NE3(void);
*/
#undef EXTERN
#endif // __PBA_VC_API_H__
