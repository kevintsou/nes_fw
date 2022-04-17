#ifndef __MCU_REG__
#define __MCU_REG__

#include "pba_test_api.h"

//==================================================================================================================

#define FARRY ((volatile unsigned char *)0x2F000UL)
#define CLKREG ((volatile unsigned char *)0x2F120UL)
#define SYSREG ((volatile unsigned char *)0x2F140UL)
#define BRAREG ((volatile unsigned char *)0x2F1C0UL)
#define SDCREG ((volatile unsigned char *)0x2F4C0UL)


//==================================================================================================================

//==================================================================================================================
//  Clock Control Registers (Base Address: 0xF120)
//==================================================================================================================
#define CLK_GATE                0x01    /* IP Clock Gating Control */
#define USB_GATE            BIT7
#define MMC_GATE            BIT6
#define DMA_GATE            BIT5
#define CPRM_GATE           BIT4
#define SECU_GATE           BIT3
#define I2C_GATE            BIT2
#define RSPI_GATE           BIT1
#define SPI_GATE            BIT0

#define CLK_SEL                 0x02    /* Clock Select Control */
#define USB_PIPE_CLKSW      BIT7
#define USB_UTMI_CLKSW      BIT6
#define MCU_RA_SYS_CLK_SRC  BIT5
#define USB_ENB             BIT4
#define PHY_U3RCOSC         BIT3
#define PHY_U2RCOSC         BIT2
#define PHY_XTALSEL         BIT1
#define SYS_PWR_OFF         BIT0

#define CLK_MCU                 0x03    /* MCU Clock Control */

#define CLK_GATE1               0x04    /* IP Clock Gating Control1 */
#define UHS2_GATE           BIT0
#define UFS_GATE            BIT4

#define CLK_UHS2_SW             0x05
#define UHS2_REFCLK_EN      BIT7
#define UHS2_EN             BIT3
#define UHS2_NRST           BIT2
// UHS2 Clock Signals
//      assign UHS2_REFCLK_EN = CLK_REG_05[7];
//      assign UHS2_EN              = CLK_REG_05[3];
//      assign UHS2_NRST            = CLK_REG_05[2];


#define CLK_MSPI                0x08

#define CLK_SPI                 0x09    /* SPI Clock Control */




#define   CLK_SYSRST        0x0E


#define CLK_WUCNT               0x0F    /* Wake Up Counter Control */
#define PIPE_CLKRDY         BIT7
#define UTMI_CLKRDY         BIT6
#define PLL_ENB             BIT5

//==================================================================================================================
//  System Control Register (Base Address: 0xF140)
//==================================================================================================================
#define SYS_INTEN               0x00    /* Interrupt Enable Register for wakeup */
#define WAKEUP_INTEN        BIT3

#define SYS_INT                 0x01    /* Interrupt Flag Register */
#define WAKEUP_FLAG         BIT3

#define SYS_MODE                0x05    /* Mode Register */
#define PRAM_ENB            BIT7
#define MODE_HWISP          BIT3
#define MODE_ICE            BIT2
#define MODE_EA             BIT0

#define SYS_MXBP                0x06    /* MOVX Base Page Number Register */

#define SYS_FLAG_BASE           0x07
//------------------------------------------------------------------------
// Buffer Flag Base Number Register (SYS_FLAG_BASE), Address Offset : 0x07
// Field    Attrib  Reset Value Description
// Bit 7~0  RW          'h20    Buffer flag base number
//------------------------------------------------------------------------

#define SYS_FW_FLAG             0x0E    /* System Flag */

#define SYS_REGVDT              0x0F    /* REGVDT Register */
#define REGVDT_RDY          BIT7    //REGVDT detect power ready (for MMC I/F)
#define SYS_REGVDT_SEL1     BIT5    //REGVDT voltage detector setting (for MMC I/F)
#define SYS_REGVDT_SEL0     BIT4
#define SYS_REGVDT_STB      BIT1    //REGVDT standby control
#define SYS_REGVDT_PD       BIT0    //SYS_REGVDT_PD

#define SYS_AES_BP              0x12



#define SYS_MXFXXX_EN           0x1D
#define SYS_CH3_DO              0x1E
#define SYS_CH4_DO              0x1F

#define SYS_MMCR_CD_CFG         0x20    //SD CD pull up / pull down 
#define CD_NOPULUP          0x00
#define CD_PULUP            BIT0
#define CD_PULDOWN          BIT2
#define CD_CMOS_LV_CTL      BIT3
#define CD_STRG_CTL         BIT4

#define SYS_MMCR_CLK_DUTY       0x21    //MMCR CLK/DS Duty control register


