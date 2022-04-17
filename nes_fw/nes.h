#pragma once

#define D_NES_CMODEL

#ifdef D_NES_CMODEL
#define xdata
#define data
#define bit
#define code
#define idata
#define sfr	extern int //special function register
#define sbit extern int

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
#endif