#include "nes.h"
#ifdef D_NES_CMODEL
#include "8051.h"
#else
#include <reg52.h>
#include <8051.h>
#endif
#include <string.h>
#include "Var_Def.h"
#include "MCU_Reg.h"
#include "USB_API.h"
#include "Buf_Def.h"
#include "GPIO_Reg.h"
#include "GPIO_API.h"
//==================================================================================================================

#define OUT     0x80
//#define IN        0x00

#define ENABLE  0x80
//#define DISABLE   0x00

void Init_GPIO_State(void)
{
    //================================================================================
    //                          [ GPIO_A ]
    //================================================================================
    // I/O      Name                In/Out  Default     Pull_H_L
    // D0       IO_FW_MODE          I                    Pull_H
    // D1       IO_SPI_IDEL_STATE   I                    Pull_H
    // D2       IO_SPI_SPD          I                    Pull_H
    // D3       IO_CLR_WDT          I                    Pull_H
    // D4                           I                    Pull_H
    // D5                           I                    Pull_H
    // D6       IO_REF_R_SEL        I                    Pull_H
    // D7       IO_CRYSTAL_SEL      I                    Pull_H
    //--------------------------------------------------------------------------------
    //Set function GPIO pull up
    GPIOREG[GPIO_A_PU] = 0xFF;
    GPIOREG[GPIO_A_PD] = 0x00;
    GPIOREG[GPIO_A_OE] = 0x00;

    //================================================================================
    //                          [ GPIO_B ]
    //================================================================================
    // I/O      Name                    In/Out  Default     Pull_H_L
    // D0       WDI (WATCHDOG_IC)       O       H            Pull_H
    // D1       [暫時] IO_SPD_BIT0      I                    Pull_H     // IO_SPD_BIT0 ~ IO_SPD_BIT2
    // D2       [暫時] IO_SPD_BIT1      I                    Pull_H     // SD ==> 000: No DDR / (BIT0==1):DDR
    // D3       [暫時] IO_SPD_BIT2      I                    Pull_H     // MMC==> 000: Default / 001: High / 010: UHS12 / 011: UHS25 / 100: UHS50 / 101: UHS104 / 110: DDR50
    // D4       [暫時] IO_BUS_WIDTH_L   I                    Pull_H     // HL==> 00:1Bit / 01: 4Bit / 10: 8 Bit
    // D5       [暫時] IO_BUS_WIDTH_H   I                    Pull_H
    // D6       [暫時] Card_Voltage_L   I                    Pull_H
    // D7       [暫時] Card_Voltage_H   I                    Pull_H
    //--------------------------------------------------------------------------------
    //Set function GPIO pull up
    GPIOREG[GPIO_B_PU] = 0xff;
    GPIOREG[GPIO_B_PD] = 0x00;
    GPIOREG[GPIO_B_OE] = (BIT0);


    //================================================================================
    //                          [ GPIO_C ]
    //================================================================================
    // I/O      Name                    In/Out  Default     Pull_H_L
    // D0       IO_PWR_SHORT_SW_1       O       L            Pull_H
    // D1       IO_PWR_SHORT_FLAG_1     I                    Pull_H
    // D2       IO_PWR_SHORT_SW_2       O       L            Pull_H
    // D3       IO_PWR_SHORT_FLAG_2     I                    Pull_H
    // D4       IO_ADC_CONV             O       L            Pull_H
    // D5       IO_PGA113_CS            O       L            Pull_H
    // D6       IO_CMD_PULL_LOW_SW      O       L            Pull_H
    // D7       IO_PWR_DISCHARGE_SW     O       H            Pull_H
    //--------------------------------------------------------------------------------
    //Set function GPIO pull up
    GPIOREG[GPIO_C_PU] = 0xff;
    GPIOREG[GPIO_C_PD] = 0x00;
    GPIOREG[GPIO_C_OE] = (BIT0 + BIT2 + BIT4 + BIT5 + BIT6 + BIT7);
    GPIOREG[GPIO_C_O] = (BIT7);


    //================================================================================
    //                          [ GPIO_D ]
    //================================================================================
    // I/O      Name                In/Out  Default     Pull_H_L
    // D0       IO_CTL_CURR_H_SW    O       L           No_Pull
    // D1       IO_CTL_CURR_L_SW    O       L           No_Pull
    // D2       IO_CARD_IO_PWR_SEL  O       H           No_Pull
    // D3       IO_EMMC_RST         O       H           No_Pull
    // D4       IO_CTL_Power        O       L           No_Pull
    // D5       IO_FLH_Power        O       L           No_Pull
    // D6       IO_FLH_CURR_H_SW    O       L           No_Pull
    // D7       IO_FLH_CURR_L_SW    O       L           No_Pull
    //--------------------------------------------------------------------------------
    //Set function GPIO pull up
    GPIOREG[GPIO_D_PU] = 0x00;
    GPIOREG[GPIO_D_PD] = 0x00;
    GPIOREG[GPIO_D_OE] = 0xFF;
    GPIOREG[GPIO_D_O] = (BIT2 + BIT3);


}

