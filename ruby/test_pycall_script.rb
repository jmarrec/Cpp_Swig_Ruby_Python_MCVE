ruby_lib_path = File.expand_path(File.join(__dir__, '../build/Products/ruby/mylib.so'))

if !File.exist?(ruby_lib_path)
  puts "Error, this assumes you already built the project in <root>/build"
  exit 1
end

require ruby_lib_path


puts "==================  RUBY SIDE  ======================"
ruby_p = Mylib::Person.new("John")
puts "ruby_p=#{ruby_p}"
puts "Mylib::personName(ruby_p)= #{Mylib::personName(ruby_p)}"


puts "\n\n"

puts "==================  PYTHON SIDE  ======================"

require 'pycall/import'
include PyCall::Import


python_lib_path = File.expand_path(File.join(__dir__, '../build/Products/python/mylib.py'))
if !File.exist?(python_lib_path)
  puts "Error, this assumes you already built the Python project in <root>/build"
  exit 1
end

sys = PyCall.import_module('sys')
sys.path.insert(0, "#{File.dirname(python_lib_path)}")

pyimport 'mylib', as: 'mylib_python'


### Now load the "measure"

#pyfrom 'measure', import: 'PythonMeasureName'
python_measure_path = File.expand_path(File.join(__dir__, '../python/measure.py'))
sys.path.insert(0, "#{File.dirname(python_measure_path)}")
pyimport 'measure', as: 'measuremodule'
measure_object = measuremodule.PythonMeasureName.new()


python_p = mylib_python.Person._fromInt(ruby_p.__toInt())

###### call the measure
new_name = "Bob"
puts "Calling measure_object.run(python_p, #{new_name})"
measure_object.run(python_p, new_name)

# Now ruby_p is still the same object
ruby_name = ruby_p.getName()
puts "ruby_name=#{ruby_name}"

python_name = python_p.getName()
puts "python_name=#{python_name}"

raise if ruby_name != new_name
raise if python_name != new_name

