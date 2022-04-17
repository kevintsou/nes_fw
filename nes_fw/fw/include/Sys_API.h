#ifndef __SYSTEM_API_H__
#define __SYSTEM_API_H__
//==================================================================================================================

//Define
#define REF_CLK         3
#define BOOT_SPD        2
#define BULK_SPD        1
#define SETUP_SPD       0

//==================================================================================================================

//Global Function
extern void Init8051Reg();
extern void Operate_Mode_Set();
extern void SPIResetMCU();
extern void Set_MCU_CLK(BYTE);
extern void Buf_Flag_Setting(BYTE ab_Base, BYTE ab_Set);
extern void PRAM_Write();
extern void PRAM_Read();
extern void JUMP_PRAM();
extern void JUMP_BOOT();

#endif  //#ifndef __SYSTEM_API_H__