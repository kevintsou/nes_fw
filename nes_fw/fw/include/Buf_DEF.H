#ifndef __BUF_DEF_H__
#define __BUF_DEF_H__

#define EN_NEW_MAPPING    (1)

#define RA_BUF_MODE     0x01
#define RA_ADDR_MODE    0x00
#define Convert_Addr2Page(addr)         ((((unsigned long)(addr)&0xFFFF)+0x10000UL)/512UL)      // 锣传    Addr --> Hardware Physical Addr
#define Convert_Page2Addr(page)         ((((unsigned long)(page)*512L)&0x1FFFFUL)+0x10000UL)    // 锣传Page No. --> Physical Addr For 8051 (Page 0x80 Physical Addr: XDATA 0x20000 )
#define Convert_Page2BufAddr(page)      ((((unsigned long)(page)*512L)&0x1FFFFUL))              // 锣传Page No. --> Buffer Page Addr
#define Page2BufAddr(page)              ((((unsigned long)(page)*512L)&0x1FFFFUL))              // 锣传Page No. --> Buffer Page Addr


#define ADDR2PAGE(addr)     Convert_Addr2Page((addr))
#define PAGE2ADDR(addr)     Convert_Page2Addr((addr))
#define PAGE2BUF_ADDR(addr) Convert_Page2BufAddr((addr))

#define Convert_Page2DomainAddr(page)   (unsigned int)((((unsigned long)(page)*512UL)&0xFFFF))  // 锣传Page No. --> Domain Addr
#define PAGE2VMEM_CHAR(page)            ((volatile unsigned char xdata *) Convert_Page2DomainAddr((page)))
#define PAGE2VMEM_INT(page)             ((volatile unsigned  int xdata *) Convert_Page2DomainAddr((page)))
#define PAGE2MEM_CHAR(page)             ((volatile unsigned char xdata *) Convert_Page2DomainAddr((page)))
#define PAGE2MEM_INT(page)              ((volatile unsigned  int xdata *) Convert_Page2DomainAddr((page)))

//==================================================================================================================
//------------------------------------------------------------------------------------
//                          Data bufffer Assign
//------------------------------------------------------------------------------------
#define USB_BLUK_DATA_PAGE          0x00    // 0X~0X7F: 128 sectors //note : page 0x00~0x7f can't use MOVX access, only byte HW access(ex: DMA)
#define USB_BLUK_DATA_PAGE_SIZE     128     // 64k: page 0 ~ 0x7f
#define SDMMC_DATA_PAGE_SIZE    (USB_BLUK_DATA_PAGE_SIZE-1) // 64KB
#define PATTERN_GEN_PAGE_SIZE   (USB_BLUK_DATA_PAGE_SIZE-1) // pattern generator buffer size

#define dUM_DATA_BUFFER_SIZE            32  //64-(0XB0) //32 -(0XCB)
#define X_USB_CMD_BUF                       ((volatile unsigned char *)0x29200UL)

#define MEM_SDMMC_DATA_PAGE         0xCB    //- 0x29600, 16K Length
#define MEM_SDMMC_DATA_PAGE_SIZE    32      // 16K
#define X_SDMMC_DATA_BUF        (PAGE2VMEM_CHAR(MEM_SDMMC_DATA_PAGE))

#define MEM_DUMMYCMD_PAGE           (0xC9)  // 0x29200 for UHS2 CMD parameter use
#define DUMMYCMD_BUF            (PAGE2MEM_CHAR(MEM_DUMMYCMD_PAGE))

