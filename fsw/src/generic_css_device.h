/*******************************************************************************
** File: generic_css_device.h
**
** Purpose:
**   This is the header file for the GENERIC_CSS device.
**
*******************************************************************************/
#ifndef _GENERIC_CSS_DEVICE_H_
#define _GENERIC_CSS_DEVICE_H_

/*
** Required header files.
*/
#include "device_cfg.h"
#include "hwlib.h"
#include "generic_css_platform_cfg.h"


/*
** Type definitions
** TODO: Make specific to your application
*/
#define GENERIC_CSS_DEVICE_HDR              0xDEAD
#define GENERIC_CSS_DEVICE_HDR_0            0xDE
#define GENERIC_CSS_DEVICE_HDR_1            0xAD

#define GENERIC_CSS_DEVICE_NOOP_CMD         0x00
#define GENERIC_CSS_DEVICE_REQ_HK_CMD       0x01
#define GENERIC_CSS_DEVICE_REQ_DATA_CMD     0x02
#define GENERIC_CSS_DEVICE_CFG_CMD          0x03

#define GENERIC_CSS_DEVICE_TRAILER          0xBEEF
#define GENERIC_CSS_DEVICE_TRAILER_0        0xBE
#define GENERIC_CSS_DEVICE_TRAILER_1        0xEF

#define GENERIC_CSS_DEVICE_HDR_TRL_LEN      4
#define GENERIC_CSS_DEVICE_CMD_SIZE         9

#define GENERIC_CSS_I2C_ADDRESS             0x40
#define GENERIC_CSS_NUM_CHANNELS            6

/*
** GENERIC_CSS device housekeeping telemetry definition
*/
typedef struct
{
    uint32_t  DeviceCounter;
    uint32_t  DeviceConfig;
    uint32_t  DeviceStatus;

} OS_PACK GENERIC_CSS_Device_HK_tlm_t;
#define GENERIC_CSS_DEVICE_HK_LNGTH sizeof ( GENERIC_CSS_Device_HK_tlm_t )
#define GENERIC_CSS_DEVICE_HK_SIZE GENERIC_CSS_DEVICE_HK_LNGTH + GENERIC_CSS_DEVICE_HDR_TRL_LEN


/*
** GENERIC_CSS device data telemetry definition
*/
typedef struct
{
    uint16 Voltage[GENERIC_CSS_NUM_CHANNELS];
} OS_PACK GENERIC_CSS_Device_Data_tlm_t;
#define GENERIC_CSS_DEVICE_DATA_LNGTH sizeof ( GENERIC_CSS_Device_Data_tlm_t )
//#define GENERIC_CSS_DEVICE_DATA_SIZE GENERIC_CSS_DEVICE_DATA_LNGTH + GENERIC_CSS_DEVICE_HDR_TRL_LEN


/*
** Prototypes
*/

int32_t GENERIC_CSS_ReadData(int32_t handle, uint8_t* read_data, uint8_t data_length);
int32_t GENERIC_CSS_RequestData(int32_t handle, GENERIC_CSS_Device_Data_tlm_t* data);
/*
int32_t GENERIC_CSS_ReadData(int32_t handle, uint8_t* read_data, uint8_t data_length);
int32_t GENERIC_CSS_CommandDevice(int32_t handle, uint8_t cmd, uint32_t payload);
int32_t GENERIC_CSS_RequestHK(int32_t handle, GENERIC_CSS_Device_HK_tlm_t* data);
int32_t GENERIC_CSS_RequestData(int32_t handle, GENERIC_CSS_Device_Data_tlm_t* data);
*/

#endif /* _GENERIC_CSS_DEVICE_H_ */