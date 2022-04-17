#ifndef __USB_REG_H__
#define __USB_REG_H__
//==================================================================================================================

//EPn offset
#define EP1_OFFSET                  0x00
#define EP2_OFFSET                  0x40
#define EP3_OFFSET                  0x80
#define EP4_OFFSET                  0xc0
#define EP5_OFFSET                  0x100
#define EP6_OFFSET                  0x140
#define EP7_OFFSET                  0x180
#define EP8_OFFSET                  0x1C0

//Register define
#define USBREG ((volatile unsigned char *)0x2F800UL)
//==================================================================================================================

#define PAGE_NO                     0x00

#define INTF_MODE                   0x04

#define EP9_MODE                    0x05
#define EP9_EN                          BIT3

#define TLU_CFG                     0x06

#define USB_FTU                     0x07
#define CPU_TYPE                        BIT6
#define SYSTEM_BUS                  BIT5
#define PRD_FUNC                        BIT4
#define INTERFACE_2                 BIT2
#define INTERFACE_1                 BIT1
#define AES_OTF_FUNC                BIT0

#define CONN_SPD                    0x08
#define CR_CONNECT                  BIT7
#define SUPER_SPD                   BIT2
#define FW_RESET                   (BIT0 + BIT1)

#define ENUM_SPD                    0x09
#define SPD_MASK                        (BIT0 + BIT1 + BIT2)
#define LOW_SPD                         BIT1
#define FULL_SPD                        BIT0
#define HIGH_SPD                        0x00
#define SUPER_SPD                       BIT2
#define SS_POLL_FAI                 BIT4
#define FW_RESET                        (BIT0 + BIT1)

#define HOST_STS                    0x0A
#define SR_VBUS                         BIT7

#define TLU_ENCLR                   0x10
#define TLU_FA_CLR                  BIT7
#define TLU_EPC_EN                  BIT6
#define TLU_EPN_EN                  BIT5

#define ABF_CLR                     0x11
#define ABF_PRT_CLR                 BIT7

#define TLU_RST_EN_0            0x12
#define TLU_EP8_RST_EN          BIT7
#define TLU_EP7_RST_EN          BIT6
#define TLU_EP6_RST_EN          BIT5
#define TLU_EP5_RST_EN          BIT4
#define TLU_EP4_RST_EN          BIT3
#define TLU_EP3_RST_EN          BIT2
#define TLU_EP2_RST_EN          BIT1
#define TLU_EP1_RST_EN          BIT0

#define TLU_RST_EN_1            0x13
#define TLU_EP9_RST_EN          BIT0

#define PHY_PLL_ST              0x14
#define FW_RCOSC_SW_REQ         BIT7
#define HW_AUTO_SW_RCOSC        BIT6
#define PIPE_PLL_LOCK               BIT1
#define UTMI_PLL_LOCK               BIT0

#define GLB_TLU_ST              0x15
#define GLB_TLU_BZ                  BIT1
#define CLR_SRL_MD_ADDR         BIT0

#define INT1_EVT0                   0x20
#define EP8_DATA_AV                 BIT4
#define EP6_DATA_AV                 BIT3
#define EP4_DATA_AV                 BIT2
#define EP2_DATA_AV                 BIT1

#define INT1_EVT1                   0x21
#define PRD1_NOR_EVT                BIT7
#define PRD1_ERR_EVT                BIT6
#define PRD1_LXTDN_EVT          BIT5
#define PRD1_LDLXT_EVT          BIT4
#define PRD0_NOR_EVT                BIT3
#define PRD0_ERR_EVT                BIT2
#define PRD0_LXTDN_EVT          BIT1
#define PRD0_LDLXT_EVT          BIT0

#define INT1_EVT2                   0x22
#define EP0_TLU_CMP                 BIT7
#define PRD1_ABT_EVT                BIT1
#define PRD0_ABT_EVT                BIT0

#define INT1_EVT3                   0x23
#define EP8_TLU_CMP                 BIT7
#define EP7_TLU_CMP                 BIT6
#define EP6_TLU_CMP                 BIT5
#define EP5_TLU_CMP                 BIT4
#define EP4_TLU_CMP                 BIT3
#define EP3_TLU_CMP                 BIT2
#define EP2_TLU_CMP                 BIT1
#define EP1_TLU_CMP                 BIT0

