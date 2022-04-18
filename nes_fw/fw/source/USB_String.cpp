#include "USB_String.h"
#include "USB_Def.h"




//==================================================================================================================

#define VID_L    0xfe
#define VID_H    0x13
#define PID_L    0x00 
#define PID_H    0x50 

unsigned char code gucStringDescriptorLid[4]=
{
	0x04,
	0x03,
	0x09,0x04 // language ID, English(US)
};

unsigned char code InquiryPageCode83[44] =   //PageCODE 83
{
	0x00,0x83,0x06,0x28,
	0x02,0x01,0x00,0x0C,
	0x55,0x53,0x42,0x20, //USB
	0x44,0x49,0x53,0x4B, //DISK
	0x20,0x32,0x2E,0x30, //2.0
	0x01,0x02,0x00,0x08,
	0x87,0x65,0x43,0x2A,
	0xB5,0x67,0xAB,0xCD,
	0x01,0x03,0x00,0x08,
	0x20,0x00,0xAC,0xDE,
	0x48,0x23,0x45,0x67
};

unsigned char code gucStringDescriptor1[50] = 
{
	50,    //STRING_LENGTH1, // length
	0x03,  // Descriptor type String = 3
	'P',
	0x00,
	'h',
	0x00,
	'i',
	0x00,
	's',
	0x00,
	'o',
	0x00,
	'n',
	0x00,
	' ',
	0x00,
	' ',
	0x00,
	' ',
	0x00,
	' ',
	0x00,
	' ',
	0x00,
	' ',
	0x00,
	' ',
	0x00,
	' ',
	0x00,
	' ',
	0x00,
	' ',
	0x00,
	' ',
	0x00,
	' ',
	0x00,
	' ',
	0x00,
	' ',
	0x00,
	' ',
	0x00,
	' ',
	0x00,
	' ',
	0x00,
	' ',
	0x00
};
//==================================================================================================================

unsigned char code gucStringDescriptor2[50] = 
{
	50,    //STRING_LENGTH2, // length
	0x03,  // Descriptor type String = 3
	'3',
	0x00,
	'4',
	0x00,
	'3',
	0x00,
	'2',
	0x00,
	' ',
	0x00,
	'F',
	0x00,
	'W',
	0x00,
	' ',
	0x00,
	' ',
	0x00,
	' ',
	0x00,
	' ',
	0x00,
	' ',
	0x00,
	' ',
	0x00,
	' ',
	0x00,
	' ',   
	0x00,
	' ',
	0x00,
	' ',   
	0x00,
	' ',   
	0x00,
	' ',   
	0x00,
	' ',   
	0x00,
	' ',
	0x00,
	' ',
	0x00,
	' ',
	0x00,
	' ',
	0x00
};
//==================================================================================================================

unsigned char code gucSerialDescriptor[26] = 
{
	26,//SERIAL_LENGTH,
	0x03,
	'0',
	0x00,
	'0',
	0x00,
	'0',
	0x00,
	'0',
	0x00,
	'0',
	0x00,
	'0',
	0x00,
	'0',
	0x00,
	'0',
	0x00,
	'0',
	0x00,
	'0',
	0x00,
	'0',
	0x00,
	'2',
	0x00
};


unsigned char code gucSsDeviceDescriptor[]=
{//[DEVICE_LENGTH]={
	0x12, // length
	0x01, // Descriptor type Device = 1
	0x00, // USB version: 0x0300, little endian
	0x03,
	0x00, // device class
	0x00, // device sub-class
	0x00, // device protocol
	0x09, // max packet size 2**9=512
	VID_L,
	VID_H,
	PID_L,
	PID_H,
	0x10, // low byte, device release number = 1.10
	0x01, // high byte
	0x01, // index of string descriptor for manufactory
	0x02, // index of string descriptor for product
	0x00, // 0x03, index of string descriptor for serial number. 0x00:no serial
	0x01  // number of configuration
};

