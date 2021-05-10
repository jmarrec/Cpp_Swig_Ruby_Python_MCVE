require '/home/julien/Software/Cpp_Swig_Ruby_Python_MCVE/build-modif/Products/ruby/mylib.so'

class RubyTestMeasure < Mylib::Measure
  def name
    return "RubyTestMeasure"
  end

  def run_impl(runner)
    puts "Ruby Model named: #{runner.get_current_model().getName()}"
    runner.get_current_model().setName("Ruby Model")
    runner.get_current_model().pushOp("A Ruby Op")
    return true;
  end
end


