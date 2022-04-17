#ifndef __SPI_FLASH_API_H__
#define __SPI_FLASH_API_H__
//==================================================================================================================

//Global function
extern void SPIFlash_Initial();
extern void SPIFlash_ReadID();
extern void SPIFlash_ReadStatus();
extern void SPIFlash_Chip_Erase();
extern void SPIFlash_Sector_Erase();
extern void SPIFlash_Read(BYTE ab_PRAM, BYTE ab_Shift, BYTE ab_USB);
extern void SPIFlash_Check();
extern BYTE SPIFlash_RDSR();
extern void SPIFlash_Write(BYTE ab_USB);
extern void SPIFlash_Write_Header();
extern void SPIFLASH_PROGRAM(BYTE ab_USB);
//==================================================================================================================

//Global variable
extern bit gi_SPIFlash_Initialed;
extern bit gi_SPIFlash_IDRead;
extern BYTE xdata gb_Manufacturer_ID;

#endif  //#ifndef __SPI_FLASH_API_H__