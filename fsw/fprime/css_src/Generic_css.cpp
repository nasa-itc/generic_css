// ======================================================================
// \title  Generic_css.cpp
// \author jstar
// \brief  cpp file for Generic_css component implementation class
// ======================================================================

#include "css_src/Generic_css.hpp"
#include "FpConfig.hpp"


namespace Components {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  Generic_css ::
    Generic_css(const char* const compName) :
      Generic_cssComponentBase(compName)
  {

    /* Initialize HWLIB */
    nos_init_link();

    int32_t status = OS_SUCCESS;
    uint16_t Voltage[GENERIC_CSS_NUM_CHANNELS];

    HkTelemetryPkt.CommandCount = 0;
    HkTelemetryPkt.CommandErrorCount = 0;
    HkTelemetryPkt.DeviceCount = 0;
    HkTelemetryPkt.DeviceErrorCount = 0;
    HkTelemetryPkt.DeviceEnabled = GENERIC_CSS_DEVICE_DISABLED;

    /* Open device specific protocols */
    Generic_CSSI2c.handle = GENERIC_CSS_CFG_HANDLE;
    Generic_CSSI2c.isOpen = PORT_CLOSED;
    Generic_CSSI2c.speed = GENERIC_CSS_CFG_BAUDRATE_HZ;
    Generic_CSSI2c.addr = GENERIC_CSS_I2C_ADDRESS;
    status = i2c_master_init(&Generic_CSSI2c);
    if (status == OS_SUCCESS)
    {
        printf("I2C device %d configured with speed %d \n", Generic_CSSI2c.handle, Generic_CSSI2c.speed);
    }
    else
    {
        printf("I2C device %d failed to initialize! \n", Generic_CSSI2c.handle);
    }

    status = i2c_master_close(&Generic_CSSI2c);
  }

  Generic_css ::
    ~Generic_css()
  {
    // Close the device 
    i2c_master_close(&Generic_CSSI2c);

    nos_destroy_link();
  }

  // ----------------------------------------------------------------------
  // Handler implementations for commands
  // ----------------------------------------------------------------------


