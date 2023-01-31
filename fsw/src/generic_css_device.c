/*******************************************************************************
** File: generic_css_device.c
**
** Purpose:
**   This file contains the source code for the GENERIC_CSS device.
**
*******************************************************************************/

/*
** Include Files
*/
#include "generic_css_device.h"

static float GENERIC_CSS_TELEM_CONVERSION = 0.001; // V

/*
** Convert data received for an ADC based on predefined conversion value
*/
float GENERIC_CSS_TelemetryConversion(uint16 read_value)
{
    float rval = GENERIC_CSS_TELEM_CONVERSION * read_value;
    return rval;
}

/* 
** Generic read data from device
*/
int32_t GENERIC_CSS_ReadData(int32_t handle, uint8_t* read_data, uint8_t data_length)
{
    int32_t status = OS_SUCCESS;
    uint8* response;

    status = i2c_master_transaction(handle, GENERIC_CSS_I2C_ADDRESS, null, 0, &response, data_length, GENERIC_CSS_CFG_MS_TIMEOUT);
    if (status != OS_SUCCESS)
    {
        *read_data = 0.0;
        //return OS_ERROR;
    }
    else
    {
        *read_data[0] = GENERIC_CSS_TelemetryConversion((response[0] << 8) && response[1]);
        *read_data[1] = GENERIC_CSS_TelemetryConversion((response[2] << 8) && response[3]);
        *read_data[2] = GENERIC_CSS_TelemetryConversion((response[4] << 8) && response[5]);
        *read_data[3] = GENERIC_CSS_TelemetryConversion((response[6] << 8) && response[7]);
        *read_data[4] = GENERIC_CSS_TelemetryConversion((response[8] << 8) && response[9]);
        *read_data[5] = GENERIC_CSS_TelemetryConversion((response[10] << 8) && response[11]);
    }

    return status;
}

/*
** Request data command
*/
int32_t GENERIC_CSS_RequestData(int32_t handle, GENERIC_CSS_Device_Data_tlm_t* data)
{
    int32_t status = OS_SUCCESS;
    uint8_t read_data[GENERIC_CSS_DEVICE_DATA_LNGTH] = {0};
    //uint8_t read_data[GENERIC_CSS_DEVICE_DATA_SIZE] = {0};

    /* Command device to send HK */
    //status = GENERIC_CSS_CommandDevice(handle, GENERIC_CSS_DEVICE_REQ_DATA_CMD, 0);
    //if (status == OS_SUCCESS)
    //{
        /* Read HK data */
        status = GENERIC_CSS_ReadData(handle, read_data, sizeof(read_data));
        if (status == OS_SUCCESS)
        {
            #ifdef GENERIC_CSS_CFG_DEBUG
                OS_printf("  GENERIC_CSS_RequestData = ");
                for (uint32_t i = 0; i < sizeof(read_data); i++)
                {
                    OS_printf("%02x", read_data[i]);
                }
                OS_printf("\n");
            #endif

            /* Verify data header and trailer */
            /*
            if ((read_data[0]  == GENERIC_CSS_DEVICE_HDR_0)     && 
                (read_data[1]  == GENERIC_CSS_DEVICE_HDR_1)     && 
                (read_data[12] == GENERIC_CSS_DEVICE_TRAILER_0) && 
                (read_data[13] == GENERIC_CSS_DEVICE_TRAILER_1) )
            {
            */
                data->Voltage = read_data;

                #ifdef GENERIC_CSS_CFG_DEBUG
                    //OS_printf("  Header  = 0x%02x%02x  \n", read_data[0], read_data[1]);
                    //OS_printf("  Counter = 0x%08x      \n", data->DeviceCounter);
                    //OS_printf("  Data X  = 0x%04x, %d  \n", data->DeviceDataX, data->DeviceDataX);
                    OS_printf("  Voltage ADC 1  = 0x%04x, %d  \n", data->Voltage[0], data->Voltage[0]);
                    OS_printf("  Voltage ADC 2  = 0x%04x, %d  \n", data->Voltage[1], data->Voltage[1]);
                    OS_printf("  Voltage ADC 3  = 0x%04x, %d  \n", data->Voltage[2], data->Voltage[2]);
                    OS_printf("  Voltage ADC 4  = 0x%04x, %d  \n", data->Voltage[3], data->Voltage[3]);
                    OS_printf("  Voltage ADC 5  = 0x%04x, %d  \n", data->Voltage[4], data->Voltage[4]);
                    OS_printf("  Voltage ADC 6  = 0x%04x, %d  \n", data->Voltage[5], data->Voltage[5]);
                    //OS_printf("  Trailer = 0x%02x%02x  \n", read_data[10], read_data[11]);
                #endif
            //}
        } 
        else
        {
            #ifdef GENERIC_CSS_CFG_DEBUG
                OS_printf("  GENERIC_CSS_RequestData: Invalid data read! \n");
            #endif 
            status = OS_ERROR;
        } /* GENERIC_CSS_ReadData */
/*
    }
    else
    {
        #ifdef GENERIC_CSS_CFG_DEBUG
            OS_printf("  GENERIC_CSS_RequestData: GENERIC_CSS_CommandDevice reported error %d \n", status);
        #endif 
    }
*/
    return status;
}

    /* Wait until all data received or timeout occurs */
    /*
    bytes_available = uart_bytes_available(handle);
    while((bytes_available < data_length) && (ms_timeout_counter < GENERIC_CSS_CFG_MS_TIMEOUT))
    {
        ms_timeout_counter++;
        OS_TaskDelay(1);
        bytes_available = uart_bytes_available(handle);
    }

    if (ms_timeout_counter < GENERIC_CSS_CFG_MS_TIMEOUT)
    {
        // Limit bytes available
        if (bytes_available > data_length)
        {
            bytes_available = data_length;
        }
        
        // Read data
        bytes = uart_read_port(handle, read_data, bytes_available);
        if (bytes != bytes_available)
        {
            #ifdef GENERIC_CSS_CFG_DEBUG
                OS_printf("  GENERIC_CSS_ReadData: Bytes read != to requested! \n");
            #endif
            status = OS_ERROR;
        } // uart_read
    }
    else
    {
        status = OS_ERROR;
    } // ms_timeout_counter

    return status;
}
    */