#define INT0EXT_EVT0            0x24

#define INT0EXT_EVT1            0x25

#define INT0EXT_EVT2            0x26

#define INT0EXT_EVT3            0x27
#define ENUM_DONE                       BIT7
#define RCOSC_SW_REQ                BIT6
#define WAKEUP                          BIT5
#define SUSPEND                         BIT4
#define SLEEP                               BIT3


#define INT1_EN0                    0x28
#define UNSVC_EP8_INT               BIT4
#define UNSVC_EP6_INT               BIT3
#define UNSVC_EP4_INT               BIT2
#define UNSVC_EP2_INT               BIT1

#define INT1_EN1                    0x29
#define SPEC_PRD1_COM_EN        BIT7
#define PRD1_ERR_EN                 BIT6
#define LST_PRD1_COM_EN         BIT5
#define LST_PRD1_LOD_EN         BIT4
#define SPEC_PRD0_COM_EN        BIT3
#define PRD0_ERR_EN                 BIT2
#define LST_PRD0_COM_EN         BIT1
#define LST_PRD0_LOD_EN         BIT0

#define INT1_EN2                    0x2A
#define EP0_TLU_CMP_EN          BIT7
#define PRD1_ABT_EN                 BIT6
#define PRD1_ABT_EN                 BIT6

#define INT1_EN3                    0x2B
#define EP8_TLU_CMP_EN          BIT7
#define EP7_TLU_CMP_EN          BIT6
#define EP6_TLU_CMP_EN          BIT5
#define EP5_TLU_CMP_EN          BIT4
#define EP4_TLU_CMP_EN          BIT3
#define EP3_TLU_CMP_EN          BIT2
#define EP2_TLU_CMP_EN          BIT1
#define EP1_TLU_CMP_EN          BIT0

#define INT0EXT_EN0             0x2C

#define INT0EXT_EN1             0x2D

#define INT0EXT_EN2             0x2E

#define INT0EXT_EN3             0x2F
#define ENUM_DONE_EN                BIT7

#define INT1_DET_EN0            0x30
#define CR_EP8_ABFVLD_EN        BIT4
#define CR_EP6_ABFVLD_EN        BIT3
#define CR_EP4_ABFVLD_EN        BIT2
#define CR_EP2_ABFVLD_EN        BIT1

#define INT1_DET_EN1            0x31

#define INT1_DET_EN2            0x32

#define INT1_DET_EN3            0x33

#define INT0EXT_DET_EN0     0x34

#define INT0EXT_DET_EN1     0x35

#define INT0EXT_DET_EN2     0x36

#define INT0EXT_DET_EN3     0x37
#define WAKEUP_DET_EN               BIT5
#define PHY_SLP_DET_EN          BIT3

#define EP1_PTR                     0x38

#define EP2_PTR                     0x39

#define EP2_WVPTR                   0x3A

#define EP5_PTR                     0x3C

#define EP6_PTR                     0x3D

#define EP7_PTR                     0x3E

#define EP8_PTR                     0x3F

#define EP_HALT0                    0x46

#define EP_HALT1                    0x47

#define EPC_RESPONSE            0x48
#define DATA_NRDY                       0x00
#define DATA_NORM                       BIT2
#define DATA_STALL                  BIT3
#define STAT_NRDY                       0x00
#define STAT_NORM                       BIT0
#define STAT_STALL                  BIT1
#define EPC_S_DPP_VLD 		BIT7

#define DEV_ADDR                    0x49

#define DEV_CFG                     0x4A
#define SS_DEV_CFG          BIT0

#define DEV_STATUS              0x4B
#define SR_SS_DEV_LTMEN         BIT6
#define SR_SS_DEV_U2EN          BIT5
#define SR_SS_DEV_U1EN          BIT4
#define SR_U2_DEV_L2_RW         BIT1
#define SR_U2_DEV_L1_RW         BIT0

#define INTF_ALT                    0x4C
#define INTF_ALT2_VAL               BIT2
#define INTF_ALT1_VAL               BIT1
#define INTF_ALT0_VAL               BIT0