  void Generic_css :: NOOP_cmdHandler(FwOpcodeType opCode, U32 cmdSeq){
    HkTelemetryPkt.CommandCount++;

    this->log_ACTIVITY_HI_TELEM("NOOP command success!");
    OS_printf("NOOP command successful!\n");

    this->tlmWrite_CommandCount(HkTelemetryPkt.CommandCount);
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  void Generic_css :: ENABLE_cmdHandler(FwOpcodeType opCode, U32 cmdSeq){
    int32_t status = OS_SUCCESS;

    if(HkTelemetryPkt.DeviceEnabled == GENERIC_CSS_DEVICE_DISABLED)
    {
      HkTelemetryPkt.CommandCount++;

      Generic_CSSI2c.handle = GENERIC_CSS_CFG_HANDLE;
      Generic_CSSI2c.isOpen = PORT_CLOSED;
      Generic_CSSI2c.speed  = GENERIC_CSS_CFG_BAUDRATE_HZ;
      Generic_CSSI2c.addr   = GENERIC_CSS_I2C_ADDRESS;

      status = i2c_master_init(&Generic_CSSI2c);
      if(status == OS_SUCCESS)
      {
        HkTelemetryPkt.DeviceEnabled = GENERIC_CSS_DEVICE_ENABLED;
        HkTelemetryPkt.DeviceCount++;

        this->log_ACTIVITY_HI_TELEM("Enable command success!");
        OS_printf("Enable command successful!\n");
      }
      else
      {
        HkTelemetryPkt.DeviceErrorCount++;

        this->log_ACTIVITY_HI_TELEM("Enable command failed to init I2C!");
        OS_printf("Enable command failed to init I2C!\n");
      }
    }
    else
    {
      HkTelemetryPkt.CommandErrorCount++;

      this->log_ACTIVITY_HI_TELEM("Enable failed, already Enabled!");
      OS_printf("Enable failed, already Enabled!\n");
    }

    this->tlmWrite_CommandCount(HkTelemetryPkt.CommandCount);
    this->tlmWrite_CommandErrorCount(HkTelemetryPkt.CommandErrorCount);
    this->tlmWrite_DeviceCount(HkTelemetryPkt.DeviceCount);
    this->tlmWrite_DeviceErrorCount(HkTelemetryPkt.DeviceErrorCount);
    this->tlmWrite_DeviceEnabled(get_active_state(HkTelemetryPkt.DeviceEnabled));

    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  void Generic_css :: DISABLE_cmdHandler(FwOpcodeType opCode, U32 cmdSeq){
    int32_t status = OS_SUCCESS;

    if(HkTelemetryPkt.DeviceEnabled == GENERIC_CSS_DEVICE_ENABLED)
    {
      HkTelemetryPkt.CommandCount++;

      status = i2c_master_close(&Generic_CSSI2c);
      if(status == OS_SUCCESS)
      {
        HkTelemetryPkt.DeviceEnabled = GENERIC_CSS_DEVICE_DISABLED;
        HkTelemetryPkt.DeviceCount++;

        this->log_ACTIVITY_HI_TELEM("Disable command success!");
        OS_printf("Disable command successful!\n");
      }
      else
      {
        HkTelemetryPkt.DeviceErrorCount++;
        
        this->log_ACTIVITY_HI_TELEM("Disable command failed to close I2C!");
        OS_printf("Disable command failed to close I2C!\n");
      }
    }
    else
    {
      HkTelemetryPkt.CommandErrorCount++;

      this->log_ACTIVITY_HI_TELEM("Disable failed, already Disabled!");
      OS_printf("Disable failed, already Disabled!\n");
    }

    this->tlmWrite_CommandCount(HkTelemetryPkt.CommandCount);
    this->tlmWrite_CommandErrorCount(HkTelemetryPkt.CommandErrorCount);
    this->tlmWrite_DeviceCount(HkTelemetryPkt.DeviceCount);
    this->tlmWrite_DeviceErrorCount(HkTelemetryPkt.DeviceErrorCount);
    this->tlmWrite_DeviceEnabled(get_active_state(HkTelemetryPkt.DeviceEnabled));

    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  void Generic_css :: REQUEST_HOUSEKEEPING_cmdHandler(FwOpcodeType opCode, U32 cmdSeq){

    if(HkTelemetryPkt.DeviceEnabled == GENERIC_CSS_DEVICE_ENABLED)
    {
      HkTelemetryPkt.CommandCount++;
      this->tlmWrite_ADCVoltage0(Generic_CSSData.Voltage[0]);
      this->tlmWrite_ADCVoltage1(Generic_CSSData.Voltage[1]);
      this->tlmWrite_ADCVoltage2(Generic_CSSData.Voltage[2]);
      this->tlmWrite_ADCVoltage3(Generic_CSSData.Voltage[3]);
      this->tlmWrite_ADCVoltage4(Generic_CSSData.Voltage[4]);
      this->tlmWrite_ADCVoltage5(Generic_CSSData.Voltage[5]);

      this->tlmWrite_CommandCount(HkTelemetryPkt.CommandCount);
      this->tlmWrite_CommandErrorCount(HkTelemetryPkt.CommandErrorCount);
      this->tlmWrite_DeviceCount(HkTelemetryPkt.DeviceCount);
      this->tlmWrite_DeviceErrorCount(HkTelemetryPkt.DeviceErrorCount);
      this->tlmWrite_DeviceEnabled(get_active_state(HkTelemetryPkt.DeviceEnabled));

      this->log_ACTIVITY_HI_TELEM("Requested Housekeeping!");
      OS_printf("Requested Housekeeping!\n");
    }
    else
    {
      this->log_ACTIVITY_HI_TELEM("Device Disabled!");
      OS_printf("Device Disabled!\n");
    }


    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

  void Generic_css :: RESET_COUNTERS_cmdHandler(FwOpcodeType opCode, U32 cmdSeq){
    HkTelemetryPkt.CommandCount = 0;
    HkTelemetryPkt.CommandErrorCount = 0;
    HkTelemetryPkt.DeviceCount = 0;
    HkTelemetryPkt.DeviceErrorCount = 0;

    this->log_ACTIVITY_HI_TELEM("Reset Counters command successful!");
    OS_printf("Reset Counters command successful!\n");
    this->tlmWrite_CommandCount(HkTelemetryPkt.CommandCount);
    this->tlmWrite_CommandErrorCount(HkTelemetryPkt.CommandErrorCount);
    this->tlmWrite_DeviceCount(HkTelemetryPkt.DeviceCount);
    this->tlmWrite_DeviceErrorCount(HkTelemetryPkt.DeviceErrorCount);

    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

 // GENERIC_CSS_RequestData
 void Generic_css :: REQUEST_DATA_cmdHandler(FwOpcodeType opCode, U32 cmdSeq) {

  int32_t status = OS_SUCCESS;

  if(HkTelemetryPkt.DeviceEnabled == GENERIC_CSS_DEVICE_ENABLED){
    HkTelemetryPkt.CommandCount++;

    status = GENERIC_CSS_RequestData(&Generic_CSSI2c, &Generic_CSSData);
    if (status == OS_SUCCESS)
    {
      HkTelemetryPkt.DeviceCount++;
      
      this->log_ACTIVITY_HI_TELEM("Request Data command success\n");
      OS_printf("RequestData command successful!\n");
    }
    else
    {
      HkTelemetryPkt.DeviceErrorCount++;
      this->log_ACTIVITY_HI_TELEM("Request Data command failed!\n");
      OS_printf("RequestData command failed!\n");
    }

  }
  else
  {
    HkTelemetryPkt.CommandErrorCount++;
    this->log_ACTIVITY_HI_TELEM("Request Data failed, Device Disabled!");
      OS_printf("Request Data failed, Device Disabled!\n");

  }

  this->tlmWrite_ADCVoltage0(Generic_CSSData.Voltage[0]);
  this->tlmWrite_ADCVoltage1(Generic_CSSData.Voltage[1]);
  this->tlmWrite_ADCVoltage2(Generic_CSSData.Voltage[2]);
  this->tlmWrite_ADCVoltage3(Generic_CSSData.Voltage[3]);
  this->tlmWrite_ADCVoltage4(Generic_CSSData.Voltage[4]);
  this->tlmWrite_ADCVoltage5(Generic_CSSData.Voltage[5]);
  this->tlmWrite_CommandCount(HkTelemetryPkt.CommandCount);
  this->tlmWrite_CommandErrorCount(HkTelemetryPkt.CommandErrorCount);
  this->tlmWrite_DeviceCount(HkTelemetryPkt.DeviceCount);
  this->tlmWrite_DeviceErrorCount(HkTelemetryPkt.DeviceErrorCount);

  // Tell the fprime command system that we have completed the processing of the supplied command with OK status
  this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

inline Generic_css_ActiveState Generic_css :: get_active_state(uint8_t DeviceEnabled)
{
  Generic_css_ActiveState state;

  if(DeviceEnabled == GENERIC_CSS_DEVICE_ENABLED)
  {
    state.e = Generic_css_ActiveState::ENABLED;
  }
  else
  {
    state.e = Generic_css_ActiveState::DISABLED;
  }

  return state;
}


}

 