unsigned char code gucSsConfigDescriptor[]=
{//[CONFIG_LENGTH] // Super Speed Configuration
	0x09, // length
	0x02, // descriptor type configuration = 2
	0x2c, //CONFIG_LENGTH,// total length
	0x00, //CONFIG_LENGTH>>8,
	0x01, // number of interface
	0x01, // configuration value???
	0x00, // index of string descriptor for configuration description
	0x80, // Configuration attributes D7: Reserved, must 1, D6: Self powered,
	   // D5: Remote wakeup
	0x0c, // max power, unit = 8mA, Self-Powered

	// interface 0 
	0x09, // length
	0x04, // descriptor type, interface = 4
	0x00, // interface value
	0x00, // alternative setting value
	0x02, // endpoints number
	0x08, // interface class, MASS SOTRAGE CLASS
	0x06, // interface sub-class, SFF8020i
	0x50, // interface protocol, BULK ONLY PROTOCAL
	0x00, // index of string descriptor for describing interface

	// endpoint 1
	0x07, // length
	0x05, // descriptor type endpoint = 5
	0x81, // Endpoint Address. D7: direction (0:out, 1:in), D6..4: reserved,
	   // D3..0: endpoint number
	0x02, // attributes: 0:control, 1:isochronous, 2:bulk, 3:interrupt
	0x00, //EPASIZE, // max packet size = 1024
	0x04,
	0x00, // polling interval, 1 for isochronous, 1~255 for interrupt

	//endpoint 1 companion.
	0x06, // length
	0x30, // descriptor type = endpoint companion = 0x30
	0x03, // maximum burst. 0 = one packet per burst, 15 = 16 packets per burst
	0x00, // maximum stream. 0 = no stream
	0x00, // reserved for periodic endpoints
	0x00, // reserved for periodic endpoints

	//endpoint 2
	0x07, // length
	0x05, // Descriptor type endpoint = 5
	0x02, // endpoint address: D7 for direction(0:out, 1:in),
	   // D3..0 endpoint number
	0x02, // attribute: 0 control, 1:isochronous, 2:bulk, 3:interrupt
	0x00, // max packet size = 1024
	0x04, //
	0x00, // polling interval

	//endpoint 2 companion.
	0x06, // length
	0x30, // descriptor type = endpoint companion = 0x30
	0x03, // maximum burst. 0 = one packet per burst, 15 = 16 packets per burst
	0x00, // maximum stream. 0 = no stream
	0x00, // reserved for periodic endpoints
	0x00, // reserved for periodic endpoints

};

unsigned char code gucBinaryObjectStore[]=
{
	0x05, // bLength
	0x0f, // bDescriptorType = 0x0f = BOS
	0x16, // wTotalLength, for this descriptors and all the other descriptors
	0x00, // wTotalLength, high byte
	0x02, // bNumDeviceCaps, number of device capabilities

	0x07, // bLength, for the descriptor of USB 2.0 extension
	0x10, // bDescriptorType = 16 = device capability
	0x02, // bDeviceCapabilityType, 02 = USB 2.0 extension descriptor
	0x02, // bmAttribute, bit 1 = LTM capability (Latency Tolerance Messages capability
	0x00, // reserved
	0x00, // reserved
	0x00, // reserved

	0x0a, // bLength, for the descriptor of superspeed USB capability
	0x10, // bDescriptorType = 16 = device capability
	0x03, // bDeviceCapabilityType, 03 = supper speed USB specific device level capability
	0x00, // bmAttribute, bit 1 = LTM capability (Latency Tolerance Messages capability
	0x0e, // wSpeedSupported, bit3= SuperSpeed, bit2=hi_speed, bit1=full_speed, bit0=low_speed
	0x00, // wSpeedSupported, high byte
	0x02, // bFunctionalitySupport. which lowest speed supporte all the function. bit1=full speed
	0x0a, // bU1DevExitLat, U1 device exit latency. latency to transition from U1 to U0, unit = usec
	0xff, // wU2DevExitLat, U2 device exit latency. latency to transition from U1 to U0, unit = usec
	0x07, // wU2DevExitLat, high byte. 0x0800 us = 2048us
};