//16K storage Buffer mapping
//#define MEM_SDMMC_DATA_PAGE           0xCB    //- 0x29600, 16K Length
#define BUFF00_                 ((volatile unsigned char *)(0x29600UL+BUF_BAR))
#define BUFF01_                 ((volatile unsigned char *)(0x29800UL+BUF_BAR))
#define BUFF02_                 ((volatile unsigned char *)(0x29A00UL+BUF_BAR))
#define BUFF03_                 ((volatile unsigned char *)(0x29C00UL+BUF_BAR))
#define BUFF04_                 ((volatile unsigned char *)(0x29E00UL+BUF_BAR))
#define BUFF05_                 ((volatile unsigned char *)(0x2A000UL+BUF_BAR))
#define BUFF06_                 ((volatile unsigned char *)(0x2A200UL+BUF_BAR))
#define BUFF07_                 ((volatile unsigned char *)(0x2A400UL+BUF_BAR))
#define BUFF08_                 ((volatile unsigned char *)(0x2A600UL+BUF_BAR))
#define BUFF09_                 ((volatile unsigned char *)(0x2A800UL+BUF_BAR))
#define BUFF0A_                 ((volatile unsigned char *)(0x2AA00UL+BUF_BAR))
#define BUFF0B_                 ((volatile unsigned char *)(0x2AC00UL+BUF_BAR))
#define BUFF0C_                 ((volatile unsigned char *)(0x2AE00UL+BUF_BAR))
#define BUFF0D_                 ((volatile unsigned char *)(0x2B000UL+BUF_BAR))
#define BUFF0E_                 ((volatile unsigned char *)(0x2B200UL+BUF_BAR))
#define BUFF0F_                 ((volatile unsigned char *)(0x2B400UL+BUF_BAR))
#define BUFF10_                 ((volatile unsigned char *)(0x2B600UL+BUF_BAR))
#define BUFF11_                 ((volatile unsigned char *)(0x2B800UL+BUF_BAR))
#define BUFF12_                 ((volatile unsigned char *)(0x2BA00UL+BUF_BAR))
#define BUFF13_                 ((volatile unsigned char *)(0x2BC00UL+BUF_BAR))
#define BUFF14_                 ((volatile unsigned char *)(0x2BE00UL+BUF_BAR))
#define BUFF15_                 ((volatile unsigned char *)(0x2C000UL+BUF_BAR))
#define BUFF16_                 ((volatile unsigned char *)(0x2C200UL+BUF_BAR))
#define BUFF17_                 ((volatile unsigned char *)(0x2C400UL+BUF_BAR))
#define BUFF18_                 ((volatile unsigned char *)(0x2C600UL+BUF_BAR))
#define BUFF19_                 ((volatile unsigned char *)(0x2C800UL+BUF_BAR))
#define BUFF1A_                 ((volatile unsigned char *)(0x2CA00UL+BUF_BAR))
#define BUFF1B_                 ((volatile unsigned char *)(0x2CC00UL+BUF_BAR))
#define BUFF1C_                 ((volatile unsigned char *)(0x2CE00UL+BUF_BAR))
#define BUFF1D_                 ((volatile unsigned char *)(0x2D000UL+BUF_BAR))
#define BUFF1E_                 ((volatile unsigned char *)(0x2D200UL+BUF_BAR))
#define BUFF1F_                 ((volatile unsigned char *)(0x2D400UL+BUF_BAR))

//------------------------------------------------------------------------------------

//#define PRAM_BASE             0x00000UL
//#define X_CMDEP0              ((unsigned char *)0x24000)  //USB EP0 Buf
//#define BulkOut               ((unsigned char *)0x24200)  //USB
//#define X_CMDBLOCK            ((unsigned char *)0x24680)  //USB Bulk Out

//#define ShortPkt_Buf          ((unsigned char *)0x24200)  //USB
#define InquiryTemp             ((unsigned char *)(0x24150))    //Need 32 Byte;
#define X_PASSWORD              ((unsigned char *)0x24170)  //~1680
#define SPI_BUF                 ((unsigned char *)0x20000)  //SIP buff
#define DMA_BUF1                ((unsigned char *)0x20000)  //DMA buff1
#define DMA_BUF2                ((unsigned char *)0x20200)  //DMA buff2
#define DMA_BUF3                ((unsigned char *)0x20400)  //DMA buff3
#define TMP_BUF                 ((unsigned char *)0x20000)  //Tmp buff

#define CR_IN_BUF_ADDR          0x2C600
#define CR_OUT_BUF_ADDR         0x29600
#define CC_IN_BUF_ADDR          0x2A600
#define CC_OUT_BUF_ADDR         0x2B600

#define Data_CR_IN_BUF          ((unsigned char *)CR_IN_BUF_ADDR)
#define Data_CR_OUT_BUF         ((unsigned char *)CR_OUT_BUF_ADDR)
#define Data_CC_IN_BUF          ((unsigned char *)CC_IN_BUF_ADDR)
#define Data_CC_OUT_BUF         ((unsigned char *)CC_OUT_BUF_ADDR)


#define SHA_SOURCE_BUF          ((unsigned char *)(0x22000))
#define SHA_RESULT_BUF          ((unsigned char *)(0x22200))

#define INFO_Base               ((unsigned char *)0x21400)
#define DESC_Base               ((unsigned char *)0x21400)

#define X_MAPPING1              ((unsigned int  *)0x22000)//0x4800~0x67ff 8K

