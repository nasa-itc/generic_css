require 'cosmos'
require 'cosmos/script'
require 'mission_lib.rb'

class CSS_LPT < Cosmos::Test
  def setup
    
  end

  def test_lpt
    start("tests/generic_css_lpt_test.rb")
  end

  def teardown

  end
end

class CSS_CPT < Cosmos::Test
  def setup
      
  end

  def test_cpt
    start("tests/generic_css_cpt_test.rb")
  end

  def teardown

  end
end

class Generic_css_Test < Cosmos::TestSuite
  def initialize
      super()
      add_test('CSS_CPT')
      add_test('CSS_LPT')
  end

  def setup
  end
  
  def teardown
  end
end