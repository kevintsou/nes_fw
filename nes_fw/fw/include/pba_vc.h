/**
 * @file ph_ap.h
 * Copyrihgt 2015 PHISON Electronics Corp.
 *
 * <b>
@verbatim
FILE NAME :  ph_ap.h
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

#ifndef __PBA_VC_H__
#define __PBA_VC_H__

#ifdef __PBA_VC_C__
#define EXTERN
#else
#define EXTERN extern
#endif

/*--------------------------------------------------------------------------*/
/* PREDEFINED DIRECTIVE                                                     */
/*--------------------------------------------------------------------------*/
#define EN_USING_PBA_TMP_BUF        (0)
#define EN_BULK_IN_OUT_MULTI_PLANE  (1)
#define EN_DYNAMIC_SET_CFDE_UNIT    (0)


/*--------------------------------------------------------------------------*/
/* INCLUDE FILE                                                             */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/* CONSTANT DECLARATION                                                     */
/*--------------------------------------------------------------------------*/
#define AD5245 0x2C //Socket Power
#define AD5245_2 0x2D   //IO Power
#define PCA9534 0x20

//Vendoe command definition
#define VC_MARK1  (0x06)
#define VC_MARK2  (0xF1)


// Buffer 133(0x85)
// Size : 1 buffer(512 bytes)
// purpose : Info Block use
#define FLAG_BASENO_INFO_BLK            0x85
#define INFO_BLK_ADR        0x20A00UL
#define INFO_BLK_BASE      ((unsigned char *)INFO_BLK_ADR)

//FW version is 2.02.05
#define FW_VER_H     (0x02)
#define FW_VER_L     (0x02)
#define FW_VER_MINOR (0x05)

//Date is 2015/09/14
#define FW_YEAR      (0x14)
#define FW_MON       (0x09)
#define FW_DAY       (0x15)

#define LEGACY_MODE     0x00
#define TOGGLE_MODE     0x01


#define REG_GLOBAL_GAP       0x100
#define CE_REG_GAP           0x100

/*--------------------------------------------------------------------------*/
/* MACRO DEFINITION                                                         */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/* TYPE DEFINITION                                                          */
/*--------------------------------------------------------------------------*/
enum VC_TYPE_SEGMENT1 {
    VC_USB_BULK_IN             = 0x01,
    VC_USB_BULK_OUT            = 0x02,
    VC_USB_BULK_OUT_MULTIPLANE = 0x04,
    VC_USB_BULK_IN_MULTIPLANE  = 0x05,
    VC_PBA_CACHE_WRITE_TEST    = 0x90,
    VC_PBA_CACHE_READ_TEST     = 0x91,
    VC_PBA_DBG_SEL             = 0x92,
    VC_GET_TAG_INFO            = 0xA0,
    VC_FILL_CFDE_BLK           = 0xA1,
    VC_SET_INFO_PAGE           = 0xA2,
    VC_SET_DPS_RAM_DATA        = 0xA3,
    VC_READ_REQUEST_DATA       = 0xA4,
    VC_SET_RDY_PAGE            = 0xA5,
    VC_SET_FLH_ID              = 0xA6,
    VC_DMA_MEMSET_BUF          = 0xA7,
    VC_DUMP_CFDE_BLK           = 0xA8,
    VC_WR_GN_BUF               = 0xA9,
    VC_RD_GN_BUF               = 0xAA,
    VC_GET_DPS_RAM_DATA        = 0xAC,
    VC_DMA_MEMCPY_BUF          = 0xAD,
    VC_GET_RDY_PAGE_DATA       = 0xAE,
    VC_GET_SINGLE_BUF_DATA     = 0xAF
};

