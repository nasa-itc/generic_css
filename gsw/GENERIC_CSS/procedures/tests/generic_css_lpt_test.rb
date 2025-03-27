require 'cosmos'
require 'cosmos/script'
require "cfs_lib.rb"
#require 'math'

##
## NOOP
##
initial_command_count = tlm("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_COUNT")
initial_error_count = tlm("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_ERR_COUNT")
initial_device_error_count = tlm("GENERIC_CSS GENERIC_CSS_HK_TLM DEVICE_ERR_COUNT")
cmd("GENERIC_CSS GENERIC_CSS_NOOP_CC")
wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_COUNT > #{initial_command_count}", 30)
wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_ERR_COUNT == #{initial_error_count}", 30)
wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM DEVICE_ERR_COUNT == #{initial_device_error_count}", 30)

sleep(5)

##
## Successful Disable
##
initial_command_count = tlm("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_COUNT")
initial_error_count = tlm("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_ERR_COUNT")
initial_device_error_count = tlm("GENERIC_CSS GENERIC_CSS_HK_TLM DEVICE_ERR_COUNT")
cmd("GENERIC_CSS GENERIC_CSS_DISABLE_CC")
wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_COUNT > #{initial_command_count}", 30)
wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_ERR_COUNT == #{initial_error_count}", 30)
wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM DEVICE_ERR_COUNT == #{initial_device_error_count}", 30)
wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM DEVICE_ENABLED == 'DISABLED'", 30)

sleep(5)

##
## Failed Disable (doubled)
##
initial_command_count = tlm("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_COUNT")
initial_error_count = tlm("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_ERR_COUNT")
initial_device_error_count = tlm("GENERIC_CSS GENERIC_CSS_HK_TLM DEVICE_ERR_COUNT")
cmd("GENERIC_CSS GENERIC_CSS_DISABLE_CC")
wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_COUNT > #{initial_command_count}", 30)
wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_ERR_COUNT == #{initial_error_count}", 30)
wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM DEVICE_ERR_COUNT > #{initial_device_error_count}", 30)
wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM DEVICE_ENABLED == 'DISABLED'", 30)

sleep(5)

##
## HK without Device
##
initial_error_count = tlm("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_ERR_COUNT")
initial_device_error_count = tlm("GENERIC_CSS GENERIC_CSS_HK_TLM DEVICE_ERR_COUNT")
cmd("GENERIC_CSS GENERIC_CSS_REQ_HK")
wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_ERR_COUNT == #{initial_error_count}", 30)
wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM DEVICE_ERR_COUNT == #{initial_device_error_count}", 30)

sleep(5)

##
## Data without Device
##
initial_error_count = tlm("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_ERR_COUNT")
initial_device_error_count = tlm("GENERIC_CSS GENERIC_CSS_HK_TLM DEVICE_ERR_COUNT")
cmd("GENERIC_CSS GENERIC_CSS_REQ_DATA")
wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_ERR_COUNT == #{initial_error_count}", 30)
wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM DEVICE_ERR_COUNT == #{initial_device_error_count}", 30)

sleep(5)

##
## Successful Enable
##
initial_command_count = tlm("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_COUNT")
initial_error_count = tlm("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_ERR_COUNT")
initial_device_error_count = tlm("GENERIC_CSS GENERIC_CSS_HK_TLM DEVICE_ERR_COUNT")
cmd("GENERIC_CSS GENERIC_CSS_ENABLE_CC")
wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_COUNT > #{initial_command_count}", 30)
wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_ERR_COUNT == #{initial_error_count}", 30)
wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM DEVICE_ERR_COUNT == #{initial_device_error_count}", 30)
wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM DEVICE_ENABLED == 'ENABLED'", 30)

sleep(5)

##
## Failed Enable (doubled)
##
initial_command_count = tlm("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_COUNT")
initial_error_count = tlm("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_ERR_COUNT")
initial_device_error_count = tlm("GENERIC_CSS GENERIC_CSS_HK_TLM DEVICE_ERR_COUNT")
cmd("GENERIC_CSS GENERIC_CSS_ENABLE_CC")
wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_COUNT > #{initial_command_count}", 30)
wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_ERR_COUNT == #{initial_error_count}", 30)
wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM DEVICE_ERR_COUNT > #{initial_device_error_count}", 30)
wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM DEVICE_ENABLED == 'ENABLED'", 30)

sleep(5)

