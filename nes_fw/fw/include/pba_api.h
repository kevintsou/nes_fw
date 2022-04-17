/**
 * @file pba_api.h
 * Copyrihgt 2015 PHISON Electronics Corp.
 *
 * <b>
@verbatim
FILE NAME :  pba_api.h
PURPOSE   :  Header file for pba.c
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
#ifndef __PBA_API_H__
#define __PBA_API_H__
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __PBA_C__
#define EXTERN
#else
#define EXTERN extern
#endif

#include "nes.h"

/*--------------------------------------------------------------------------*/
/* PREDEFINED DIRECTIVE                                                     */
/*--------------------------------------------------------------------------*/
//#define EN_PS3432    (1)
#define EN_PBA_V15   (0)
#define EN_PBA_V20   (1)

#define CE_MODE      (3) //0:X1,1:X2,2:X4,3:X8      
#define LUN_MODE     (1) //0:X1,1:X2,2:X4,3:X8 
#define PLANE_MODE   (2) //0:X1,1:X2,2:X4,3:X8 

#define MAX_CE_CNT      (1 << CE_MODE)
#define MAX_LUN_CNT     (1 << LUN_MODE)
#define MAX_PLANE_CNT   (1 << PLANE_MODE)

#define LUN_CNT_CE     (8)
#define PLANE_CNT_LUN  (8) 

#define MAX_TAG_GP     0x0D
#define CACHE_BUF_CNT      (64)

/*--------------------------------------------------------------------------*/
/* INCLUDE FILE                                                             */
/*--------------------------------------------------------------------------*/
//#include "USB_API.h"
#include "Var_def.h"

/*--------------------------------------------------------------------------*/
/* CONSTANT DECLARATION                                                     */
/*--------------------------------------------------------------------------*/
#define PBAREG ((volatile unsigned char *)0x2F000UL)


#define PBAREG_CE0 ((volatile unsigned char *)0x2E800UL)

#define PBAREG_CE1 ((volatile unsigned char *)0x2E900UL)

#define PBAREG_CE2 ((volatile unsigned char *)0x2EA00UL)

#define PBAREG_CE3 ((volatile unsigned char *)0x2EB00UL)

#define PBAREG_CE4 ((volatile unsigned char *)0x2EC00UL)

#define PBAREG_CE5 ((volatile unsigned char *)0x2ED00UL)

#define PBAREG_CE6 ((volatile unsigned char *)0x2EE00UL)

#define PBAREG_CE7 ((volatile unsigned char *)0x2EF00UL)


/*
 * PBA IP V2.0 Register Definition for 2016/03/03 PBA spec
 * GLOBAL LEVEL
 */
#define CRBYTE0                             0x00
#define S_PBA_RST_SW                    SET_BIT7
#define PBA_CALI_FORCE_MODE             SET_BIT6
#define PBA_CALI_MODE                   SET_BIT5
#define H_IO_OE                         SET_BIT2

#define SET_PBA_SW_RST()                (_setreg8(PBAREG[CRBYTE0], BIT7))

#define MASK_MODE                           0x9F
#define S_LEGACY_MODE                   (0x00<<5)
#define S_TOGGLE_MODE                   (0x01<<5)
#define S_ONFI_MODE                     (0x02<<5)
//#define S_RESV_MODE                   (0x03<<5)

#define IS_TOGGLE_MODE                  (0x20)

//#define H_IO_IE_POWER_EN            SET_BIT4
//#define H_IO_IE_POWER_DIS           CLR_BIT4
#define EN_AFIFO_RST                SET_BIT4
#define S_H_IO_IN_EN                SET_BIT3
#define C_H_IO_IN_DIS               CLR_BIT3
#define S_H_IO_OUT_EN               SET_BIT2
#define C_H_IO_OUT_DIS              CLR_BIT2
#define EN_PRELOAD_AFIFO_DATA       SET_BIT1
#define S_RE_OVF_VALUEFF            SET_BIT0
#define C_RE_OVF_VALUE00            CLR_BIT0

#define PWR_CHG_INT                         0x01
#define MASK_RSTN_BSY                    CLR_BIT4
#define CHG_INT_CLR                     SET_BIT1
#define CHG_INT_EN                      SET_BIT0
#define CHG_INT_DIS                     CLR_BIT0

#define H_IO_GPO                            0x02

#define PAGE_SIZE                           0x03
#define PAGE8K                              0x00
#define PAGE16K                             0x01
#define PAGE32K                             0x10

#define EN_ALL_CE_WP_LOCK                   0x04
#define EN_ALL_CE                   SET_BIT4
#define REF_CE_NUM                  CLR_BIT4
#define EN_LOCK_UNLOCK_FUN          SET_BIT1
#define DIS_LOCK_UNLOCK_FUN         CLR_BIT1
#define EN_WP_FUN                   SET_BIT0
#define DIS_WP_NUM                  CLR_BIT0


//==============================================================================
/*
0x006
  0 R/W 7   WR_CACHE_MODE_SEL   Write cache mode selection. PBA support at most 16 page buffers, so there are 2 cache modes to be provided for multi-page and FW selection modes. If WR_CACHE_MODE_SEL=0, the current used page buffer is decided by WR_CACHE_PTR. If WR_CACHE_MODE_SEL=1, the current used page buffer is decided by WR_CACHE_SEL_PAx which is on address 0x19~0x1B.
    0   R/W 6   FORCE_WR_CACHE_MODE_SEL Enable this bit the WR_CACHE_MODE_SEL register setting will not auto clear by HW.
    0   R/W 5:0 WR_CACHE_PTR    Write cache pointer indicator and configuration. If WR_CACHE_MODE_SEL=0, FW needs to specify the target page buffer to do the write data transfer.
*/
//==============================================================================
#define WR_CACHE                            0x06
#define HW_WR_CCMODE_SEL            SET_BIT7
#define SW_WR_CCMODE_SEL            CLR_BIT7
#define FORCE_WR_CACHE_MODE_SEL     SET_BIT6
#define MASK_WR_CCPTR                   0x0F

#define ADJ_CC_PTR                      0x01
#define PBA_CC_BUF0                     0x00
#define PBA_CC_BUF1                     0x01
#define PBA_CC_BUF2                     0x02
#define PBA_CC_BUF3                     0x03
#define PBA_CC_BUF4                     0x04
#define PBA_CC_BUF5                     0x05
#define PBA_CC_BUF6                     0x06
#define PBA_CC_BUF7                     0x07
#define PBA_CC_BUF8                     0x08
#define PBA_CC_BUF9                     0x09
#define PBA_CC_BUF10                    0x0A
#define PBA_CC_BUF11                    0x0B
#define PBA_CC_BUF12                    0x0C
#define PBA_CC_BUF13                    0x0D
#define PBA_CC_BUF14                    0x0E
#define PBA_CC_BUF15                    0x0F

//==============================================================================
/*
  0x007 0   R/W 5:4 CE_NUMBER   CE definition:
                  2'b00: CE0
                  2'b01: CE0,CE1
                  2'b10: CE0,CE1,CE2,CE3
                  2'b11: CE0,CE1,CE2,CE3,CE4,CE5,CE6,CE7
          0 R/W 3:2 LUN_NUMBER  LUN definition:
                  2'b00: LUN0
                  2'b01: LUN0,LUN1
                  2'b10: LUN0,LUN1,LUN2,LUN3
                  2'b11: LUN0,LUN1,LUN2,LUN3,LUN4,LUN5,LUN6,LUN7
          0 R/W 1:0 PLANE_NUMBER    PLANE definition:
                  2'b00: PLANE0
                  2'b01: PLANE0,PLANE1
                  2'b10: PLANE0,PLANE1,PLANE2,PLANE3
                  2'b11: PLANE0,PLANE1,PLANE2,PLANE3,PLANE4,PLANE5,PLANE6,PLANE7
*/
//==============================================================================
#define SET_CE_LUN_PLANE                    0x07
#define SET_CE_LUN_PLANE_MASK               0xC0

#define ONECE_MODE                         (0x00<<4)
#define TWOCE_MODE                         (0x01<<4)
#define FOURCE_MODE                        (0x02<<4)
#define EIGHTCE_MODE                       (0x03<<4)

#define ONELUN_MODE                        (0x00<<2)
#define TWOLUN_MODE                        (0x01<<2)
#define FOURLUN_MODE                       (0x02<<2)
#define EIGHTLUN_MODE                      (0x03<<2)

#define ONEPLANE_MODE                      (0x00)
#define TWOPLANE_MODE                      (0x01)
#define FOURPLANE_MODE                     (0x02)
#define EIGHTPLANE_MODE                    (0x03)


//==============================================================================
/*
 0x00A
  0 R/W 7   RD__CACHE_MODE_SEL  Read cache mode selection. PBA support at most 16 page buffers, so there are 2 cache modes to be provided for multi-page and FW selection modes. If RD_CACHE_MODE_SEL=0, the current used page buffer is decided by RD_CACHE_PTR. If RD_CACHE_MODE_SEL=1, the current used page buffer is decided by RD_CACHE_SEL_PAx which is on address 0x19~0x1B.
    0   R/W 6   FORCE_RD_CACHE_MODE_SEL Enable this bit the RD_CACHE_MODE_SEL register setting will not auto clear by HW.
    0   R/W 5:0 RD_CACHE_PTR    Read cache pointer indicator and configuration. If RD_CACHE_MODE_SEL=0, FW needs to specify the target page buffer to do the read data transfer.
*/
//==============================================================================
#define RD_CACHE                            0x0A
#define HW_RD_CCMODE_SEL            SET_BIT7  // this register is no longer useful
#define SW_RD_CCMODE_SEL            CLR_BIT7
#define FORCE_RD_CACHE_MODE_SEL     SET_BIT6
#define MASK_RD_CCPTR                   0x0F

//==============================================================================
/*
0x00B
  0 R   7:5 Reserved
    0   RW  4   CLR_CFDE_TAG_ALL    Assert when CFDE special event occur for corresponding tags. All tags are cleared when CLR_CFDE_TAG_ALL(optional) or HW auto to clear which setting cache_ptr_inc of CFDE format bit 4 =1.It's used for cache header information and check CFDE tag from cache header information.
    0   RW  3   CTL_CLR_RR_CACHE_OFFSET_PTR Clear the Round Robin Cache offset point(auto clear)
          '1' Clear
          '0' NOP
    0   RW  2   CTL_CLR_CACHE_ERR_FLAG  Clear the Cache Error Flag when data transfer overflow PBA Cache
    0   RW  1   CTL_CACHE_RR    '1' : Round Robin process to assign the W/R cache ptr
          '0' : the FW to assign or HW address to assign the W/R cache ptr
    0   RW  0   CTL_ADD_BadCol_Cache    '1' : PBA can used cache buffer extend to 64 units
          '0' : PBA can used cache buffer 32 units
*/
//==============================================================================
#define RR_CACHE                            0x0B
#define SET_CLR_CFDE_TAG_ALL             SET_BIT4
#define FORCE_CLR_CFDE_TAG_ALL_DEFAUL    CLR_BIT4
#define SET_CTL_RR_CACHE_OFFSET_PTR      SET_BIT3
#define SET_RR_MODE                      SET_BIT1
#define SET_FW_OR_HWADDR_MODE            CLR_BIT1
#define SET_64SETS_CACHE_BUF             SET_BIT0
#define SET_32SETS_CACHE_BUF             CLR_BIT0

//==============================================================================
/*
  0x00C 0   R   7:4 Reserved
          0 R   3   PWR_STAT    Show power status. 1: power on, 0: power off.
          0 R   2:0 OUT_SRC Indicate the data source of output data which is set by CFDE "command latch" which definition is shown as following:
          3'b000: from transaction data
          3'b001: from status register data
          3'b010: from ID register data
          3'b011: from status register data but its buffer pointer is determined by HW
          3'b100 : from block lock status register data.
*/
//==============================================================================
#define DATABUS_OUT_SRC                     0x0C
#define DATABUS_TRANS_DATA          0x00
#define DATABUS_STS_REG             0x01
#define DATABUS_ID_REG              0x02
#define DATABUS_STS_REG_HW          0x03
#define PWR_STATE                       BIT2
#define STATE_PWR_ON                    BIT2
#define STATE_PWR_OFF                   0x00


//==============================================================================
/*
  0x00D 0   R   7:6 Reserved
          0 R   5:0 CUR_RD_CACHE_PTR    Indicate the current cache buffer number no
                                    matter RD_CACHE_MODE_SEL is 0 or 1.
*/
//==============================================================================
#define CUR_RD_CACHE_PTR                    0x0D


//==============================================================================
/*
  0x00E 0   R   7:6 Reserved
          0 R   5:0 CUR_WR_CACHE_PTR    Indicate the current cache buffer number no
                                    matter WR_CACHE_MODE_SEL is 0 or 1.
*/
//==============================================================================
#define CUR_WR_CACHE_PTR                    0x0E