enum VC_TYPE_SEGMENT2 {
    VC_GET_PBA_REG             = 0xB0,
    VC_SET_GPIO_EN_DIS         = 0xB1,
    VC_SET_GPIO_VALUE          = 0xB2,
    VC_FORCE_H_IO_STATE        = 0xB4,
    VC_SET_STATUS_BUF          = 0xB5,
    VC_GET_CFDE_REQUEST        = 0xB6,
    VC_SWITCH_FLH_MODE         = 0xB7,
    VC_CLR_BUS_ANA_STATUS      = 0xB8,
    VC_GET_BUS_ANA_INT_REQUEST = 0xB9,
    VC_GET_SYS_REG_INFO        = 0xBA,
    VC_SET_SYS_REG_INFO        = 0xBB,
    VC_GET_BUS_ANA_CC19_TEST   = 0xBC,
    VC_GET_INVAILD_CFDE_REQ    = 0xBD,
    VC_SET_REWRITE_FLASH_ID    = 0xBE,
    VC_Get_Add_From_SRAM       = 0xBF,
    VC_GET_CACHE_HEAD          = 0xC0,
    VC_EN_DIS_CFDE_UNIT        = 0xC1,
    VC_CLR_PWR_CHG_INT         = 0xC2,
    VC_GET_CACHE_HEADER_INF0   = 0xC3,
    VC_SET_READY_BUSY          = 0xC4,
    VC_Covertion               = 0xC5,
    VC_SET_IO_DRVING           = 0xC6,
    VC_TRIG_HOST_WRITE         = 0xC7,
    VC_GET_PBA_REG_Cache       = 0xC8,
    VC_GET_HOST_REG            = 0xC9,
    VC_TRIG_HOST_READ          = 0xCA,
    VC_GET_SYS_REG             = 0xCB,
    VC_WR_SYS_REG              = 0xCC,
    VC_GET_IO_REG              = 0xCD,
    VC_Set_Differential_DQS    = 0xCE,
    VC_Reset_NE_2G             = 0xCF,
    VC_SET_CLEAR_INT_CMD       = 0xD0,
    VC_GET_CH_INFO_BY_RANGE    = 0xD1,
};



//////////////////////////////////////////////////////////////


#define       DQ_PU5P5_PU75_PD   0x22   
#define       RE_RSEL_PU_PD      0x2F
#define       DQS_RSEL_PU_PD     0x31

//////////////////////////////////////////////////////////////////
#define      Host_DQ_CONOF_SONOF  0x34
#define      Host_DQ_SR_DTY       0x71
#define      Host_DQ_PMODE_ODTEN_IOLH   0x70
#define      Host_DQ_CAL_N        0x72
#define      Host_DQ_CAL_P        0x73
#define      Host_A0_D0_VREF_REN  0x88
#define      Host_DQ_TX_RX_DCTRL_D0    0xBA
#define      Host_DQ_TX_RX_DCTRL_D1    0xB9
#define      Host_DQ_TX_RX_DCTRL_D2    0xB8
#define      Host_DQ_TX_RX_DCTRL_D3    0xBF
#define      Host_DQ_TX_RX_DCTRL_D4    0xBE
#define      Host_DQ_TX_RX_DCTRL_D5    0xBD
#define      Host_DQ_TX_RX_DCTRL_D6    0xBC
#define      Host_DQ_TX_RX_DCTRL_D7    0xC1

//////////////////////////////////////////////////////////////////
#define      Host_RE_CONOF_SONOF                    0x3A
#define      Host_RE_SR_CALN                        0x7E
#define      Host_RE_SR_SRIOB_DTY                   0x7D
#define      Host_RE_SRIOB_IOLH_PMODE_ODTEN         0x7C
#define      Host_RE_OutputEN_CALP                  0x7F
#define      Host_RE_TX_RX_DCTRL                    0xC0
////////////////////////////////////////////////////////////////////
#define Host_DQS_CONOF_SONOF      0x3B
#define Host_DQS_SR_CALN          0x6E
#define Host_DQS_SR_SRIOB_DTY     0x6D
#define Host_DQS_SRIOB_IOLH_PMODE_ODTEN   0x6C
#define Host_DQS_OutputEN_CALP        0x6F
#define Host_DQS_TX_RX_DCTRL     0xBB
//////////////////////////////////////////////////////////////////////
#define  Host_WE_CONOF_SONOF  0x37
#define  Host_WE_PMODE_ODTEN_IOLH    0x24
#define  Host_WE_SR_DTY             0x25
#define  Host_WE_CAL_N              0x26
#define  Host_WE_CAL_P              0x27


