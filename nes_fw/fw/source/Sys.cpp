#include "nes.h"
#ifdef D_NES_CMODEL
#include "8051.h"
#else
#include <reg52.h>
#include <8051.h>
#endif
#include <string.h>
#include <buf_def.h>
#include "Var_Def.h"
#include "MCU_Reg.h"
#include "USBReg.h"
#include "USB_API.h"
#include "USB_DEF.h"
#include "UART_API.h"
#include "GPIO_API.h"
#include "GPIO_Reg.h"
#include "Sys_api.h"
#include "USB_api.h"
#include "Interrupt_API.h"
#include "a.h"


extern xdata BYTE gb_CurBank;

//==================================================================================================================

//==================================================================================================================
//  FUNCTION DESCRIPTION:
//      Init 8051 Regs.
//
//  INPUT:
//      None
//
//  OUTPUT:
//      None
//
//MODIFIED GLOBAL VARIABLE:
//      None
//
//==================================================================================================================
void Init8051Reg(void)
{
	TMOD       = tmod_gate1_off & tmod_timer1_sel & tmod_mode1_of_t1 &
        		tmod_gate0_off & tmod_timer0_sel & tmod_mode1_of_t0;
	TH0	      = (unsigned char)(61535 >> 8);
	TL0	      = (unsigned char)(61535);
	TH1	      = (unsigned char)(61535 >> 8);
	TL1	      = (unsigned char) 61535;

	IP         = ip_pt2_off & ip_ps_off & ip_pt1_off & ip_px1_off &
	        	ip_pt0_off & ip_px0_on;
	TCON = 0x10;   // need to modify for using different interrupt to wake up
	SCON = 0;
	IE	      = ie_ea_on & ie_et2_off & ie_es_off & ie_et1_off &
	            ie_ex1_off & ie_et0_on& ie_ex0_off ;

	return;
}

//==================================================================================================================
//  FUNCTION DESCRIPTION:
//      Set MCU clock.
//
//  INPUT:
//      ab_SPD_Mod: Bulk speed(1) or setup speed(0).
//
//  OUTPUT:
//      None
//
//MODIFIED GLOBAL VARIABLE:
//      None
//
//==================================================================================================================
void Set_MCU_CLK(BYTE ab_SPD_Mod)
{
    //FPGA U3: 125M, U2: 30M
    //ASIC U3: 250M, U2: 240M
    switch (ab_SPD_Mod) {
        case REF_CLK:
            /* USB reset and MCU/RA/SYS clock source from REF_CLK25
             * MCU clock is sys clock 25Mhz/2 = 12.5Mhz
             */
            CLKREG[CLK_MCU] = 0x00;
            CLKREG[CLK_SEL] &= ~(MCU_RA_SYS_CLK_SRC + USB_ENB);
            break;

        case SETUP_SPD:
            /* 這邊還沒完成 USB_Enum_Complete()，所以IC預設跑U3 Speed Mode 250MHz */

            //CLK = 250Mhz
            CLKREG[CLK_MCU] =0x00; //0x02;     //SysClk is 250M/4 , 62.5Mhz
            TH0_Reload = 0x34;
            TL0_Reload = 0x8D;
            
            /* USB enable and MCU/RA/SYS clock source from PLL(UTMI/PIPE)
             * USB enable, SYS clk src to PLL, active when USB connect
             */
            CLKREG[CLK_SEL] |= (MCU_RA_SYS_CLK_SRC + USB_ENB);

            break;

        case BULK_SPD:
            CLKREG[CLK_SEL] |= (MCU_RA_SYS_CLK_SRC + USB_ENB);

            //RA = 250Mhz
            CLKREG[CLK_MCU] = 0x00;     //SysClk is 250M/4 , 62.5Mhz
            TH0_Reload = 0x34;
            TL0_Reload = 0x8D;
            break;

        case BOOT_SPD:

            //RA = 250Mhz
            CLKREG[CLK_MCU] = 0x00;//0x02;     //SysClk is 250M/4 , 62.5Mhz
            TH0_Reload = 0x34;
            TL0_Reload = 0x8D;
            /* USB enable and MCU/RA/SYS clock source from PLL(UTMI/PIPE) */
            CLKREG[CLK_SEL] |= (MCU_RA_SYS_CLK_SRC + USB_ENB);
            break;
    }
}

