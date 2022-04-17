#include "nes.h"
#ifdef D_NES_CMODEL
#include "8051.h"
#else
#include <reg52.h>
#include <8051.h>
#endif
#include "Var_Def.h"

#include "MCU_Reg.h"
#include "M_SPI_Reg.h"

#include <buf_def.h>
#include "USB_API.h"
#include "GPIO_Reg.h"
#include "GPIO_API.h"
#include "IO_Macro.h"

//==================================================================================================================

bit gi_MSPIFlash_Initialed = 0;
//==================================================================================================================

//==================================================================================================================
//  FUNCTION DESCRIPTION:
//      Initial MSPI.
//
//  INPUT:
//      None
//
//  OUTPUT:
//      None
//
//  MODIFIED GLOBAL VARIABLE:
//      gi_MSPIFlash_Initialed
//
//==================================================================================================================
void MSPI_Initial()
{
    CLKREG[CLK_MSPI] = 0x12;     //SPI_clk = 60MHz/20 from PLL
    //CLKREG[CLK_MSPI] = 0x00;   //SPI_clk = 60MHz/20 from PLL
    M_SPIREG[MSPI_CFG] = MSPI_EN;   //Enable

    GPIOREG[GPIO_C0_OE] = 1;    //CS
    IO_ADC_CS1 = 1; //CS H

    GPIOREG[GPIO_C1_OE] = 1;    //CS
    IO_ADC_CS0 = 1; //CS H

    //Set Driven
    SYSREG[SYS_GPSPI_CFG] = (GPSPI_DRN_E8 + GPSPI_CONOF);   //GPSPI driven

    gi_MSPIFlash_Initialed = 1;
}

//==================================================================================================================
//  FUNCTION DESCRIPTION:
//      Use the MSPI to send command to A2D IC to get SD vol.
//
//  INPUT:
//      ab_Port: which port, 0: port0, 1: port 1
//
//  OUTPUT:
//      None
//
//  MODIFIED GLOBAL VARIABLE:
//      gi_MSPIFlash_Initialed
//
//==================================================================================================================
SWORD Read_Vol(BYTE ab_Port)
{
    SWORD rl_Vol;
    SWORD rb_Dly;

    if (!gi_MSPIFlash_Initialed) {
        MSPI_Initial();
    }

    //MUXREG[SPI_MUX] |= PROM_MUX;  //Disable PROM SPI interface.
    M_SPIREG[MSPI_CMD_DATA] = 0xC0;
    M_SPIREG[MSPI_CMD_DATA + 1] = 0x00;
    M_SPIREG[MSPI_CMD_BCNT] = 2;
    M_SPIREG[MSPI_RSP_BCNT] = 2;


    if (ab_Port) {
        IO_ADC_CS0 = 0;    //CS L
    } else {
        IO_ADC_CS1 = 0;    //CS L
    }

    M_SPIREG[MSPI_MST_TRIG] = MSPI_FDT;

    while ( M_SPIREG[MSPI_MST_TRIG] & MSPI_FDT );

    M_SPIREG[MSPI_CMD_BCNT] = 2;
    M_SPIREG[MSPI_RSP_BCNT] = 2;

    if (ab_Port) {
        IO_ADC_CS0 = 1;    //CS H
    } else {
        IO_ADC_CS1 = 1;    //CS H
    }

    for (rb_Dly = 0; rb_Dly < 0x3FF; rb_Dly++);

    rl_Vol = (M_SPIREG[MSPI_RSP_DATA] << 8) | (M_SPIREG[MSPI_RSP_DATA + 1]);
    //MUXREG[SPI_MUX] &= ~(PROM_MUX);   //Enable PROM SPI interface.



    for (rb_Dly = 0; rb_Dly < 0x3FF; rb_Dly++);

    return rl_Vol;
}

void MSPI_RDID()
{
    if (!gi_MSPIFlash_Initialed) {
        MSPI_Initial();
    }

    //MUXREG[SPI_MUX] &= ~(PROM_MUX);   //Enable PROM SPI interface.

    M_SPIREG[MSPI_RSP_DATA] = 0;
    M_SPIREG[MSPI_RSP_DATA + 1] = 0;
    M_SPIREG[MSPI_RSP_DATA + 2] = 0;

    M_SPIREG[MSPI_CMD_DATA] = 0x9F;

    M_SPIREG[MSPI_CMD_BCNT] = 1;
    M_SPIREG[MSPI_RSP_BCNT] = 3;

    IO_ADC_CS1 = 0; //CS L

    M_SPIREG[MSPI_MST_TRIG] = BIT0;

    while ( M_SPIREG[MSPI_MST_TRIG] & BIT0 );

    M_SPIREG[MSPI_MST_TRIG] = BIT1;

    while ( M_SPIREG[MSPI_MST_TRIG] & BIT1 );

    IO_ADC_CS1 = 1; //CS H

    SPI_BUF[0] = M_SPIREG[MSPI_RSP_DATA];
    SPI_BUF[1] = M_SPIREG[MSPI_RSP_DATA + 1];
    SPI_BUF[2] = M_SPIREG[MSPI_RSP_DATA + 2];
    //MUXREG[SPI_MUX] &= ~(PROM_MUX);   //Enable PROM SPI interface.

    USBBulkInSectorFromPageBuffer(0x80, 1, 0);
}

void MSPI_RDSR()
{
    SWORD rw_Vol;

    rw_Vol = Read_Vol(0);

    SPI_BUF[0] = (BYTE)(rw_Vol >> 8);
    SPI_BUF[0] = (BYTE)(rw_Vol);

    USBBulkInSectorFromPageBuffer(0x80, 1, 0);
}