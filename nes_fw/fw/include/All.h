#ifndef _ALL_H
#define _ALL_H
#include "BaseType.h"


bit gbDoCSW,gbEPxHalt,gbBadBulkCmd;
bit MassDirection;
bit fBadCBW;
bit MassError;
bit gbGetFromXdata;
bit gbStallEP0Cmd,gbAddressed,gbEP0CmdFinish;
bit Resetting;
bit EraseCheck, ReadCheck, WriteCheck;

data unsigned char gucRequestType, gucRequest, gucValue_L, gucValue_H, gucIndex_L, gucIndex_H;
data unsigned char gucxLength_L, gucxLength_H;

//For interrupt
data unsigned int  usb_interrupt_level2_H,usb_interrupt_level2_L;
idata unsigned char usb_interrupt_Event;
xdata unsigned char poll_fail_count;
data unsigned char  usb_interrupt_level1;
xdata unsigned char gucUsbSpeed;
xdata unsigned char MASS_LUN;
xdata unsigned char ShortPkt_Base[64];
xdata unsigned char X_CMDBLOCK[16];
xdata unsigned char gucStallOnGoing,gucStallMask;
xdata unsigned char X_bASC[MAX_CARD],X_bASCQ[MAX_CARD],X_SenseKey[MAX_CARD];
xdata unsigned int gu16EP0DataCount;
unsigned char code *ptrEP0DataPointer;
xdata unsigned char gucxMiscDescriptors[64];
xdata unsigned char gucConfiguredValue;
xdata unsigned char gucInterface;
xdata unsigned char MediaStatus[MAX_CARD];
xdata unsigned char X_CSWStatus;
xdata union LongInt X_MediaSize[MAX_CARD];
xdata unsigned char ErrorCode;

xdata union LongLBA X_Residue;
xdata union LongInt X_MASS_TAG;
xdata union LongInt X_MASS_XFR_LEN;
xdata union LongLBA LBA;
xdata union LongLBA LBA2;
xdata unsigned int SEC_CNT;
xdata unsigned char gucxMaxCard;

#endif