//==============================================================================
/*
  0x00F 0   R     7:6   Reserved
          0 R/W 5:0 CFDE_CACHE_BASE_PTR Configure the initial cache base pointer of Round Robin.

*/
//==============================================================================
#define RR_CACHE_BUF_BASE_PTR               0x0F



//==============================================================================
/*
  0x010 0   RW  7:0     CTL_CFDE_7_0_RESET  Reset CFDE N unit queue buffer state (auto clear)

*/
//==============================================================================
#define CTL_CFDE_7_0_RESET                  0x10


//==============================================================================
/*
  0x011 0   RW  7:0     CTL_CFDE_15_8_RESET Reset CFDE N unit queue buffer state (auto clear)

*/
//==============================================================================
#define CTL_CFDE_15_8_RESET                 0x11


//==============================================================================
/*
  0x012 0   RW  7:0     CTL_CFDE_23_16_RESET    Reset CFDE N unit queue buffer state (auto clear)

*/
//==============================================================================
#define CTL_CFDE_23_16_RESET                0x12


//==============================================================================
/*
  0x013 0   RW  7:0     CTL_CFDE_31_24_RESET    Reset CFDE N unit queue buffer state (auto clear)

*/
//==============================================================================
#define CTL_CFDE_31_24_RESET                0x13


//==============================================================================
/*
  0x014 0   RW  7:0     CTL_CFDE_39_32_RESET    Reset CFDE N unit queue buffer state (auto clear)

*/
//==============================================================================
#define CTL_CFDE_39_32_RESET                0x14


//==============================================================================
/*
  0x015 0   RW  7:0     CTL_CFDE_47_40_RESET    Reset CFDE N unit queue buffer state (auto clear)

*/
//==============================================================================
#define CTL_CFDE_47_40_RESET                0x15


//==============================================================================
/*
  0x016 0   RW  7:0     CTL_CFDE_55_48_RESET    Reset CFDE N unit queue buffer state (auto clear)

*/
//==============================================================================
#define CTL_CFDE_55_48_RESET                0x16


//==============================================================================
/*
  0x017 0   RW  7:0     CTL_CFDE_63_56_RESET    Reset CFDE N unit queue buffer state (auto clear)

*/
//==============================================================================
#define CTL_CFDE_63_56_RESET                0x17


//==============================================================================
/*
  0x018 0   RW  7:0     CTL_CFDE_71_64_RESET    Reset CFDE N unit queue buffer state (auto clear)

*/
//==============================================================================
#define CTL_CFDE_71_64_RESET                0x18


//==============================================================================
/*
  0x019 0   RW  7:0     CTL_CFDE_79_72_RESET    Reset CFDE N unit queue buffer state (auto clear)

*/
//==============================================================================
#define CTL_CFDE_79_72_RESET                0x19


//==============================================================================
/*
  0x01A 0   RW  7:0     CTL_CFDE_87_80_RESET    Reset CFDE N unit queue buffer state (auto clear)

*/
//==============================================================================
#define CTL_CFDE_87_80_RESET                0x1A


//==============================================================================
/*
  0x019 0   RW  7:0     CTL_CFDE_79_72_RESET    Reset CFDE N unit queue buffer state (auto clear)

*/
//==============================================================================
#define CTL_CFDE_79_72_RESET                0x19


//==============================================================================
/*
  0x01A 0   RW  7:0     CTL_CFDE_87_80_RESET    Reset CFDE N unit queue buffer state (auto clear)

*/
//==============================================================================
#define CTL_CFDE_87_80_RESET                0x1A


//==============================================================================
/*
  0x01B 0   RW  7:0     CTL_CFDE_95_88_RESET    Reset CFDE N unit queue buffer state (auto clear)

*/
//==============================================================================
#define CTL_CFDE_95_88_RESET                0x1B


//==============================================================================
/*
  0x01C 0   RW  7:0     CTL_CFDE_99_96_RESET    Reset CFDE N unit queue buffer state (auto clear)

*/
//==============================================================================
#define CTL_CFDE_99_96_RESET                0x1C


//==============================================================================
/*
  0x020 0   RW  5:0     CTL_PBA_CACHE_HEADER_SEL    Select which PBA CACHE HEADER
                                              (CACHE_HEADER0~ CACHE_HEADER63) DATA.
*/
//==============================================================================
#define CTL_PBA_CACHE_HEADER_SEL            0x20

#define DQSCLK_DLY_COAR                     0x22
#define DQSCLK_DLY_FINE_7_1                 0x23


//==============================================================================
/*
  0x024~0x2B 0   R   7:0 CTL_PBA_CACHE_HEADER_DAT0~7   PBA CACHE HEADER Bytes 0~7 DATA.
*/
//==============================================================================
#define CTL_PBA_CACHE_HEADER_DAT0           0x24
#define CTL_PBA_CACHE_HEADER_DAT1           0x25
#define CTL_PBA_CACHE_HEADER_DAT2           0x26
#define CTL_PBA_CACHE_HEADER_DAT3           0x27
#define CTL_PBA_CACHE_HEADER_DAT4           0x28
#define CTL_PBA_CACHE_HEADER_DAT5           0x29
#define CTL_PBA_CACHE_HEADER_DAT6           0x2A
#define CTL_PBA_CACHE_HEADER_DAT7           0x2B


//==============================================================================
/*
  0x030 0   RW  6:0 CEB_DLY_COARS_22_16 Configure programmable 128-stage delay buffer for CEB delay wire.
*/
//==============================================================================
#define CEB_DLY_COARS_22_16                 0x30

#define CEB_DLY_COARS_15_8                  0x31

#define CEB_DLY_FINE_7_1                    0x32

#define REB_DLY_COARS_22_16                 0x33

#define REB_DLY_COARS_15_8                  0x34

#define REB_DLY_FINE_7_1                    0x35

#define DQSCLK1_DLY_COARS_22_16             0x36

#define DQSCLK1_DLY_COARS_15_8              0x37

#define DQSCLK1_DLY_FINE_7_1                0x38

#define DQSCLK2_DLY_COARS_22_16             0x39

#define DQSCLK2_DLY_COARS_15_8              0x3A

#define DQSCLK2_DLY_FINE_7_1                0x3B

#define HIO_OE_DLY_COARS_22_16              0x3C

#define HIO_OE_DLY_COARS_15_8               0x3D

#define HIO_OE_DLY_FINE_7_1                 0x3E


//==============================================================================
/*
  0x040 0   RW  7:0 CFDE_CMDQ_47_40 Command sequence configuration entry.
  0x041 0   RW  7:0 CFDE_CMDQ_39_32 After CFDE_SEL specifies a CFDE unit, FW needs to
  0x042 0   RW  7:0 CFDE_CMDQ_31_24 write the command sequence into the SRAM of the
  0x043 0   RW  7:0 CFDE_CMDQ_23_16 command queue through writing command sets into
  0x044 0   RW  7:0 CFDE_CMDQ_15_8  CFDE_CMDQ_47_40, CFDE_CMDQ_39_32,CFDE_CMDQ_31_24,
  0x045 0   RW  7:0 CFDE_CMDQ_7_0   CFDE_CMDQ_23_16, CFDE_CMDQ_15_8, and CFDE_CMDQ_7_0.
                                  The write pointer of SRAM is increased after all
                                  of CFDE_CMDQ_47_40, CFDE_CMDQ_39_32,CFDE_CMDQ_31_24,
                                  CFDE_CMDQ_23_16, CFDE_CMDQ_15_8, and CFDE_CMDQ_7_0
                                  registers are written for 1 time in order.
*/
//==============================================================================
//CFDE OP Code
#define OP_CMD_LATCH                0x01
#define OP_ADR0_LATCH               0x02
#define OP_ADR1_LATCH               0x03
#define OP_ADR2_LATCH               0x04
#define OP_ADR3_LATCH               0x05
#define OP_ADR4_LATCH               0x06
#define OP_ADR5_LATCH               0x07
#define OP_ADR6_LATCH               0x08
#define OP_ADR7_LATCH               0x09
#define OP_ADR8_LATCH               0x0A
#define OP_ADR9_LATCH               0x0B
#define OP_ID_ADR_LATCH             0x0C
#define OP_ID_VALUE_READ            0x0D
#define OP_WDATA_DDR                0x0E
#define OP_RDATA_DDR                0x0F
#define OP_WDATA_SDR                0x10
#define OP_RDATA_SDR                0x11
#define OP_STS_READ                 0x12
#define OP_SG_WDATA_LATCH           0x13
#define OP_EOC                      0x14

#define TRANS_DATA                  0x00
#define STS_REG                     0x01
#define ID_REG                      0x02
#define STS_REG_HW                  0x03 //for cmd78/enhance cmd71
#define STS_REG_BLK_LOCK            0x04 //for block lock
#define DPS_SRAM_DATA               0x05 //from DPS sram data

//Address buffer define
enum ADDR_BUF {
    ADR_BUFNO_0 = 0,
    ADR_BUFNO_1,
    ADR_BUFNO_2,
    ADR_BUFNO_3,
    ADR_BUFNO_4,
    ADR_BUFNO_5,
    ADR_BUFNO_6,
    ADR_BUFNO_7,
    ADR_BUFNO_8,
    ADR_BUFNO_9,
    ADR_BUFNO_10,
    ADR_BUFNO_11,
    ADR_BUFNO_12,
    ADR_BUFNO_13,
    ADR_BUFNO_14,
    ADR_BUFNO_15
};

/*
#define STS_BUFNO_ENH_78CMD        (0x00)

#define STS_BUFNO_ENH_71CMD        (0x40)
#define ENH_71CMD_SIZE             (0x08)

#define STS_BUFNO_70CMD        (STS_BUFNO_ENH_71CMD + ENH_71CMD_SIZE)
#define STS_BUFNO_71CMD        (STS_BUFNO_70CMD + 1)
#define STS_BUFNO_73CMD        (0x5F)
#define STS_BUFNO_F2CMD        (0x7F)
#define STS_BUFNO_F1CMD        (STS_BUFNO_F2CMD - 1)
*/
enum STS_BUFNO {
     //Cmd78 
    STS_BUFNO_ENH_78CMD = 0x00,

    //Enhance Cmd71
    STS_BUFNO_ENH_71CMD = 0x40,

    //F1~F8
    //LUN0(0x48~0x4C)
    STS_BUFNO_F1CMD     = 0x48,

    //LUN1(0x4D~0x51)
    STS_BUFNO_F2CMD     = 0x4D, 

    //LUN2(0x52~0x56)
    STS_BUFNO_F3CMD     = 0x52, 

    //LUN3(0x57~0x5B)
    STS_BUFNO_F4CMD     = 0x57,

    //LUN4(0x5C~0x60)
    STS_BUFNO_F5CMD     = 0x5C,

    //LUN5(0x61~0x65)
    STS_BUFNO_F6CMD     = 0x61,

    //LUN6(0x66~0x6A)
    STS_BUFNO_F7CMD     = 0x66,

    //LUN7(0x6B~0x6F)
    STS_BUFNO_F8CMD     = 0x6B,

    //MPL status (0x6C/0x6D)
    STS_BUFNO_6CCMD     = 0x6C,
    STS_BUFNO_6BCMD     = 0x6D,

    //Global Cmd 7x(0x70~0x7F)
    STS_BUFNO_70CMD     = 0x70,
    STS_BUFNO_71CMD     = 0x71,
    STS_BUFNO_72CMD     = 0x72,
    STS_BUFNO_73CMD     = 0x73,
    STS_BUFNO_74CMD     = 0x74,
    STS_BUFNO_75CMD     = 0x75,
    STS_BUFNO_76CMD     = 0x76,
    STS_BUFNO_77CMD     = 0x77,
    STS_BUFNO_7C_CMD     = 0x7C,
};

#define CFDE_CMDQ_47_40                     0x40
#define DPS_EN                     SET_BIT7
#define STA_ENH_EN                 SET_BIT6
#define AFIFO_RST                  SET_BIT5
#define AFIFO_PREWRITE             SET_BIT4
#define LOCK_TIGHT                 SET_BIT3
#define LOCK                       SET_BIT2
#define CFDE_SET_BSY7              SET_BIT0
#define CFDE_SET_BSY8              SET_BIT1

#define CFDE_CMDQ_39_32                     0x41
//CFDE W/R Tag
#define CACHE_WRITE_TAG            SET_BIT0
#define CACHE_READ_TAG             SET_BIT1
//CFDE SET BUSY
#define CFDE_SET_BSY1              SET_BIT2
#define CFDE_SET_BSY2              SET_BIT3
#define CFDE_SET_BSY3              SET_BIT4
#define CFDE_SET_BSY4              SET_BIT5
#define CFDE_SET_BSY5              SET_BIT6
#define CFDE_SET_BSY6              SET_BIT7