#define X_CopyBuf               ((unsigned char *)0x28000)//within 8K
#define X_CopyBufI              ((unsigned int  *)0x28000)//within 8K

//SD buffer
#define BUFFA_                  ((volatile unsigned char *)0x29600UL)
#define BUFFB_                  ((volatile unsigned char *)0x2A600UL)

#define CHALLENGE1_             ((volatile unsigned char *)0x2A800UL)
#define CHALLENGE2_             ((volatile unsigned char *)0x2A810UL)
#define RESOPNCE1_              ((volatile unsigned char *)0x2A820UL)
#define RESOPNCE2_              ((volatile unsigned char *)0x2A830UL)

//Buffer address
#define BUF_NO2ADDR(BUF_NO) ((LWORD)BUF_NO << 9)
#define BUF_PAGE2ADDR(page) ((volatile unsigned char xdata *)  Convert_Page2DomainAddr((page)))

#define    X_BUFFER        		((unsigned char *)(0x28000U))


//UFS buffer
#define UM_BUFF_NO_                 0x40
#define UM_DUMP_BUFF_NO_        0xCB

#define UFS_CMD_PAGE            0xC9    // 0x29200, PC --> host
#define UFS_BK_PAGE             0xCA    // 0x29400, PC <-- host, (save match CMD timer register value)
#define UFS_RSP_PAGE            0xCB    // 0x29600, UFS response page (save sense data)
#define UFS_REV_PAGE            0xA2    // 0x24400 , host --> PC, receive page
#define UFS_TX_PAGE             0x00    // UFS Tx DATA OUT buffer,  can't use movx access page
#define UFS_TX_PAGE_SIZE        64
#define UFS_RX_PAGE             0x40    // UFS Rx DATA IN buffer, can't use movx access page
#define UFS_RX_PAGE_SIZE        64

#define UFS_CMD_ADDR            BUF_PAGE2ADDR(UFS_CMD_PAGE)
#define UFS_BK_ADDR             BUF_PAGE2ADDR(UFS_BK_PAGE)
#define UFS_RSP_ADDR            BUF_PAGE2ADDR(UFS_RSP_PAGE)
#define UFS_REV_ADDR            BUF_PAGE2ADDR(UFS_REV_PAGE)

#define UM_DUMP_BUFF_               ((volatile unsigned char *)0x29600UL)

/* PBA use buffer */
/* BRAM_0
 * CPU Address Range : 0x01_0000 ~ 0x01_FFFF
 * DMA Address Range : 0x01_0000 ~ 0x01_FFFF,  buffer0~127
 * Size : 64KB
 */
#define BASE_START_ADR    0x10000UL