#define INTF_FRW                    0x4D
#define SR_SS_INTF_FRW2         BIT2
#define SR_SS_INTF_FRW1         BIT1
#define SR_SS_INTF_FRW0         BIT0

#define INTF_FSUSPEND           0x4E

#define EP_HALT                     0x4F

#define ISOCH_DLY_L             0x50

#define ISOCH_DLY_H             0x51

#define DEV_U1SEL                   0x52

#define DEV_U1PEL                   0x53

#define DEV_U2SEL_L             0x54

#define DEV_U2SEL_H             0x55

#define DEV_U2PEL_L             0x56

#define DEV_U2PEL_H             0x57

#define VNDR_PTN0                   0x58

#define VNDR_PTN1                   0x59

#define VNDR_PTN2                   0x5A

#define VNDR_PTN3                   0x5B

#define VNDR_PTN4                   0x5C

#define VNDR_PTN5                   0x5D

#define VNDR_PTN6                   0x5E

#define VNDR_PTN7                   0x5F

#define VNDR_TEST                   0x60

#define CAP_NUM_HP              0x61

#define CAP_CFG                     0x62
#define CAP_SPD                         BIT7
#define CAP_DIR                         BIT1
#define CAP_TB                          BIT0

#define FORCE_ACK                   0x63
#define FRCE_LPM_ACK                BIT0

#define U2_INA_TMOUT            0x64

#define CFG_STATUS              0x65
#define CFG_SPD                         BIT7
#define LMP_DONE                        BIT0

#define TX_SPP_ADDR             0x67

#define ENTER_UX                    0x68
#define LXU_U2                          BIT7    //send LXU_U2
#define LXU_U1                          BIT6    //send LXU_U1
#define LGO_N_U2                        BIT5    //never send LGO_U2
#define LGO_N_U1                        BIT4    //never send LGO_U1
#define LGO_U2                          BIT1    //send LGO_U2
#define LGO_U1                          BIT0    //send LGO_U1

#define P2PM_EP_BUSY            0x69

#define LT_ACTIVE_L             0x6A

#define LT_ACTIVE_H             0x6B

#define LT_IDLE_L                   0x6C

#define LT_IDLE_H                   0x6D

#define DN_SEND                     0x6F
#define SS_DN_SEND     BIT0

#define DN_CFG_L                    0x70

#define DN_CFG_H                    0x71
#define HP_TP               0x00
#define HP_DP                               BIT7

#define DN_HP_SID_L             0x72

#define DN_HP_SID_H             0x73

#define DN_HP_DW1_LL            0x74

#define DN_HP_DW1_LH            0x75

#define DN_HP_DW1_HL            0x76

#define DN_HP_DW1_HH            0x77

#define LTSSM0                      0x78
#define U2_STATE                        BIT7
#define U1_STATE                        BIT6
#define RECOVERY_STATE          BIT5
#define U0_STATE                        BIT4
#define POLLING_STATE               BIT3
#define RX_DET_STATE                BIT2
#define INACTIVE_STATE          BIT1
#define DISABLE_STATE               BIT0

#define LTSSM1                      0x79
#define L3_STATE                        BIT7
#define L2_STATE                        BIT6
#define L1_STATE                        BIT5
#define L0_STATE                        BIT4
#define COMPLIANCE_STATE        BIT3
#define LOOPBACK_STATE          BIT2
#define HOTRESET_STATE          BIT1
#define U3_STATE                        BIT0

#define RST_ST                      0x7A
#define WIRE_WARM_BUS_RST       BIT1
#define WIRE_HOT_RST                BIT0

#define U2_LPM                      0x7B

#define RESUME                      0x7C
#define L2_U3_SUSPEND               BIT7
#define L1_L2_U3_RW                 BIT0

#define INT0_EVT0                   0x80
#define FEATURE_INTRFC0         BIT7
#define FEATURE_U1                  BIT6
#define FEATURE_U2                  BIT5
#define FEATURE_LTM                 BIT4
#define SET_INTERFACE2          BIT3
#define SET_INTERFACE1          BIT2
#define SET_INTERFACE0          BIT1
#define EP0_SETUP                       BIT0

