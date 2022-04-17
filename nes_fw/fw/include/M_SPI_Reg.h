#ifndef __M_SPI_REG_H__
#define __M_SPI_REG_H__
//==================================================================================================================

#define M_SPIREG ((volatile unsigned char *)0x2F2A0UL)  //Manual SPI, no DMA interface.
//==================================================================================================================

#define MSPI_CFG                0x00
#define MSPI_EN             BIT0    //1: enable ; 0: disable

#define MSPI_MST_TRIG           0x01
#define MSPI_WRT            BIT0    //write trig
#define MSPI_RDT            BIT1    //read trig
#define MSPI_FDT            BIT2    //Full-Duplex trig
#define WB_DUAL             BIT4    //Winbond Dual read mode
#define MXIC_2X             BIT5    //MXIC 2X read mode

#define MSPI_CMD_BCNT           0x02

#define MSPI_RSP_BCNT           0x03

#define MSPI_CMD_DATA           0x10

#define MSPI_RSP_DATA           0x18

#endif  //#ifndef __M_SPI_REG_H__