#define SYS_MMCR_CLK_CFG        0x22    //SD CLK pull up / pull down 
#define CLK_NOPULUP         0x00
#define CLK_PULUP_60K       (BIT0)
#define CLK_PULUP_30K       (BIT1)
#define CLK_PULUP_20K       (BIT1+BIT0)
#define CLK_PULDOWN         (BIT2)
#define DS_NOPULUP          0x00
#define DS_PULUP_60K        (BIT4)
#define DS_PULUP_30K        (BIT5)
#define DS_PULUP_20K        (BIT5 + BIT4)
#define DS_PULDOWN          (BIT6)

#define SYS_MMCR_CLK_DRN        0x23    //MMCR CLK/DS Configuration 1
#define CLK_DRV0_           BIT0
#define CLK_DRV1_           BIT1
#define CLK_DRV2_           BIT2
#define CLK_CONOF           BIT3
#define CLK_SONOF           BIT4
#define CLK_SR0_            BIT5
#define CLK_SR1_            BIT6
#define CLK_SR2_            BIT7

#define SYS_MMCR_CMD_CFG        0x24    //MMCR CMD Configuration 0
#define CMD_NOPULUP         0x00
#define CMD_PULUP_60K       (BIT0)
#define CMD_PULUP_30K       (BIT1)
#define CMD_PULUP_20K       (BIT1 + BIT0)
#define CMD_PULDOWN         (BIT2)

#define SYS_MMCR_CMD_DRN        0x25    //MMCR CMD Configuration 1
#define CMD_DRV0_           BIT0
#define CMD_DRV1_           BIT1
#define CMD_DRV2_           BIT2
#define CMD_CONOF           BIT3
#define CMD_SONOF           BIT4
#define CMD_SR0_            BIT5
#define CMD_SR1_            BIT6
#define CMD_SR2_            BIT7

#define SYS_MMCR_DS_DRN         0x26    //SD DS driven
#define DS_DRV0_            BIT0
#define DS_DRV1_            BIT1
#define DS_DRV2_            BIT2
#define DS_CONOF            BIT3
#define DS_SONOF            BIT4
#define DS_SR0_             BIT5
#define DS_SR1_             BIT6
#define DS_SR2_             BIT7

#define SYS_MMCR_DAT_CFG        0x27    //SD DAT pull up 
#define DAT_NOPULUP         0x00
#define DAT_PULUP_60K       (BIT0)
#define DAT_PULUP_30K       (BIT1)
#define DAT_PULUP_20K       (BIT1 + BIT0)
#define DAT_PULDOWN         (BIT2)


#define SYS_MMCR_DAT_DRN        0x28    //SD DAT driven
#define DAT_DRV0_           BIT0
#define DAT_DRV1_           BIT1
#define DAT_DRV2_           BIT2
#define DAT_CONOF           BIT3
#define DAT_SONOF           BIT4
#define DAT_SR0_            BIT5
#define DAT_SR1_            BIT6
#define DAT_SR2_            BIT7

/* PBA cache buffer */
#define SYS_PBA_CASEL_CFG0  0x29
#define SYS_PBA_CEB_CFG0    0x29

#define RAM_DEF                 0x00
#define SEL_ORI_MAPPING         0x00
/* 1~64 : Cache0~Cache63
 * 65   : BusAna1
 * 66   : BusAna2
 * 67   : Sysinfo
 */
#define SEL_CACHE0                  0x01
#define SEL_CACHE63                 0x40
#define SEL_BUS_ANA1_BUF            0x41
#define SEL_BUS_ANA2_BUF            0x42
#define SEL_SYS_INFO                0x43

/*
 * CR(GCR,CR0~CR7) Part:256*9(2304Bs)
 * Address Part: 1024*10(10240Bs)
 * Cache Header Info Part: 8*64(512Bs)
 * Cmd Queue for all CE Part: 16*8(128Bs)
 * Total:13184Bs
 */
//#define SYS_INFO_SIZE               (26)
#define SYS_INFO_SIZE               (39)

#define SYS_SPI_CFG             0x2B    //SPI IO Configuration
#define SPI_DRN_E4          BIT0
#define SPI_DRN_E8          BIT1
#define SPI_CONOF           BIT4
#define SPI_SONOF           BIT5

#define SYS_GPSPI_CFG           0x2C    //GP_SPI IO Configuration
#define GPSPI_DRN_E4        BIT0
#define GPSPI_DRN_E8        BIT1
#define GPSPI_CONOF         BIT4
#define GPSPI_SONOF         BIT5

#define SYS_I2C_CFG             0x2D    //I2C IO Configuration
#define I2C_DRN_E4          BIT0
#define I2C_DRN_E8          BIT1
#define I2C_CONOF           BIT4
#define I2C_SONOF           BIT5

#define SYS_UART_CFG            0x2E    //UART IO Configuration
#define UART_DRN_E4         BIT0
#define UART_DRN_E8         BIT1
#define UART_CONOF          BIT4
#define UART_SONOF          BIT5


#define SYS_UART_SEL            0x2F
#define UART_HW_SEL         BIT7


