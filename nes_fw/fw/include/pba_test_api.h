/**
 * @file pba_test_api.h
 * Copyrihgt 2015 PHISON Electronics Corp.
 *
 * <b>
@verbatim
FILE NAME :  pba_test_api.h
PURPOSE   :  Header file for pba.c
AUTHOR    :
PHASE     :
Document  :
History   :
Date        Author          Version  Descrption
----------  --------------  -------  --------------------------------


@endverbatim
 * </b>\n
 *
 * @note
 * @bug
 */

#ifndef __PBA_TEST_API_H__
#define __PBA_TEST_API_H__

/*--------------------------------------------------------------------------*/
/* INCLUDE FILE                                                             */
/*--------------------------------------------------------------------------*/
#include "Var_def.h"
#include "pba_api.h"

/*--------------------------------------------------------------------------*/
/* PREDEFINED DIRECTIVE                                                     */
/*--------------------------------------------------------------------------*/
#ifdef __PBA_TEST_C__
#define EXTERN
#else
#define EXTERN extern
#endif

//#define EN_PBA_FEATURE    (1)
#define EN_PBA_TEST            (0)
#define EN_TOGGLE_MODE_TEST    (0)
#define EN_ONFI_MODE_TEST      (0)

//cache mode
#define EN_SW_MODE_TEST        (0)
#define EN_HW_ADDR_MODE_TEST   (1)
#define EN_RR_MODE_TEST        (0)

#define EN_CACHE_32SETS_TEST   (0)

#define EN_ADDR_BUF_TEST       (0)
#define EN_STATUS_BUF_BSY_TEST (0)
#define EN_BUSY_WINDOW_TEST    (0)
#define EN_INFO_BIT_TEST       (0)

#define EN_REG_WR_TEST         (0)

#define EN_CFDE_20_SETS        (1)
#define EN_CFDE_10_SETS        (0)




/*--------------------------------------------------------------------------*/
/* INCLUDE FILE                                                             */
/*--------------------------------------------------------------------------*/
//#include "USB_API.h"
//#include "Var_def.h"
//#include "pba_api.h"

/*--------------------------------------------------------------------------*/
/* CONSTANT DECLARATION                                                     */
/*--------------------------------------------------------------------------*/
#if (LUN_MODE == 0x00)
#define LUN_BIT0      (0x15)
#define LUN_BIT1      (0x7F)
#define LUN_BIT2      (0x7F)

#define LUN_BIT0_EN71 (0x00)
#define LUN_BIT1_EN71 (0x7F)
#define LUN_BIT2_EN71 (0x7F)

#elif (LUN_MODE == 0x01)
#define LUN_BIT0      (0x15)
#define LUN_BIT1      (0x7F)
#define LUN_BIT2      (0x7F)

#define LUN_BIT0_EN71 (0x00)
#define LUN_BIT1_EN71 (0x7F)
#define LUN_BIT2_EN71 (0x7F)

#elif (LUN_MODE == 0x02)
#define LUN_BIT0      (0x15)
#define LUN_BIT1      (0x16)
#define LUN_BIT2      (0x7F)

#define LUN_BIT0_EN71 (0x00)
#define LUN_BIT1_EN71 (0x01)
#define LUN_BIT2_EN71 (0x7F)

#elif (LUN_MODE == 0x03)
#define LUN_BIT0      (0x15)
#define LUN_BIT1      (0x16)
#define LUN_BIT2      (0x17)

#define LUN_BIT0_EN71 (0x00)
#define LUN_BIT1_EN71 (0x01)
#define LUN_BIT2_EN71 (0x02)
#endif

#if (PLANE_MODE == 0x00)
#define PLANE_BIT0    (0x08)
#define PLANE_BIT1    (0x7F)
#define PLANE_BIT2    (0x7F)
#elif (PLANE_MODE == 0x01)
#define PLANE_BIT0    (0x08)
#define PLANE_BIT1    (0x7F)
#define PLANE_BIT2    (0x7F)
#elif (PLANE_MODE == 0x02)
#define PLANE_BIT0    (0x08)
#define PLANE_BIT1    (0x09)
#define PLANE_BIT2    (0x7F)
#elif (PLANE_MODE == 0x03)
#define PLANE_BIT0    (0x08)
#define PLANE_BIT1    (0x09)
#define PLANE_BIT2    (0x0A)
#endif
/*--------------------------------------------------------------------------*/
/* MACRO DEFINITION                                                         */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/* TYPE DEFINITION                                                          */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/* GLOBAL VARIABLES DEFINITION                                              */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/* GLOBAL FUNCTION DECLARATION                                              */
/*--------------------------------------------------------------------------*/
//EXTERN void Init_PBA_Sys_Reg(void);
EXTERN void auto_handler(void);
EXTERN void pba_test(void);

#undef EXTERN
#endif // __PBA_TEST_API_H__