#define INT0_EVT1                   0x81
#define U2_TIMEOUT_LMP          BIT7
#define VENDOR_TEST_LMP         BIT6
#define PORT_CAP_LMP                BIT5
#define PORT_CONF_LMP               BIT4
#define SET_SEL                         BIT3
#define ISOCHR_DELAY                BIT2
#define FEATURE_INTRFC2         BIT1
#define FEATURE_INTRFC1         BIT0

#define INT0_EVT2                   0x82
#define WARM_BUS_RST                BIT7
#define HOT_RST                         BIT6
#define ENTER_SS_INACT          BIT5
#define ENTER_L0U0                  BIT4
#define ENTER_U1                        BIT3
#define ENTER_L1U2                  BIT2
#define ENTER_L2U3                  BIT1
#define SET_LINK_LMP                BIT0

#define INT0_EVT3                   0x83
#define FEATURE_EP8                 BIT7
#define FEATURE_EP7                 BIT6
#define FEATURE_EP6                 BIT5
#define FEATURE_EP5                 BIT4
#define FEATURE_EP4                 BIT3
#define FEATURE_EP3                 BIT2
#define FEATURE_EP2                 BIT1
#define FEATURE_EP1                 BIT0

#define INT0_EVT6                   0x86
#define HOST_RESUME                 BIT7

#define INT0_EVT7                   0x87
#define ERR_8B10B                       BIT7
#define ERR_LOSS_SYM                BIT6
#define ERR_SS_LINK                 BIT5
#define ERR_HSFS_LINK               BIT4
// bit assignment for gucUsbEventL
#define EP0_SETUP                       BIT0
#define EP2_BKORDY                  BIT1

#define INT0_EN0                    0x88
#define FEATURE_INTRFC0_EN  BIT7
#define FEATURE_U1_EN               BIT6
#define FEATURE_U2_EN               BIT5
#define FEATURE_LTM_EN          BIT4
#define SET_INTERFACE2_EN       BIT3
#define SET_INTERFACE1_EN       BIT2
#define SET_INTERFACE0_EN       BIT1
#define EP0_SETUP_EN                BIT0

#define INT0_EN1                    0x89
#define U2_TIMEOUT_LMP_EN       BIT7
#define VENDOR_TEST_LMP_EN  BIT6
#define PORT_CAP_LMP_EN         BIT5
#define PORT_CONF_LMP_EN        BIT4
#define SET_SEL_EN                  BIT3
#define ISOCHR_DELAY_EN         BIT2
#define FEATURE_INTRFC2_EN  BIT1
#define FEATURE_INTRFC1_EN  BIT0

#define INT0_EN2                    0x8A
#define WARM_BUS_RST_EN         BIT7
#define HOT_RST_EN                  BIT6
#define ENTER_SS_INACT_EN       BIT5
#define ENTER_L0U0_EN               BIT4
#define ENTER_U1_EN                 BIT3
#define ENTER_L1U2_EN               BIT2
#define ENTER_L2U3_EN               BIT1
#define SET_LINK_LMP_EN         BIT0

#define INT0_EN3                    0x8B
#define FEATURE_EP8_EN          BIT7
#define FEATURE_EP7_EN          BIT6
#define FEATURE_EP6_EN          BIT5
#define FEATURE_EP5_EN          BIT4
#define FEATURE_EP4_EN          BIT3
#define FEATURE_EP3_EN          BIT2
#define FEATURE_EP2_EN          BIT1
#define FEATURE_EP1_EN          BIT0

#define INT0_EN4                    0x8C
#define ERR_8B10B_EN                BIT7
#define ERR_LOSS_SYM_EN         BIT6
#define ERR_SS_LINK_EN          BIT5
#define ERR_HSFS_LINK_EN        BIT4

#define INT0_EN5                    0x8D

#define INT0_EN6                    0x8E

#define INT0_EN7                    0x8F

#define INT0_DET_EN0            0x90

#define INT0_DET_EN1            0x91

#define INT0_DET_EN2            0x92
#define L0_U0_DET_EN                BIT4

#define INT0_DET_EN3            0x93

#define INT0_DET_EN4            0x94

#define INT0_DET_EN5            0x95

#define INT0_DET_EN6            0x96
#define RESUME_DET_EN               BIT7