//////////////////////////////////////////////////////////////////////
#define      PBA_DQ_CONOF_SONOF  0x23
#define      PBA_DQ_SR_DTY       0x15
#define      PBA_DQ_PMODE_ODTEN_IOLH   0x14
#define      PBA_DQ_CAL_N        0x16
#define      PBA_DQ_CAL_P        0x17

#define      PBA_DQS_TX_RX_DCTRL      0xAF
#define      PBA_DQ_TX_RX_DCTRL_D0    0xAE
#define      PBA_DQ_TX_RX_DCTRL_D1    0xAD
#define      PBA_DQ_TX_RX_DCTRL_D2    0xAC
#define      PBA_DQ_TX_RX_DCTRL_D3    0xB3
#define      PBA_DQ_TX_RX_DCTRL_D4    0xB2
#define      PBA_DQ_TX_RX_DCTRL_D5    0xB1
#define      PBA_DQ_TX_RX_DCTRL_D6    0xB0
#define      PBA_DQ_TX_RX_DCTRL_D7    0xB5
#define      PBA_DQ_ODT_CAL_N         0xC8
#define      PBA_DQ_ODT_CAL_P         0xC9


#define      F140_OFS                0x800 
/////////////////////////////////////////////////////////////////////

#define		 PBA_RE_CONOF_SONOF                 0x30
#define      PBA_RE_SR_DTY                      0x21
#define      PBA_RE_SRIOB_IOLH_PMODE_ODTEN      0x20
#define      PBA_RE_IOLH_CALN                   0x22
#define      PBA_RE_CAL_P                       0x23
#define      PBA_RE_TX_RX_DCTRL                 0xB4


/////////////////////////////////////////////////////////////////////
#define  PBA_DQS_CONOF_SONOF                      0x32
#define  PBA_DQS_SR_DTY                           0x11
#define  PBA_DQS_SRIOB_IOLH_PMODE_ODTEN           0x10
#define  PBA_DQS_IOLH_CALN                        0x12
#define  PBA_DQS_CAL_P                            0x13
#define  PBA_DQS_TX_RX_DCTRL                      0xAF
#define  PBA_DQS_ODT_CAL_P                        0xC5                       
#define  PBA_DQS_ODT_CAL_N                        0xC4

///////////////////////////////////////////////////////////////////
#define  GPIO_IOLH_ODTEN_PMODE                   0xA0
#define  GPIO_SR_DTY                             0xA1
#define  GPIO_CAL_N                              0xA2
#define  GPIO_CAL_P                              0xA3

///////////////////////////////////////////////////////////////////
#define  DRIVE_CONFIG_RE_TRIGGER                 0x0
#define  DRIVE_CONFIG_DQS_STRENGTH               0x1
#define  DRIVE_CONFIG_DQ_STRENGTH                0x2

/*
enum VC_TYPE_ISP {
    VC_ISP_FW                  = 0xB1,
    VC_VERIFY_FW               = 0xB2,
    VC_JUMP_PRAM               = 0xB3
};
*/

/*--------------------------------------------------------------------------*/
/* LOCAL GLOBAL VARIABLES DEFINITION                                        */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTION PROTOTYPE                                                 */
/*--------------------------------------------------------------------------*/
static void multi_dma_read(void);
static void dma_read_using_temp_buf(U8 byBufNum, U16 wSecLen);

#undef EXTERN
#endif // __PBA_VC_H__
