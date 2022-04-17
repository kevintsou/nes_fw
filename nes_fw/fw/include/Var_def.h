#ifndef __VAR_DEF_H__
#define __VAR_DEF_H__
//==================================================================================================================

#define  BufferMethodChange  1
#define  EnableleInitSD      1
#define  IsBigEndian         1       //0: lettle Endian
#define  ChangCardDect       1
#define  Item_Peter               0
#define  Item_CRC_Eng_Test        0
#define  Item_CPRM_Test           0
#define  Item_WDT_Test            0  //runing at FW mode
#define  Item_RTC_Test            0
#define  Item_Standard_Test       0
#define  Item_WR_Burnin_Test      1


//result define
#define FALSE       0
#define TRUE        1
#define TYPE_ERR    0x81
#define TIME_OUT    0x82
#define STATUS_FAIL 0x83
#define REJECT      0x84
#define TASK_RSP    0x85
#define PTN_ERR     0x86    // pattern gernerate error
#define PTN2_ERR    0x87
#define EXCEPTION   0x88
#define PTR_NULL    0xFFFF

#define FAIL  (1)
#define PASS  (0)

//8bit define
#define BIT0        0x01
#define BIT1        0x02
#define BIT2        0x04
#define BIT3        0x08
#define BIT4        0x10
#define BIT5        0x20
#define BIT6        0x40
#define BIT7        0x80

#define BIT8        0x0100
#define BIT9        0x0200
#define BIT10       0x0400
#define BIT11       0x0800
#define BIT12       0x1000
#define BIT13       0x2000
#define BIT14       0x4000
#define BIT15       0x8000

#define SET_BIT0    0x01
#define SET_BIT1    0x02
#define SET_BIT2    0x04
#define SET_BIT3    0x08
#define SET_BIT4    0x10
#define SET_BIT5    0x20
#define SET_BIT6    0x40
#define SET_BIT7    0x80

#define CHK_BIT0    0x01
#define CHK_BIT1    0x02
#define CHK_BIT2    0x04
#define CHK_BIT3    0x08
#define CHK_BIT4    0x10
#define CHK_BIT5    0x20
#define CHK_BIT6    0x40
#define CHK_BIT7    0x80


#define CLR_BIT0    0xFE
#define CLR_BIT1    0xFD
#define CLR_BIT2    0xFB
#define CLR_BIT3    0xF7
#define CLR_BIT4    0xEF
#define CLR_BIT5    0xDF
#define CLR_BIT6    0xBF
#define CLR_BIT7    0x7F
//==================================================================================================================

//16 bit define
#define BUS16_BIT0_         0x0001
#define BUS16_BIT1_         0x0002
#define BUS16_BIT2_         0x0004
#define BUS16_BIT3_         0x0008
#define BUS16_BIT4_         0x0010
#define BUS16_BIT5_         0x0020
#define BUS16_BIT6_         0x0040
#define BUS16_BIT7_         0x0080
#define BUS16_BIT8_         0x0100
#define BUS16_BIT9_         0x0200
#define BUS16_BIT10_        0x0400
#define BUS16_BIT11_        0x0800
#define BUS16_BIT12_        0x1000
#define BUS16_BIT13_        0x2000
#define BUS16_BIT14_        0x4000
#define BUS16_BIT15_        0x8000
//==================================================================================================================

//32 bit define
#define BUS32_BIT0_         0x00000001
#define BUS32_BIT1_         0x00000002
#define BUS32_BIT2_         0x00000004
#define BUS32_BIT3_         0x00000008
#define BUS32_BIT4_         0x00000010
#define BUS32_BIT5_         0x00000020
#define BUS32_BIT6_         0x00000040
#define BUS32_BIT7_         0x00000080
#define BUS32_BIT8_         0x00000100
#define BUS32_BIT9_         0x00000200
#define BUS32_BIT10_        0x00000400
#define BUS32_BIT11_        0x00000800
#define BUS32_BIT12_        0x00001000
#define BUS32_BIT13_        0x00002000
#define BUS32_BIT14_        0x00004000
#define BUS32_BIT15_        0x00008000
#define BUS32_BIT16_        0x00010000
#define BUS32_BIT17_        0x00020000
#define BUS32_BIT18_        0x00040000
#define BUS32_BIT19_        0x00080000
#define BUS32_BIT20_        0x00100000
#define BUS32_BIT21_        0x00200000
#define BUS32_BIT22_        0x00400000
#define BUS32_BIT23_        0x00800000
#define BUS32_BIT24_        0x01000000
#define BUS32_BIT25_        0x02000000
#define BUS32_BIT26_        0x04000000
#define BUS32_BIT27_        0x08000000
#define BUS32_BIT28_        0x10000000
#define BUS32_BIT29_        0x20000000
#define BUS32_BIT30_        0x40000000
#define BUS32_BIT31_        0x80000000
//==================================================================================================================

typedef unsigned char UINT8;
typedef unsigned int  UINT16;
typedef unsigned long UINT32;

#define BOOLEAN unsigned char       /* size: 1 byte  */
#define BYTE    unsigned char       /* size: 1 byte  */
#define SWORD   unsigned int        /* size: 2 bytes */
#define LWORD   unsigned long       /* size: 4 bytes */
//#define CBYTE const unsigned char
//#define CWORD const unsigned short

typedef unsigned char    BOOL;
typedef unsigned char    U8;
typedef unsigned short   U16;
//typedef unsigned int     U16;
typedef unsigned long    U32;

//==================================================================================================================

#define TW_H    0
#define TW_L    1

#define TDW_HH  0
#define TDW_HL  1
#define TDW_LH  2
#define TDW_LL  3
//==================================================================================================================

#ifndef __UNION__
#define __UNION__
//==================================================================================================================

//==================================================================================================================
// DESCRIPTION:Setting 2 BYTE  variable UNION
//==================================================================================================================
struct _HL_struct_ {
    BYTE H;  // H
    BYTE L;  // L
};

//==================================================================================================================
// DESCRIPTION:Setting 4 BYTE  variable UNION
//==================================================================================================================
struct _BYTE_struct_ {
    BYTE HH;  // HH 1
    BYTE HL;  // HL 2
    BYTE LH;  // LH 3
    BYTE LL;  // LL 4
};

//==================================================================================================================
// DESCRIPTION:Setting 32 bit  variable for 4 BYTE UNION
//==================================================================================================================
union  DataLen {
    LWORD All;
    struct _BYTE_struct_ BYTEMODE;
};
//==================================================================================================================
// DESCRIPTION:Setting 16 bit variable UNION
//==================================================================================================================
union _TWORD {
    SWORD Word;
    BYTE Byte[2];
    struct _HL_struct_ HLMODE;
};
typedef union _TWORD TWORD;

//==================================================================================================================
// DESCRIPTION:Setting 32 bit  variable UNION
//==================================================================================================================
union _TDWORD {
    LWORD DWord;
    SWORD Word[2];
    BYTE Byte[4];
    struct _BYTE_struct_ BYTEMODE;
    LWORD All;
};
typedef union _TDWORD TDWORD;
#endif



#endif  //#ifndef __VAR_DEF_H__