#define INT0_DET_EN7            0x97

#define CLR_ERRCNT              0xA0

#define FOX_PTR_RST0            0xA2

#define FOX_PTR_RST1            0xA3

#define SETUP_DATA0             0xB8

#define SETUP_DATA1             0xB9

#define SETUP_DATA2             0xBA

#define SETUP_DATA3             0xBB

#define SETUP_DATA4             0xBC

#define SETUP_DATA5             0xBD

#define SETUP_DATA6             0xBE

#define SETUP_DATA7             0xBF

#define CSW_EP1                     0xC0    //32 bytes

#define CBW_EP2                     0xE0    //32 bytes

#define EP3_SIU                     0x100   //32 bytes

#define CIU_EP4                     0x120   //32 bytes

#define EP5_BKI_DATA            0x140   //32 bytes

#define EP6_BKO_DATA            0x160   //32 bytes

#define EP7_BKI_DATA            0x180   //32 bytes

#define EP8_BKO_DATA            0x1A0   //32 bytes

#define EP0_OTF_LBA0            0x1C0

#define EP0_OTF_LBA1            0x1C1

#define EP0_OTF_LBA2            0x1C2

#define EP0_OTF_LBA3            0x1C3

#define EP0_OTF_CFG             0x1C4
#define OTF_EN                          BIT7
#define OTF_CRYPT                       BIT6
#define OTF_INDEX                       BIT1

#define EP0_BUF_TOP0            0x1C5

#define EP0_BUF_TOP1            0x1C6

#define EP0_BUF_TOP2            0x1C7

#define EP0_BUF_LEN0            0x1C8

#define EP0_BUF_LEN1            0x1C9

#define EP0_BUF_START0      0x1CA

#define EP0_BUF_START1      0x1CB

#define EP0_TRIG_BCNT0      0x1CC

#define EP0_TRIG_BCNT1      0x1CD

#define EP0_TRIG_BCNT2      0x1CE

#define EP0_TRIG_CMPL           0x1D2
#define CR_EP0_STOP_EN          BIT7

#define EP0_TRIG_CFG            0x1D3
#define EP0_TRIG_EN                 BIT7
#define INC_PTR                         BIT6
#define NEXT_BUF                        BIT5
#define STALL_ONCE                  BIT1
#define EP0_STOP                        BIT0

#define EP0_T_SC0                   0x1D4

#define EP0_T_SC1                   0x1D5

#define EP0_T_SC2                   0x1D6

#define EP0_TLU_STATUS      0x1D7
#define EP0_FULL                        BIT1
#define EP0_EMPTY                       BIT0

#define EP0_RX_SIZE0            0x1DA

#define EP0_RX_SIZE1            0x1DB

#define EP0_SERIAL_D            0x1DC

#define EP0_ST_CMP              0x1E0
#define EP0_ST_RST                  BIT7
#define EP0_ST_RCV_STPPKT       BIT6
#define EP0_ST_STOP                 BIT3
#define EP0_ST_MORE                 BIT2
#define EP0_ST_LESS                 BIT1
#define EP0_ST_FINISH               BIT0

#define EP0_ST_BC0              0x1EC

#define EP0_ST_BC1              0x1ED

#define EP0_ST_BC2              0x1EE

#define EP0_ST_BC3              0x1EF

#define EP0_ST_SC0              0x1F4

#define EP0_ST_SC1              0x1F5

#define EP0_ST_SC2              0x1F6

//page number = 0x00
#define EPn_OTF_LBA0            0x200

#define EPn_OTF_LBA1            0x201

#define EPn_OTF_LBA2            0x202

#define EPn_OTF_LBA3            0x203

#define EPn_OTF_CFG             0x204
#define EPn_OTF_EN                  BIT7
#define EPn_OTF_CRYPT               BIT6
#define EPn_OTF_INDEX               BIT0

#define EPn_BUF_TOP0            0x205

#define EPn_BUF_TOP1            0x206

#define EPn_BUF_TOP2            0x207

#define EPn_BUF_LEN0            0x208

#define EPn_BUF_LEN1            0x209

#define EPn_BUF_START0      0x20A

#define EPn_BUF_START1      0x20B

