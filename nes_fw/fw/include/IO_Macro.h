#ifndef __IO_MACRO__
#define __IO_MACRO__

#define PWR_ON_DELAY_TIME       (50 )
#define PWR_OFF_DELAY_TIME      (100)
#define PWR_ADJUST_DELAY_TIME   (200)

//============================================================================================================
//                                          [ Input I/O Setting ]
//============================================================================================================

//  Not Used I/O List as below
//  GPIO_A7_I
//  GPIO_B1_I

//====================================================================
//  0: Force_BOOT_Code
//  1: Normal_Code
#define IO_FW_MODE              GPIOREG[GPIO_A0_I]
//--------------------------------------------------------------------

//====================================================================
//  IO_SPI_IDEL_STATE   ==> 0: SPI_IDEL_HI      /   1: SPI_IDEL_LOW
//  IO_SPI_SPD          ==> 0: LOW_SPEED_SPI    /   1: HIGH_SPEED_SPI
//--------------------------------------------------------------------
#define IO_SPI_IDEL_STATE       GPIOREG[GPIO_A1_I]
#define IO_SPI_SPD              GPIOREG[GPIO_A2_I]
//--------------------------------------------------------------------


//====================================================================
//  0: No Clear Timer   /   1: Clear timer
#define IO_CLR_WDT              GPIOREG[GPIO_A3_I]
//--------------------------------------------------------------------

//====================================================================
//  0: Have Card Inserted   /   1: No Card Inserted
#define IO_CARD_DETECT          (SDReg_[CD_CHK_] & CD_)
#define IO_CARD_DETECT_DBNC     (SDReg_[CD_CHK_] & CD_DBNC_)
//--------------------------------------------------------------------


//====================================================================
//  Crystal Select
//  0: Select Non-Crystal
//  1: Select External Crystal ( 30MHz )
//--------------------------------------------------------------------
#define IO_USB_CRY_MOD              GPIOREG[GPIO_B6_I]
//--------------------------------------------------------------------

//====================================================================
//  Reference Resistor Select
//  0: Select External REF 1.35K ohm
//  1: Select Internal REF Resistor
//--------------------------------------------------------------------
#define IO_USB_REF_MOD              GPIOREG[GPIO_B7_I]
//--------------------------------------------------------------------


//====================================================================
// SPD Setting
//  Bit 2 1 0   SD_Mode     MMC_Mode
//      0 0 0   Default     No DDR
//      0 0 1   High        DDR (Bit0=1 then DDR)
//      0 1 0   UHS12
//      0 1 1   UHS25
//      1 0 0   UHS50
//      1 0 1   UHS104
//      1 1 0   DDR50
//--------------------------------------------------------------------
#define IO_SPD_BIT0                 GPIOREG[GPIO_B0_I]
#define IO_SPD_BIT1                 GPIOREG[GPIO_B1_I]
#define IO_SPD_BIT2                 GPIOREG[GPIO_B2_I]
//--------------------------------------------------------------------


//====================================================================
//  H L
//  0 0 => 1 Bit BusWidth
//  0 1 => 4 Bit BusWidth
//  1 0 => 8 Bit BusWidth
//--------------------------------------------------------------------

#define IO_BUS_WIDTH_L              GPIOREG[GPIO_A4_I]
#define IO_BUS_WIDTH_H              GPIOREG[GPIO_A5_I]
//--------------------------------------------------------------------



//====================================================================
//  H L
//  0 0 => 3.3V
//  0 1 => 1.8V
//  1 0 => 1.2V

#define IO_CARD_VOLTAGE_L       GPIOREG[GPIO_A6_I]
#define IO_CARD_VOLTAGE_H       GPIOREG[GPIO_A7_I]
//--------------------------------------------------------------------





//============================================================================================================
//                                          [ Output I/O Setting ]
//============================================================================================================


//====================================================================
// MSPI
//--------------------------------------------------------------------
#define IO_ADC_CS0                  GPIOREG[GPIO_C1_O]
#define IO_ADC_CS1                  GPIOREG[GPIO_C0_O]
//--------------------------------------------------------------------


//--------------------------------------------------------------------
#define IO_CTL_CURR_H_SW            GPIOREG[GPIO_D0_O]
#define IO_CTL_CURR_L_SW            GPIOREG[GPIO_D1_O]

//====================================================================
// PW_Card_IO =>SD/MMC Pull High ¹qÀ£¿ï¾Ü
// 0: VCC A2 (SD ¤Á1.8V¥Î)
// 1: VCC A (For Normal Case)
//--------------------------------------------------------------------
#define IO_CARD_IO_PWR_SEL          GPIOREG[GPIO_D2_O]
//--------------------------------------------------------------------
#define IO_CARD_IO_SEL_PWR1()       {IO_CARD_IO_PWR_SEL = 1;}
#define IO_CARD_IO_SEL_PWR2()       {IO_CARD_IO_PWR_SEL = 0;}


#define IO_EMMC_RST                 GPIOREG[GPIO_D3_O]

#define IO_CTL_Power_ON()           GPIOREG[GPIO_D4_O] = 1;
#define IO_CTL_Power_OFF()          GPIOREG[GPIO_D4_O] = 0;
#define IO_FLH_Power_ON()           GPIOREG[GPIO_D5_O] = 1;
#define IO_FLH_Power_OFF()          GPIOREG[GPIO_D5_O] = 0;

#define IO_FLH_CURR_H_SW_ON()       GPIOREG[GPIO_D6_O] = 1;
#define IO_FLH_CURR_H_SW_OFF()      GPIOREG[GPIO_D6_O] = 0;

#define IO_FLH_CURR_L_SW_ON()       GPIOREG[GPIO_D7_O] = 1;
#define IO_FLH_CURR_L_SW_OFF()      GPIOREG[GPIO_D7_O] = 0;
//--------------------------------------------------------------------
#endif