//==================================================================================================================
//  FUNCTION DESCRIPTION:
//      Set IP CH
//
//  INPUT:
//      None
//
//  OUTPUT:
//      None
//
//MODIFIED GLOBAL VARIABLE:
//      None
//
//==================================================================================================================
void IP_Set_CH(void)
{
    //Original (Program ram for CH0, Data ram for CH1)
    BRAREG[BRA_MMCR_CH]     = IP_SEL_CH1;
    BRAREG[BRA_UFSRX_CH]    = IP_SEL_CH1;
    BRAREG[BRA_UFSTX_CH]    = IP_SEL_CH1;
    BRAREG[BRA_UHS2_CH]     = IP_SEL_CH1;
    BRAREG[BRA_CPRM_CH]     = IP_SEL_CH1;
    BRAREG[BRA_AES_CH]      = IP_SEL_CH1;
    BRAREG[BRA_SHA_CH]      = IP_SEL_CH1;
    BRAREG[BRA_DMA_CH]      = IP_SEL_CH1;
    BRAREG[BRA_SPI_CH]      = IP_SEL_CH1;
    //BRAREG[BRA_MOVX_CH]       = IP_SEL_CH1;
    BRAREG[BRA_MCU_CH]      = IP_SEL_CH0;
    BRAREG[BRA_PTNGEN_CH]   = IP_SEL_CH1;
    BRAREG[BRA_USBIN_CH]    = IP_SEL_CH1;
    BRAREG[BRA_USBOUT_CH]   = IP_SEL_CH1;
    BRAREG[BRA_CTLSEL]      = IP_SEL_CH1;
}

//==================================================================================================================
//  FUNCTION DESCRIPTION:
//      Buffer flag setting
//
//  INPUT:
//      ab_Base: Flag base
//      ab_Set: Set the flag full(0x80), empty(0x00)
//
//  OUTPUT:
//      None
//
//MODIFIED GLOBAL VARIABLE:
//      None
//
//==================================================================================================================
void Buf_Flag_Setting(BYTE ab_Base, BYTE ab_Set)
{
#if 0
    SYSREG[SYS_FLAG_BASE] = ab_Base;    //flag no 0 form buf page x, 0x19600
    BRAREG[BRA_FLAG_SEL] = Sel_Buffer_Flag;
    BRAREG[BRA_FBSAddr] = 0x00;    //Set Start Flag = 0
    BRAREG[BRA_FBLen] = 0x10;   //Set Flag Length
    BRAREG[BRA_BUFF_CTRL] = ab_Set; //Set Empty (Ready to Written)
#else
 //USB will hang if move sysbase within data trans
    SYSREG[SYS_FLAG_BASE] = ab_Base;    //flag no 0 form buf page x, 0x19600

    BRAREG[BRA_FLAG_SEL] = Sel_Buffer_Flag;

    //if (gb_BOT_Index) {
    //    BRAREG[BRA_FBSAddr] = 0x10;    //Set Start Flag = 0x10
    //} else {
        BRAREG[BRA_FBSAddr] = 0x00;    //Set Start Flag = 0
    //}
		
    BRAREG[BRA_FBLen] = 0x10;   //Set Flag Length
    BRAREG[BRA_BUFF_CTRL] = ab_Set; //Set Empty (Ready to Written)
#endif

}

//==================================================================================================================
//  FUNCTION DESCRIPTION:
//      Set operation mode.
//
//  INPUT:
//      None
//
//  OUTPUT:
//      None
//
//MODIFIED GLOBAL VARIABLE:
//      gb_X_MaxCard
//      gb_X_LUN_2_MEDIA
//
//==================================================================================================================
void Operate_Mode_Set(void)
{
	//GPIOREG[GPIO_D_OE] = 0xFF;
    //GPIOREG[GPIO_D_O] = 0x00;
    //GPIOREG[GPIO_A_OE] = 0xFF;
    //GPIOREG[GPIO_A_O] = 0x00;
    /* Open all IP clocks. USB/MMCR/DMA/CPRM/AES/I2C/RSPI/SPI */
    CLKREG[CLK_GATE] = 0x00;

    /* Init GPIO state */
    Init_GPIO_State();
    //GPIOREG[GPIO_A_OE] = 0xFF;

    /* Init all IP use CH0/CH1 */
    IP_Set_CH();

    /* Normal condition shall enter this, no enter this when just FW reset */
    //if ((USBREG[CONN_SPD] != FW_RESET)) {
    if( !(CLKREG[CLK_WUCNT] & (PIPE_CLKRDY + UTMI_CLKRDY) )){
        /* Setting default CPU clock */
        Set_MCU_CLK(REF_CLK);

        /* Set De-glitch Counter when Crystal Start */
        CLKREG[CLK_WUCNT] = 0x10;

        /* in FPGA mode, if PLL have been enable, no more PHY setting available
         * USB PHY setting, 這個不能設兩次，不然PHY會fail
         */
        PHY_Set_Reg();

        /* If Non_Boot or Boot Non_FPGA, run PLL Enable */
        /* PLL enable */
        CLKREG[CLK_WUCNT] = PLL_ENB | 0x10;

        /* Wait U2/U3 PLL ready,disable U2 PLL ready check for 1711 U2 HW bug */
        //while (!(CLKREG[CLK_WUCNT] & (PIPE_CLKRDY + UTMI_CLKRDY)));
        while (!(CLKREG[CLK_WUCNT] & PIPE_CLKRDY));

    }


	gucxMaxCard = 1U;
    MediaStatus[0] = NO_MEDIA_BIT;
    MediaStatus[1] = NO_MEDIA_BIT;
    //Init_Chahe();

    /* Setting CPU clock during setup speed */
    Set_MCU_CLK(SETUP_SPD);

    /* Set Flag Type */
    BRAREG[BRA_FLAG_SEL] = Sel_Buffer_Flag;
}