#if EN_NEW_MAPPING //Program data use 0x2400~0x2BFF
/*
// Buffer 128(0x80)
// Size : 1 buffer(512 bytes)
// purpose : FAT MBR use
#define FLAG_BASENO_FAT_MBR             0x80
#define FAT_MBR_ADR         0x20000UL
#define FAT_MBR_Base        ((unsigned char *)FAT_MBR_ADR)

// Buffer 129(0x81)
// Size : 1 buffer(512 bytes)
// purpose : USB EP0 Buffer point
#define USBEP0BUF_START_ADR0            0x02
#define USBEP0BUF_START_ADR1            0x01
#define USBEP0BUF_START_ADR2            0x00
#define USBEP0BUF_LEN0                      0x01
#define USBEP0BUF_LEN1                      0x00

// Buffer 130~133(0x82~0x85), 0x2400~0x2BFF
// Size : 4 buffer(2K bytes)
// purpose : Program data use use

// Buffer 134~173(0x86~0xAD)
// Size : 40 buffer(20K bytes)
// purpose : temp buffer use
#define BUF_NUM_TEMP_BUF                0x86
#define BASE_ADDR_TEMP_BUF  0x20C00UL
#define TEMP_BUF            ((unsigned char *)BASE_ADDR_TEMP_BUF)

// Buffer 140(0x8C)
// Size : 2 buffer(512 bytes)
// purpose : CFDE use
#define FLAG_BASENO_CFDE_BLK            0x8C
#define CFDE_BLK_ADR        0x21800UL
#define CFDE_BLK_Base       ((unsigned char *)CFDE_BLK_ADR)

// Buffer 142(0x8E)
// Size : 4 buffer(2048 bytes)
// purpose : Info Page use
#define FLAG_BASENO_INFO_PAGE           0x8E
#define INFO_PAGE_ADR       0x21C00UL
#define INFO_PAGE_Base      ((unsigned char *)INFO_PAGE_ADR)

// Buffer 144~145(0x90~0x91)
// Size : 2 buffer(1K bytes)
// purpose : Set Ready use
#define FLAG_BASENO_SET_RDY             0x90
#define SET_RDY_ADR         0x22000UL
#define SET_RDY_Base        ((unsigned char *)SET_RDY_ADR)

// Buffer 146~183(0x92~0xB7)
// Size : 38 buffer(19K bytes)
// purpose : empty system info use
#define FLAG_BASENO_REQ_BLK            0x92
#define REQ_BLK_ADR         0x22400UL
#define REQ_BLK_Base        ((unsigned char *)REQ_BLK_ADR)

// Buffer 192~255(0xC0~0xFF)
// Size : 64 buffer(32K bytes)
// purpose : PBA cache use
#define FLAG_BASENO_PBA_CACHE           0xC0
#define PBA_CACHE_ADR       0x28000UL
#define PBA_Cache_Buf       ((unsigned char *)PBA_CACHE_ADR)

// Buffer 192~255(0xC0~0xFF)
// Size : 64 buffer(32K bytes)
// purpose : USB EPn Buffer point
#define USBEPNBUF_START_ADR0            0x80
#define USBEPNBUF_START_ADR1            0x01
#define USBEPNBUF_START_ADR2            0x00

#define USBEPNBUF_LEN0                      0x40
#define USBEPNBUF_LEN1                      0x00
*/
//extern int mem_bar;
//#define MEM_BAR	mem_bar
// Buffer 128(0x80)
// Size : 1 buffer(512 bytes)
// purpose : FAT MBR use
#define FLAG_BASENO_FAT_MBR             0x80
#define FAT_MBR_ADR         (0x20000UL + MEM_BAR)
#define FAT_MBR_Base        ((unsigned char *)FAT_MBR_ADR)

// Buffer 129~132(0x81~0x84), 0x2200~0x29FF
// Size : 4 buffer(2K bytes)
// purpose : Program data use use

// Buffer 133(0x85)
// Size : 1 buffer(512 bytes)
// purpose : USB EP0 Buffer point
#define USBEP0BUF_START_ADR0            0x0A
#define USBEP0BUF_START_ADR1            0x01
#define USBEP0BUF_START_ADR2            0x00
#define USBEP0BUF_LEN0                      0x01
#define USBEP0BUF_LEN1                      0x00

// Buffer 134~135(0x86~0x87)
// Size : 2 buffer(1024 bytes)
// purpose : CFDE use
#define FLAG_BASENO_CFDE_BLK            0x86
#define CFDE_BLK_ADR        (0x20C00UL + MEM_BAR)
#define CFDE_BLK_Base       ((unsigned char *)CFDE_BLK_ADR)

// Buffer 136~139(0x88~0x8B)
// Size : 4 buffer(2048 bytes)
// purpose : Info Page use
#define FLAG_BASENO_INFO_PAGE           0x88
#define INFO_PAGE_ADR       (0x21000UL + MEM_BAR)
#define INFO_PAGE_Base      ((unsigned char *)INFO_PAGE_ADR)

// Buffer 140~141(0x8C~0x8D)
// Size : 2 buffer(1024 bytes)
// purpose : Set Ready use
#define FLAG_BASENO_SET_RDY             0x8C
#define SET_RDY_ADR         (0x21800UL + MEM_BAR)
#define SET_RDY_Base        ((unsigned char *)SET_RDY_ADR)


// Buffer 142~151(0x8E~0x97)
// Size : 10 buffer
// purpose : Reserved for furture

// Buffer 152~189(0x98~0xBD)
// Size : 38 buffer(19K bytes)
// purpose : empty system info use
#define FLAG_BASENO_REQ_BLK            0x98
#define REQ_BLK_ADR         (0x23000UL + MEM_BAR)
#define REQ_BLK_Base        ((unsigned char *)REQ_BLK_ADR)

// Buffer 152~191(0x98~0xBF)
// Size : 40 buffer(20K bytes)
// purpose : temp buffer use
#define BUF_NUM_TEMP_BUF                0x98
#define BASE_ADDR_TEMP_BUF  (0x23000UL + MEM_BAR)
#define TEMP_BUF            ((unsigned char *)BASE_ADDR_TEMP_BUF)

