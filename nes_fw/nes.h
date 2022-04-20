#pragma once

#define D_NES_CMODEL

#ifdef D_NES_CMODEL
#define xdata
#define data
#define bit	int
#define code
#define idata
#define sfr	extern int //special function register
#define sbit extern int

#define MEM_BAR	((unsigned long)mem_bar - 0x20000UL)
#define BUF_BAR ((unsigned long long)buf_bar - 0x29000UL)
//extern unsigned char* MEM_BAR;
//extern unsigned char* BUF_BAR;

extern int P1;
extern int TMOD;
extern int TH0;
extern int TH1;
extern int TL1;
extern int IP;
extern int TCON;
extern int SCON;
extern int IE;
extern int EX1;
extern int EX0;
extern int TR0;
extern int TL0;
extern int RI;
extern int TI;
extern int IE0;
extern int IE1;

extern int* mem_bar;
extern int* buf_bar;
#endif