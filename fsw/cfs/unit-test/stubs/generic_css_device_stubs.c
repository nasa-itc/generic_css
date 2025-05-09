/*******************************************************************************
** File: generic_css_device.h
**
** Purpose:
**   This is the header file for the GENERIC_CSS device.
**
*******************************************************************************/
#define _GENERIC_CSS_DEVICE_H_

/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in generic_css_device header
 */

#include "generic_css_device.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for GENERIC_CSS_RequestData()
 * ----------------------------------------------------
 */
int32_t GENERIC_CSS_RequestData(i2c_bus_info_t *device, GENERIC_CSS_Device_Data_tlm_t *data)
{
    UT_GenStub_SetupReturnBuffer(GENERIC_CSS_RequestData, int32_t);

    UT_GenStub_AddParam(GENERIC_CSS_RequestData, i2c_bus_info_t *, device);
    UT_GenStub_AddParam(GENERIC_CSS_RequestData, GENERIC_CSS_Device_Data_tlm_t *, data);

    UT_GenStub_Execute(GENERIC_CSS_RequestData, Basic, NULL);

    return UT_GenStub_GetReturnValue(GENERIC_CSS_RequestData, int32_t);
}
