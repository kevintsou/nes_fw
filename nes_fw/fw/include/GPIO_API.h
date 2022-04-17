#ifndef __GPIO_TEST_API_H__
#define __GPIO_TEST_API_H__
//==================================================================================================================

//only valid in FPGA mode
//Global variable
extern void Init_GPIO_State();
extern void GPIO_Test(BYTE ab_Dir, BYTE *ab_Port_Out_Val);
extern void SD_GPIO_Test(BYTE ab_Enable, BYTE ab_Dir, BYTE *ab_Port_Out_Val);


extern void UHS2_3_3V_ON();
extern void UHS2_3_3V_OFF();
extern void UHS2_1_8V_ON();
extern void UHS2_1_8V_OFF();

#endif  //#ifndef __SYSTEM_API_H__
