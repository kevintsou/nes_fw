#ifndef __USB_DEF_H__
#define __USB_DEF_H__
//==================================================================================================================

//Bot COUNT
#define BOT_COUNT               1

//
#define _WRITE      0x00        // For ab_RW_Option.Bit0 define
#define _READ       0x01        // For ab_RW_Option.Bit0 define

//Bot switch
#define BOT_0                   0  /* Interface 0(EP1in, EP6out) */
#define BOT_1                   1  /* Interface 1(EP5in, EP2out) */

//Bot Situation
#define IDEL_SAT                0
#define CSW_SAT                 1
#define WAIT_TRAN_SAT           2
#define WAIT_QUEUE_SAT          3


//Mass Storage define
#define NO_MEDIA_BIT            BIT0
#define STOP_DEVICE_BIT         BIT1
#define PASSWORD_LOCK_BIT       BIT2
#define MEDIA_ALTERNATE_BIT     BIT3
#define MEDIA_WRITEPROTECT_BIT  BIT4
#define MEDIA_SFWP_BIT          BIT5
#define INIT_FAIL_BIT           BIT7

#define EP1 0x00
#define EP2 0x40
#define EP3 0x80
#define EP4 0xC0
#define EP5 0x100
#define EP6 0x140

#define NUM_EP0                 0
#define NUM_EP1                 1
#define NUM_EP2                 2
#define NUM_EP3                 3
#define NUM_EP4                 4
#define NUM_EP5                 5
#define NUM_EP6                 6

#define EP1_NO    (1U)
#define EP2_NO    (2U)
#define EP3_NO    (3U)
#define EP4_NO    (4U)
#define EP5_NO    (5U)
#define EP6_NO    (6U)
#define EP7_NO    (7U)
#define EP8_NO    (8U)

#define ENDPOINT1    (0x40 * (EP1_NO - 1))
#define ENDPOINT2    (0x40 * (EP2_NO - 1))
#define ENDPOINT3    (0x40 * (EP3_NO - 1))
#define ENDPOINT4    (0x40 * (EP4_NO - 1))
#define ENDPOINT5    (0x40 * (EP5_NO - 1))
#define ENDPOINT6    (0x40 * (EP6_NO - 1))
#define ENDPOINT7    (0x40 * (EP7_NO - 1))
#define ENDPOINT8    (0x40 * (EP8_NO - 1))

//-----Event handle---
// bit assignment for gucBusEvent
#define USB2_RSUM               BIT0
#define USB2_SPND               BIT1
#define USB2_L1RUSM             BIT2
#define USB2_RST                BIT3
#define USB3_DISABLE            BIT4
#define USB3_HOTRST             BIT5
#define USB3_WRMRST             BIT6
#define USB3_LNKCNG             BIT7

// bit assignment for gucUsbEventL
#define EP0_SETUP               BIT0
#define EP2_BKORDY              BIT1

//constant for USB Descriptors
#define DEVICE_LENGTH				0x12
#define CONFIG_LENGTH				0x2C
#define CONFIG_LENGTH20			0x20
#define STRING_LENGTH1				50
#define STRING_LENGTH2				50
#define STRING_LENGTH3				26
#define DEVICE_QUALIFIER_LENGTH	0x0A
#define OTHER_SPEED_CONFIGURATION_LENGTH	0x20 //0x27
#define SERIAL_LENGTH				STRING_LENGTH3
#define BOS_LENGTH					0x16

#define DEVICE                      0x01
#define CONFIGURATION               0x02
#define STRING                      0x03
#define INTERFACE                   0x04
#define ENDPOINT                    0x05
#define DEVICE_QUALIFIER            0x06
#define OTHER_SPEED_CONFIGURATION   0x07
#define INTERFACE_POWER             0x08
#define BOS_ID                      0x0f
#define LANGUAGE_ID                 0x00

#define MANUFACTURER_STR            0x01
#define PRODUCT_STR                 0x02
#define SERIAL_NUMBER               0x03

#define DEVICE_REMOTE_WAKEUP        1
#define ENDPOINT_STALL              0

#define U1_REQEST                   0x30
#define U2_REQEST                   0x31
#define LTM_REQUEST                 0x32
#define LTM_ENABLED                 0x10
#define U2_ENABLED                  0x08
#define U1_ENABLED                  0x04

#define EP0MAXPACKETSIZE            512
#endif //#ifndef __USB_DEF_H__