unsigned char code guc20DeviceDescriptor[]=
{//[DEVICE_LENGTH]={
	0x12, // length
	0x01, // Descriptor type Device = 1
	0x00, // USB version: 0x0200, little endian
	0x02,
	0x00, // device class
	0x00, // device sub-class
	0x00, // device protocol
	0x40, // max packet size
	VID_L,
	VID_H,
	PID_L,
	PID_H,
	0x10, // device release number = 1.10
	0x01,
	0x01, // index of string descriptor for manufactory
	0x02, // index of string descriptor for product
	0x00, // 0x03, index of string descriptor for serial number. 0x00:no serial
	0x01, // number of configuration
};

unsigned char code gucHSConfigDescriptor[0x20]=
{//[CONFIG_LENGTH]={ 		// High Speed Configuration
	0x09, // length
	0x02, // descriptor type configuration = 2
	0x20, //CONFIG_LENGTH,	// total length
	0x00, //CONFIG_LENGTH>>8,
	0x01, // number of interface
	0x01, // configuration value???
	0x00, // index of string descriptor for configuration description
	0x80, // Configuration attributes D7: Reserved, must 1, D6: Self powered,
	   //D5: Remote wakeup
	0x32, //0xF9, // max power, unit = 2mA, Self-Powered

	// interface 0 (0)
	0x09, // length
	0x04, // descriptor type, interface = 4
	0x00, // interface value
	0x00, // alternative setting value
	0x02, // endpoints number
	0x08, // interface class, MASS SOTRAGE CLASS
	0x06, // interface sub-class, SFF8020i
	0x50, // interface protocol, BULK ONLY PROTOCAL
	0x00, // index of string descriptor for describing interface

	// end point 1
	0x07, // length
	0x05, // descriptor type endpoint = 5
	0x81, // Endpoint Address. D7: direction (0:out, 1:in), D6..4: reserved,
	   // D3..0: endpoint number
	0x02, // attributes: 0:control, 1:isochronous, 2:bulk, 3:interrupt
	0x00, //EPASIZE, // max packet size = 512
	0x02,
	0x00, // polling interval, 1 for isochronous, 1~255 for interrupt

	// end point 2
	0x07, // length
	0x05, // Descriptor type endpoint = 5
	0x02, // endpoint address: D7 for direction(0:out, 1:in),
	   // D3..0 endpoint number
	0x02, // attribute: 0 control, 1:isochronous, 2:bulk, 3:interrupt
	0x00, // max packet size
	0x02, //
	0x00, // polling interval
	// end point 3
	//0x07, // length
	//0x05,	// descriptor type endpoint = 5
	//0x83,	// Endpoint Address. D7: direction (0:out, 1:in), D6..4: reserved,
	//      // D3..0: endpoint number
	//0x03,	// attributes: 0:control, 1:isochronous, 2:bulk, 3:interrupt
	//0x40,	// max packet size = 0x40
	//0x00,
	//0x04,	// polling interval, 1 for isochronous, 1~255 for interrupt
};

