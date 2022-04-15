// nes_fw.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>

#define D_NES_CMODEL
#ifdef D_NES_CMODEL
#include "nes_cmodel.h"
#pragma comment(lib, "nes_cmodel.lib")
#else
#define _OUT  
#define _IN	
#endif


int testCode() {
	//REG_R16 R16_MR_QINFO(123);
	printf("\n\n");
	printf("code: R16_MR_QINFO[(unsigned int)2]=3;\n");
	R16_MR_QINFO[_IN 2] = 3;
	printf("\n\n");
	printf("unsigned char getv = R16_MR_QINFO[3];\n");
	unsigned char getv = R16_MR_QINFO[3];
	printf("get R16_MR_QINFO-->%d\n", getv);

	//printf("\n\n");
	//R16_MR_QINFO[REG_IN 3]++;
	//R16_MR_QINFO[REG_IN 3]--;

	//printf("\n\n");
	//R16_MR_QINFO[REG_IN 3] += 1;
	//R16_MR_QINFO[REG_IN 3] -= 1;

	//printf("\n\n");
	//R16_MR_QINFO[REG_IN 3] += R16_MR_QINFO[REG_IN 2];
	//R16_MR_QINFO[REG_IN 3] -= R16_MR_QINFO[REG_IN 2];

	printf("\n\n");
	R16_MR_QINFO[_IN 3] &= 1;
	R16_MR_QINFO[_IN 3] &= 1;

	printf("\n\n");
	R16_MR_QINFO[_IN 3] &= R16_MR_QINFO[_IN 2];
	R16_MR_QINFO[_IN 3] &= R16_MR_QINFO[_IN 2];

	printf("\n\n");
	R16_MR_QINFO[_IN 3] |= 0xFE;
	R16_MR_QINFO[_IN 3] |= 1;

	printf("\n\n");
	R16_MR_QINFO[_IN 3] |= R16_MR_QINFO[_IN 2];
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