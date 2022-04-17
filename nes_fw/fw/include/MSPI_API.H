#ifndef __MSPI_API_H__
#define __MSPI_API_H__
//==================================================================================================================

//Global function
extern void MSPI_RDID();
extern void MSPI_RDSR();
extern SWORD Read_Vol(BYTE ab_Port);
extern void Noise_Gen_ToSRAM();
//==================================================================================================================

//Global variable
#endif  //#ifndef __MSPI_API_H__