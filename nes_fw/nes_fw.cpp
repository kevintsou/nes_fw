// nes_fw.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
#include "nes.h"
#include "Buf_DEF.h"
#include "MCU_Reg.h"

#ifdef D_NES_CMODEL
#include "nes_cmodel.h"
#pragma comment(lib, "nes_cmodel.lib")
#else
#define _OUT  
#define _IN	
#endif

int P1;
int TMOD;
int TH0;
int TH1;
int TL1;
int IP;
int TCON;
int SCON;
int IE;
int EX1;
int EX0;
int TR0;
int TL0;
int RI;
int TI;
int IE0;
int IE1;

extern BYTE memcpy_DMA(UINT8 ab_Sur_Page, UINT8 ab_Des_Page, UINT32 al_len);

int testCode() {

	memcpy_DMA(FLAG_BASENO_PBA_CACHE, BUF_NUM_TEMP_BUF, (512 * SYS_INFO_SIZE));

	return 0;
	//REG_R16 R16_MR_QINFO(123);
	printf("\n\n");
	printf("code: R16_MR_QINFO[(unsigned int)2]=3;\n");
	R16_MR_QINFO[_IN 3] = 3;
	printf("\n\n");
	printf("unsigned char getv = R16_MR_QINFO[3];\n");
	unsigned char getv = R16_MR_QINFO[_OUT 3];
	printf("get R16_MR_QINFO-->%d\n", getv);

	printf("\n\n");
	R16_MR_QINFO[_IN 3] &= 1;

	printf("\n\n");
	R16_MR_QINFO[_IN 2] &= R16_MR_QINFO[_IN 3];

	printf("\n\n");
	R16_MR_QINFO[_IN 3] |= 0xFE;

	printf("\n\n");
	R16_MR_QINFO[_IN 3] |= R16_MR_QINFO[_IN 2];





	return 0;
}


int main()
{

#ifdef D_NES_CMODEL
	initCmodelMem();	// init cmodel mem space
	initCmodelReg();	// init cmodel reg default value
#endif

	testCode();
	return 0;
}