##
## Housekeeping w/ Device
##
initial_error_count = tlm("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_ERR_COUNT")
initial_device_error_count = tlm("GENERIC_CSS GENERIC_CSS_HK_TLM DEVICE_ERR_COUNT")
cmd("GENERIC_CSS GENERIC_CSS_REQ_HK")
wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_ERR_COUNT == #{initial_error_count}", 30)
wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM DEVICE_ERR_COUNT == #{initial_device_error_count}", 30)

sleep(5)

##
## Data w/ Device
##
initial_error_count = tlm("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_ERR_COUNT")
initial_device_error_count = tlm("GENERIC_CSS GENERIC_CSS_HK_TLM DEVICE_ERR_COUNT")

cmd("GENERIC_CSS GENERIC_CSS_REQ_DATA")

css_raw0 = tlm("GENERIC_CSS GENERIC_CSS_DATA_TLM RAW_CSS_0")
css_raw1 = tlm("GENERIC_CSS GENERIC_CSS_DATA_TLM RAW_CSS_1")
css_raw2 = tlm("GENERIC_CSS GENERIC_CSS_DATA_TLM RAW_CSS_2")
css_raw3 = tlm("GENERIC_CSS GENERIC_CSS_DATA_TLM RAW_CSS_3")
css_raw4 = tlm("GENERIC_CSS GENERIC_CSS_DATA_TLM RAW_CSS_4")
css_raw5 = tlm("GENERIC_CSS GENERIC_CSS_DATA_TLM RAW_CSS_5")




adcs_percent_on0 = tlm("GENERIC_ADCS GENERIC_ADCS_DI PERCENTON0")
adcs_percent_on1 = tlm("GENERIC_ADCS GENERIC_ADCS_DI PERCENTON1")
adcs_percent_on2 = tlm("GENERIC_ADCS GENERIC_ADCS_DI PERCENTON2")
adcs_percent_on3 = tlm("GENERIC_ADCS GENERIC_ADCS_DI PERCENTON3")
adcs_percent_on4 = tlm("GENERIC_ADCS GENERIC_ADCS_DI PERCENTON4")
adcs_percent_on5 = tlm("GENERIC_ADCS GENERIC_ADCS_DI PERCENTON5")


adcs_percent_on_diff0 = (css_raw0 - adcs_percent_on0).abs()
adcs_percent_on_diff1 = (css_raw1 - adcs_percent_on1).abs()
adcs_percent_on_diff2 = (css_raw2 - adcs_percent_on2).abs()
adcs_percent_on_diff3 = (css_raw3 - adcs_percent_on3).abs()
adcs_percent_on_diff4 = (css_raw4 - adcs_percent_on4).abs()
adcs_percent_on_diff5 = (css_raw5 - adcs_percent_on5).abs()

diff_margin = 1000

wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_ERR_COUNT == #{initial_error_count}", 30)
wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM DEVICE_ERR_COUNT == #{initial_device_error_count}", 30)

wait_check_expression("adcs_percent_on_diff0 <= diff_margin # #{adcs_percent_on_diff0} >= #{diff_margin}", 15)
wait_check_expression("adcs_percent_on_diff1 <= diff_margin # #{adcs_percent_on_diff1} >= #{diff_margin}", 15)
wait_check_expression("adcs_percent_on_diff2 <= diff_margin # #{adcs_percent_on_diff2} >= #{diff_margin}", 15)
wait_check_expression("adcs_percent_on_diff3 <= diff_margin # #{adcs_percent_on_diff3} >= #{diff_margin}", 15)
wait_check_expression("adcs_percent_on_diff4 <= diff_margin # #{adcs_percent_on_diff4} >= #{diff_margin}", 15)
wait_check_expression("adcs_percent_on_diff5 <= diff_margin # #{adcs_percent_on_diff5} >= #{diff_margin}", 15)


sleep(5)

##
## Reset Counters
##
cmd("GENERIC_CSS GENERIC_CSS_DISABLE_CC") # disable to be able to properly check that device count gets reset
cmd("GENERIC_CSS GENERIC_CSS_RST_COUNTERS_CC")
wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_COUNT == 0", 30)
wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_ERR_COUNT == 0", 30)
wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM DEVICE_COUNT == 0", 30)
wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM DEVICE_ERR_COUNT == 0", 30)

##
## Reenable so test can be run multiple times
##
cmd("GENERIC_CSS GENERIC_CSS_ENABLE_CC")