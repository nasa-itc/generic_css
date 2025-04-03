# Library for GENERIC_CSS Target
require 'cosmos'
require 'cosmos/script'

#
# Definitions
#
GENERIC_CSS_CMD_SLEEP = 0.25
GENERIC_CSS_RESPONSE_TIMEOUT = 5
GENERIC_CSS_TEST_LOOP_COUNT = 1
GENERIC_CSS_DEVICE_LOOP_COUNT = 5

#
# Functions
#
def get_generic_css_hk()
    cmd("GENERIC_CSS GENERIC_CSS_REQ_HK")
    wait_check_packet("GENERIC_CSS", "GENERIC_CSS_HK_TLM", 1, GENERIC_CSS_RESPONSE_TIMEOUT)
    sleep(GENERIC_CSS_CMD_SLEEP)
end

def get_generic_css_data()
    cmd("GENERIC_CSS GENERIC_CSS_REQ_DATA")
    wait_check_packet("GENERIC_CSS", "GENERIC_CSS_DATA_TLM", 1, GENERIC_CSS_RESPONSE_TIMEOUT)
    sleep(GENERIC_CSS_CMD_SLEEP)
end

def generic_css_cmd(*command)
    count = tlm("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_COUNT") + 1

    if (count == 256)
        count = 0
    end

    cmd(*command)
    get_generic_css_hk()
    current = tlm("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_COUNT")
    if (current != count)
        # Try again
        cmd(*command)
        get_generic_css_hk()
        current = tlm("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_COUNT")
        if (current != count)
            # Third times the charm
            cmd(*command)
            get_generic_css_hk()
            current = tlm("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_COUNT")
        end
    end
    check("GENERIC_CSS GENERIC_CSS_HK_TLM CMD_COUNT >= #{count}")
end

def enable_generic_css()
    # Send command
    generic_css_cmd("GENERIC_CSS GENERIC_CSS_ENABLE_CC")
    # Confirm
    check("GENERIC_CSS GENERIC_CSS_HK_TLM DEVICE_ENABLED == 'ENABLED'")
end

def disable_generic_css()
    # Send command
    generic_css_cmd("GENERIC_CSS GENERIC_CSS_DISABLE_CC")
    # Confirm
    check("GENERIC_CSS GENERIC_CSS_HK_TLM DEVICE_ENABLED == 'DISABLED'")
end

def safe_generic_css()
    get_generic_css_hk()
    state = tlm("GENERIC_CSS GENERIC_CSS_HK_TLM DEVICE_ENABLED")
    if (state != "DISABLED")
        disable_generic_css()
    end
end

def confirm_generic_css_data()
    dev_cmd_cnt = tlm("GENERIC_CSS GENERIC_CSS_HK_TLM DEVICE_COUNT")
    dev_cmd_err_cnt = tlm("GENERIC_CSS GENERIC_CSS_HK_TLM DEVICE_ERR_COUNT")
    
    get_generic_css_data()
    # Note these checks assume default simulator configuration
    diff_margin = 5

    adcs_percent_on0 = tlm("GENERIC_ADCS GENERIC_ADCS_DI PERCENTON0")
    adcs_adjusted_0 = adcs_percent_on0*1000
    wait_check_tolerance("GENERIC_CSS GENERIC_CSS_DATA_TLM RAW_CSS_0", adcs_adjusted_0, diff_margin, 15)

    adcs_percent_on1 = tlm("GENERIC_ADCS GENERIC_ADCS_DI PERCENTON1")
    adcs_adjusted_1 = adcs_percent_on1*1000
    wait_check_tolerance("GENERIC_CSS GENERIC_CSS_DATA_TLM RAW_CSS_1", adcs_adjusted_1, diff_margin, 15)

    adcs_percent_on2 = tlm("GENERIC_ADCS GENERIC_ADCS_DI PERCENTON2")
    adcs_adjusted_2 = adcs_percent_on2*1000
    wait_check_tolerance("GENERIC_CSS GENERIC_CSS_DATA_TLM RAW_CSS_2", adcs_adjusted_2, diff_margin, 15)

    adcs_percent_on3 = tlm("GENERIC_ADCS GENERIC_ADCS_DI PERCENTON3")
    adcs_adjusted_3 = adcs_percent_on3*1000
    wait_check_tolerance("GENERIC_CSS GENERIC_CSS_DATA_TLM RAW_CSS_3", adcs_adjusted_3, diff_margin, 15)

    adcs_percent_on4 = tlm("GENERIC_ADCS GENERIC_ADCS_DI PERCENTON4")
    adcs_adjusted_4 = adcs_percent_on4*1000
    wait_check_tolerance("GENERIC_CSS GENERIC_CSS_DATA_TLM RAW_CSS_4", adcs_adjusted_4, diff_margin, 15)

    adcs_percent_on5 = tlm("GENERIC_ADCS GENERIC_ADCS_DI PERCENTON5")
    adcs_adjusted_5 = adcs_percent_on5*1000
    wait_check_tolerance("GENERIC_CSS GENERIC_CSS_DATA_TLM RAW_CSS_5", adcs_adjusted_5, diff_margin, 15)

    get_generic_css_hk()
    check("GENERIC_CSS GENERIC_CSS_HK_TLM DEVICE_COUNT >= #{dev_cmd_cnt}")
    check("GENERIC_CSS GENERIC_CSS_HK_TLM DEVICE_ERR_COUNT == #{dev_cmd_err_cnt}")
end

def confirm_generic_css_data_loop()
    GENERIC_CSS_DEVICE_LOOP_COUNT.times do |n|
        confirm_generic_css_data()
    end
end

#
# Simulator Functions
#
def generic_css_prepare_ast()
    # Get to known state
    safe_generic_css()

    # Enable
    enable_generic_css()

    # Confirm data
    confirm_generic_css_data_loop()
end

def generic_css_sim_enable()
    cmd("SIM_CMDBUS_BRIDGE GENERIC_CSS_ENABLE")
end

def generic_css_sim_disable()
    cmd("SIM_CMDBUS_BRIDGE GENERIC_CSS_DISABLE")
end