#define CONOF               BIT0
#define SONOF               BIT1
#define PBA_DAT_CONOF            0x23
#define PBA_CLE_CONOF            0x25
#define PBA_ALE_CONOF            0x27
#define PBA_WE_CONOF             0x2A
#define PBA_CE_CONOF             0x2C
#define PBA_RDY_CONOF            0x2E
#define PBA_REB_CONOF            0x30
#define PBA_DQS_CONOF            0x32



//#define SYS_PWR_TEST          0x3D

//#define SYS_DEBUG_SEL         0x3E

//#define SYS_TEST_EN           0x3F

//===========================================================================C=======================================
//  Buffer RAM Control Registers (Base Address: 0xF1C0)
//==================================================================================================================
#define IP_SEL_CH0              0x00    //IP Select CH 0
#define IP_SEL_CH1              BIT0    //IP Select CH 1

#define BRA_MMCR_CH           0x00

#define BRA_UFSRX_CH          0x01

#define BRA_UFSTX_CH          0x02

#define BRA_UHS2_CH           0x03

#define BRA_CPRM_CH           0x04

#define BRA_AES_CH            0x05

#define BRA_SHA_CH            0x06

#define BRA_DMA_CH            0x07

#define BRA_SPI_CH            0x08

#define BRA_MOVX_CH           0x09

#define BRA_MCU_CH            0x0A

#define BRA_PTNGEN_CH         0x0B

#define BRA_USBIN_CH        0x0C

#define BRA_USBOUT_CH         0x0D

#define BRA_CTLSEL              0x18

#define BRA_FBSize              0x19    //BRA_FBSize

#define BRA_FBSAddr             0x1A
//------------------------------------------------------------------------------------
// Buffer RAM Arbiter Flag Bit Start Address Register (BRA_FBSAddr), Address: 0x1A
// Field    Attrib  Reset Value Description
// Bit 7~0  RW          0       Flag Bit Address. It indicates each Flag (AES, Buffer, AlwaysValid). Indicates 0~159 Flags.
//------------------------------------------------------------------------------------

#define BRA_FBLen                   0x1B
//------------------------------------------------------------------------------------
// Buffer RAM Arbiter Flag Bit Length Register (BRA_FBLen), Address: 0x1B
// Field    Attrib  Reset Value Description
// Bit 7~0  RW          0       Indicates how many flags from BRA_FStartBitAddr would be set/clear.
//------------------------------------------------------------------------------------

#define BRA_BUFF_CTRL           0x1D
//------------------------------------------------------------------------------------
// Buffer RAM Arbiter Buffer Flag Control Register (BRA_BUFF_CTRL), Address: 0x1D
// Field    Attrib  Reset Value Description
// Bit 7    RW          0       When write to this register, cpu can set/clear indicated Buffer flags which are selected by BRA_FStartBitAddr and BRA_FBitLen.
//                              1 : set. (ready to be read)
//                              0 : clear. (ready to be written)
// Bit 6~0  RO  0   Reserved.
//------------------------------------------------------------------------------------
#define Ready_To_Read   SET_BIT7
#define RA_SetBufFull       BIT7
#define Ready_To_Write  0
#define RA_Clr_BUF      0

#define BRA_BAV_CTRL            0x1E
//------------------------------------------------------------------------------------
// Buffer RAM Arbiter Buffer Always Valid Control Register (BRA_BAV_CTRL), Address: 0x1E
// Field    Attrib  Reset Value Description
// Bit 7    RW          0       When write to this register, cpu can set/clear indicated Buffer flags which are selected by BRA_FStartBitAddr and BRA_FBitLen.
//                              1 : set. (indicated buffer is always valid)
//                              0 : clear. (valid by buffer flag and read/write direction)
// Bit 6~0  RO  0   Reserved.
//------------------------------------------------------------------------------------


#define BRA_FLAG_SEL            0x1F
//------------------------------------------------------------------------------------
// Buffer RAM Arbiter Flag Selector Register (BRA_FLAG_SEL), Address: 0x1F
// Field        Attrib  Reset   Description
// Bit[7:2]     RW      0       Reserved.
// Bit[1:0]     RW      0       0 : Reserved ;
//                              1 : Buffer Flags ;
//                              2 : Buffer Valid Flags
//------------------------------------------------------------------------------------
#define Sel_Buffer_Flag         0x01
#define Sel_Buffer_Valid_Flag   0x02


#define BRA_FVAL                    0x20    //0x20 ~ 0x33 Buffer RAM arbiter buffer flag VAL register
//==================================================================================================================
//  SDRAM Control Registers (Base Address: 0xF600)
//==================================================================================================================
#define MEM_CTL                     0x00
#define SDRAM                           BIT7
#define PIPE_LINE                   BIT6
#define DBX8n16                     BIT5
#define MEMX8n16                    BIT4


#endif  //#ifndef __MCU_REG__
