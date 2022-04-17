#ifndef __SPI_ROM_H__
#define __SPI_ROM_H__


#define _SPI_READ    1
#define _SPI_WRITE   0

#define _SPI_BUF_MOD    0x80
#define _SPI_ADDR_MOD   0x00

extern void SPI_CMD(BYTE cmd0, BYTE cmd1, BYTE cmd2, BYTE cmd3, BYTE cmd_cnt);
extern void SPI_Set_Common_Register();
extern void SPI_RW_CMD(BYTE cmd0, BYTE cmd1, BYTE cmd2, BYTE cmd3, BYTE cmd_cnt);
extern void SPI_RSP(BYTE rsp_cnt);
extern void SPI_WEn(BYTE enable);
extern UCHAR SPI_Init();
extern UCHAR SPI_Read_Status();
extern void SPI_Write_Status(UCHAR sts);
extern void SPI_WEn(UCHAR enable);
extern UCHAR SPI_DMA_RW(UCHAR buf_mode, unsigned long buf_addr, UCHAR rwdir, unsigned long rom_addr, unsigned long bcnt);
extern UCHAR SPI_Erase_CHIP();
extern UCHAR SPI_Set_Mark(UCHAR* DAT);

extern UCHAR xdata gb_ROM_Info;
extern UCHAR xdata gb_ROM_Offset;   //offset for FW switch CMD or flow
extern UCHAR xdata gb_ROM_Manu; //Manufacturer ID
extern UCHAR xdata gb_ROM_Type; //type ID
extern UCHAR xdata gb_ROM_Density;  //desity ID

#endif