//Set Ready Busy
#define SET_READY    0x01
#define SET_BUSY     0x00

enum SUB_BSY_TYPE {
    TYPE_SUB_BSY1 = 0x01,
    TYPE_SUB_BSY2,
    TYPE_SUB_BSY3,
    TYPE_SUB_BSY4,
    TYPE_SUB_BSY5,
    TYPE_SUB_BSY6,
    TYPE_SUB_BSY7,
    TYPE_SUB_BSY8,
};


enum CE_T
{
     RDY_CE_0 = 0,
     RDY_CE_1 ,
     RDY_CE_2 ,
     RDY_CE_3 ,
     RDY_CE_4 ,
     RDY_CE_5 ,
     RDY_CE_6 ,
     RDY_CE_7 ,
     RDY_ALL_CE
};

enum LUN_T
{
	RDY_LUN_0 = 0,
	RDY_LUN_1 ,
	RDY_LUN_2 ,
	RDY_LUN_3 ,
	RDY_LUN_4 ,
	RDY_LUN_5 ,
	RDY_LUN_6 ,
	RDY_LUN_7 ,
	RDY_ALL_LUN
};

enum BSY_T
{
	RDY_BSY_1 = 1,
	RDY_BSY_2 ,
	RDY_BSY_3 ,
	RDY_BSY_4 ,
	RDY_BSY_5 ,
	RDY_BSY_6 ,
	RDY_BSY_7 ,
	RDY_BSY_8 ,
	RDY_ALL_BSY
};



#define CFDE_CMDQ_31_24                     0x42
#define SPECIAL_TAG                         SET_BIT7
#define DATA_LEN_H(x)                       (unsigned char)((x)>>15)
#define CMD_SEQ_WORD_H(x)                   (unsigned char)((x)>>7)
#define OP_CODE(x)                          (unsigned char)((x)<<1)
#define MASK_LATCH_ADDR_EN                  SET_BIT0

#define CFDE_CMDQ_23_16                     0x43
#define DATA_LEN_M(x)                       (unsigned char)((x)>>7)
#define CMD_SEQ_WORD_L(x)                   (unsigned char)((x)<<1)
#define ADR_BUF_NO(x)                       (unsigned char)((x)&0x1F)
#define STS_BUF_NO_H(x)                     (unsigned char)((x)>>2)
#define DATA_BUF_NO_H(x)                    (unsigned char)((x)>>2)
#define SINGLE_DATA_ADDR_EN                 SET_BIT7
#define LATCH_LOCK_ADDR_EN                  SET_BIT6
#define LATCH_ADDR_EN                       SET_BIT5
#define STA_INCR                            SET_BIT0

#define CFDE_CMDQ_15_8                      0x44
#define DATA_LEN_L(x)                       (unsigned char)((x)<<1)
#define STS_BUF_NO_L(x)                     (unsigned char)((x)<<6)
#define DATA_BUF_NO_L(x)                    (unsigned char)((x)<<6)
//#define OUT_SEL(x)                          (unsigned char)(x<<1)
#define OUT_SEL(x)                          (unsigned char)(x)
#define SG_WDATA_LEN(x)                     (unsigned char)(x&0x3F)
#define HW_ADDR                             SET_BIT7
#define WR_DIR                              SET_BIT6
#define WR_CHG_COL                          SET_BIT5
#define RD_CHG_COL                          SET_BIT4
#define RD_DIR                              SET_BIT3
#define RST_ALL                             SET_BIT2
#define RST_LOCAL                           SET_BIT1


#define CFDE_CMDQ_7_0                       0x45
#define SET_BAD_COLUMN                      SET_BIT7
#define BYPASS_HWADDR                       SET_BIT6
#define HW_RST                              SET_BIT5
#define CACHE_PTR_INC                       SET_BIT4
#define LATCH_ADDRESS_END                   SET_BIT2



//this CFDE INT assert category not implement by HW
//Bit0~3
#define NO_INT_ASSERT                       0x00
#define PRG_INT_ASSERT                      0x01
#define READ_INT_ASSERT                     0x02
#define ERASE_INT_ASSERT                    0x03
#define W_CHG_COL_INT_ASSERT                0x04
#define R_CHG_COL_INT_ASSERT                0x05
#define RESV_INT_ASSERT                     0x06
#define OTHER_INT_ASSERT                    0x07


//==============================================================================
/*
  0x046 0   R   7:0 CFDE_CUR_CMDQ_PTR   After selecting specific CFDE unit by setting
                                        CFDE_SEL, FW read this address to know the
                                        current command queue pointer in the current CFDE unit.
*/
//==============================================================================
#define CFDE_CUR_CMDQ_PTR                  0x46


//==============================================================================
/*
  0x047 0   RW  7:0 CFDE_SEL    Select the current configure the number of the CFDE unit. CFDE unit is shown in Figure 3.4.
                            CFDE_SEL range: 0~99
*/
//==============================================================================
#define CFDE_SEL                           0x47


//==============================================================================
/*
  0x04C 0   RW  1   CTL_ADR_BFU_SRAM_ADDR_SEL   '0': choose Address buffer SRAM address from ADR_BUF_SRAM_ADDR_9_8 and ADR_BUF_SRAM_ADDR_7_0 by FW
                                          '1': choose Address buffer SRAM address from cache19 address by HW
*/
//==============================================================================
#define CTL_ADR_BFU_SRAM_ADDR_SEL          0x4C

#define CTL_ADR_BUF_SRAM_ADDR_7_0          0x4D

#define CTL_ADR_BUF_SRAM_ADDR_9_8          0x4E

//#define ADR_BUF_NO_SEL                   0x51
#define ADR9_BUF0                          0x52
#define ADR8_BUF0                          0x53
#define ADR7_BUF0                          0x54
#define ADR6_BUF0                          0x55
#define ADR5_BUF0                          0x56
#define ADR4_BUF0                          0x57
#define ADR3_BUF0                          0x58
#define ADR2_BUF0                          0x59
#define ADR1_BUF0                          0x5A
#define ADR0_BUF0                          0x5B

#define CFDE_EN_7_0                        0x60
#define CFDE_EN_15_8                       0x61
#define CFDE_EN_23_16                      0x62
#define CFDE_EN_31_24                      0x63
#define CFDE_EN_39_32                      0x64
#define CFDE_EN_47_40                      0x65
#define CFDE_EN_55_48                      0x66
#define CFDE_EN_63_56                      0x67
#define CFDE_EN_71_64                      0x68
#define CFDE_EN_79_72                      0x69
#define CFDE_EN_87_80                      0x6A
#define CFDE_EN_95_88                      0x6B
#define CFDE_EN_99_96                      0x6C

#define BUS_ANA_INT_STS                    0x7C
#define INT_CC_0                            BIT4
#define INT_CC_1                            BIT5
#define INT_ERR                             BIT6
#define INT_PWR_CHG                         BIT7
#define MASK_INT                            0x70

#define PBA_INT_EN                         0x7E
#define INT2_EN                             SET_BIT0
#define INT3_EN                             SET_BIT1
#define INT4_EN                             SET_BIT2
#define INT5_EN                             SET_BIT3
#define INT2_DIS                            CLR_BIT0
#define INT3_DIS                            CLR_BIT1
#define INT4_DIS                            CLR_BIT2
#define INT5_DIS                            CLR_BIT3
#define EN_BUS_ANA_STACK_MODE               SET_BIT4
#define DIS_BUS_ANA_STACK_MODE              CLR_BIT4
//#define ALL_DIS                             0x00

#define CLEAR_BUS_ANA_INT                    0x7F
//#define FLAG_CLEAR_BAD_COL                 SET_BIT2
#define FLAG_CLEAR_BUS_ANA                  SET_BIT2
//#define FLAG_CLEAR_INT                     SET_BIT0

//==============================================================================
/*
  0x090 0   RW  7:0 CLR_CACHE_7_0_HEADER    Clear the Cache Header information when MCU
                                        read the data from PBA Cache0~ Cache7 finish.
                                        (auto clear)rom cache19 address by HW
*/
//==============================================================================
#define CLR_CACHE_7_0_HEADER               0x90

#define CLR_CACHE_15_8_HEADER              0x91

#define CLR_CACHE_23_16_HEADER             0x92

#define CLR_CACHE_31_24_HEADER             0x93

#define CLR_CACHE_39_32_HEADER             0x94

#define CLR_CACHE_47_40_HEADER             0x95

#define CLR_CACHE_55_48_HEADER             0x96

#define CLR_CACHE_63_56_HEADER             0x97


//==============================================================================
/*
  0x098 0   R   7:0 CFDE_CMDQ_RD_47_40  After selecting specific CFDE unit by setting
                                    CFDE_SEL, FW can read the CFDE instructions from
                                    address 0x9c~0x9f which instruction location is
                                    assigned by CFDE_CMDQ_RDPTR.
*/
//==============================================================================
#define CFDE_CMDQ_RD_47_40                 0x98
#define CFDE_CMDQ_RD_39_32                 0x99
#define CFDE_CMDQ_RDPTR                    0x9B
#define CFDE_CMDQ_RD_31_24                 0x9C
#define CFDE_CMDQ_RD_23_16                 0x9D
#define CFDE_CMDQ_RD_15_8                  0x9E
#define CFDE_CMDQ_RD_7_0                   0x9F


//==============================================================================
/*
  0x0A0 0   RW  4   bus_ana_err_int_en  Enable interrupt "bus_ana_err" at address 0x7C.
          0 RW  3   bus_ana_cache_int1_en   Enable interrupt "bus_ana_flag1" at address 0x7C.
          0 RW  2   bus_ana_cache_int0_en   Enable interrupt "bus_ana_flag0" at address 0x7C.
          0 RW  1   bus_ana_cache_swap  Manual swap the ping-pong cache for bus analyzer.
          0 RW  0   bus_ana_en  Enable bus analyzer function.
*/
//==============================================================================
#define BUS_ANA_EN                         0xA0
#define ANA_EN                              BIT0
#define CC_MENU_SWAP_EN                     BIT1
#define CC_INT0_EN                          BIT2
#define CC_INT1_EN                          BIT3
#define ERR_INT_EN                          BIT4

#define BUS_ANA_THRE_L                     0xA1
#define BUS_ANA_THRE_H                     0xA2
#define BUS_ANA_CC_STS                     0xA3
#define BUS_ANA_NOW_THRE_L                 0xA4
#define BUS_ANA_NOW_THRE_H                 0xA5

#define BUS_ANA_STS                        0xA6
#define ERR_STS                             BIT2
#define DONE1                               BIT1
#define DONE0                               BIT0

//==============================================================================
/*
  0x0A8 0   R   7:0 cache_valid_flag_7_0    PBA Cache valid from Cache0~ Cache7
  0x0A9 0   R   7:0 cache_valid_flag_15_8   PBA Cache valid from Cache8~ Cache15
  0x0AA 0   R   7:0 cache_valid_flag_23_16  PBA Cache valid from Cache16~ Cache23
  0x0AB 0   R   7:0 cache_valid_flag_31_24  PBA Cache valid from Cache24~ Cache31
  0x0AC 0   R   7:0 cache_valid_flag_39_32  PBA Cache valid from Cache32~ Cache39
  0x0AD 0   R   7:0 cache_valid_flag_47_40  PBA Cache valid from Cache40~ Cache47
  0x0AE 0   R   7:0 cache_valid_flag_55_48  PBA Cache valid from Cache48~ Cache55
  0x0AF 0   R   7:0 cache_valid_flag_63_56  PBA Cache valid from Cache56~ Cache63
*/
//==============================================================================
#define CACHE_VAILD_FLAG_7_0               0xA8
#define CACHE_VAILD_FLAG_15_8              0xA9
#define CACHE_VAILD_FLAG_23_16             0xAA
#define CACHE_VAILD_FLAG_31_24             0xAB
#define CACHE_VAILD_FLAG_39_32             0xAC
#define CACHE_VAILD_FLAG_47_40             0xAD
#define CACHE_VAILD_FLAG_55_48             0xAE
#define CACHE_VAILD_FLAG_63_56             0xAF


