import sys
import glob

for p in glob.glob('/gems/gems/openc3-cosmos-nos3-*/targets/GENERIC_CSS/scripts'):
    if p not in sys.path:
        sys.path.append(p)

from openc3.script import cmd, tlm, check
from openc3.script.suite import Suite, Group

try:
    from nos3.generic_css_lib import safe_generic_css
    from nos3.generic_css_app_test import run_generic_css_app_test
    from nos3.generic_css_device_test import run_generic_css_device_test
    from nos3.generic_css_ast_test import run_generic_css_ast_test
except ImportError:
    pass

class GENERIC_CSS_Functional_Test(Group):
    def setup(self):
        safe_generic_css()

    def script_application(self):
        run_generic_css_app_test()

    def script_device(self):
        run_generic_css_device_test()

    def teardown(self):
        safe_generic_css()

class GENERIC_CSS_Automated_Scenario_Test(Group):
    def setup(self): 
        safe_generic_css()

    def script_cpt(self):
        run_generic_css_ast_test()

    def teardown(self):
        safe_generic_css()

class Generic_css_Test(Suite):
    def __init__(self):
        super().__init__()
        self.add_group(GENERIC_CSS_Functional_Test)
        self.add_group(GENERIC_CSS_Automated_Scenario_Test)

    def setup(self):
        safe_generic_css()
  
    def teardown(self):
        safe_generic_css()