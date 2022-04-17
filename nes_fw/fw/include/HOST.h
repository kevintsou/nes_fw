



#define HSTREG     ((volatile unsigned char *)0x2E000UL) //Host IP base address
#define HOST_PBA_IO_REG ((volatile unsigned char *)0x2F700UL)

#define FORCE_CALI_MODE   0x01




#define HOST_CALI_CTL       0x7A
#define HOST_CALI_FORCE_MODE       SET_BIT6

//----------------------- register offset ------------------- 
#define  HOST_PLL_DIV0             (0x0<<2)              
#define  HOST_PLL_DIV1             (0x1<<2)              
#define  HOST_PLL_CTRL             (0x2<<2)              
#define  HOST_PLL_CFG0_0           (0x3<<2)              
#define  HOST_PLL_CFG0_1           (0x4<<2)              
#define  HOST_PLL_CFG1_0           (0x5<<2)              
#define  HOST_PLL_CFG1_1           (0x6<<2)              
#define  HOST_PLL_STAT             (0x7<<2)

#define  HOST_CALI_CTRL            (0x8<<2)  
 #define CALI_EN                    SET_BIT7
 #define RSTJ_CALI                  SET_BIT6
 
#define  FLH_PHI_CTRL1             (0x9<<2)              
#define  FLH_PHI_CTRL2             (0xA<<2)              
#define  RCT_CTRL                  (0xB<<2)              
#define  FLH_PHI_CTRL3             (0xC<<2)              
#define  CRC_CTRL1                 (0xD<<2)              
#define  CRC_CTRL2                 (0xE<<2)              
#define  CRC_CTRL3                 (0xF<<2)              
#define  CRC_CTRL4                 (0x10<<2)              
#define  CRC_CTRL5                 (0x11<<2)              
#define  HOST_RDAT                 (0x12<<2)              
#define  HOST_DBG                  (0x13<<2)   





#define  DLL_DLL_CTL               (0x0<<2 | 0x400)
#define  DLL_MDLL_STA              (0x1<<2 | 0x400)
#define  DLL_LOAD_CTL              (0x2<<2 | 0x400)
#define  DLL_SEL_VAL_0             (0x8<<2 | 0x400)
#define  DLL_SEL_VAL_L             (DLL_SEL_VAL_0+2)
#define  DLL_SEL_VAL_H             (DLL_SEL_VAL_0+3)

#define  HOST_CALI_LOAD                0x23
  #define CALI_LOAD               SET_BIT0
  

#define PIO_POL_MASK              0x2A
#define PIO_POL_EXP               0x2B

#define HOST_DMA_LEN_L            0x30
#define HOST_DMA_LEN_H            0x31
 


#define  CRC_CTRL_INT              0x34
   #define   CRC_CTRL_32_EN        SET_BIT1
   #define   CRC_CTRL_64_EN        SET_BIT2
   
#define  PMODE_18V             CLR_BIT0
#define  PMODE_12V             SET_BIT0
#define  FOR_Voltage_12V            0x26

#define  PBA_RDY                    0x0C
#define  PBA_RDY_CAL_N              0x0E
#define  PBA_RDY_CAL_P              0x0F


#define  PBA_DQS                    0x10
#define  PBA_DQS_SR_Dty                 0x11
#define  PBA_DQS_CAL_N              0x12
#define  PBA_DQS_CAL_P              0x13

#define  PBA_HIO                    0x14
#define  PBA_HIO_SR_Dty             0x15
#define  PBA_HIO_CAL_N              0x16
#define  PBA_HIO_CAL_P              0x17


#define  PBA_CLE                    0x18
#define  PBA_CLE_CAL_N              0x1A
#define  PBA_CLE_CAL_P              0x1B


#define  PBA_ALE                    0x1C
#define  PBA_ALE_CAL_N              0x1E
#define  PBA_ALE_CAL_P              0x1F


#define  PBA_REB                    0x20
#define  PBA_REB_CAL_N              0x22
#define  PBA_REB_CAL_P              0x23


#define  PBA_WEB                    0x24
#define  PBA_WEB_CAL_N              0x26
#define  PBA_WEB_CAL_P              0x27


#define  PBA_CE                     0x28
#define  PBA_CE_CAL_N               0x2A
#define  PBA_CE_CAL_P               0x2B


#define  HOST_LOCK                  0x60
#define  HOST_LOCK_CAL_N            0x62
#define  HOST_LOCK_CAL_P            0x63


#define  HOST_WP                    0x64
#define  HOST_WP_CAL_N              0x66
#define  HOST_WP_CAL_P              0x67

#define  HOST_RDY                    0x68
#define  HOST_RDY_CAL_N              0x6A
#define  HOST_RDY_CAL_P              0x6B

#define  HOST_DQS                    0x6C
#define  HOST_DQS_CAL_N              0x6E
#define  HOST_DQS_CAL_P              0x6F

#define  HOST_HIO                    0x70
#define  HOST_HIO_CAL_N              0x72
#define  HOST_HIO_CAL_P              0x73

#define  HOST_CLE                    0x74
#define  HOST_CLE_CAL_N              0x76
#define  HOST_CLE_CAL_P              0x77

#define  HOST_ALE                    0x78
#define  HOST_ALE_CAL_N              0x7A
#define  HOST_ALE_CAL_P              0x7B

#define  HOST_REB                    0x7C
#define  HOST_REB_CAL_N              0x7E
#define  HOST_REB_CAL_P              0x7F

#define  HOST_WEB                    0x80
#define  HOST_WEB_CAL_N              0x82
#define  HOST_WEB_CAL_P              0x83

#define  HOST_CE                    0x84
#define  HOST_CE_CAL_N              0x86
#define  HOST_CE_CAL_P              0x87

//----------------------- field define -------------------- 