/* 
** Generic command to device
*/
/*
int32_t GENERIC_EPS_CommandDevice(int32_t handle, uint8_t cmd, uint8_t value)
{
    int32_t status = OS_SUCCESS;
    uint8_t write_data[3] = {0};
    uint8_t i;

    /* Confirm command valid 
    if (cmd < 0xAB)
    {
        /* Prepare command 
        write_data[0] = cmd;
        write_data[1] = value;
        write_data[2] = GENERIC_EPS_CRC8(write_data, 1);

        /* Initiate transaction 
        i2c_master_transaction(handle, GENERIC_EPS_CFG_I2C_ADDRESS,
                               write_data, 3, 
                               NULL, 0, 
                               GENERIC_EPS_CFG_I2C_TIMEOUT);
    }
    else
    {
        status = OS_ERROR;
        #ifdef GENERIC_EPS_CFG_DEBUG
            OS_printf("  GENERIC_EPS_CommandDevice: Command 0x%02x is above valid range! (>= 0xAB expected) \n", cmd);
        #endif 
    }
    return status;
}
*/

/*
** Request housekeeping command
*/
/*
int32_t GENERIC_CSS_RequestHK(int32_t handle, GENERIC_CSS_Device_HK_tlm_t* data)
{
    int32_t status = OS_SUCCESS;
    uint8_t read_data[GENERIC_CSS_DEVICE_HK_SIZE] = {0};

    /* Command device to send HK 
    status = GENERIC_CSS_CommandDevice(handle, GENERIC_CSS_DEVICE_REQ_HK_CMD, 0);
    if (status == OS_SUCCESS)
    {
        /* Read HK data 
        status = GENERIC_CSS_ReadData(handle, read_data, sizeof(read_data));
        if (status == OS_SUCCESS)
        {
            #ifdef GENERIC_CSS_CFG_DEBUG
                OS_printf("  GENERIC_CSS_RequestHK = ");
                for (uint32_t i = 0; i < sizeof(read_data); i++)
                {
                    OS_printf("%02x", read_data[i]);
                }
                OS_printf("\n");
            #endif

            /* Verify data header and trailer 
            if ((read_data[0]  == GENERIC_CSS_DEVICE_HDR_0)     && 
                (read_data[1]  == GENERIC_CSS_DEVICE_HDR_1)     && 
                (read_data[14] == GENERIC_CSS_DEVICE_TRAILER_0) && 
                (read_data[15] == GENERIC_CSS_DEVICE_TRAILER_1) )
            {
                data->DeviceCounter  = read_data[2] << 24;
                data->DeviceCounter |= read_data[3] << 16;
                data->DeviceCounter |= read_data[4] << 8;
                data->DeviceCounter |= read_data[5];

                data->DeviceConfig  = read_data[6] << 24;
                data->DeviceConfig |= read_data[7] << 16;
                data->DeviceConfig |= read_data[8] << 8;
                data->DeviceConfig |= read_data[9];

                data->DeviceStatus  = read_data[10] << 24;
                data->DeviceStatus |= read_data[11] << 16;
                data->DeviceStatus |= read_data[12] << 8;
                data->DeviceStatus |= read_data[13];

                #ifdef GENERIC_CSS_CFG_DEBUG
                    OS_printf("  Header  = 0x%02x%02x  \n", read_data[0], read_data[1]);
                    OS_printf("  Counter = 0x%08x      \n", data->DeviceCounter);
                    OS_printf("  Config  = 0x%08x      \n", data->DeviceConfig);
                    OS_printf("  Status  = 0x%08x      \n", data->DeviceStatus);
                    OS_printf("  Trailer = 0x%02x%02x  \n", read_data[14], read_data[15]);
                #endif
            }
            else
            {
                #ifdef GENERIC_CSS_CFG_DEBUG
                    OS_printf("  GENERIC_CSS_RequestHK: GENERIC_CSS_ReadData reported error %d \n", status);
                #endif 
                status = OS_ERROR;
            }
        } /* GENERIC_CSS_ReadData 
    }
    else
    {
        #ifdef GENERIC_CSS_CFG_DEBUG
            OS_printf("  GENERIC_CSS_RequestHK: GENERIC_CSS_CommandDevice reported error %d \n", status);
        #endif 
    }
    return status;
}
*/
