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

# We're going to use C++ to marshal between a ruby object and a python object
# We take the already created ruby_p object and get back an int.
#
# What is the int (actually unsigned long long)? it's just the underlying pointer.
# Why an int? Because we need a type that Python, Ruby, and PyCall all recognize
#
# Next we take that int and say "please give me a Python Person object from this"
#
# What does fromInt do? It simply reinterpret_casts the integral value back into a Person &
#
# Note: it is critical that we call toInt from Ruby (because it's a Ruby `Person`)
#  and critical that we call fromInt from Python (because we want a Python `Person`)
#
# **** Very Important: there are *all kinds of ways* this can go wrong. we need to provide a
# handy wrapper in ruby that gets a python object back and not directly tell the users
# of either library about these fromInt/toInt functions

python_p2 = mylib_python.Person._fromInt(ruby_p.__toInt())

# Now ruby_p is still the same object
ruby_name = ruby_p.getName()
puts "ruby_name=#{ruby_name}"

# And python_p2 is a reference to the ruby_p object.
mylib_python.personName(python_p2)

# We verify this reference relationship asking Python to update the name
mylib_python.setName(python_p2, "Bob")

# Then verifying that it's the name we now expect
puts "Ruby name snould now be 'Bob' also: #{ruby_p.getName()}"

raise if ruby_p.getName() != 'Bob'
