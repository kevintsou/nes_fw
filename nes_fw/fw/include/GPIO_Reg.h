#ifndef __GPIO_REG__
#define __GPIO_REG__
//==================================================================================================================

#define GPIOREG ((volatile unsigned char *)0x2F380UL)
//==================================================================================================================

//==================================================================================================================
//  GPIO Control Registers (Base Address : 0xFF00)      GPIOREG
//==================================================================================================================

//----------------------------
//  GPIO Bit Output H/L
//----------------------------
#define GPIO_A0_O               0x00
#define GPIO_A1_O               0x01
#define GPIO_A2_O               0x02
#define GPIO_A3_O               0x03
#define GPIO_A4_O               0x04
#define GPIO_A5_O               0x05
#define GPIO_A6_O               0x06
#define GPIO_A7_O               0x07

#define GPIO_B0_O               0x08
#define GPIO_B1_O               0x09
#define GPIO_B2_O               0x0A
#define GPIO_B3_O               0x0B
#define GPIO_B4_O               0x0C
#define GPIO_B5_O               0x0D
#define GPIO_B6_O               0x0E
#define GPIO_B7_O               0x0F

#define GPIO_C0_O               0x10
#define GPIO_C1_O               0x11
#define GPIO_C2_O               0x12
#define GPIO_C3_O               0x13
#define GPIO_C4_O               0x14
#define GPIO_C5_O               0x15
#define GPIO_C6_O               0x16
#define GPIO_C7_O               0x17

#define GPIO_D0_O               0x18
#define GPIO_D1_O               0x19
#define GPIO_D2_O               0x1A
#define GPIO_D3_O               0x1B
#define GPIO_D4_O               0x1C
#define GPIO_D5_O               0x1D
#define GPIO_D6_O               0x1E
#define GPIO_D7_O               0x1F



//----------------------------
//  GPIO Output Gate Enable
//----------------------------
#define GPIO_A0_OE              0x20
#define GPIO_A1_OE              0x21
#define GPIO_A2_OE              0x22
#define GPIO_A3_OE              0x23
#define GPIO_A4_OE              0x24
#define GPIO_A5_OE              0x25
#define GPIO_A6_OE              0x26
#define GPIO_A7_OE              0x27

#define GPIO_B0_OE              0x28
#define GPIO_B1_OE              0x29
#define GPIO_B2_OE              0x2A
#define GPIO_B3_OE              0x2B
#define GPIO_B4_OE              0x2C
#define GPIO_B5_OE              0x2D
#define GPIO_B6_OE              0x2E
#define GPIO_B7_OE              0x2F

#define GPIO_C0_OE              0x30
#define GPIO_C1_OE              0x31
#define GPIO_C2_OE              0x32
#define GPIO_C3_OE              0x33
#define GPIO_C4_OE              0x34
#define GPIO_C5_OE              0x35
#define GPIO_C6_OE              0x36
#define GPIO_C7_OE              0x37

#define GPIO_D0_OE              0x38
#define GPIO_D1_OE              0x39
#define GPIO_D2_OE              0x3A
#define GPIO_D3_OE              0x3B
#define GPIO_D4_OE              0x3C
#define GPIO_D5_OE              0x3D
#define GPIO_D6_OE              0x3E
#define GPIO_D7_OE              0x3F



//----------------------------
//  GPIO Bit Input H/L
//----------------------------
#define GPIO_A0_I               0x40
#define GPIO_A1_I               0x41
#define GPIO_A2_I               0x42
#define GPIO_A3_I               0x43
#define GPIO_A4_I               0x44
#define GPIO_A5_I               0x45
#define GPIO_A6_I               0x46
#define GPIO_A7_I               0x47

#define GPIO_B0_I               0x48
#define GPIO_B1_I               0x49
#define GPIO_B2_I               0x4A
#define GPIO_B3_I               0x4B
#define GPIO_B4_I               0x4C
#define GPIO_B5_I               0x4D
#define GPIO_B6_I               0x4E
#define GPIO_B7_I               0x4F

#define GPIO_C0_I               0x50
#define GPIO_C1_I               0x51
#define GPIO_C2_I               0x52
#define GPIO_C3_I               0x53
#define GPIO_C4_I               0x54
#define GPIO_C5_I               0x55
#define GPIO_C6_I               0x56
#define GPIO_C7_I               0x57

#define GPIO_D0_I               0x58
#define GPIO_D1_I               0x59
#define GPIO_D2_I               0x5A
#define GPIO_D3_I               0x5B
#define GPIO_D4_I               0x5C
#define GPIO_D5_I               0x5D
#define GPIO_D6_I               0x5E
#define GPIO_D7_I               0x5F



