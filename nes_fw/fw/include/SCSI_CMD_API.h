#ifndef __SCSI_CMD_API_H__
#define __SCSI_CMD_API_H__
//==================================================================================================================

//Define
//==================================================================================================================

//==================================================================================================================
//Global variable

//==================================================================================================================

//Global Function
extern void Inquiry();
//extern void SCSI_Read();
//extern void SCSI_Write();
extern void Test_Unit_Ready();
extern void Prevent_Allow_Removal();
extern void Read_Format_Capacity();
extern void Request_Sense();
extern void Mode_Sense6();
extern void Mode_Sense();

#endif  //#ifndef __SCSI_CMD_API_H__