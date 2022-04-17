#ifndef __PRAM_DEF_H__
#define __PRAM_DEF_H__
//==================================================================================================================

//Allocate PRAM area
#define BANK_8K //4K, 8K, 16K, 32K

#ifdef BANK_4K
#define COM_SEC_CNT         0x78
#define BNK_SEC_CNT         0x08
#define COM_SIZE_           0x0000F000 /* 60K */
#define BANK_SIZE_          0x00001000 /* 4K */
#define BANK_MASK           0xF0
#define FIX_BANK_CNT        0x10
#endif  //#ifdef BANK_8K

#ifdef BANK_8K
#define COM_SEC_CNT         0x70
#define BNK_SEC_CNT         0x10
#define COM_SIZE_           0x0000E000 /* 56K */
#define BANK_SIZE_          0x00002000 /* 8K */
#define BANK_MASK           0xE0
#define FIX_BANK_CNT        0x03
#endif  //#ifdef BANK_8K

#ifdef BANK_16K
#define COM_SEC_CNT         0x60
#define BNK_SEC_CNT         0x20
#define COM_SIZE_           0x0000C000 /* 48K */
#define BANK_SIZE_          0x00004000 /* 16K */
#define BANK_MASK           0xC0
#define FIX_BANK_CNT        0x02
#endif  //#ifdef BANK_16K

#ifdef BANK_32K
#define COM_SEC_CNT         0x40
#define BNK_SEC_CNT         0x40
#define COM_SIZE_           0x00008000 /* 32K */
#define BANK_SIZE_          0x00008000 /* 32K */
#define BANK_MASK           0x80
#define FIX_BANK_CNT        0x01
#endif  //#ifdef BANK_16K

#define FULL_SEC_CNT        0x80  /* */
#define PRAM_SEC_CNT        0x100 /* */

#define BANK_CNT            ((PRAM_SEC_CNT - COM_SEC_CNT) / BNK_SEC_CNT)
#define FIFO_SLOT_CNT       (((PRAM_SEC_CNT - COM_SEC_CNT) / BNK_SEC_CNT) - FIX_BANK_CNT)

//#define MARK_POSITION 0x0002FE00  //64K last 512B
#define MARK_POSITION       0x00000000  //First 512B
#define HEAD_LAST_ADDR  0x0001FE00
#define SHIFT_ADDR          0x00020000

//define bank type
#define FIFO_BANK               0x0
#define FIX_BANK                0x1

//define bank mark
#define EMPTY_SLOT              0xFF
#define BANK_0                  0x00
#define BANK_1                  0x01
#define BANK_2                  0x02
#define BANK_3                  0x03
#define BANK_4                  0x04
#define BANK_5                  0x05
#define BANK_6                  0x06
#define BANK_7                  0x07
#define BANK_8                  0x08
#define BANK_9                  0x09
#define BANK_A                  0x0A
#define BANK_B                  0x0B
#define BANK_C                  0x0C
#define BANK_D                  0x0D
#define BANK_E                  0x0E
#define BANK_F                  0x0F

//==================================================================================================================

//define BANK_SLOT struct
typedef struct _BANK_SLOT_struct_ {
    BYTE b_Slot_No;
    BYTE b_Bank_No;
    BYTE b_Bank_Type;
    struct _BANK_SLOT_struct_ *strp_Next;
    struct _BANK_SLOT_struct_ *strp_Pre;
};
typedef struct _BANK_SLOT_struct_ BANK_SLOT;
#endif  //#ifndef __PRAM_DEF_H__