//----------------------------
//  GPIO Byte Output H/L
//----------------------------
#define GPIO_A_O                0x60    // GPIO_A[7:0]_O
#define GPIO_B_O                0x61    // GPIO_B[7:0]_O
#define GPIO_C_O                0x62    // GPIO_C[7:0]_O
#define GPIO_D_O                0x63    // GPIO_D[7:0]_O

#define GPIO_A_OE               0x64    // GPIO_A[7:0]_OE
#define GPIO_B_OE               0x65    // GPIO_B[7:0]_OE
#define GPIO_C_OE               0x66    // GPIO_C[7:0]_OE
#define GPIO_D_OE               0x67    // GPIO_D[7:0]_OE

#define GPIO_A_I                0x68    // GPIO_A[7:0]_I
#define GPIO_B_I                0x69    // GPIO_B[7:0]_I
#define GPIO_C_I                0x6A    // GPIO_C[7:0]_I                    
#define GPIO_D_I                0x6B    // GPIO_D[7:0]_I



//----------------------------
//  GPIO Byte Access : GPIO Interrupt -
//  Rising / FALLing INT
//----------------------------
//-------------------------------------------------------------------------------------------------------------------------
#define PORT_INT                0x6C
#define P0_FEN              BIT7
#define P1_FEN              BIT6
#define P2_FEN              BIT5
#define P3_FEN              BIT4
#define P0_REN              BIT3
#define P1_REN              BIT2
#define P2_REN              BIT1
#define P3_REN              BIT0
//----------------------------------------------------------
//  BitNum      Name            Attribute   Reset   Description
//  Bit[7:4]    GPIO_FIEN[3:0]  R/W         0       Bit 7~4: Falling edge interrupt enable for {GPIO_D[0], GPIO_C[0], GPIO_B[0],GPIO_A[0]}.
//  Bit[3:0]    GPIO_RIEN[3:0]  R/W         0       Bit 3~0: Rising edge interrupt enable for {GPIO_D[0], GPIO_C[0], GPIO_B[0],GPIO_A[0]}.
//-------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------
#define PORT_INT_STS            0x6D
#define P0_STS              BIT3
#define P1_STS              BIT2
#define P2_STS              BIT1
#define P3_STS              BIT0
//----------------------------------------------------------
//  BitNum      Name            Attribute   Reset   Description
//  Bit[3:0]    GPIO_IF[3:0]    R/W         0       Interrupt flags for {GPIO_D[0], GPIO_C[0], GPIO_B[0],GPIO_A[0]}.
//-------------------------------------------------------------------------------------------------------------------------




//----------------------------
//  GPIO Pull High / Pull Down
//----------------------------
//#define GPIO_A_PU             0x70
//#define GPIO_B_PU             0x71
//#define GPIO_C_PU             0x72
//#define GPIO_D_PU             0x73


#define GPIO_A_PU               0x70    // GPIO_A[7:0] Pull-Up  Enable.
#define GPIO_A_PD               0x71    // GPIO_A[7:0] Pull-Down Enable.
#define GPIO_B_PU               0x72    // GPIO_B[7:0] Pull-Up  Enable.
#define GPIO_B_PD               0x73    // GPIO_B[7:0] Pull-Down Enable.
#define GPIO_C_PU               0x74    // GPIO_C[7:0] Pull-Up  Enable.
#define GPIO_C_PD               0x75    // GPIO_C[7:0] Pull-Down Enable.    
#define GPIO_D_PU               0x76    // GPIO_D[7:0] Pull-Up  Enable.
#define GPIO_D_PD               0x77    // GPIO_D[7:0] Pull-Down Enable.


//----------------------------
//  GPIO Driving Strength -
//----------------------------
//#define GPIO_A_DRV                0x74
//#define GPIO_B_DRV                0x75
//#define GPIO_C_DRV                0x76
//#define GPIO_D_DRV                0x77


#define GPIO_A_DRV              0x78    // GPIO_A I/O Driving Setting. Only 2 bit ([1:0]) is valid.  
#define GPIO_B_DRV              0x79    // GPIO_B I/O Driving Setting. Only 2 bit ([1:0]) is valid.  
#define GPIO_C_DRV              0x7A    // GPIO_C I/O Driving Setting. Only 2 bit ([1:0]) is valid.  
#define GPIO_D_DRV              0x7B    // GPIO_D I/O Driving Setting. Only 2 bit ([1:0]) is valid.  
//----------------------------------------------------------
//  BitNum      Name        Attribute   Reset   Description
//  Bit[2:0]                R/W         0       GPIO Driving Setting  Bit 0 : 4mA , Bit 1 : 8mA , Ex: [000]=>4mA , [001]=>4mA + 4mA = 8mA
//-------------------------------------------------------------------------------------------------------------------------

#endif  //#ifndef __GPIO_REG__
