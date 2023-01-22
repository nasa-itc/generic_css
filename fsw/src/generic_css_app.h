/*******************************************************************************
** File: generic_css_app.h
**
** Purpose:
**   This is the main header file for the GENERIC_CSS application.
**
*******************************************************************************/
#ifndef _GENERIC_CSS_APP_H_
#define _GENERIC_CSS_APP_H_

/*
** Include Files
*/
#include "cfe.h"
#include "generic_css_device.h"
#include "generic_css_events.h"
#include "generic_css_platform_cfg.h"
#include "generic_css_perfids.h"
#include "generic_css_msg.h"
#include "generic_css_msgids.h"
#include "generic_css_version.h"
#include "hwlib.h"


/*
** Specified pipe depth - how many messages will be queued in the pipe
*/
#define GENERIC_CSS_PIPE_DEPTH            32


/*
** Enabled and Disabled Definitions
*/
#define GENERIC_CSS_DEVICE_DISABLED       0
#define GENERIC_CSS_DEVICE_ENABLED        1


/*
** GENERIC_CSS global data structure
** The cFE convention is to put all global app data in a single struct. 
** This struct is defined in the `generic_css_app.h` file with one global instance 
** in the `.c` file.
*/
typedef struct
{
    /*
    ** Housekeeping telemetry packet
    ** Each app defines its own packet which contains its OWN telemetry
    */
    GENERIC_CSS_Hk_tlm_t   HkTelemetryPkt;   /* GENERIC_CSS Housekeeping Telemetry Packet */
    
    /*
    ** Operational data  - not reported in housekeeping
    */
    CFE_SB_MsgPtr_t MsgPtr;             /* Pointer to msg received on software bus */
    CFE_SB_PipeId_t CmdPipe;            /* Pipe Id for HK command pipe */
    uint32 RunStatus;                   /* App run status for controlling the application state */

    /*
	** Device data 
	*/
	uint32 DeviceID;		            /* Device ID provided by CFS on initialization */
    GENERIC_CSS_Device_tlm_t DevicePkt;      /* Device specific data packet */

    /* 
    ** Device protocol
    ** TODO: Make specific to your application
    */ 
    uart_info_t Generic_cssUart;             /* Hardware protocol definition */

} GENERIC_CSS_AppData_t;


/*
** Exported Data
** Extern the global struct in the header for the Unit Test Framework (UTF).
*/
extern GENERIC_CSS_AppData_t GENERIC_CSS_AppData; /* GENERIC_CSS App Data */


/*
**
** Local function prototypes.
**
** Note: Except for the entry point (GENERIC_CSS_AppMain), these
**       functions are not called from any other source module.
*/
void  GENERIC_CSS_AppMain(void);
int32 GENERIC_CSS_AppInit(void);
void  GENERIC_CSS_ProcessCommandPacket(void);
void  GENERIC_CSS_ProcessGroundCommand(void);
void  GENERIC_CSS_ProcessTelemetryRequest(void);
void  GENERIC_CSS_ReportHousekeeping(void);
void  GENERIC_CSS_ReportDeviceTelemetry(void);
void  GENERIC_CSS_ResetCounters(void);
void  GENERIC_CSS_Enable(void);
void  GENERIC_CSS_Disable(void);
int32 GENERIC_CSS_VerifyCmdLength(CFE_SB_MsgPtr_t msg, uint16 expected_length);

#endif /* _GENERIC_CSS_APP_H_ */