//==================================================================================================================
//  FUNCTION DESCRIPTION:
//      Reset 8051.
//
//  INPUT:
//      ab_PRAM: Data will read to PRAM, 1: yes, 0: no
//
//  OUTPUT:
//      None
//
//MODIFIED GLOBAL VARIABLE:
//      None
//
//==================================================================================================================
void SPIResetMCU()
{
    Set_Media_OK();



    if (gb_Interface) {
        //333 SenseIU(gb_Exe_Point);
        while ((USBREG[EPn_TLU_STATUS + EP3_OFFSET] & EPn_EMPTY) == 0);
    } else {
        Return_CSW();

        while ((USBREG[EPn_TLU_STATUS + EP1_OFFSET] & EPn_EMPTY) == 0);
    }


    USBREG[CONN_SPD] &= 0x7F;

    //Delay 5 sec for USB host cut off link
    RTC_Delay(5000);

    //FW reset
    //USBREG[CONN_SPD] = FW_RESET;

    SYSREG[SYS_MODE] &= ~(BIT7);    //PRAM disable

}

//==================================================================================================================
//  FUNCTION DESCRIPTION:
//      CPU jump to PRAM.
//
//  INPUT:
//      None
//
//  OUTPUT:
//      None
//
//MODIFIED GLOBAL VARIABLE:
//      None
//
//==================================================================================================================
void JUMP_PRAM()
{

    Set_Media_OK();



    if (gb_Interface) {
        //333 SenseIU(gb_Exe_Point);
        while ((USBREG[EPn_TLU_STATUS + EP3_OFFSET] & EPn_EMPTY) == 0);
    } else {
        Return_CSW();

        while (USBREG[EPn_TRIG_CFG1 + EP1_OFFSET] & INC_PTR);

        while (!(gb_USB_INT1_EVT[0] & EP2_DATA_AV));
    }



    USBREG[CONN_SPD] &= 0x7F;

    //Delay 2 sec for USB host cut off link
    //RTC_Delay((SWORD)2000);
    //Delay 5 sec for USB host cut off link
    RTC_Delay(5000);

    //FW reset
    //USBREG[CONN_SPD] = FW_RESET;

   

    SYSREG[SYS_MODE] |= PRAM_ENB;   //PRAM enable
}

//==================================================================================================================
//  FUNCTION DESCRIPTION:
//      Write data to PRAM.
//
//  INPUT:
//      None
//
//  OUTPUT:
//      None
//
//MODIFIED GLOBAL VARIABLE:
//      None
//
//==================================================================================================================
void PRAM_Write()
{
    BRAREG[BRA_USBOUT_CH] = IP_SEL_CH0;
    USBBulkOutSectorToPageBuffer(X_CMDBLOCK[5], 1, 0); //Recv 512B data form USB
    BRAREG[BRA_USBOUT_CH] = IP_SEL_CH1;
}

//==================================================================================================================
//  FUNCTION DESCRIPTION:
//      Read data form PRAM.
//
//  INPUT:
//      None
//
//  OUTPUT:
//      None
//
//MODIFIED GLOBAL VARIABLE:
//      None
//
//==================================================================================================================
void PRAM_Read()
{
    BRAREG[BRA_USBIN_CH] = IP_SEL_CH0;
    USBBulkInSectorFromPageBuffer(X_CMDBLOCK[5], 1, 0);
    BRAREG[BRA_USBIN_CH] = IP_SEL_CH1;
}


