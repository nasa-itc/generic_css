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

css_raw = tlm("GENERIC_CSS GENERIC_CSS_DATA_TLM RAW_CSS_N")

adcs_percent_on = tlm("GENERIC_ADCS GENERIC_ADCS_DI PERCENTONN")

adcs_percent_on_diff = (css_raw - adcs_percent_on).abs()
diff_margin = 0.025

wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_ERR_COUNT == #{initial_error_count}", 30)
wait_check("GENERIC_CSS GENERIC_CSS_HK_TLM DEVICE_ERR_COUNT == #{initial_device_error_count}", 30)
if css_error == 0
  wait_check_expression("truth_42_alpha_diff <= diff_margin # #{truth_42_alpha_diff} >= #{diff_margin}", 15)

  wait_check_expression("truth_42_beta_diff <= diff_margin # #{truth_42_beta_diff} >= #{diff_margin}", 15)
end

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