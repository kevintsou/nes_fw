
#ifndef _BASE_TYPE_H
#define _BASE_TYPE_H

#define BIT0	    0x01
#define BIT1    	0x02
#define BIT2    	0x04
#define BIT3    	0x08
#define BIT4    	0x10
#define BIT5    	0x20
#define BIT6    	0x40
#define BIT7    	0x80

#define SET_BIT0	0x01
#define SET_BIT1	0x02
#define SET_BIT2	0x04
#define SET_BIT3	0x08
#define SET_BIT4	0x10
#define SET_BIT5	0x20
#define SET_BIT6	0x40
#define SET_BIT7	0x80

#define CHK_BIT0	0x01
#define CHK_BIT1	0x02
#define CHK_BIT2	0x04
#define CHK_BIT3	0x08
#define CHK_BIT4	0x10
#define CHK_BIT5	0x20
#define CHK_BIT6	0x40
#define CHK_BIT7	0x80


#define CLR_BIT0	0xFE
#define CLR_BIT1	0xFD
#define CLR_BIT2	0xFB
#define CLR_BIT3	0xF7
#define CLR_BIT4	0xEF
#define CLR_BIT5	0xDF
#define CLR_BIT6	0xBF
#define CLR_BIT7	0x7F


#define MAX_CARD	2
#define	X_MaxCard	1
#define SEL_MEDIA 0

typedef unsigned char uint8;
typedef unsigned char UCHAR;
typedef unsigned int uint16;
typedef unsigned int WORD;
typedef unsigned long uint32;
typedef unsigned long dWORD;

typedef void (code *pFuncPtr) (void);


struct CHS_struct {
	unsigned char Head;
	unsigned int  Cyl;
	unsigned char Sec_Num;
};

struct INT_struct {
	unsigned int INT2;
	unsigned int INT1;
};


struct BYTE_struct {
	unsigned char BYTE4;
	unsigned char BYTE3;
	unsigned char BYTE2;
	unsigned char BYTE1;
};

struct BYTE_struct_new 
{
	unsigned char HH;
	unsigned char HL;
	unsigned char LH;
	unsigned char LL;
};
union  LongInt {
	unsigned long All;
	struct BYTE_struct BYTEMODE;
	struct INT_struct INTMODE;
};


union LongLBA {
	unsigned long All;
	struct CHS_struct CHS;
	struct INT_struct INTMODE;
	struct BYTE_struct BYTEMODE;
};   //LBAVALUE,LBA_Max_Sector,CHS_Max_Sector;



struct HL_struct {
	unsigned char H;
	unsigned char L;
};

typedef union intstru {
	unsigned int All;
	struct HL_struct Byte;
} INTSTRU;

union U16Int
{
	unsigned short All;
	struct HL_struct HLMODE;
};

union  U32Int 
{
	unsigned long All;
	struct BYTE_struct_new BYTEMODE;
};
#endif