//==============================================================================
/*
 0x0B0  0   RW  7:0 BUS_ANA_CMD1
 0x0B1  0   RW  7:0 BUS_ANA_CMD2
 0x0B2  0   RW  7:0 BUS_ANA_CMD3
 0x0B3  0   RW  7:0 BUS_ANA_ADDR1_CMD1
 0x0B4  0   RW  7:0 BUS_ANA_ADDR2_CMD1
 0x0B5  0   RW  7:0 BUS_ANA_ADDR3_CMD1
 0x0B6  0   RW  7:0 BUS_ANA_ADDR4_CMD1
 0x0B7  0   RW  7:0 BUS_ANA_ADDR5_CMD1
 0x0B8  0   RW  7:0 BUS_ANA_ADDR1_CMD2
 0x0B9  0   RW  7:0 BUS_ANA_ADDR2_CMD2
 0x0BA  0   RW  7:0 BUS_ANA_ADDR3_CMD2
 0x0BB  0   RW  7:0 BUS_ANA_ADDR4_CMD2
 0x0BC  0   RW  7:0 BUS_ANA_ADDR5_CMD2
*/
//==============================================================================
#define BUS_ANA_CMD1                      0xB0
#define BUS_ANA_CMD2                      0xB1
#define BUS_ANA_CMD3                      0xB2
#define BUS_ANA_ADDR1_CMD1                0xB3
#define BUS_ANA_ADDR2_CMD1                0xB4
#define BUS_ANA_ADDR3_CMD1                0xB5
#define BUS_ANA_ADDR4_CMD1                0xB6
#define BUS_ANA_ADDR5_CMD1                0xB7
#define BUS_ANA_ADDR1_CMD2                0xB8
#define BUS_ANA_ADDR2_CMD2                0xB9
#define BUS_ANA_ADDR3_CMD2                0xBA
#define BUS_ANA_ADDR4_CMD2                0xBB
#define BUS_ANA_ADDR5_CMD2                0xBC


//==============================================================================
/*
 0x0E0  0   R   7   BSY_O_CE7   Indicate the busy status of CE7. 1: ready, 0: busy
          0 R   6   BSY_O_CE6   Indicate the busy status of CE6. 1: ready, 0: busy
          0 R   5   BSY_O_CE5   Indicate the busy status of CE5. 1: ready, 0: busy
          0 R   4   BSY_O_CE4   Indicate the busy status of CE4. 1: ready, 0: busy
          0 R   3   BSY_O_CE3   Indicate the busy status of CE3. 1: ready, 0: busy
          0 R   2   BSY_O_CE2   Indicate the busy status of CE2. 1: ready, 0: busy
          0 R   1   BSY_O_CE1   Indicate the busy status of CE1. 1: ready, 0: busy
          0 R   0   BSY_O_CE0   Indicate the busy status of CE0. 1: ready, 0: busy
 0XE1   0   R   7:1 reserved
          0 R   0   BSY_O   Indicate the busy status 1: ready, 0: busy. BSY_O = BSY_O_CE0 & BSY_O_CE1 & BSY_O_CE2 & BSY_O_CE3 & BSY_O_CE4 & BSY_O_CE5 & BSY_O_CE6 & BSY_O_CE7

*/
//==============================================================================
#define BSY_O_CE7_0                       0xE0
#define BSY_O_ALL                         0xE1

#define CTL_PBA_CACHE_HEADER_DAT8           0xE8
#define CTL_PBA_CACHE_HEADER_DAT9           0xE9
#define CTL_PBA_CACHE_HEADER_DAT10           0xEA
#define CTL_PBA_CACHE_HEADER_DAT11           0xEB
#define CTL_PBA_CACHE_HEADER_DAT12           0xEC
#define CTL_PBA_CACHE_HEADER_DAT13           0xED
#define CTL_PBA_CACHE_HEADER_DAT14           0xEE
#define CTL_PBA_CACHE_HEADER_DAA15           0xEF

//==============================================================================
/*
 0xF0   8'h05   RW  7:0 Ctl_EXT_CACHE_CEN_R_PER
                      PBA provide Configure programmable 128-stage delay buffer for extend PBA Cache Rising CEN period
 0xF1   8'h05   RW  7:0 Ctl_EXT_CACHE_CEN_F_PER
                      PBA provide Configure programmable 128-stage delay buffer for extend PBA Cache Falling CEN period
*/
//==============================================================================

#define CTL_EXT_CACHE_CEN_R_PER           0xF0

#define CTL_EXT_CACHE_CEN_F_PER           0xF1

#define TOG_READ_ID_TR                    0xF8
 #define SET_TOG_READ_ID_TR_CNT           CLR_BIT4

#define CFDE_GPIO_DATA_OUTP               0xF9

#define CFDE_DBG_SEL                      0xFA
//disply bit0~63 for debug

#define HW_DBG_MODE1               (0x01)
//disply system clock on bit22,tie H bit10~19
#define HW_DBG_MODE2               (0x02)

#define HW_DBG_MODE4               (0x04)
#define HW_DBG_MODE5               (0x05)

#define HW_DBG_MODE7               (0x07)
#define HW_DBG_MODE8               (0x08)
#define HW_DBG_MODE9               (0x09)
#define HW_DBG_MODE10              (0x0A)

#define HW_DBG_MODE11              (0x0B)
#define HW_DBG_MODE12              (0x0C)
#define HW_DBG_MODE13              (0x0D)
#define HW_DBG_MODE14              (0x0E)

#define HW_DBG_MODE15              (0x0F)
#define HW_DBG_MODE16              (0x10)
#define HW_DBG_MODE17              (0x11)

/*--------------------------------------------------------------------------*/
/* CE LEVEL(CE0~CE7)                                                        */
/*--------------------------------------------------------------------------*/
#define CRBYTE0_CE                          0x00
#define CLR_WR_CNT                     SET_BIT7
#define CLR_RD_CNT                     SET_BIT4
#define CLR_SG_ADDR_QUEUE              SET_BIT3
#define MASK_MODE                           0x9F
#define S_LEGACY_MODE                   (0x00<<5)
#define S_TOGGLE_MODE                   (0x01<<5)
#define S_ONFI_MODE                     (0x02<<5)

#define DATA_BUF_PTR                        0x04

#define DATA_BUF                            0x05

#define BAD_COL_BASE_ADR                    0x07

//==============================================================================
/*
0x00A
(CE)    0   R   7   Reserved
    0   W   6   RR_RPTR_LD_EN   Enable to load RR_RPTR_LD_VALUE to the read pointer of cache buffer for Round Robin. It will be cleared after asserting
    0   R/W 5:0 RR_RPTR_LD_VALUE    When RPTR_LD_EN asserts, WPTR_LD_VALUE is loaded into cache read buffer pointer for Round Robin.
0x00B
(CE)    0   R   7   Reserved
    0   W   6   RPTR_LD_EN  Enable to load RPTR_LD_VALUE to the read  pointer of cache buffer. It will be cleared after asserting
    0   R/W 4:0 RPTR_LD_VALUE   When RPTR_LD_EN asserts, 2*WPTR_LD_VALUE is loaded into cache read buffer pointer.

*/
//==============================================================================
#define RR_RPTR_LD                          0x0A
#define RR_RPTR_LD_EN                           SET_BIT6

#define RPTR_LD                             0x0B
#define RPTR_LD_EN                              SET_BIT6

#define RD_XFER_NO_H                        0x0C
#define RD_XFER_NO_L                        0x0D
#define RD_TRF_CNT_H                        0x0E
#define RD_TRF_CNT_L                        0x0F

//==============================================================================
/*
 0x010
(CE)    0   RW  7:0 CMD90_ID7   Command 90 ID output #7
 0x011
 (CE)   0   RW  7:0 CMD90_ID6   Command 90 ID output #6
 0x012
(CE)    0   RW  7:0 CMD90_ID5   Command 90 ID output #5
 0x013
(CE)    0   RW  7:0 CMD90_ID4   Command 90 ID output #4
 0x014
(CE)    0   RW  7:0 CMD90_ID3   Command 90 ID output #3
 0x015
(CE)    0   RW  7:0 CMD90_ID2   Command 90 ID output #2
 0x016
(CE)    0   RW  7:0 CMD90_ID1   Command 90 ID output #1
 0x017
(CE)    0   RW  7:0 CMD90_ID0   Command 90 ID output #0
*/
//==============================================================================
#define CMD90_ID7                           0x10
#define CMD90_ID6                           0x11
#define CMD90_ID5                           0x12
#define CMD90_ID4                           0x13
#define CMD90_ID3                           0x14
#define CMD90_ID2                           0x15
#define CMD90_ID1                           0x16
#define CMD90_ID0                           0x17

//==============================================================================
/*
0x018
(CE)    0   R   7   Reserved
    7'h7f   R/W 6:0 CACHE_SEL_PA0   This register is valid when CACHE_MODE_SEL=1. The PBA page buffer enable bit 0 is determined by this register which value is choose from page address (PA) for write CACHE.
Note: 7'd64: 1'b0, 7'd65:1'b1;
0x019
(CE)    0   R   7   Reserved
    7'h7f   R/W 6:0 CACHE_SEL_PA1   This register is valid when CACHE_MODE_SEL=1. The PBA page buffer enable bit 1 is determined by this register which value is choose from page address (PA) for write CACHE.
Note: 7'd64: 1'b0, 7'd65:1'b1;
0x01A
(CE)    0   R   7   Reserved
    7'h7f   R/W 6:0 CACHE_SEL_PA2   This register is valid when CACHE_MODE_SEL=1. The PBA page buffer enable bit 2 is determined by this register which value is choose from page address (PA) for write CACHE.
Note: 7'd64: 1'b0, 7'd65:1'b1;
0x01B
(CE)    0   R   7   Reserved
    7'h7f   R/W 6:0 CACHE_SEL_PA3   This register is valid when CACHE_MODE_SEL=1. The PBA page buffer enable bit 3 is determined by this register which value is choose from page address (PA) for write CACHE.
Note: 7'd64: 1'b0, 7'd65:1'b1;
*/
//==============================================================================
#define CC_SEL_PA0                      0x18
#define CC_SEL_PA1                      0x19
#define CC_SEL_PA2                      0x1A
#define CC_SEL_PA3                      0x1B


//#define RD_TRF_CNT_H                    0x1C
//#define RD_TRF_CNT_L                    0x1D

#define WR_TRF_CNT_H                    0x1E
#define WR_TRF_CNT_L                    0x1F

#define ADR_PTR                         0x20

//==============================================================================
/*
0x021
(CE)    8'h10   R/W 7:0 RDY_STAT_DLY1   Configure programmable 256-stage delay for BSY1 delay wire
0x022
(CE)    8'h10   R/W 7:0 RDY_STAT_DLY2   Configure programmable 256-stage delay for BSY2 delay wire
0x023
(CE)    8'h10   R/W 7:0 RDY_STAT_DLY3   Configure programmable 256-stage delay for BSY3 delay wire
0x024
(CE)    8'h10   R/W 7:0 RDY_STAT_DLY4   Configure programmable 256-stage delay for BSY4 delay wire
0x025
(CE)    8'h10   R/W 7:0 RDY_STAT_DLY5   Configure programmable 256-stage delay for BSY5 delay wire
0x026
(CE)    8'h10   R/W 7:0 RDY_STAT_DLY6   Configure programmable 256-stage delay for BSY6 delay wire
0x027
(CE)    8'h10   R/W 7:0 RDY_STAT_DLY7   Configure programmable 256-stage delay for BSY7 delay wire
0x028
(CE)    8'h10   R/W 7:0 RDY_STAT_DLY8   Configure programmable 256-stage delay for BSY8 delay wire

*/
//==============================================================================
#define RDY_STAT_DLY1_LUN0              0x21
#define RDY_STAT_DLY2_LUN0              0x22
#define RDY_STAT_DLY3_LUN0              0x23
#define RDY_STAT_DLY4_LUN0              0x24
#define RDY_STAT_DLY5_LUN0              0x25
#define RDY_STAT_DLY6_LUN0              0x26
#define RDY_STAT_DLY7_LUN0              0x27
#define RDY_STAT_DLY8_LUN0              0x28

//==============================================================================
/*
0x02A
(CE)    0   R/W 6:0 DAT_BUF_PTR_TOG Select the control data from data buffer to read.
0x02B
(CE)    0   R   7:0 DAT_BUF_TOG     Show control data from data buffer and the pointer is DAT_BUF_PTR_TOG.

*/
//==============================================================================
#define DAT_BUF_PTR_TOG                 0x2A
#define DAT_BUF_TOG                     0x2B


//==============================================================================
/*
(CE)    0   R   7   Reserved
    7'h7f   R/W 6:0 CACHE_SEL_PA4   This register is valid when CACHE_MODE_SEL=1. The PBA page buffer enable bit 4 is determined by this register which value is choose from page address (PA) for write CACHE.
Note: 7'd64: 1'b0, 7'd65:1'b1;
0x02E
(CE)    0   R   7   Reserved
    7'h7f   R/W 6:0 CACHE_SEL_PA5   This register is valid when CACHE_MODE_SEL=1. The PBA page buffer enable bit 5 is determined by this register which value is choose from page address (PA) for write CACHE.
Note: 7'd64: 1'b0, 7'd65:1'b1;
*/
//==============================================================================
#define CC_SEL_PA4                      0x2D
#define CC_SEL_PA5                      0x2E


