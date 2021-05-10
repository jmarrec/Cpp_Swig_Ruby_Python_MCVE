#include <iostream>
#include <cassert>
#include "RubyEngine.hpp"
#include "PythonEngine.hpp"
#include "Measure.hpp"
#include "SpecialRunner.hpp"

#include "Model.hpp"

int main(const int argc, const char* argv[]) {
  Test::RubyEngine ruby;
  Test::PythonEngine python{argc, argv};

  python.exec(R"(print("Hello From Python"))");
  ruby.exec(R"(puts("Hello from Ruby"))");

  ruby.registerType<Test::Measure*>("Test::Measure *");
  python.registerType<Test::Measure*>("Test::Measure *");

  ruby.exec("require '/home/julien/Software/Cpp_Swig_Ruby_Python_MCVE/ruby/test_measure.rb'");
  auto ruby_measure = ruby.eval("RubyTestMeasure.new()");
  auto* ruby_measure_from_cpp = ruby.getAs<Test::Measure*>(ruby_measure);
  assert(ruby_measure_from_cpp);
  std::cout << "Ruby measure name: " << ruby_measure_from_cpp->name() << '\n';

  python.exec("import sys\nsys.path.append('/home/julien/Software/Cpp_Swig_Ruby_Python_MCVE/python/')");
  python.exec("import test_measure");
  auto python_measure = python.eval("test_measure.PythonTestMeasure()");
  auto* python_measure_from_cpp = python.getAs<Test::Measure*>(python_measure);
  assert(python_measure_from_cpp);

  // this all works up until this moment right here.
  // At this point it doesn't call the SWIG Director for Python
  // It actually calls the SWIG Director for Ruby.
  // Why? Because we've broken the One Definition Rule.
  // SWIG defines functions with the same names for both the Python bindings and the Ruby bindings.
  // whichever one gets loaded first wins.
  //
  // Can we work around this? Maybe. But having both Ruby and Python linked into the same application
  // is asking for many problems.
  std::cout << "Python measure name: " << python_measure_from_cpp->name() << '\n';

  Test::SpecialRunner sr(Test::Model{"C++ Model"});
  std::cout << "Starting out with a Model in C++ called: " << sr.get_current_model().getName() << '\n';
  ruby_measure_from_cpp->run(sr);
  python_measure_from_cpp->run(sr);

  std::cout << "After Running Ruby and Python: model is named " << sr.get_current_model().getName() << '\n';

  for (const auto& op : sr.get_current_model().opsPerformed()) {
    std::cout << "Op 'run' from script: " << op << '\n';
  }
}
