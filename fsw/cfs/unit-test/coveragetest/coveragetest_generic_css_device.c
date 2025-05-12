
#include "generic_css_app_coveragetest_common.h"

void Test_GENERIC_CSS_RequestData(void)
{
    i2c_bus_info_t              device;
    GENERIC_CSS_Device_Data_tlm_t data;
    GENERIC_CSS_RequestData(&device, &data);
}

void Test_GENERIC_CSS_RequestData_Hook(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context, va_list va) {}

/*
 * Setup function prior to every test
 */
void generic_css_UT_Setup(void)
{
    UT_ResetState(0);
}

/*
 * Teardown function after every test
 */
void generic_css_UT_TearDown(void) {}

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    UT_SetVaHandlerFunction(UT_KEY(Test_GENERIC_CSS_RequestData), Test_GENERIC_CSS_RequestData_Hook, NULL);
}