//==============================================================================
/*
0x030
(CE)    0   R   7   BSY8_FLAG_LUN1  Busy flag 8 of LUN1. 1: ready, 0: busy
    0   R   6   BSY7_FLAG_LUN1  Busy flag 7 of LUN1. 1: ready, 0: busy
    0   R   5   BSY6_FLAG_LUN1  Busy flag 6 of LUN1. 1: ready, 0: busy
    0   R   4   BSY5_FLAG_LUN1  Busy flag 5 of LUN1. 1: ready, 0: busy
    0   R   3   BSY4_FLAG_LUN1  Busy flag 4 of LUN1. 1: ready, 0: busy
    0   R   2   BSY3_FLAG_LUN1  Busy flag 3 of LUN1. 1: ready, 0: busy
    0   R   1   BSY2_FLAG_LUN1  Busy flag 2 of LUN1. 1: ready, 0: busy
    0   R   0   BSY1_FLAG_LUN1  Busy flag 1 of LUN1. 1: ready, 0: busy
*/
//==============================================================================
#define BSY8_1_FLAG_LUN1               0x30

#define BSY8_1_FLAG_LUN2               0x31

#define BSY8_1_FLAG_LUN3               0x32

#define BSY8_1_FLAG_LUN4               0x33

#define BSY8_1_FLAG_LUN5               0x34

#define BSY8_1_FLAG_LUN6               0x35

#define BSY8_1_FLAG_LUN7               0x36


//==============================================================================
/*
0x038
 (CE)   0   R   7   reserved
    0   R/W 6:0 STA_BUF_DLY_T_NO    PBA has 128 Byte registers which are used as for second status buffers (0~127) for delay T timing by FW then HW will auto to choose second status buffers from first status buffer. If FW wants to write the target buffer and define the signal definition for each bit. STA_BUF_DLY_T_NO has to be set to the wanted location.
0x039
 (CE)   0   R/W 7:0 STA_WR_DLY_T_ENTRY  If STA_BUF_DLY_T_NO is set to N(0~127), the status buffer N can be written as a specific data through STA_WR_DLY_T_ ENTRY.
*/
//==============================================================================
#define STA_BUF_DLY_T_NO               0x38  //Staus buffer 2 for ready 

#define STA_WR_DLY_T_ENTRY             0x39


//==============================================================================
/*
0x03A
(CE)    0   R/W 2:0 SIG_DAT_LEG_SRAM_LUN_SEL    PBA has 512 Bytes SRAM by LUN which FW can to select LUN number (LUN0~LUN7) to setting data for LEGACY mode.
(i.e.Fn-0x55-addr-data) by command latch (Fn) for DPS function.
0x03B
(CE)    0   R/W 7:0 SIG_DAT_LEG_SRAM_ADDR_SEL   PBA has 512 SRAM address by LUN which FW can to select SRAM address to setting data for LEGACY mode.
(i.e.Fn-0x55-addr-data) by command latch (Fn) for DPS function.The SRAM must be selected by HW, when using OUT_SRC=3'd5.
0x03C
(CE)    0   R/W 7:0 SIG_DAT_LEG_SRAM_RW PBA has 512 SRAM address by LUN which FW can to select SRAM address then used this bit to setting data for LEGACY mode.
0x03D
 (CE)   0   R/W 2:0 SIG_DAT_TOG_SRAM_LUN_SEL    PBA has 512 Bytes SRAM by LUN which FW can to select LUN number (LUN0~LUN7) to setting data for TOGGLE mode.
(i.e.Fn-0x55-addr-data) by command latch (Fn) for DPS function.
0x03E
 (CE)   0   R/W 7:0 SIG_DAT_TOG_SRAM_ADDR_SEL   PBA has 512 SRAM address by LUN which FW can to select SRAM address to setting data for TOGGLE mode.
(i.e.Fn-0x55-addr-data) by command latch (Fn) for DPS function.The SRAM must be selected by HW, when using OUT_SRC=3'd5.
0x03F
(CE)    0   R/W 7:0 SIG_DAT_TOG_SRAM_RW PBA has 512 SRAM address by LUN which FW can to select SRAM address then used this bit to setting data for TOGGLE mode.
*/
//==============================================================================
#define SIG_DAT_LEG_SRAM_LUN_SEL       0x3A
#define SIG_DAT_LEG_SRAM_ADDR_SEL      0x3B
#define SIG_DAT_LEG_SRAM_RW            0x3C
#define SIG_DAT_TOG_SRAM_LUN_SEL       0x3D
#define SIG_DAT_TOG_SRAM_ADDR_SEL      0x3E
#define SIG_DAT_TOG_SRAM_RW            0x3F


//==============================================================================
/*
0x040
(CE)    0   RW  7   SET_BSY4_BUSY   SET BSY4
        '0' : NOP
        '1' : Set BSY4 to busy, and this bit would be auto cleared.
      0 RW  6   SET_BSY4_RDY    SET BSY4 to ready
        '0' : NOP
        '1' : Set BSY4 to ready, and this bit would be auto cleared.
      0 RW  5   SET_BSY3_BUSY   SET BSY3
        '0' : NOP
        '1' : Set BSY3 to busy, and this bit would be auto cleared.
      0 RW  4   SET_BSY3_RDY    SET BSY3 to ready
        '0' : NOP
        '1' : Set BSY3 to ready, and this bit would be auto cleared.
      0 RW  3   SET_BSY2_BUSY   SET BSY2
        '0' : NOP
        '1' : Set BSY2 to busy, and this bit would be auto cleared.
      0 RW  2   SET_BSY2_RDY    SET BSY2 to ready
        '0' : NOP
        '1' : Set BSY2 to ready, and this bit would be auto cleared.
      0 RW  1   SET_BSY1_BUSY   SET BSY1
        '0' : NOP
        '1' : Set BSY1 to busy, and this bit would be auto cleared.
      0 RW  0   SET_BSY1_RDY    SET BSY1 to ready
        '0' : NOP
        '1' : Set BSY1 to ready, and this bit would be auto cleared.
*/
//==============================================================================
#define SET_BSY4_1_BUSY_RDY_LUN0      0x40
#define SET_BSY4                            SET_BIT7
#define SET_RDY4                            SET_BIT6
#define SET_BSY3                            SET_BIT5
#define SET_RDY3                            SET_BIT4
#define SET_BSY2                            SET_BIT3
#define SET_RDY2                            SET_BIT2
#define SET_BSY1                            SET_BIT1
#define SET_RDY1                            SET_BIT0

#define SET_ALL_RDY                         (0x55)
#define SET_ALL_BSY                         (0xAA)

//==============================================================================
/*
0x041
(CE)    0   RW  7:0 CFDE_SEL    Select the current configure the number of CFDE unit and for to get the interrupt type on CFDE_INT_TYPE[3:0](0x4B) register offset.

*/
//==============================================================================
#define CFDE_SEL_CE                   0x41


//==============================================================================
/*
0x042
(CE)    0   R   7   Reserved
    7'h7f   R/W 6:0 RD_CACHE_SEL_PA0    This register is valid when CACHE_MODE_SEL=1. The PBA page buffer enable bit 0 is determined by this register which value is choose from page address (PA) for read CACHE.
Note: 7'd64: 1'b0, 7'd65:1'b1;
0x043
(CE)    0   R   7   Reserved
    7'h7f   R/W 6:0 RD_CACHE_SEL_PA1    This register is valid when CACHE_MODE_SEL=1. The PBA page buffer enable bit 1 is determined by this register which value is choose from page address (PA)
for read CACHE.
Note: 7'd64: 1'b0, 7'd65:1'b1;
0x044
(CE)    0   R   7   Reserved
    7'h7f   R/W 6:0 RD_CACHE_SEL_PA2    This register is valid when CACHE_MODE_SEL=1. The PBA page buffer enable bit 2 is determined by this register which value is choose from page address (PA)
for read CACHE.
Note: 7'd64: 1'b0, 7'd65:1'b1;
0x045
(CE)    0   R   7   Reserved
    7'h7f   R/W 6:0 RD_CACHE_SEL_PA3    This register is valid when CACHE_MODE_SEL=1. The PBA page buffer enable bit 3 is determined by this register which value is choose from page address (PA)
for read CACHE.
Note: 7'd64: 1'b0, 7'd65:1'b1;
0x046
(CE)    0   R   7   Reserved
    7'h7f   R/W 6:0 RD_CACHE_SEL_PA4    This register is valid when CACHE_MODE_SEL=1. The PBA page buffer enable bit 4 is determined by this register which value is choose from page address (PA)
for read CACHE.
Note: 7'd64: 1'b0, 7'd65:1'b1;
0x047
(CE)    0   R   7   Reserved
    7'h7f   R/W 6:0 RD_CACHE_SEL_PA5    This register is valid when CACHE_MODE_SEL=1. The PBA page buffer enable bit 5 is determined by this register which value is choose from page address (PA)
for read CACHE.
Note: 7'd64: 1'b0, 7'd65:1'b1;
*/
//==============================================================================
#define RD_CACHE_SEL_PA0                0x42
#define RD_CACHE_SEL_PA1                0x43
#define RD_CACHE_SEL_PA2                0x44
#define RD_CACHE_SEL_PA3                0x45
#define RD_CACHE_SEL_PA4                0x46
#define RD_CACHE_SEL_PA5                0x47


//==============================================================================
/*
0x049
(CE)    0   RW  7   SET_BSY8_BUSY   SET BSY8
        '0' : NOP
        '1' : Set BSY8 to busy, and this bit would be auto cleared.
      0 RW  6   SET_BSY8_RDY    SET BSY8 to ready
        '0' : NOP
        '1' : Set BSY8 to ready, and this bit would be auto cleared.
      0 RW  5   SET_BSY7_BUSY   SET BSY7
        '0' : NOP
        '1' : Set BSY7 to busy, and this bit would be auto cleared.
      0 RW  4   SET_BSY7_RDY    SET BSY7 to ready
        '0' : NOP
        '1' : Set BSY7 to ready, and this bit would be auto cleared.
      0 RW  3   SET_BSY6_BUSY   SET BSY6
        '0' : NOP
        '1' : Set BSY6 to busy, and this bit would be auto cleared.
      0 RW  2   SET_BSY6_RDY    SET BSY6 to ready
        '0' : NOP
        '1' : Set BSY6 to ready, and this bit would be auto cleared.
      0 RW  1   SET_BSY5_BUSY   SET BSY5
        '0' : NOP
        '1' : Set BSY5 to busy, and this bit would be auto cleared.
      0 RW  0   SET_BSY5_RDY    SET BSY5 to ready
        '0' : NOP
        '1' : Set BSY5 to ready, and this bit would be auto cleared.
*/
//==============================================================================
#define SET_BSY8_5_BUSY_RDY_LUN0        0x49
#define SET_BSY8                            SET_BIT7
#define SET_RDY8                            SET_BIT6
#define SET_BSY7                            SET_BIT5
#define SET_RDY7                            SET_BIT4
#define SET_BSY6                            SET_BIT3
#define SET_RDY6                            SET_BIT2
#define SET_BSY5                            SET_BIT1
#define SET_RDY5                            SET_BIT0


//==============================================================================
/*
0x04B
(CE)    0   R   7:5 reserved
        0   R   4   INT_STAT    Indicate the interrupt status. 1: interrupt assert, 0: interrupt deassert.
        0   R   3:0 CFDE_INT_TYPE   Indicate CFDE_SEL corresponding CFDE interrupt category. This register is cleared after reading this address.
*/
//==============================================================================
#define CFDE_INT_TYPE                  0x4B
#define CFDE_INT                            CHK_BIT4


//==============================================================================
/*
0x04D
(CE)    0   R   7   BSY8_FLAG   Busy flag 8 of LUN0. 1: ready, 0: busy
        0   R   6   BSY7_FLAG   Busy flag 7 of LUN0. 1: ready, 0: busy
        0   R   5   BSY6_FLAG   Busy flag 6 of LUN0. 1: ready, 0: busy
        0   R   4   BSY5_FLAG   Busy flag 5 of LUN0. 1: ready, 0: busy
        0   R   3   BSY4_FLAG   Busy flag 4 of LUN0. 1: ready, 0: busy
        0   R   2   BSY3_FLAG   Busy flag 3 of LUN0. 1: ready, 0: busy
        0   R   1   BSY2_FLAG   Busy flag 2 of LUN0. 1: ready, 0: busy
        0   R   0   BSY1_FLAG   Busy flag 1 of LUN0. 1: ready, 0: busy
*/
//==============================================================================
#define BSY8_1_FLAG_LUN0                0x4D


