#ifndef __DMA_REG_H__
#define __DMA_REG_H__
//==================================================================================================================

#ifndef D_NES_CMODEL
#define DMAREG ((volatile unsigned char *)0x2F180UL)
#else
#include "nes_cmodel.h"
#pragma comment(lib, "nes_cmodel.lib")
#endif
//==================================================================================================================

#define DMA_SRHB_L              0x00    /* Operation Start/Source Address Low */
#define DMA_SRHB_M              0x01    /* Operation Start/Source Address Mid */
#define DMA_SRHB_H              0x02    /* Operation Start/Source Address High */
#define DMA_SRHE_L              0x04    /* Operation Destination Address Low */
#define DMA_SRHE_M              0x05    /* Operation Destination Address Mid */
#define DMA_SRHE_H              0x06    /* Operation Destination Address High */
#define DMA_LEN_L               0x08    /* Operation Length */
#define DMA_LEN_M               0x09
#define DMA_LEN_H               0x0A
#define DMA_PTN_L               0x0C    /* DMA Pattern Register (Low) */
#define DMA_PTN_M               0x0D
#define DMA_PTN_H               0x0E
#define DMA_CTRL                0x0F    /* DMA Control Register */
#define WLC_BYTE_MODE       BIT7
#define COMPARE_EQUAL       BIT6
#define PATTERN_MATCH       BIT6
#define SDRAM2RA                BIT4
#define OP_WLC              0x1
#define OP_COPY             0x2
#define OP_COMP             0x3
#define OP_SET              0x4
#define OP_CHKSUM           0x5
#define OP_SDRAM                0x6

#define DMA_MAXADR_L            0x10    /* WLC Max Count Address Register Low */

#define DMA_MAXADR_M            0x11    /* WLC Max Count Address Register Mid */

#define DMA_MAXADR_H            0x12    /* WLC Max Count Address Register High */

#define DMA_MAXVAL_L            0x14    /* WLC Max Count Value Register Low */

#define DMA_MAXVAL_H            0x15    /* WLC Max Count Value Register High */

#define DMA_MINADR_L            0x18    /* WLC Min Count Address Register Low */

#define DMA_MINADR_M            0x19    /* WLC Min Count Address Register Mid */

#define DMA_MINADR_H            0x1A    /* WLC Min Count Address Register High */

#define DMA_MINVAL_L            0x1C    /* WLC Min Count Value Register Low */

#define DMA_MINVAL_H            0x1D    /* WLC Min Count Value Register High */

#define DMA_EQUADR_L            0x20    /* WLC Equivalent Address Register Low */

#define DMA_EQUADR_M            0x21    /* WLC Equivalent Address Register Mid */

#define DMA_EQUADR_H            0x22    /* WLC Equivalent Address Register High */

#define DMA_CHKSUML             0x24    /* Check Sum Register(Low Byte)*/

#define DMA_CHKSUMH             0x25    /* Check Sum Register(High Byte)*/

#define DMA_CRC32VAL_LL         0x2C    /* CRC32 value Register(Low Low Byte)*/

#define DMA_CRC32VAL_LH         0x2D    /* CRC32 value Register(Low High Byte)*/

#define DMA_CRC32VAL_HL         0x2E    /* CRC32 value Register(High Low Byte)*/

#define DMA_CRC32VAL_HH         0x2F    /* CRC32 value Register(High High Byte)*/

#define SDRAM_SADR_L            0x30    /*SDRAM souce address L*/

#define SDRAM_SADR_M            0x31    /*SDRAM souce address M*/

#define SDRAM_SADR_H            0x32    /*SDRAM souce address H*/

#define SDRAM_SADR_HH           0x33    /*SDRAM souce address HH*/

#define SDRAM_LEN_L             0x34    /*SDRAM byte len L*/

#define SDRAM_LEN_M             0x35    /*SDRAM byte len M*/

#define SDRAM_LEN_H             0x36    /*SDRAM byte len H*/

#define SDRAM_LEN_HH            0x37    /*SDRAM byte len HH*/

#define SDRAM_BUF_SIZE      0x38    /*SDRAM buffer size*/

#define SDRAM_SPTR              0x39    /*SDRAM start point*/

#define SDRAM_PTR_BASE      0x3A    /*SDRAM point base*/

#define SDRAM_BUF_OFFSET    0x3B    /*SDRAM buffer no offset*/


#endif  //#ifndef __DMA_REG_H__