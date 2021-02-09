ruby_lib_path = File.expand_path(File.join(__dir__, '../build/Products/ruby/mylib.so'))

if !File.exist?(ruby_lib_path)
  puts "Error, this assumes you already built the project in <root>/build"
  exit 1
end

require ruby_lib_path

p = Mylib::Person.new("John")
puts p

puts "Mylib::personName(p)= #{Mylib::personName(p)}"
