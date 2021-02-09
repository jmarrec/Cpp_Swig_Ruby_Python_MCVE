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
python_p = mylib_python.Person.new("John")
puts "python_p=#{python_p}"
puts "mylib_python.personName(python_p)= #{mylib_python.personName(python_p)}"


puts "\n\n"

puts "==================  PASSING RUBY TO PYTHON  ======================"

# OK! Here's the thing, now I want to call the python version with the ruby one
puts "mylib_python.personName(ruby_p)"
mylib_python.personName(ruby_p)
