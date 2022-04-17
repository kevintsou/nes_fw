#ifndef __SCSI_TABLE_H__
#define __SCSI_TABLE_H__
//==================================================================================================================
//SCSI CMD table
#define FORMAT_UNIT                 0x04
#define INQUIRY                     0x12
#define MODE_SELECT_6               0x15
#define MODE_SELECT_10              0x55
#define MODE_SENSE_6                0x1A
#define MODE_SENSE_10               0x5A
#define PRE_FETCH_10                0x34
#define PRE_FETCH_16                0x90
#define PREVENT_ALLOW_REMOVAL   		0x1E
#define READ_6                      0x08
#define READ_10                     0x28
#define READ_16                     0x88
#define READ_32                     0xA8
#define READ_BUFFER                 0x3C
#define READ_CAPACITY_10            0x25
#define READ_CAPACITY_16            0x9E
#define READ_FORMAT_CAPACITY    0x23
#define REPORT_LUNS                 0xA0
#define REQUEST_SENSE               0x03
#define SECURITY_PROTOCOL_IN        0xA2
#define SECURITY_PROTOCOL_OUT       0xB5
#define RECIEVE_DIAGONOSTIC         0x1C
#define SEND_DIAGONOSTIC            0x1D
#define START_STOP_UNIT             0x1B
#define SYNCHRONIZE_CACHE_10        0x35
#define SYNCHRONIZE_CACHE_16        0x91
#define TEST_UNIT_READY             0x00
#define UNMAP                       0x42
#define VERIFY_10                   0x2F
#define WRITE_6                     0x0A
#define WRITE_10                    0x2A
#define WRITE_16                    0x8A
#define WRITE_32                    0xAA
#define WRITE_BUFFER                0x3B
#define PHISON_VENDOR               0x06
#define CARDREADER_CMD          0xCA
#define STREAM_CMD              0xCB

#endif  //#ifndef __SCSI_TABLE_H__