//==============================================================================
/*
0x04E
(CE)    0   R   7   BSY_O_LUN7  Indicate the busy status 1: ready, 0: busy, BSY_O shows 0 if either BSY_FLAG or BSY2_FLAG or BSY3_FLAG or BSY4_FLAG or BSY5_FLAG or BSY6_FLAG or BSY7_FLAG or BSY8_FLAG of LUN7.
        0   R   6   BSY_O_LUN6  Indicate the busy status 1: ready, 0: busy, BSY_O shows 0 if either BSY_FLAG or BSY2_FLAG or BSY3_FLAG or BSY4_FLAG or BSY5_FLAG or BSY6_FLAG or BSY7_FLAG or BSY8_FLAG of LUN6.
        0   R   5   BSY_O_LUN5  Indicate the busy status 1: ready, 0: busy, BSY_O shows 0 if either BSY_FLAG or BSY2_FLAG or BSY3_FLAG or BSY4_FLAG or BSY5_FLAG or BSY6_FLAG or BSY7_FLAG or BSY8_FLAG of LUN5.
        0   R   4   BSY_O_LUN4  Indicate the busy status 1: ready, 0: busy, BSY_O shows 0 if either BSY_FLAG or BSY2_FLAG or BSY3_FLAG or BSY4_FLAG or BSY5_FLAG or BSY6_FLAG or BSY7_FLAG or BSY8_FLAG of LUN4.
        0   R   3   BSY_O_LUN3  Indicate the busy status 1: ready, 0: busy, BSY_O shows 0 if either BSY_FLAG or BSY2_FLAG or BSY3_FLAG or BSY4_FLAG or BSY5_FLAG or BSY6_FLAG or BSY7_FLAG or BSY8_FLAG of LUN3.
        0   R   2   BSY_O_LUN2  Indicate the busy status 1: ready, 0: busy, BSY_O shows 0 if either BSY_FLAG or BSY2_FLAG or BSY3_FLAG or BSY4_FLAG or BSY5_FLAG or BSY6_FLAG or BSY7_FLAG or BSY8_FLAG of LUN2.
        0   R   1   BSY_O_LUN1  Indicate the busy status 1: ready, 0: busy, BSY_O shows 0 if either BSY_FLAG or BSY2_FLAG or BSY3_FLAG or BSY4_FLAG or BSY5_FLAG or BSY6_FLAG or BSY7_FLAG or BSY8_FLAG of LUN1.
        0   R   0   BSY_O_LUN0  Indicate the busy status 1: ready, 0: busy, BSY_O shows 0 if either BSY_FLAG or BSY2_FLAG or BSY3_FLAG or BSY4_FLAG or BSY5_FLAG or BSY6_FLAG or BSY7_FLAG or BSY8_FLAG of LUN0.
*/
//==============================================================================
#define BSY_STS_O_LUN7_0                0x4E


//==============================================================================
/*
0x04F
(CE)    8'h40   R/W 7:0 STA_SEL_ENH_BASE_PTR    The default enhance base status buffer pointer start from 64.
                    If PBA encounter the command 71-addr(R1)-read like sequence. The status buffers bust be selected by HW.
*/
//==============================================================================
#define STA_SEL_ENH_BASE_PTR            0x4F


//==============================================================================
/*
0x051
(CE)    0   R   7:5 Reserved
    0   R/W 4:0 ADR_BUF_NO  CFDE has 32 address buffers and each address buffer can store 10 cycle addresses. When FW sets ADR_BUF_NO, FW can read 10 cycle addresses of the address buffer ADR_BUF_NO through offset address 0x52~0x5B.
0x052
(CE)    0   R   7:0 ADR9    Receive address buffer #9 by ADR_BUF_NO
0x053
(CE)    0   R   7:0 ADR8    Receive address buffer #8 by ADR_BUF_NO
0x054
(CE)    0   R   7:0 ADR7    Receive address buffer #7 by ADR_BUF_NO
0x055
(CE)    0   R   7:0 ADR6    Receive address buffer #6 by ADR_BUF_NO
0x056
(CE)    0   R   7:0 ADR5    Receive address buffer #5 by ADR_BUF_NO
0x057
(CE)    0   R   7:0 ADR4    Receive address buffer #4 by ADR_BUF_NO
0x058
(CE)    0   R   7:0 ADR3    Receive address buffer #3 by ADR_BUF_NO
0x059
(CE)    0   R   7:0 ADR2    Receive address buffer #2 by ADR_BUF_NO
0x05A
(CE)    0   R   7:0 ADR1    Receive address buffer #1 by ADR_BUF_NO
0x05B
(CE)    0   R   7:0 ADR0    Receive address buffer #0 by ADR_BUF_NO

*/
//==============================================================================
#define ADDR_BUF_NO_SG_DATA             0x51
//#define CYCLE9                          0x52
//#define CYCLE8                          0x53
//#define CYCLE7                          0x54
#define CYCLE6                          0x55
#define CYCLE5                          0x56
#define CYCLE4                          0x57
#define CYCLE3                          0x58
#define CYCLE2                          0x59
#define CYCLE1                          0x5A
#define CYCLE0                          0x5B


//==============================================================================
/*
0x05C
(CE)	0	R	7	Reserved	It's determined by this register which value is choose from plane bit0 address.

	7'h7f	R/W	6:0	Plane_SEL_PA0	
0x05D
 (CE)	0	R	7	Reserved	It's determined by this register which value is choose from plane bit1 address.

	7'h7f	R/W	6:0	Plane_SEL_PA1	
0x05E
 (CE)	0	R	7	Reserved	
	7'h7f	R/W	6:0	Plane_SEL_PA2	It's determined by this register which value is choose from plane bit2 address.

0x05F
 (CE)	0	R	7	Reserved
	7'h7f	R/W	6:0	LUN_SEL_PA0 It's determined by this register which value is choose from lun bit0 address.
*/
//==============================================================================
#define PLANE_SEL_PA0                   0x5C
#define PLANE_SEL_PA1                   0x5D
#define PLANE_SEL_PA2                   0x5E

#define LUN_SEL_PA0                     0x5F

//==============================================================================
/*
0x060
(CE)    0   R/W 7:0 STA0_bit0
0x061
(CE)    0   R/W 7:0 STA0_bit1
0x062
(CE)    0   R/W 7:0 STA0_bit2
0x063
(CE)    0   R/W 7:0 STA0_bit3
0x064
(CE)    0   R/W 7:0 STA0_bit4
0x065
(CE)    0   R/W 7:0 STA0_bit5
0x066
(CE)    0   R/W 7:0 STA0_bit6
0x067
(CE)    0   R/W 7:0 STA0_bit7
*/
//==============================================================================
#define STA0_BIT0                       0x60
#define STA0_BIT1                       0x61
#define STA0_BIT2                       0x62
#define STA0_BIT3                       0x63
#define STA0_BIT4                       0x64
#define STA0_BIT5                       0x65
#define STA0_BIT6                       0x66
#define STA0_BIT7                       0x67

enum INFORMATION_BIT_LIST_LUN0 {
    IB_NO_SRC_TIE_L = 0,           //0
    IB_BSY1_LUN0,                  //1
    IB_BSY2_LUN0,                  //2
    IB_BSY3_LUN0,                  //3
    IB_BSY4_LUN0,                  //4
    IB_BSY5_LUN0,                  //5
    IB_BSY6_LUN0,                  //6
    IB_BSY7_LUN0,                  //7
    IB_BSY8_LUN0,                  //8

    IB_BSY8_DLY_AND_BSY1_DLY_LUN0, //9
    IB_BSY8_OR_BSY1_LUN0,          //10
    IB_BSY8_AND_BSY1_LUN0,         //11
    IB_WP,                         //12
    IB_STABUF_ENTERY,              //13
    IB_NO_SRC_TIE_H                //14
};

enum INFORMATION_BIT_LIST_LUN1 {
    IB_BSY1_LUN1 = 15,             //15
    IB_BSY2_LUN1,                  //16
    IB_BSY3_LUN1,                  //17
    IB_BSY4_LUN1,                  //18
    IB_BSY5_LUN1,                  //19
    IB_BSY6_LUN1,                  //20
    IB_BSY7_LUN1,                  //21
    IB_BSY8_LUN1,                  //22

    IB_BSY8_DLY_AND_BSY1_DLY_LUN1, //23
    IB_BSY8_OR_BSY1_LUN1,          //24
    IB_BSY8_AND_BSY1_LUN1,         //25
};

enum INFORMATION_BIT_LIST_LUN2 {
    IB_BSY1_LUN2 = 26,             //26
    IB_BSY2_LUN2,                  //27
    IB_BSY3_LUN2,                  //28
    IB_BSY4_LUN2,                  //29
    IB_BSY5_LUN2,                  //30
    IB_BSY6_LUN2,                  //31
    IB_BSY7_LUN2,                  //32
    IB_BSY8_LUN2,                  //33

    IB_BSY8_DLY_AND_BSY1_DLY_LUN2, //34
    IB_BSY8_OR_BSY1_LUN2,          //35
    IB_BSY8_AND_BSY1_LUN2,         //36
};

enum INFORMATION_BIT_LIST_LUN3 {
    IB_BSY1_LUN3 = 37,             //37
    IB_BSY2_LUN3,                  //38
    IB_BSY3_LUN3,                  //39
    IB_BSY4_LUN3,                  //40
    IB_BSY5_LUN3,                  //41
    IB_BSY6_LUN3,                  //42
    IB_BSY7_LUN3,                  //43
    IB_BSY8_LUN3,                  //44

    IB_BSY8_DLY_AND_BSY1_DLY_LUN3, //45
    IB_BSY8_OR_BSY1_LUN3,          //46
    IB_BSY8_AND_BSY1_LUN3,         //47
};

enum INFORMATION_BIT_LIST_LUN4 {
    IB_BSY1_LUN4 = 48,             //48
    IB_BSY2_LUN4,                  //49
    IB_BSY3_LUN4,                  //50
    IB_BSY4_LUN4,                  //51
    IB_BSY5_LUN4,                  //52
    IB_BSY6_LUN4,                  //53
    IB_BSY7_LUN4,                  //54
    IB_BSY8_LUN4,                  //55

    IB_BSY8_DLY_AND_BSY1_DLY_LUN4, //56
    IB_BSY8_OR_BSY1_LUN4,          //57
    IB_BSY8_AND_BSY1_LUN4,         //58
};

enum INFORMATION_BIT_LIST_LUN5 {
    IB_BSY1_LUN5 = 59,             //59
    IB_BSY2_LUN5,                  //60
    IB_BSY3_LUN5,                  //61
    IB_BSY4_LUN5,                  //62
    IB_BSY5_LUN5,                  //63
    IB_BSY6_LUN5,                  //64
    IB_BSY7_LUN5,                  //65
    IB_BSY8_LUN5,                  //66

    IB_BSY8_DLY_AND_BSY1_DLY_LUN5, //67
    IB_BSY8_OR_BSY1_LUN5,          //68
    IB_BSY8_AND_BSY1_LUN5,         //69
};

enum INFORMATION_BIT_LIST_LUN6 {
    IB_BSY1_LUN6 = 70,             //70
    IB_BSY2_LUN6,                  //71
    IB_BSY3_LUN6,                  //72
    IB_BSY4_LUN6,                  //73
    IB_BSY5_LUN6,                  //74
    IB_BSY6_LUN6,                  //75
    IB_BSY7_LUN6,                  //76
    IB_BSY8_LUN6,                  //77

    IB_BSY8_DLY_AND_BSY1_DLY_LUN6, //78
    IB_BSY8_OR_BSY1_LUN6,          //79
    IB_BSY8_AND_BSY1_LUN6,         //80
};

enum INFORMATION_BIT_LIST_LUN7 {
    IB_BSY1_LUN7 = 81,             //81
    IB_BSY2_LUN7,                  //82
    IB_BSY3_LUN7,                  //83
    IB_BSY4_LUN7,                  //84
    IB_BSY5_LUN7,                  //85
    IB_BSY6_LUN7,                  //86
    IB_BSY7_LUN7,                  //87
    IB_BSY8_LUN7,                  //88

    IB_BSY8_DLY_AND_BSY1_DLY_LUN7, //89
    IB_BSY8_OR_BSY1_LUN7,          //90
    IB_BSY8_AND_BSY1_LUN7,         //91

    IB_LUN7_AND_LUN0,               //92
    IB_LUN7_AND_LUN0_DLY,          //93
};


//==============================================================================
/*

*/
//==============================================================================
#define STA_SEL_PA0_ENH                 0x6D
#define STA_SEL_PA1_ENH                 0x6E
#define STA_SEL_PA2_ENH                 0x6F