// Buffer 192~255(0xC0~0xFF)
// Size : 64 buffer(32K bytes)
// purpose : ???
#define FLAG_BASENO_PBA_CACHE           0xC0
#define PBA_CACHE_ADR       (0x28000UL + MEM_BAR)
#define PBA_Cache_Buf       ((unsigned char *)PBA_CACHE_ADR)


// Buffer 192~255(0xC0~0xFF)
// Size : 64 buffer(32K bytes)
// purpose : USB EPn Buffer point
#define USBEPNBUF_START_ADR0            0x80
#define USBEPNBUF_START_ADR1            0x01
#define USBEPNBUF_START_ADR2            0x00

#define USBEPNBUF_LEN0                      0x40
#define USBEPNBUF_LEN1                      0x00
/*
#else  //Old mapping Program data use 0x3800~0x3FFF
// Buffer 128(0x80)
// Size : 1 buffer(512 bytes)
// purpose : FAT MBR use
#define FLAG_BASENO_FAT_MBR             0x80
#define FAT_MBR_ADR         0x20000UL
#define FAT_MBR_Base        ((unsigned char *)FAT_MBR_ADR)

// Buffer 137(0x89)
// Size : 1 buffer(512 bytes)
// purpose : DMA copy use
#define FLAG_BASENO_TEMP_USB            0x89
#define TEMP_USB_ADR        0x21200UL
#define TEMP_USB_Base      ((unsigned char *)TEMP_USB_ADR)

// Buffer 140(0x8C)
// Size : 2 buffer(512 bytes)
// purpose : CFDE use
#define FLAG_BASENO_CFDE_BLK            0x8C
#define CFDE_BLK_ADR        0x21800UL
#define CFDE_BLK_Base       ((unsigned char *)CFDE_BLK_ADR)

// Buffer 142(0x8E)
// Size : 4 buffer(2048 bytes)
// purpose : Info Page use
#define FLAG_BASENO_INFO_PAGE           0x8E
#define INFO_PAGE_ADR       0x21C00UL
#define INFO_PAGE_Base      ((unsigned char *)INFO_PAGE_ADR)

// Buffer 144~145(0x90~0x91)
// Size : 2 buffer(1K bytes)
// purpose : Set Ready use
#define FLAG_BASENO_SET_RDY             0x90
#define SET_RDY_ADR         0x22000UL
#define SET_RDY_Base        ((unsigned char *)SET_RDY_ADR)

// Buffer 146~177(0x92~0xB1)
// Size : 32 buffer(16K bytes)
// purpose : empty system info use
#define FLAG_BASENO_REQ_BLK            0x92
#define REQ_BLK_ADR         0x22400UL
#define REQ_BLK_Base        ((unsigned char *)REQ_BLK_ADR)

// Buffer 152(0x98)
// Size : 1 buffer(512 bytes)
// purpose : ???
#define FLAG_BASENO_SG_DATA             0x98
#define BA_PG_ADR           0x23000UL
#define BA_PG_Base          ((unsigned char *)BA_PG_ADR)


// Buffer 152(0x98)
// Size : 1 buffer(512 bytes)
// purpose : ???
#define SINGLE_DATA_ADR     0x23000UL
#define SG_DATA_Base        ((unsigned char *)SINGLE_DATA_ADR)

// Buffer 154(0x9A)
// Size : 1 buffer(512 bytes)
// purpose : USB EP0 Buffer point
#define USBEP0BUF_START_ADR0            0x34
#define USBEP0BUF_START_ADR1            0x01
#define USBEP0BUF_START_ADR2            0x00
#define USBEP0BUF_LEN0                      0x01
#define USBEP0BUF_LEN1                      0x00

// Buffer 156~159(0x9C~0x9F), 0x3800~0x3FFF
// Size : 4 buffer(2K bytes)
// purpose : Program data use use







// Buffer 192~255(0xC0~0xFF)
// Size : 64 buffer(32K bytes)
// purpose : ???
#define FLAG_BASENO_PBA_CACHE           0xC0
#define PBA_CACHE_ADR       0x28000UL
#define PBA_Cache_Buf       ((unsigned char *)PBA_CACHE_ADR)

// Buffer 192~255(0xC0~0xFF)
// Size : 64 buffer(32K bytes)
// purpose : USB EPn Buffer point
#define USBEPNBUF_START_ADR0            0x80
#define USBEPNBUF_START_ADR1            0x01
#define USBEPNBUF_START_ADR2            0x00

#define USBEPNBUF_LEN0                      0x40
#define USBEPNBUF_LEN1                      0x00
*/
#endif
#endif  //#ifndef __BUF_DEF_H__
