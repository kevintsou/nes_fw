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
#endif