//==============================================================================
/*
0x070
(CE)    0   R   7:0 CFDE_INTV_7_0   Indicate which CFDE unit interrupt occur.
0x071
(CE)    0   R   7:0 CFDE_INTV_15_8  Indicate which CFDE unit interrupt occur.
0x072
(CE)    0   R   7:0 CFDE_INTV_23_16 Indicate which CFDE unit interrupt occur.
0x073
(CE)    0   R   7:0 CFDE_INTV_31_24 Indicate which CFDE unit interrupt occur.
0x074
(CE)    0   R   7:0 CFDE_INTV_39_32 Indicate which CFDE unit interrupt occur.
0x075
(CE)    0   R   7:0 CFDE_INTV_47_40 Indicate which CFDE unit interrupt occur.
0x076
(CE)    0   R   7:0 CFDE_INTV_55_48 Indicate which CFDE unit interrupt occur.
0x077
(CE)    0   R   7:0 CFDE_INTV_63_56 Indicate which CFDE unit interrupt occur.
0x078
(CE)    0   R   7:0 CFDE_INTV_71_64 Indicate which CFDE unit interrupt occur.
0x079
(CE)    0   R   7:0 CFDE_INTV_79_72 Indicate which CFDE unit interrupt occur.
0x07A
(CE)    0   R   7:0 CFDE_INTV_87_80 Indicate which CFDE unit interrupt occur.
0x07B
(CE)    0   R   7:0 CFDE_INTV_95_88 Indicate which CFDE unit interrupt occur.
0x07C
(CE)    0   R   7:4 reserved
    0   R   3:0 CFDE_INTV_99_96 Indicate which CFDE unit interrupt occur.
*/
//==============================================================================
#define CFDE_INTV_7_0                   0x70
#define CFDE_INTV_15_8                  0x71
#define CFDE_INTV_23_16                 0x72
#define CFDE_INTV_31_24                 0x73
#define CFDE_INTV_39_32                 0x74
#define CFDE_INTV_47_40                 0x75
#define CFDE_INTV_55_48                 0x76
#define CFDE_INTV_63_56                 0x77
#define CFDE_INTV_71_64                 0x78
#define CFDE_INTV_79_72                 0x79
#define CFDE_INTV_87_80                 0x7A
#define CFDE_INTV_95_88                 0x7B
#define CFDE_INTV_99_96                 0x7C


#define CFDE_STS_FLAG                   0x7D
#define BAD_COL_                            BIT7
#define WR_EQUAL                            BIT6
#define WR_LESS                             BIT5
#define WR_OVFLOW                           BIT4
#define RD_EQUAL                            BIT2
#define RD_LESS                             BIT1
#define RD_OVFLOW                           BIT0

//==============================================================================
/*
0x07F
(CE)
  0 R/W 4   dps_stack_mode  Enable used DPS SRAM to store data on stack flow of toggle mode.
    0   W   3   clr_dps_stack_adr   Clear stack address (auto clear)for dps sram address used when dps_stack_mode = 1'b1,the FW can to initial the address on stack flow.
    0   W   2   bad_col_flag_clr    Bad column flag clear bit (auto clear).
    0   R/W 1   ctl_addr_clr    Clear current address buffer0~9  value(auto clear)
    0   W   0   INT_CLR Clear PBA interrupt and all interrupt flag.
*/
//==============================================================================
#define CFDE_CLR_ALL_INTR               0x7F

#define EN_DPS_STACK_MODE             SET_BIT4
#define DIS_DPS_STACK_MODE            CLR_BIT4

#define CLR_DPS_STACK_ADDR            SET_BIT3
#define CLR_BAD_COL_FLAG              SET_BIT2
#define CLR_CUR_TEMP_ADDR_BUF         SET_BIT1
#define FLAG_CLEAR_INT                SET_BIT0

//==============================================================================
/*
0x080
(CE)    0   R   7   reserved
    0   R/W 6:0 STA_BUF_NO  PBA has 128 Byte registers which are used as status buffers (0~127). If FW wants to write the target buffer and define the signal definition for each bit. STA_BUF_NO has to be set to the wanted location.
0x081
(CE)    0   R   7   reserved
    0   R   6:0 STA_BUF_PTR Indicate the pointer of the current used status buffer which assigned by CFDE command.
0x082
(CE)    0   R/W 7:0 STA_WR_ENTRY    If STA_BUF_NO is set to N(0~127), the status buffer N can be written as a specific data through STA_WR_ENTRY.

0x083
(CE)	0	R	7	Reserved	It's determined by this register which value is choose from lun bit1 address.
	7'h7f	R/W	6:0	LUN_SEL_PA1	
0x084
(CE)	0	R	7	Reserved	It's determined by this register which value is choose from lun bit2 address.
  7'h7f	R/W	6:0	LUN_SEL_PA2
  
0x087
(CE)    0   R   7   reserved    There are 128 byte status registers to record the data written by FW. If PBA encounter the command 78-addr-read like sequence. The status buffers bust be selected by HW, when using OUT_SRC=3'd3 and the command function bits of load_add_sta is high. PBA determines the LUN,PLANE status register from page address by using control registers STA_SEL_PA0,STA_SEL_PA1,STA_SEL_PA2,STA_SEL_PA3,STA_SEL_PA4, and STA_SEL_PA5.

*/
//==============================================================================
#define STA_BUF_NO                      0x80  //Staus buffer 1 for busy 

#define STA_BUF_PTR                     0x81

#define STA_WR_ENTRY                    0x82

#define LUN_SEL_PA1                     0x83
#define LUN_SEL_PA2                     0x84

#define STA_SEL_PA0                     0x87
#define STA_SEL_PA1                     0x88
#define STA_SEL_PA2                     0x89
#define STA_SEL_PA3                     0x8A
#define STA_SEL_PA4                     0x8B
#define STA_SEL_PA5                     0x8C

//==============================================================================
/*
0x090
(CE)    0   RW  7   SET_BSY4_BUSY_LUN1  SET BSY4 of LUN1
'0' : NOP
'1' : Set BSY4 to busy, and this bit would be auto cleared.
    0   RW  6   SET_BSY4_RDY_LUN1   SET BSY4 to ready of LUN1
'0' : NOP
'1' : Set BSY4 to ready, and this bit would be auto cleared.
    0   RW  5   SET_BSY3_BUSY_LUN1  SET BSY3 of LUN1
'0' : NOP
'1' : Set BSY3 to busy, and this bit would be auto cleared.
    0   RW  4   SET_BSY3_RDY_LUN1   SET BSY3 to ready of LUN1
'0' : NOP
'1' : Set BSY3 to ready, and this bit would be auto cleared.
    0   RW  3   SET_BSY2_BUSY_LUN1  SET BSY2 of LUN1
'0' : NOP
'1' : Set BSY2 to busy, and this bit would be auto cleared.
    0   RW  2   SET_BSY2_RDY_LUN1   SET BSY2 to ready of LUN1
'0' : NOP
'1' : Set BSY2 to ready, and this bit would be auto cleared.
    0   RW  1   SET_BSY1_BUSY_LUN1  SET BSY1 of LUN1
'0' : NOP
'1' : Set BSY1 to busy, and this bit would be auto cleared.
    0   RW  0   SET_BSY1_RDY_LUN1   SET BSY1 to ready of LUN1
'0' : NOP
'1' : Set BSY1 to ready, and this bit would be auto cleared.
*/
//==============================================================================
#define SET_BSY4_1_BUSY_RDY_LUN1        0x90
#define SET_BSY8_5_BUSY_RDY_LUN1        0x91

#define SET_BSY4_1_BUSY_RDY_LUN2        0x92
#define SET_BSY8_5_BUSY_RDY_LUN2        0x93

#define SET_BSY4_1_BUSY_RDY_LUN3        0x94
#define SET_BSY8_5_BUSY_RDY_LUN3        0x95

#define SET_BSY4_1_BUSY_RDY_LUN4        0x96
#define SET_BSY8_5_BUSY_RDY_LUN4        0x97

#define SET_BSY4_1_BUSY_RDY_LUN5        0x98
#define SET_BSY8_5_BUSY_RDY_LUN5        0x99

#define SET_BSY4_1_BUSY_RDY_LUN6        0x9A
#define SET_BSY8_5_BUSY_RDY_LUN6        0x9B

#define SET_BSY4_1_BUSY_RDY_LUN7        0x9C
#define SET_BSY8_5_BUSY_RDY_LUN7        0x9D


//==============================================================================
/*
0x0A0
(CE)    8'h10   R/W 7:0 RDY_STAT_DLY1_LUN1  Configure programmable 256-stage delay for BSY1 of LUN1 delay wire
0x0A1
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY2_LUN1  Configure programmable 256-stage delay for BSY2 of LUN1 delay wire
0x0A2
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY3_LUN1  Configure programmable 256-stage delay for BSY3 of LUN1 delay wire
0x0A3
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY4_LUN1  Configure programmable 256-stage delay for BSY4 of LUN1 delay wire
0x0A4
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY5_LUN1  Configure programmable 256-stage delay for BSY5 of LUN1 delay wire
0x0A5
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY6_LUN1  Configure programmable 256-stage delay for BSY6 of LUN1 delay wire
0x0A6
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY7_LUN1  Configure programmable 256-stage delay for BSY7 of LUN1 delay wire
0x0A7
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY8_LUN1  Configure programmable 256-stage delay for BSY8 of LUN1 delay wire
0x0A8
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY1_LUN2  Configure programmable 256-stage delay for BSY1 of LUN2 delay wire
0x0A9
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY2_LUN2  Configure programmable 256-stage delay for BSY2 of LUN2 delay wire
0x0AA
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY3_LUN2  Configure programmable 256-stage delay for BSY3 of LUN2 delay wire
0x0AB
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY4_LUN2  Configure programmable 256-stage delay for BSY4 of LUN2 delay wire
0x0AC
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY5_LUN2  Configure programmable 256-stage delay for BSY5 of LUN2 delay wire
0x0AD
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY6_LUN2  Configure programmable 256-stage delay for BSY6 of LUN2 delay wire
0x0AE
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY7_LUN2  Configure programmable 256-stage delay for BSY7 of LUN2 delay wire
0x0AF
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY8_LUN2  Configure programmable 256-stage delay for BSY8 of LUN2 delay wire
0x0BX
0x0B0
(CE)    8'h10   R/W 7:0 RDY_STAT_DLY1_LUN3  Configure programmable 256-stage delay for BSY1 of LUN3 delay wire
0x0B1
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY2_LUN3  Configure programmable 256-stage delay for BSY2 of LUN3 delay wire
0x0B2
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY3_LUN3  Configure programmable 256-stage delay for BSY3 of LUN3 delay wire
0x0B3
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY4_LUN3  Configure programmable 256-stage delay for BSY4 of LUN3 delay wire
0x0B4
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY5_LUN3  Configure programmable 256-stage delay for BSY5 of LUN3 delay wire
0x0B5
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY6_LUN3  Configure programmable 256-stage delay for BSY6 of LUN3 delay wire
0x0B6
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY7_LUN3  Configure programmable 256-stage delay for BSY7 of LUN3 delay wire
0x0B7
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY8_LUN3  Configure programmable 256-stage delay for BSY8 of LUN3 delay wire
0x0B8
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY1_LUN4  Configure programmable 256-stage delay for BSY1 of LUN4 delay wire
0x0B9
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY2_LUN4  Configure programmable 256-stage delay for BSY2 of LUN4 delay wire
0x0BA
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY3_LUN4  Configure programmable 256-stage delay for BSY3 of LUN4 delay wire
0x0BB
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY4_LUN4  Configure programmable 256-stage delay for BSY4 of LUN4 delay wire
0x0BC
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY5_LUN4  Configure programmable 256-stage delay for BSY5 of LUN4 delay wire
0x0BD
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY6_LUN4  Configure programmable 256-stage delay for BSY6 of LUN4 delay wire
0x0BE
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY7_LUN4  Configure programmable 256-stage delay for BSY7 of LUN4 delay wire
0x0BF
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY8_LUN4  Configure programmable 256-stage delay for BSY8 of LUN4 delay wire
0x0CX
0x0C0
(CE)    8'h10   R/W 7:0 RDY_STAT_DLY1_LUN5  Configure programmable 256-stage delay for BSY1 of LUN5 delay wire
0x0C1
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY2_LUN5  Configure programmable 256-stage delay for BSY2 of LUN5 delay wire
0x0C2
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY3_LUN5  Configure programmable 256-stage delay for BSY3 of LUN5 delay wire
0x0C3
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY4_LUN5  Configure programmable 256-stage delay for BSY4 of LUN5 delay wire
0x0C4
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY5_LUN5  Configure programmable 256-stage delay for BSY5 of LUN5 delay wire
0x0C5
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY6_LUN5  Configure programmable 256-stage delay for BSY6 of LUN5 delay wire
0x0C6
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY7_LUN5  Configure programmable 256-stage delay for BSY7 of LUN5 delay wire
0x0C7
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY8_LUN5  Configure programmable 256-stage delay for BSY8 of LUN5 delay wire
0x0C8
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY1_LUN6  Configure programmable 256-stage delay for BSY1 of LUN6 delay wire
0x0C9
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY2_LUN6  Configure programmable 256-stage delay for BSY2 of LUN6 delay wire
0x0CA
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY3_LUN6  Configure programmable 256-stage delay for BSY3 of LUN6 delay wire
0x0CB
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY4_LUN6  Configure programmable 256-stage delay for BSY4 of LUN6 delay wire
0x0CC
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY5_LUN6  Configure programmable 256-stage delay for BSY5 of LUN6 delay wire
0x0CD
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY6_LUN6  Configure programmable 256-stage delay for BSY6 of LUN6 delay wire
0x0CE
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY7_LUN6  Configure programmable 256-stage delay for BSY7 of LUN6 delay wire
0x0CF
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY8_LUN6  Configure programmable 256-stage delay for BSY8 of LUN6 delay wire

*/