unsigned char code gucFSConfigDescriptor[0x20]=
{//[CONFIG_LENGTH]={ 		// Full speed configuration
	0x09, // length
	0x02, // descriptor type configuration = 2
	0x20, //CONFIG_LENGTH, // total length
	0x00, //CONFIG_LENGTH>>8,
	0x01, // number of interface
	0x01, // configuration value???
	0x00, // index of string descriptor for configuration description
	0x80, // Configuration attributes D7: Reserved, must 1, D6: Self powered,
	   // D5: Remote wakeup
	0x32, // max power, unit=2mA for hispeed. Self-Powered

	// interface 0 (0)
	0x09, // length
	0x04, // descriptor type, interface = 4
	0x00, // interface value
	0x00, // alternative setting value
	0x02, // endpoints number
	0x08, // interface class, MASS SOTRAGE CLASS
	0x06, // interface sub-class, SFF8020i
	0x50, // interface protocol, BULK ONLY PROTOCAL
	0x00, // index of string descriptor for describing interface

	// end point 1
	0x07, // length
	0x05, // descriptor type endpoint = 5
	0x81, // Endpoint Address. D7: direction (0:out, 1:in), D6..4: reserved,
	   // D3..0: endpoint number
	0x02, // attributes: 0:control, 1:isochronous, 2:bulk, 3:interrupt
	0x40, // max packet size = 0
	0x00,
	0x00, // polling interval, 1 for isochronous, 1~255 for interrupt

	// end point 2
	0x07, // length
	0x05, // Descriptor type endpoint = 5
	0x02, // endpoint address: D7 for direction(0:out, 1:in),
	   // D3..0 endpoint number
	0x02, // attribute: 0 control, 1:isochronous, 2:bulk, 3:interrupt
	0x40, // max packet size
	0x00, //
	0x00, // polling interval

	// end point 3
	//0x07, // length
	//0x05, // descriptor type endpoint = 5
	//0x83, // Endpoint Address. D7: direction (0:out, 1:in), D6..4: reserved,
	     // D3..0: endpoint number
	//0x03, // attributes: 0:control, 1:isochronous, 2:bulk, 3:interrupt
	//0x40, // max packet size = 0x40
	//0x00,
	//0x01, // polling interval, 1 for isochronous, 1~255 for interrupt
};

unsigned char code gucHSDeviceQualifier[10]=
{ //[DEVICE_QUALIFIER_LENGTH]=
	10,   //DEVICE_QUALIFIER_LENGTH,// length
	0x06, //DEVICE_QUALIFIER, // Device Qualifier Type
	0x00, // USB version: 0x0200, little endian
	0x02,
	0x00, // device class, FS
	0x00, // device sub-class, FS
	0x00, // device protocol, FS
	0x40, // max packet size, FS
	0x01, // Number of Other-speed Configurations, FS
	0x00  // Reserved
};

unsigned char code gucFSDeviceQualifier[10]=
{//[DEVICE_QUALIFIER_LENGTH]={
	10,   // DEVICE_QUALIFIER_LENGTH, // length
	0x06, // DEVICE_QUALIFIER, // Device Qualifier Type
	0x00, // USB version: 0x0200, little endian
	0x02,
	0x00, // device class
	0x00, // device sub-class, HS
	0x00, // device protocol, HS
	0x40, // max packet size, HS
	0x01, // Number of Other-speed Configurations, HS
	0x00  // Reserved
};

unsigned char code  gucOtherSpeedConfigurationHS[0x20]=
{//[OTHER_SPEED_CONFIGURATION_LENGTH]={
	0x09, // bLength
	0x07, // OTHER_SPEED_CONFIGURATION,  // bDescriptorType
	0x20, // OTHER_SPEED_CONFIGURATION_LENGTH,// wTotalLength
	0x00, // OTHER_SPEED_CONFIGURATION_LENGTH>>8,
	0x01, // bNumberInterfaces
	0x01, // bConfigurationValue
	0x00, // iConfiguration
	0x80, // Configuration attributes D7: Reserved, must 1, D6: Self powered,
	   // D5: Remote wakeup
	0xF9, // bMaxPower, mA

	// interface 0 (0)
	0x09, // length
	0x04, // descriptor type, interface = 4
	0x00, // interface value
	0x00, // alternative setting value
	0x02, // endpoints number
	0x08, // interface class, MASS SOTRAGE CLASS
	0x06, // interface sub-class, SFF8020i
	0x50, // interface protocol, BULK ONLY PROTOCAL
	0x00, // index of string descriptor for describing interface

	// endpoint 1
	0x07, // length
	0x05, // descriptor type endpoint = 5
	0x81, // Endpoint Address. D7: direction (0:out, 1:in), D6..4: reserved,
	   // D3..0: endpoint number
	0x02, // attributes: 0:control, 1:isochronous, 2:bulk, 3:interrupt
	0x40, //EPASIZE, // max packet size = 0
	0x00,
	0x00, // polling interval, 1 for isochronous, 1~255 for interrupt

	// endpoint 2
	0x07, // length
	0x05, // Descriptor type endpoint = 5
	0x02, // endpoint address: D7 for direction(0:out, 1:in),
	   // D3..0 endpoint number
	0x02, // attribute: 0 control, 1:isochronous, 2:bulk, 3:interrupt
	0x40, // max packet size
	0x00, //
	0x00, // polling interval

	// end point 3
	//0x07, // length
	//0x05, // descriptor type endpoint = 5
	//0x83, // Endpoint Address. D7: direction (0:out, 1:in), D6..4: reserved,
	//      //D3..0: endpoint number
	//0x03, // attributes: 0:control, 1:isochronous, 2:bulk, 3:interrupt
	//0x40, // max packet size = 0x40
	//0x00,
	//0x04, // polling interval, 1 for isochronous, 1~255 for interrupt
};