#define EPn_T_BC0                   0x20C

#define EPn_T_BC1                   0x20D

#define EPn_T_BC2                   0x20E

#define EPn_T_BC3                   0x20F

#define EPn_SID_L                   0x210

#define EPn_SID_H                   0x211

#define EPn_TRIG_CFG0           0x212
#define CR_EPN_KPLXABT          BIT7

#define EPn_TRIG_CFG1           0x213
#define EPn_TRIG_EN                 BIT7
#define INC_PTR                         BIT6
#define NEXT_BUF                        BIT5
#define OTF_MD                          BIT3
#define STALL_ALWAYS                BIT2
#define STALL_ONCE                  BIT1
#define EPn_STOP                        BIT0

#define EPn_T_SC0                   0x214

#define EPn_T_SC1                   0x215

#define EPn_T_SC2                   0x216

#define EPn_TLU_STATUS      0x217
#define RAM_VALID                       BIT7
#define EPn_FULL                        BIT1
#define EPn_EMPTY                       BIT0

#define EPn_T_SEC_CNT_L_0x218  ((unsigned short)0x218) /* SC = SEC_CNT, T = TRIGGER */
#define EPn_SECTOR_CNT_L(x)  (x) /* [0:7] */

#define EPn_T_SEC_CNT_H_0x219  ((unsigned short)0x219)
#define EPn_SECTOR_CNT_H(x)  (x) /* [0:6] */

#define EPn_DATA_LEN_L      0x21A

#define EPn_DATA_LEN_H      0x21B

#define EPn_SERIAL_D            0x21C

#define EPn_TRIG_CMPL           0x220
#define EPN_ST_RST                  BIT7
#define EPN_ST_CLR_FUTURE       BIT6
#define EPN_ST_LST_PTR          BIT5
#define EPN_ST_STOP                 BIT3
#define EPN_ST_MORE                 BIT2
#define EPN_ST_LESS                 BIT1
#define EPN_ST_FINISH               BIT0

#define EPn_ST_HBC0             0x224

#define EPn_ST_HBC1             0x225

#define EPn_ST_HBC2             0x226

#define EPn_ST_HBC3             0x227

#define EPn_ST_BC0              0x22C

#define EPn_ST_BC1              0x22D

#define EPn_ST_BC2              0x22E

#define EPn_ST_BC3              0x22F

#define EPn_ST_SC0              0x234

#define EPn_ST_SC1              0x235

#define EPn_ST_SC2              0x236

#define DAT_EP9                     0x500   //32 bytes

#define CDB_INDEX                   0x580   //32 bytes

#define USB_CUR_CBW             0x600   //32 bytes

#define USB_LAST_CBW            0x620   //32 bytes

#define USB_LBA0                    0x640

#define USB_LBA1                    0x641

#define USB_LBA2                    0x642

#define USB_LBA3                    0x643

#define USB_SC0                     0x644

#define USB_SC1                     0x645

#define USB_BKCMD_CAP           0x648

#define USB_BKCMD_CTLR      0x649

#define USB_BKCMD_CLR           0x64A

#define USB_LUN_SHF0            0x64C
#define SIZE_512                        0x00
#define SIZE_2K                         BIT0
#define SIZE_4K                         BIT1
#define SIZE_8K                         (BIT0 + BIT1)

#define USB_LUN_SHF1            0x64D

#define USB_BOT_CASE            0x650

#define TOTAL_SEC_CTL           0x652

#define OT_CONTI_STS            0x653

#define USB_BK_PSTS             0x654
#define ILLEGAL_LUN                 BIT6
#define ILLEGAL_CAPA                BIT5
#define PERFECT_CBW                 BIT4
#define VALID_CBW                       BIT1

#define USB_EPNUM_PSR           0x658

#define CHG_SEQ_ORDER           0x659

#define CMD_SUP_RW              0x65A

#define SCSI_OP_BYTE            0x65B

#define USB_CSW_RES0            0x660

#define USB_CSW_RES1            0x661

#define USB_CSW_RES2            0x662

#define USB_CSW_RES3            0x663

#define USB_CSW_STS             0x664

#define USB_SUPT_LUN            0x665

#define USB_LUNX_CAP0           0x668

