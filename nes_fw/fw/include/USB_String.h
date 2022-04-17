#ifndef __USB_STRING_H__
#define __USB_STRING_H__
//==================================================================================================================

#define FW_MAJ 0x32
#define FW_MIN 0x30
#ifdef __cplusplus
extern "C" {
#endif

#include "nes.h"

//==================================================================================================================

//Global Function
extern unsigned char code gucStringDescriptorLid[4];
extern unsigned char code gucStringDescriptor1[50];
extern unsigned char code gucStringDescriptor2[50];
extern unsigned char code gucSerialDescriptor[26];
extern unsigned char code gucSsDeviceDescriptor[];
extern unsigned char code gucSsConfigDescriptor[];
extern unsigned char code gucBinaryObjectStore[];
extern unsigned char code guc20DeviceDescriptor[];
extern unsigned char code gucHSConfigDescriptor[0x20];
extern unsigned char code gucFSConfigDescriptor[0x20];
extern unsigned char code gucHSDeviceQualifier[10];
extern unsigned char code gucFSDeviceQualifier[10];
extern unsigned char code  gucOtherSpeedConfigurationHS[0x20];
extern unsigned char code gucOtherSpeedConfigurationFS[0x20];
extern unsigned char code gucInquiryData[44] ;
extern unsigned char code gucMode_Sense6_053f[];
extern unsigned char code gucMode_Sense6_other[];
extern unsigned char code  gbc_Mode_Sense_PageCode05[];
extern unsigned char code  gbc_Mode_Sense_PageCode01[];
extern unsigned char code  gbc_Mode_Sense_PageCode1A[];
extern unsigned char code  gbc_Mode_Sense_PageCode2A[];
extern code unsigned char gbc_4K_Format_Capacity[];
extern code unsigned char gbc_Format_Capacity[];
extern code unsigned char gbc_4K_Capacity[];
extern code unsigned char gbc_4K_Capacity_16[];
extern code unsigned char gbc_Capacity[];
extern code unsigned char gbc_Capacity_16[];

extern code unsigned char gcodeFormat_Capacity[];

extern code unsigned char InquiryPageCode83[44];

#ifdef __cplusplus
}
#endif
#endif  //#ifndef __USB_STRING_H__
