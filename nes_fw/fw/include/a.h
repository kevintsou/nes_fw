#ifndef _A_H
#define _A_H
#include "BaseType.h"
#include "nes.h"

extern bit gbDoCSW,gbEPxHalt,gbBadBulkCmd;
extern bit MassDirection;
extern bit fBadCBW;
extern bit MassError;
extern bit gbGetFromXdata;
extern bit gbStallEP0Cmd,gbAddressed,gbEP0CmdFinish;
extern bit Resetting;
extern bit EraseCheck, ReadCheck, WriteCheck;

extern data unsigned char gucRequestType, gucRequest, gucValue_L, gucValue_H, gucIndex_L, gucIndex_H;
extern data unsigned char gucxLength_L, gucxLength_H;

//For interrupt
extern data unsigned int  usb_interrupt_level2_H,usb_interrupt_level2_L;
extern idata unsigned char  usb_interrupt_Event;
extern xdata unsigned char poll_fail_count;
extern data unsigned char  usb_interrupt_level1;
extern xdata unsigned char MASS_LUN;
extern xdata unsigned char ShortPkt_Base[64];
extern xdata unsigned char X_CMDBLOCK[16];
extern xdata unsigned char gucStallOnGoing,gucStallMask;
extern xdata unsigned char X_bASC[MAX_CARD],X_bASCQ[MAX_CARD],X_SenseKey[MAX_CARD];
extern xdata unsigned int gu16EP0DataCount;
extern unsigned char code *ptrEP0DataPointer;
extern xdata unsigned char gucxMiscDescriptors[64];
extern xdata unsigned char gucConfiguredValue;
extern xdata unsigned char gucInterface;
extern xdata unsigned char MediaStatus[MAX_CARD];
extern xdata unsigned char X_CSWStatus;
extern xdata union LongInt X_MediaSize[MAX_CARD];
extern xdata unsigned char ErrorCode;
	
extern xdata unsigned char gucUsbSpeed;
extern xdata union LongLBA X_Residue;
extern xdata union LongInt X_MASS_TAG;
extern xdata union LongInt X_MASS_XFR_LEN;
extern xdata union LongLBA LBA;
extern xdata unsigned int SEC_CNT;
extern xdata unsigned char gucxMaxCard;
extern xdata union LongLBA LBA2;

#endif