//==============================================================================
#define RDY_STAT_DLY1_LUN1              0xA0
#define RDY_STAT_DLY1_LUN2              0xA8
#define RDY_STAT_DLY1_LUN3              0xB0
#define RDY_STAT_DLY1_LUN4              0xB8
#define RDY_STAT_DLY1_LUN5              0xC0
#define RDY_STAT_DLY1_LUN6              0xC8

//.
//.
#define SUB_BSY_COUNT_LUN               0x08

//==============================================================================
/*
0x0D0
(CE)    0   R   7:0 CTL_CUR_CMD Latch current command data
0x0D1
(CE)    0   R   7:0 CTL_PREV_CMD1   Latch previous CTL_CUR_CMD command data
0x0D2
(CE)    0   R   7:0 CTL_PREV_CMD2   Latch previous CTL_PREV_CMD1 command data
0x0D3
(CE)    0   R   7:0 CTL_PREV_CMD3   Latch previous CTL_PREV_CMD2 command data
0x0D4
(CE)    0   R   7:0 CTL_PREV_CMD4   Latch previous CTL_PREV_CMD3 command data
*/
//==============================================================================
#define CTL_CUR_CMD                     0xD0
#define CTL_PREV_CMD1                   0xD1
#define CTL_PREV_CMD2                   0xD2
#define CTL_PREV_CMD3                   0xD3
#define CTL_PREV_CMD4                   0xD4

//==============================================================================
/*
0x0D5
(CE)    0   RW  6:0 STA_READ_PTR
*/
//==============================================================================
#define STA_READ_PTR                    0xD5


//==============================================================================
/*
0x0D8
(CE)    8'h10   R/W 7:0 RDY_STAT_DLY1_LUN7  Configure programmable 256-stage delay for BSY1 of LUN7 delay wire
0x0D9
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY2_LUN7  Configure programmable 256-stage delay for BSY2 of LUN7 delay wire
0x0DA
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY3_LUN7  Configure programmable 256-stage delay for BSY3 of LUN7 delay wire
0x0DB
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY4_LUN7  Configure programmable 256-stage delay for BSY4 of LUN7 delay wire
0x0DC
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY5_LUN7  Configure programmable 256-stage delay for BSY5 of LUN7 delay wire
0x0DD
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY6_LUN7  Configure programmable 256-stage delay for BSY6 of LUN7 delay wire
0x0DE
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY7_LUN7  Configure programmable 256-stage delay for BSY7 of LUN7 delay wire
0x0DF
 (CE)   8'h10   R/W 7:0 RDY_STAT_DLY8_LUN7  Configure programmable 256-stage delay for BSY8 of LUN7 delay wire

*/
//==============================================================================
#define RDY_STAT_DLY1_LUN7              0xD8
#define RDY_STAT_DLY8_LUN7              0xDF


//==============================================================================
/*
0x0E0
(CE)    0   RW  7:0 CMD90_ID7_2 Command 90 ID2 output #7
0x0E1
(CE)    0   RW  7:0 CMD90_ID6_2 Command 90 ID2 output #6
0x0E2
(CE)    0   RW  7:0 CMD90_ID5_2 Command 90 ID2 output #5
0x0E3
(CE)    0   RW  7:0 CMD90_ID4_2 Command 90 ID2 output #4
0x0E4
(CE)    0   RW  7:0 CMD90_ID3_2 Command 90 ID2 output #3
0x0E5
(CE)    0   RW  7:0 CMD90_ID2_2 Command 90 ID2 output #2
0x0E6
(CE)    0   RW  7:0 CMD90_ID1_2 Command 90 ID2 output #1
0x0E7
(CE)    0   RW  7:0 CMD90_ID0_2 Command 90 ID2 output #0
0x0E8
(CE)    0   RW  7:0 CMD90_ID7_3 Command 90 ID3 output #7
0x0E9
(CE)    0   RW  7:0 CMD90_ID6_3 Command 90 ID3 output #6
0x0EA
(CE)    0   RW  7:0 CMD90_ID5_3 Command 90 ID3 output #5
0x0EB
(CE)    0   RW  7:0 CMD90_ID4_3 Command 90 ID3 output #4
0x0EC
(CE)    0   RW  7:0 CMD90_ID3_3 Command 90 ID3 output #3
0x0ED
(CE)    0   RW  7:0 CMD90_ID2_3 Command 90 ID3 output #2
0x0EE
(CE)    0   RW  7:0 CMD90_ID1_3 Command 90 ID3 output #1
0x0EF
(CE)    0   RW  7:0 CMD90_ID0_3 Command 90 ID3 output #0
*/
//==============================================================================
#define CMD90_ID7_2                    0xE0
#define CMD90_ID6_2                    0xE1
#define CMD90_ID5_2                    0xE2
#define CMD90_ID4_2                    0xE3
#define CMD90_ID3_2                    0xE4
#define CMD90_ID2_2                    0xE5
#define CMD90_ID1_2                    0xE6
#define CMD90_ID0_2                    0xE7

#define CMD90_ID7_3                    0xE8
#define CMD90_ID6_3                    0xE9
#define CMD90_ID5_3                    0xEA
#define CMD90_ID4_3                    0xEB
#define CMD90_ID3_3                    0xEC
#define CMD90_ID2_3                    0xED
#define CMD90_ID1_3                    0xEE
#define CMD90_ID0_3                    0xEF


//==============================================================================
/*
0x0F0
(CE)    0   R/W 2:0 DPS_SEL_LUN_bit0    PBA determines the DPS SRAM of LUN by vendor command (i.e.Fn-0x55-addr-data) Fn to using control registers DPS_SEL_LUN_bit0,DPS_SEL_LUN_bit1, PS_SEL_LUN_bit2.
0x0F1
(CE)    0   R/W 2:0 DPS_SEL_LUN_bit1
0x0F2
(CE)    0   R/W 2:0 DPS_SEL_LUN_bit2

*/
//==============================================================================
#define DPS_SEL_LUN_BIT0               0xF0
#define DPS_SEL_LUN_BIT1               0xF1
#define DPS_SEL_LUN_BIT2               0xF2


//==============================================================================
/*
0x0F3
(CE)    0   R   4:1 CFDE_TAG_99_96  Assert when CFDE special event occur for corresponding tags. All tags are cleared when CLR_CFDE_TAG set.
        0   W   0   CLR_CFDE_TAG
0x0F4
(CE)    0   R   7:0 CFDE_TAG_7_0
0x0F5
(CE)    0   R   7:0 CFDE_TAG_15_8
0x0F6
(CE)    0   R   7:0 CFDE_TAG_23_16
0x0F7
(CE)    0   R   7:0 CFDE_TAG_31_24
0x0F8
(CE)    0   R   7:0 CFDE_TAG_39_32
0x0F9
(CE)    0   R   7:0 CFDE_TAG_47_40
0x0FA
(CE)    0   R   7:0 CFDE_TAG_55_48
0x0FB
(CE)    0   R   7:0 CFDE_TAG_63_56
0x0FC
(CE)    0   R   7:0 CFDE_TAG_71_64
0x0FD
(CE)    0   R   7:0 CFDE_TAG_79_72
0x0FE
(CE)    0   R   7:0 CFDE_TAG_87_80
0x0FF
(CE)  0 R   7:0 CFDE_TAG_95_88
*/
//==============================================================================

#define CFDE_TAG_99_96                 0xF3
#define CLR_CFDE_TAG               SET_BIT0
#define CLR_ALL_CFDE_TAG()         (_setreg8(PBAREG[CLR_CFDE_TAG], BIT0))

#define CFDE_TAG_7_0                   0xF4
#define CFDE_TAG_15_8                  0xF5
#define CFDE_TAG_23_16                 0xF6
#define CFDE_TAG_31_24                 0xF7
#define CFDE_TAG_39_32                 0xF8
#define CFDE_TAG_47_40                 0xF9
#define CFDE_TAG_55_48                 0xFA
#define CFDE_TAG_63_56                 0xFB
#define CFDE_TAG_71_64                 0xFC
#define CFDE_TAG_79_72                 0xFD
#define CFDE_TAG_87_80                 0xFE
#define CFDE_TAG_95_88                 0xFF




/*--------------------------------------------------------------------------*/
/* HW UART  DEFINITION  0xE400 ~ 0xE7FF                                     */
/*--------------------------------------------------------------------------*/
#define HW_UART_OFS 0x33 
#define HW_UART_Enable SET_BIT7
#define MDLL_SWITCH         BIT6

#define HW_FUART_REG ((volatile unsigned char *)0x2E400UL)
#define MAX_FIFO_DEPTH        0x10
#define Receive_Buf           0x00
#define Transmit_Holding      0x00 
#define Divisor_Latch_H       0x04
#define Divisor_Latch_L       0x00
#define Interrupt_Identity    0x08
#define FIFO_Control          0x08
#define Line_Control          0x0C
#define Transmit_FIFO_Level   0x80
#define Software_Reset        0x88






/*--------------------------------------------------------------------------*/
/* COVERTION DEFINITION                                                         */
/*--------------------------------------------------------------------------*/

#define COVREG ((volatile unsigned char *)0x2F780UL)

#define COV_START    0x00
#define COV_MODE     0x04
#define COV_SOC_ADR  0x08
#define COV_PARA     0x0C
#define COV_SPR0     0x10
#define COV_SPR1     0x14
#define COV_SPR2     0x18
#define COV_SPR3     0x1C
#define COV_PAGE_L   0x20
#define COV_PAGE_H   0x21
#define COV_INIT     0x24



/*--------------------------------------------------------------------------*/
/* MACRO DEFINITION                                                         */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/* TYPE DEFINITION                                                          */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/* GLOBAL VARIABLES DEFINITION                                              */
/*--------------------------------------------------------------------------*/
//EXTERN xdata unsigned char error_state_RDY = 0;
EXTERN xdata unsigned char error_state_RDY;

/*--------------------------------------------------------------------------*/
/* GLOBAL FUNCTION DECLARATION                                              */
/*--------------------------------------------------------------------------*/
extern void USB_DEBUG_MSG(unsigned char a, unsigned char b);
EXTERN void Init_PBA_Sys_Reg(void);
EXTERN void init_STA_Buf_Def(void);
EXTERN int dump_CFDE_block(unsigned char CFDE_no);
EXTERN int test_write_PBA_cache(unsigned char sec_cnt);
EXTERN int test_read_PBA_cache(unsigned char sec_cnt);
EXTERN BOOL set_FlashID(void);

//new
EXTERN void pba_initial_sys_reg(void);
EXTERN BOOL pba_set_info_page(void);
EXTERN void pba_initial_status_buf(void);
EXTERN int pba_fill_CFDE_blk(void);
EXTERN BOOL pba_set_rdy_page(void);
EXTERN void pba_clear_allceint_and_setallrdy(void);
EXTERN void pba_clear_int_and_setrdy(U8 byCeIdx);
EXTERN int pba_fill_Cache_Header(void);
EXTERN int pba_get_dps_ram_data(U8 byflashType, U8 byCeIdx, U8 byLunIdx);
EXTERN int pba_Set_dps_ram_data(U8 byflashType, U8 byCeIdx, U8 byLunIdx,U8 Set_Ready);
EXTERN int pba_fill_SG_buf_data(U8 byflashType, U8 byCeIdx);
EXTERN int pba_switch_flash_type(U8 byflashType, U8 byCeIdx);
EXTERN int pba_get_Tag_Info(U8 byCeIdx);
EXTERN int pba_Set_Ready_Busy(BOOL RDY,U8 byCeIdx,U8 byLunIdx);
EXTERN void pba_set_delay(U32 dwDelayCnt);
EXTERN BOOL pba_Initial_Emulator(void);

#undef EXTERN
#ifdef __cplusplus
}
#endif
#endif // __PBA_API_H__
