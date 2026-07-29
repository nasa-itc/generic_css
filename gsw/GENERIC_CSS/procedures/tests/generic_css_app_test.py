import sys
import glob

for p in glob.glob('/gems/gems/openc3-cosmos-nos3-*/targets/GENERIC_CSS/scripts'):
    if p not in sys.path:
        sys.path.append(p)

from openc3.script import cmd, tlm, check

try:
    from nos3.generic_css_lib import get_generic_css_hk, generic_css_cmd, GENERIC_CSS_TEST_LOOP_COUNT
except ImportError:
    pass

def run_generic_css_app_test():
    ##
    ## This script tests the standard cFS component application functionality.
    ## Currently this includes: 
    ##   Housekeeping, request telemetry to be published on the software bus
    ##   NOOP, no operation but confirm correct counters increment
    ##   Reset counters, increment as done in NOOP and confirm ability to clear repeatably
    ##   Invalid ground command, confirm bad lengths and codes are rejected
    ##

    ##
    ##   Housekeeping, request telemetry to be published on the software bus
    ##
    for n in range(GENERIC_CSS_TEST_LOOP_COUNT):
        get_generic_css_hk()

    ##
    ## NOOP, no operation but confirm correct counters increment
    ##
    for n in range(GENERIC_CSS_TEST_LOOP_COUNT):
        generic_css_cmd("GENERIC_CSS_DEBUG GENERIC_CSS_NOOP_CC")

    ##
    ## Reset counters, increment as done in NOOP and confirm ability to clear repeatably
    ##
    for n in range(GENERIC_CSS_TEST_LOOP_COUNT):
        generic_css_cmd("GENERIC_CSS_DEBUG GENERIC_CSS_NOOP_CC")
        cmd("GENERIC_CSS_DEBUG GENERIC_CSS_RST_COUNTERS_CC") # Note standard `cmd` as we can't reset counters and then confirm increment
        get_generic_css_hk()
        check("GENERIC_CSS_DEBUG GENERIC_CSS_HK_TLM CMD_COUNT == 0")
        check("GENERIC_CSS_DEBUG GENERIC_CSS_HK_TLM CMD_ERR_COUNT == 0")

    ##
    ##   Invalid ground command, confirm bad lengths and codes are rejected
    ##
    for n in range(GENERIC_CSS_TEST_LOOP_COUNT):
        # Bad length
        cmd_cnt = tlm("GENERIC_CSS_DEBUG GENERIC_CSS_HK_TLM CMD_COUNT")
        cmd_err_cnt = tlm("GENERIC_CSS_DEBUG GENERIC_CSS_HK_TLM CMD_ERR_COUNT")
        cmd(f"GENERIC_CSS_DEBUG GENERIC_CSS_NOOP_CC with CCSDS_LENGTH {n+2}") # Note +2 due to CCSDS already being +1
        get_generic_css_hk()
        check(f"GENERIC_CSS_DEBUG GENERIC_CSS_HK_TLM CMD_COUNT == {cmd_cnt}")
        check(f"GENERIC_CSS_DEBUG GENERIC_CSS_HK_TLM CMD_ERR_COUNT == {cmd_err_cnt+1}")

    # Bad command codes (Equivalent to Ruby's 4..(4 + GENERIC_CSS_TEST_LOOP_COUNT))
    # Python's range(start, stop) is exclusive at the stop index, so we add 1 to the Ruby end condition
    for n in range(4, 5 + GENERIC_CSS_TEST_LOOP_COUNT):
        cmd_cnt = tlm("GENERIC_CSS_DEBUG GENERIC_CSS_HK_TLM CMD_COUNT")
        cmd_err_cnt = tlm("GENERIC_CSS_DEBUG GENERIC_CSS_HK_TLM CMD_ERR_COUNT")
        cmd(f"GENERIC_CSS_DEBUG GENERIC_CSS_NOOP_CC with CCSDS_FC {n+1}")
        get_generic_css_hk()
        check(f"GENERIC_CSS_DEBUG GENERIC_CSS_HK_TLM CMD_COUNT == {cmd_cnt}")
        check(f"GENERIC_CSS_DEBUG GENERIC_CSS_HK_TLM CMD_ERR_COUNT == {cmd_err_cnt+1}")