unsigned char code gucOtherSpeedConfigurationFS[0x20]=
{//[OTHER_SPEED_CONFIGURATION_LENGTH]={
	0x09, // bLength
	0x07, // OTHER_SPEED_CONFIGURATION, // bDescriptorType
	0x20, // OTHER_SPEED_CONFIGURATION_LENGTH, // wTotalLength
	0x00, // OTHER_SPEED_CONFIGURATION_LENGTH>>8,
	0x01, // bNumberInterfaces
	0x01, // bConfigurationValue
	0x00, // iConfiguration
	0x80, // Configuration attributes D7: Reserved, must 1, D6: Self powered,
	   // D5: Remote wakeup
	0xF9, // bMaxPower, mA

	// interface 0 (0)
	0x09, // length
	0x04, // descriptor type, interface = 4
	0x00, // interface value
	0x00, // alternative setting value
	0x02, // endpoints number
	0x08, // interface class, MASS SOTRAGE CLASS
	0x06, // interface sub-class, SFF8020i
	0x50, // interface protocol, BULK ONLY PROTOCAL
	0x00, // index of string descriptor for describing interface

	// endpoint 1
	0x07, // length
	0x05, // descriptor type endpoint = 5
	0x81, // Endpoint Address. D7: direction (0:out, 1:in), D6..4: reserved,
	   // D3..0: endpoint number
	0x02, // attributes: 0:control, 1:isochronous, 2:bulk, 3:interrupt
	0x00, //EPASIZE, // max packet size = 512
	0x02,
	0x00, // polling interval, 1 for isochronous, 1~255 for interrupt

	// endpoint 2
	0x07, // length
	0x05, // Descriptor type endpoint = 5
	0x02, // endpoint address: D7 for direction(0:out, 1:in),
	   // D3..0 endpoint number
	0x02, // attribute: 0 control, 1:isochronous, 2:bulk, 3:interrupt
	0x00, // max packet size
	0x02, //
	0x00, // polling interval

	// end point 3
	//0x07,		// length
	//0x05,		// descriptor type endpoint = 5
	//0x83,		// Endpoint Address. D7: direction (0:out, 1:in), D6..4: reserved,
	//			//	D3..0: endpoint number
	//0x03,		// attributes: 0:control, 1:isochronous, 2:bulk, 3:interrupt
	//0x40,	// max packet size = 0x40
	//0x00,
	//0x01,		// polling interval, 1 for isochronous, 1~255 for interrupt
};

unsigned char code gucInquiryData[44] =
{//head of inquiry data
	0x00, // (Byte 0) bit7: Reserved, bit6~0: Peripheral Device Type
	0x80, // (Byte 1) bit7: RMB, bit6~0: Reserved
	0x00, // (Byte 2) ISO, ECMA, ANSI Version
	0x01, // (Byte 3) bit7~4: ATAPI Version, bit3~0: Response Data Format
	0x1F, // (Byte 4) Additional Length (n-4)
	0x00, 0x00, 0x00, // (Byte 5 ~ 7) Reserved

	//' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  //(Byte 8~15) manufacture name
	//'U', 'S', 'B', ' ', 'D', 'I', 'S', 'K',  //(Byte 16~31) product name
	//' ', '5', '0', 'X', ' ', ' ', ' ', ' ',
	//'3', '.', '0', '0',                      //(Byte 32~35) version
	//'P', 'M', 'A', 'P','1', '2', '3', '4',   //special tag

  'P',    'h',    'i',    's',    'o',    'n',    ' ',    ' ',    //(Byte 8~15) manufacture name
  'P',    'S',    '3',    '4',    '3',    '4',    ' ',    'N',    //(Byte 16~31) product name
  'A',    'N',    'D',    '_',    'E',    'm',    'u',    ' ',
	'P', 'M', 'A', 'P',                      //(Byte 32~35) version
	'1', '2', '3', '4', ' ', ' ', ' ', ' '   //special tag
};


