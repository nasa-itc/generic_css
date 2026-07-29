import sys
import glob

for p in glob.glob('/gems/gems/openc3-cosmos-nos3-*/targets/GENERIC_CSS/scripts'):
    if p not in sys.path:
        sys.path.append(p)

from openc3.script import cmd, tlm, check

try:
    from nos3.generic_css_lib import generic_css_prepare_ast, generic_css_sim_disable, generic_css_sim_enable, confirm_generic_css_data_loop, GENERIC_CSS_TEST_LOOP_COUNT
except ImportError:
    pass

def run_generic_css_ast_test():
    ##
    ## This script tests the cFS component in an automated scenario.
    ## Currently this includes: 
    ##   Hardware failure
    ##   Hardware status reporting fault
    ##


    ##
    ## Hardware failure
    ##
    for n in range(GENERIC_CSS_TEST_LOOP_COUNT):
        # Prepare
        generic_css_prepare_ast()

        # Disable sim and confirm device error counts increase
        dev_cmd_cnt = tlm("GENERIC_CSS_DEBUG GENERIC_CSS_HK_TLM DEVICE_COUNT")
        dev_cmd_err_cnt = tlm("GENERIC_CSS_DEBUG GENERIC_CSS_HK_TLM DEVICE_ERR_COUNT")
        generic_css_sim_disable()
        check(f"GENERIC_CSS_DEBUG GENERIC_CSS_HK_TLM DEVICE_COUNT == {dev_cmd_cnt}")
        check(f"GENERIC_CSS_DEBUG GENERIC_CSS_HK_TLM DEVICE_ERR_COUNT >= {dev_cmd_err_cnt}")

        # Enable sim and confirm return to nominal operation
        generic_css_sim_enable()
        confirm_generic_css_data_loop()