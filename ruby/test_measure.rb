require '/home/jason/Cpp_Swig_Ruby_Python_MCVE/cmake-build-debug/Products/ruby/mylib.so'

class RubyTestMeasure < Mylib::Measure
  def name
    return "RubyTestMeasure"
  end

  def run_impl(runner)
    runner.get_current_model().pushOp("A Ruby Op")
    return true;
  end
end