#define USB_LUNX_CAP1           0x669

#define USB_LUNX_CAP2           0x66A

#define USB_LUNX_CAP3           0x66B

#define TOTAL_RD_SEC            0x670   //8 bytes

#define TOTAL_WR_SEC            0x678   //8 bytes

#define EPN_BF_SKP              0x680   //16 bytes

#define CDB_CEI_LL              0x690   //4 bytes

#define CDB_CEI_LH              0x691   //4 bytes

#define CDB_CEI_HL              0x692   //4 bytes

#define CDB_CEI_HH              0x693   //4 bytes

#define CDB_CEI_NCLR            0x698   //4 bytes

#define CDB_OCI                     0x6A0   //4 bytes

#define CDB_T_CON                   0x6B0
#define SR_CDB_T_STS                BIT7
#define SR_CDB_TFUL                 BIT6
#define SR_CDB_OVLP                 BIT5
#define SR_CDB_NC                       BIT4
#define CR_CDB_T_EN                 BIT0

#define CDB_CFG                     0x6B2
#define CR_CDB_CLEN                 BIT7
#define QUEUE_4                         0x00
#define QUEUE_8                         0x01
#define QUEUE_16                        0x02
#define QUEUE_32                        0x03

#define CDB_NCMD                    0x6B4

#define CDB_OVLP_INDX           0x6B5

//===============================================================================================
//page number = 0x04
#define OP_SS_L2P                   0x200

#define OP_SS_L2P2              0x201
#define SS_U12ITM_EN                BIT1
#define SS_U2INATO_EN               BIT0

#define OP_SS_PLU_EPI0      0x202

#define OP_SS_PLU_EPI1      0x203

#define OP_SS_PLUEPO            0x204

#define REG280                      0x280
#define DIS_SCR                         BIT1

#define ERR_NUM_8B10B           0x288

#define LOSS_SYM_HUM            0x289

#define LTSSM_CUR_ST0           0x28A

#define LTSSM_CUR_ST1           0x28B

#define SS_P2M_LNKRTN           0x28c
#define LINK_RIN            BIT0

#define U1_TMR_P1               0x2C0

#define U2_TMR_P1               0x2C1

#define U2_CFG                  0x340

#define U2_DEV_DISCONN          0x341

#define U2_MAC_FLG              0x342

#define U2_ENUM_SPD             0x343

//===============================================================================================
//page number = 0x05
#define CR_OP_GLB_OP0           0x200
#define AUTO_TRIM_DONE      BIT7
#define PHY_XTAL_SEL        BIT0

#define INTF2_MXSZ              0x201

#define FAST_SIM                0x205
#define FAST_SIM_EN         BIT0

#define HW_SPDSW_EN             0x206

#define CR_OP_GLB_OP7           0x207
#define fw_frc_brst2ss_en_w     BIT5
#define st_disabled_err_en_w  BIT4
#define disabled_cnt_lmt_w      (BIT3 + BIT2 + BIT1 + BIT0)

#define CR_OP_GLB_OP8           0x208
#define PIPE_EN             BIT1
#define UTMI_EN             BIT0

#define CR_OP_GLB_OP9           0x209
#define PIPE_CLKSW          BIT1
#define UTMI_CLKSW          BIT0

#define CR_OP_GLB_OPA           0x20A           // write available when cmu_hw_spdsw_en = 0
#define AB_SW_RST           BIT7
#define SS_SW_RST           BIT1
#define U2_SW_RST           BIT0

#define PHY_TRACK_0             0x210
#define PHY_TRACK_1             0x211
#define PHY_TRACK_2             0x212
#define PHY_TRACK_3             0x213
#define PHY_TRACK_4             0x214
#define PHY_TRACK_5             0x215
#define PHY_TRACK_6             0x216
#define PHY_TRACK_7             0x217
#define PHY_TRACK_8             0x218
#define PHY_TRACK_9             0x219
#define PHY_TRACK_A             0x21A
#define PHY_TRACK_B             0x21B

#define PHY_MISC_0              0x21C
#define PHY_MISC_1              0x21D
#define PHY_MISC_2              0x21E

#define PHY_RCOSC                   0x228

//===============================================================================================
//page number = 0x06
#define CR_OP_SS_PLU_OP7        0x207

