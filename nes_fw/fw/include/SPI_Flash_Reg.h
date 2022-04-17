#ifndef __SPI_FLAH_REG_H__
#define __SPI_FLAH_REG_H__
//==================================================================================================================

#define SPIREG ((volatile unsigned char *)0x2F400UL)
//==================================================================================================================

#define SPI_CFG                 0x00

#define SPI_MST_TRIG            0x01

#define SPI_CS_N_CTRL           0x02

#define SPI_DMA_CTRL            0x03

#define SPI_DMA_CRC_CTRL        0x04

#define SPI_MST_STATUS          0x05

#define SPI_CRC_STATUS          0x08

#define SPI_CRC7_VAL            0x09

#define SPI_CRC16_VAL_L         0x0A

#define SPI_CRC16_VAL_H         0x0B

#define SPI_CMD_CRC_CTRL        0x0C

#define SPI_RSP_CRC_CTRL        0x0D

#define SPI_CMD_BCNT            0x0E

#define SPI_RSP_BCNT            0x0F

#define SPI_CMD_DATA            0x10

#define SPI_RSP_DATA            0x18

#define SPI_DMA_BCNT_L          0x20

#define SPI_DMA_BCNT_M          0x21

#define SPI_DMA_BCNT_H          0x22

#define SPI_DMA_CCNT_L          0x24

#define SPI_DMA_CCNT_M          0x25

#define SPI_DMA_CCNT_H          0x26

#define SPI_DMA_RAM_ADDR_L      0x28

#define SPI_DMA_RAM_ADDR_M      0x29

#define SPI_DMA_RAM_ADDR_H      0x2A

#define SPI_DMA_BUFCTRL         0x2B

#define SPI_DMA_BUFTOP          0x2C

#endif  //#ifndef __SPI_FLAH_REG_H__