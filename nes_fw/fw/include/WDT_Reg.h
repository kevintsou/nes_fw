#ifndef __WDT_REG_H__
#define __WDT_REG_H__

//==================================================================================================================
// WDT (Watch Dog Timer) Control Registers (Base address : 0xFD00)
//==================================================================================================================
#define WDTREG ((volatile unsigned char *)0x2F340UL)
//==================================================================================================================


//-------------------------------------------------------------------------------------------------------------------------
#define WDT_TO_CYC_H            0x00    // WDT_TO_CYC[15:8]
#define WDT_TO_CYC_L            0x01    // WDT_TO_CYC[7:0]  
//----------------------------------------------------------
//  BitNum      Name                Attribute   Reset   Description
//              WDT_TO_CYC[7:0]     R/W         FF
//              WDT_TO_CYC[15:8]    R/W         FF      WDT_TO_CYC[15:0] : Watch Do Timer Time-out Cycles. 設定WDT的time out時間,以mini seconds為單位.
//-------------------------------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------------------------------
#define WDT_CON                 0x02
#define RTC0_EN             BIT0
#define RTC1_EN             BIT1
#define RTC2_EN             BIT2
#define RTC3_EN             BIT3
#define RTC4_EN             BIT4
#define RTC5_EN             BIT5
#define RTC6_EN             BIT6
#define WDT_EN              BIT7
//----------------------------------------------------------
//  BitNum      Name        Attribute   Reset   Description
//  Bit 6~0     RTC_EN      R/W         0       RTC Enable. 這個bit要打開RTC_CNT 才會開始數.    1-Enable / 0-Disable
//  Bit 7       WDT_EN      R/W         0       WDT Enable. 這個bit要打開WDT_CNT 才會開始數.    1-Enable / 0-Disable
//-------------------------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------------------------
#define WDT_CLR                 0x03
//----------------------------------------------------------
//  BitNum      Name                Attribute   Reset                       Description
//  Bit[7:0]    WDT_CLR / WDT_TAG   R/W         0(power on only & SW_RST)   Write to this address will clear WDT_CNT (reset WDT)
//                                                                          And store a tag into this register (WDT_TAG)
//                                                                          WDT_TAG can't be cleared by WDT timeout reset
//-------------------------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------------------------
#define WDT_CNT_H               0x04    // WDT_CNT[15:8]
#define WDT_CNT_L               0x05    // WDT_CNT[7:0]          
//----------------------------------------------------------
//  BitNum      Name            Attribute   Reset   Description
//              WDT_CNT[7:0]    R           0
//              WDT_CNT[15:8]   R           0       WDT Counter. if WDT_CNT == WDT_TO_CYC then
//                                                  ==> Reset 8051
//                                                  ==> Reset WDT_EN (Disable WDT)
//-------------------------------------------------------------------------------------------------------------------------




//===================================
// Real Time Clock -
//===================================

//-------------------------------------------------------------------------------------------------------------------------
#define REAL_TIMER0_H               0x08
#define REAL_TIMER0_L               0x09
#define REAL_TIMER1_H               0x0A
#define REAL_TIMER1_L               0x0B
#define REAL_TIMER2_HH              0x0C
#define REAL_TIMER2_HL              0x0D
#define REAL_TIMER2_LH              0x0E
#define REAL_TIMER2_LL              0x0F
#define REAL_TIMER3_HH              0x10
#define REAL_TIMER3_HL              0x11
#define REAL_TIMER3_LH              0x12
#define REAL_TIMER3_LL              0x13
#define REAL_TIMER4_H               0x14
#define REAL_TIMER4_L               0x15
#define REAL_TIMER5_H               0x16
#define REAL_TIMER5_L               0x17
#define REAL_TIMER6_H               0x18
#define REAL_TIMER6_L               0x19
//----------------------------------------------------------
//  BitNum      Name        Attribute           Reset   Description
//  Bit[7:0]    RTC[7:0]    RO & Write_Clear    0       RTC[7~0] : Real Time Clock.
//                                                      This counter is increased by 1 for every 10 ms when RTC_EN == 1.
//                                                      Read  operation will Read out RTC value,
//                                                      Write operation will Clear RTC to 0.
//-------------------------------------------------------------------------------------------------------------------------



#endif  //#ifndef __WDT_REG_H__