unsigned char code gucMode_Sense6_053f[]=
{
	0x00, //byte 3
	0x00, //byte 2
	0x00, //byte 1  
	0x35, //byte 0

	0x00, //byte 7
	0xf0, //byte 6
	0x1e, //byte 5
	0x05, //byte 4

	0x00, //byte 11
	0x02, //byte 10 
	0x20, //byte 9, 0x12  for floppy
	0xff, //byte 8, 0x02  for floppy

	0x00, //byte 15
	0x00, //byte 14
	0x00, //byte 13, cylinder_L
	0x02  //byte 12, cylinder_H
};

unsigned char code gucMode_Sense6_other[]=
{
	0x00, //byte 3
	0x08, //byte 2
	0x00, //byte 1, // 0x94 for MEDIA_FLOPPY
	0x03  //byte 0
};

code unsigned char gcodeFormat_Capacity[]=
{
	0x00U, /*reserved*/
	0x00U, /*reserved*/
	0x00U, /*reserved*/
	0x08U, /*capacity list length*/
	0x00U, /*number of blocks, MSB*/
	0x00U,
	0x10U,
	0x00U, /*number of blocks, LSB*/
	0x03U, /*descriptor code*/
	0x00U, /*block length, MSB*/
	0x02U,
	0x00U  /*block length, LSB*/
};

code unsigned char gbc_4K_Format_Capacity[] = {
    0x00,   //reserved
    0x00,   //reserved
    0x00,   //reserved
    0x08,   //capacity list length
    0x00,   //number of blocks, MSB
    0x00,
    0x00,
    0x00,   //number of blocks, LSB,
    0x02,   //descriptor code
    0x00,   //block length, MSB
    0x10,   //4KB for a block
    0x00,   //block length, LSB
};

code unsigned char gbc_Format_Capacity[] = {
    0x00,   //reserved
    0x00,   //reserved
    0x00,   //reserved
    0x08,   //capacity list length
    0x00,   //number of blocks, MSB
    0x00,
    0x00,
    0x00,   //number of blocks, LSB,
    0x02,   //descriptor code
    0x00,   //block length, MSB
    0x02,   //512B for a block
    0x00,   //block length, LSB
};

//==================================================================================================================
code unsigned char gbc_4K_Capacity[] = {
    0x00,   //last logical block address, MSB
    0x00,
    0x00,
    0x00,   //last logical block address, LSB
    0x00,   //block length in byte, MSB
    0x00,
    0x10,   //4KB
    0x00,   //block length in byte, LSB
};

code unsigned char gbc_4K_Capacity_16[32] = {
    0x00,   //last logical block address, MSB
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,   //last logical block address, LSB
    0x00,   //block length in byte, MSB
    0x00,
    0x10,   //4KB
    0x00,   //block length in byte, LSB
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
};

code unsigned char gbc_Capacity[] = {
    0x00,   //last logical block address, MSB
    0x00,
    0x00,
    0x00,   //last logical block address, LSB
    0x00,   //block length in byte, MSB
    0x00,
    0x02,   //512B
    0x00,   //block length in byte, LSB
};

code unsigned char gbc_Capacity_16[32] = {
    0x00,   //last logical block address, MSB
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,   //last logical block address, LSB
    0x00,   //block length in byte, MSB
    0x00,
    0x02,
    0x00,   //block length in byte, LSB
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
};
//==================================================================================================================
