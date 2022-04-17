#ifndef __USB_API_H__
#define __USB_API_H__
//==================================================================================================================
#ifdef __cplusplus
extern "C" {
#endif
#include "nes.h"
#define  FREQ125MHZ  (1)
#if FREQ125MHZ
#define MAX_VAL_CFDE_INT                5000
#define MAX_VAL_WHILE_POLL_CMD  7000
#define MAX_VAL_WHILE                       2000
#define MAX_VAL_WHILE2                  5500
#define MAX_VAL_REQ_INT                 5
#else
#define MAX_VAL_CFDE_INT                2500
#define MAX_VAL_WHILE_POLL_CMD  3500
#define MAX_VAL_WHILE                       1000
#define MAX_VAL_WHILE2                  2750
#define MAX_VAL_REQ_INT                 3
#endif//#if FREQ125MHZ


extern BYTE xdata gb_Interface;
extern data SWORD gw_In_EP_Offset, gw_Out_EP_Offset;

//==================================================================================================================

//Global Function
//bit BotHxDn();
extern void Usb_Stall_Bulk();
extern void Trig_USB(SWORD aw_EPn_Offset);
extern void PhyInit(void);
extern void UsbInit();
extern void PHY_Set_Reg();
extern void usb_Bus_Condition();
extern void Usb_ep0setup();
extern void Usb_CBW_Out(void);
extern void Set_Media_OK();
extern void Set_Media_NG();
extern void errASCSenseKey(BYTE ucASC, BYTE ucSenseKey);
extern void FifoShortPkt(BYTE *ab_ptrString, SWORD aw_Length);
extern void Set_LUN_Error();
extern void Invalid_Field_In_Command();
extern void USB_Trig_Receive(bit is_hold);
extern void Receive_All_data();
extern void Invalid_Cmd();
extern void UsbHandler(void);
extern void UsbHandler_Bulk();
extern void Return_CSW();
extern void Init_PBA();
extern void Set_NO_Media();
extern void USBBulkInSectorFromPageBuffer(BYTE Buf_No, SWORD Sec_Cnt, BYTE Buf_Mode);
extern void USBBulkOutSectorToPageBuffer(BYTE Buf_No, SWORD Sec_Cnt, BYTE Buf_Mode);
extern void Save_Info();
//extern void Get_LBA();
extern void Send_Link_Err_Count();

void Set_USB_Transfer_Buf_Setting(UINT8 ab_Buf_Page, UINT8 ab_Flag_Length, UINT16 aw_SEC_Cnt);
void Set_BRA_Buf_Flag(UINT8 ab_WR, UINT8 ab_BufPage, UINT8 ab_FBLen);
void Set_USB_Buf_TOP_Addr(UINT8 ab_WR, UINT8 ab_TOP_Start_Buf, UINT8 ab_Buf_Size);

#ifdef __cplusplus
}
#endif
#endif  //#ifndef __USB_API_H__
