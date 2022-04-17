/**
 * @file pba.h
 * Copyrihgt 2015 PHISON Electronics Corp.
 *
 * <b>
@verbatim
FILE NAME :  pba.h
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

#ifndef __PBA_H__
#define __PBA_H__

#ifdef __PBA_C__
#define EXTERN
#else
#define EXTERN extern
#endif

/*--------------------------------------------------------------------------*/
/* PREDEFINED DIRECTIVE                                                     */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/* INCLUDE FILE                                                             */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/* CONSTANT DECLARATION                                                     */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/* MACRO DEFINITION                                                         */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/* TYPE DEFINITION                                                          */
/*--------------------------------------------------------------------------*/
typedef struct {
    U8 *pbyPBACEReg;
    U8 byStatusBufIdx;

} STATUS_INFO_T, *STATUS_INFO_PTR;

/*--------------------------------------------------------------------------*/
/* LOCAL GLOBAL VARIABLES DEFINITION                                        */
/*--------------------------------------------------------------------------*/
/*
volatile unsigned char once_set_sts;
volatile unsigned int gw_truerdy_gap;
volatile unsigned char set_rdy_pos;
*/

//xdata unsigned char once_set_sts;
//xdata unsigned int gw_truerdy_gap;
//xdata unsigned char set_rdy_pos;
 //xdata unsigned char _gMax_Ce_Cnt;
 //xdata unsigned char _gMax_Lun_Cnt;
 //xdata unsigned char _gMax_Plane_Cnt;
/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTION PROTOTYPE                                                 */
/*--------------------------------------------------------------------------*/
static void init_STA_Buf_Def(void);

#undef EXTERN
#endif // __PBA_H__