#define CR_OP_SS_MAC_OPC        0x28C
#define CR_SS_U3_KEEP_P2        BIT2
#define CR_SS_DIS_KEEP_P2       BIT1

#define PHY_PMA_0               0x300
#define PHY_PMA_1               0x301
#define PHY_PMA_2               0x302
#define PHY_PMA_3               0x303
#define PHY_PMA_4               0x304
#define PHY_PMA_5               0x305
#define PHY_PMA_6               0x306
#define PHY_PMA_7               0x307
#define PHY_PMA_8               0x308
#define PHY_PMA_9               0x309
#define PHY_PMA_A               0x30A
#define PHY_PMA_B               0x30B
#define PHY_PMA_C               0x30C
#define PHY_PMA_D               0x30D
#define PHY_PMA_E               0x30E
#define PHY_PMA_F               0x30F
#define PHY_PMA_10              0x310
#define PHY_PMA_11              0x311
#define PHY_PMA_12              0x312
#define PHY_PMA_13              0x313
#define PHY_PMA_14              0x314
#define PHY_PMA_15              0x315

#define PHY_PCS_0               0x320
#define PHY_PCS_1               0x321
#define PHY_PCS_2               0x322
#define PHY_PCS_3               0x323
#define PHY_PCS_4               0x324
#define PHY_PCS_5               0x325
#define PHY_PCS_6               0x326
#define PHY_PCS_7               0x327
#define PHY_PCS_8               0x328
#define PHY_PCS_9               0x329

//===============================================================================================
//page number = 0x07
#define PHY_UTMA_0              0x300
#define PHY_UTMA_1              0x301
#define PHY_UTMA_2              0x302
#define PHY_UTMA_3              0x303
#define PHY_UTMA_4              0x304
#define PHY_UTMA_5              0x305
#define PHY_UTMA_6              0x306
#define PHY_UTMA_7              0x307
#define PHY_UTMA_8              0x308
#define PHY_UTMA_9              0x309
#define PHY_UTMA_A              0x30A
#define PHY_UTMA_B              0x30B
#define PHY_UTMA_C              0x30C
#define PHY_UTMA_D              0x30D
#define PHY_UTMA_E              0x30E
#define PHY_UTMA_F              0x30F
#define PHY_UTMA_10             0x310
#define PHY_UTMA_11             0x311

#define PHY_UTML_0              0x320
#define PHY_UTML_1              0x321
#define PHY_UTML_2              0x322
#define PHY_UTML_3              0x323
#define PHY_UTML_4              0x324
#define PHY_UTML_5              0x325
#define PHY_UTML_6              0x326
#define PHY_UTML_7              0x327
#define PHY_UTML_8              0x328
#define PHY_UTML_9              0x329

//===============================================================================================
//page number = 0x08
#define EP0_RAM0                0x200

//===============================================================================================
//page number = 0x10
#define EP0_RAM1                0x200

//===============================================================================================
//page nymber = 0x12
#define EP0_RAM2                0x200

//===============================================================================================
//page nymber = 0x13
#define RX_RAM0                 0x200

//===============================================================================================
//page nymber = 0x14
#define RX_RAM1                 0x200

//===============================================================================================
//page number = 0x15
#define RX_RAM2                 0x200

//===============================================================================================
//page number = 0x16
#define RX_RAM3                 0x200

//===============================================================================================
//page number = 0x17
#define RX_RAM4                 0x200

//===============================================================================================
//page nymber = 0x18
#define TX_RAM0                 0x200

//===============================================================================================
//page nymber = 0x19
#define TX_RAM1                 0x200

//===============================================================================================
//page number = 0x1a
#define TX_RAM2                 0x200

//===============================================================================================
//page number = 0x1b
#define TX_RAM3                 0x200

//===============================================================================================
//page number = 0x1c
#define TX_RAM4                 0x200

//===============================================================================================
//page nymber = 0x1D
#define TX_RAM5                 0x200

//===============================================================================================
//page nymber = 0x1e
#define TX_RAM6                 0x200

//===============================================================================================
//page number = 0x1f
#define TX_RAM7                 0x200
#endif  //#ifnDef __USB_REG_H__
