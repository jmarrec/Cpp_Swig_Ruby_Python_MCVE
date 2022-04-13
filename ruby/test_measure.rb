require '/home/julien/Software/Cpp_Swig_Ruby_Python_MCVE/build-modif/Products/ruby/mylib.so'

class RubyTestMeasure < Mylib::Measure
  def name
    return "RubyTestMeasure"
  end

  def run_impl(runner)
    m = runner.get_current_model()
    puts "Ruby Model named: #{m.getName()}"
    m.setName("Ruby Model")
    m.pushOp("A Ruby Op")

    puts "Ruby: Model has: #{m.numObjects()} spaces"
    m.pushObject("Ruby Space")
    m.numObjects().times do |i|
      puts "* #{i} = #{m.getObject(i).getName()}"
    end

    # This crashes
    m.getObject(0).setName("MODIFIED FROM RUBY")
    return true;
  end
end


