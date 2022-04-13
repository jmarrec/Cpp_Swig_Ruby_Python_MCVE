// CMake defines WITHPYTHON (or not)

#include <iostream>
#include <cassert>
#include "RubyEngine.hpp"

#ifdef WITHPYTHON
#include "PythonEngine.hpp"
#endif

#include "Measure.hpp"
#include "SpecialRunner.hpp"

#include "Model.hpp"

int main([[maybe_unused]] const int argc, [[maybe_unused]] const char* argv[]) {
  Test::RubyEngine ruby;
  #ifdef WITHPYTHON
  Test::PythonEngine python{argc, argv};
  #endif

  ruby.exec(R"(puts("Hello from Ruby"))");

  #ifdef WITHPYTHON
  python.exec(R"(print("Hello From Python"))");
  #endif

  ruby.registerType<Test::Measure*>("Test::Measure *");

  #ifdef WITHPYTHON
  python.registerType<Test::Measure*>("Test::Measure *");
  #endif

  ruby.exec("require '/home/julien/Software/Cpp_Swig_Ruby_Python_MCVE/ruby/test_measure.rb'");
  auto ruby_measure = ruby.eval("RubyTestMeasure.new()");
  auto* ruby_measure_from_cpp = ruby.getAs<Test::Measure*>(ruby_measure);
  assert(ruby_measure_from_cpp);
  std::cout << "Ruby measure name: " << ruby_measure_from_cpp->name() << '\n';

  #ifdef WITHPYTHON
  python.exec("import sys\nsys.path.append('/home/julien/Software/Cpp_Swig_Ruby_Python_MCVE/python/')");
  python.exec("import test_measure");
  auto python_measure = python.eval("test_measure.PythonTestMeasure()");
  auto* python_measure_from_cpp = python.getAs<Test::Measure*>(python_measure);
  assert(python_measure_from_cpp);
  #endif

  // this all works up until this moment right here.
  // At this point it doesn't call the SWIG Director for Python
  // It actually calls the SWIG Director for Ruby.
  // Why? Because we've broken the One Definition Rule.
  // SWIG defines functions with the same names for both the Python bindings and the Ruby bindings.
  // whichever one gets loaded first wins.
  //
  // Can we work around this? Maybe. But having both Ruby and Python linked into the same application
  // is asking for many problems.
  #ifdef WITHPYTHON
    std::cout << "Python measure name: " << python_measure_from_cpp->name() << '\n';
  #endif

  auto printObjectNames = [](Test::Model& m) {
    for (int i = 0; i < m.numObjects(); ++i) {
      std::cout << " * " << i <<" = " << m.getObject(i).getName() << '\n';
    }
  };

  Test::Model m{"C++ Model"};
  m.pushObject("C++ object");
  Test::SpecialRunner sr(m);
  std::cout << "Starting out with a Model in C++ called: " << sr.get_current_model().getName() << '\n';
  std::cout << "C++: starting with " <<  sr.get_current_model().numObjects() << " objects\n";
  printObjectNames(sr.get_current_model());

  std::cout << "\n\n========== START RUBY ==========\n";

  ruby_measure_from_cpp->run(sr);

  std::cout << "========== FINISHED RUBY ==========\n\n";
  std::cout << "C++: " <<  sr.get_current_model().numObjects() << " objects\n";
  printObjectNames(sr.get_current_model());

  #ifdef WITHPYTHON
    std::cout << "\n\n========== START PYTHON ==========\n";
    python_measure_from_cpp->run(sr);
    std::cout << "========== FINISHED PYTHON ==========\n\n";
    std::cout << "After Running Ruby and Python: model is named " << sr.get_current_model().getName() << '\n';
  #else
    std::cout << "\n\n########## PYTHON ISN'T ENABLED ##########\n\n";
    std::cout << "After Running Ruby only: model is named " << sr.get_current_model().getName() << '\n';
  #endif

  for (const auto& op : sr.get_current_model().opsPerformed()) {
    std::cout << "Op 'run' from script: " << op << '\n';
  }

  std::cout << "C++: " <<  sr.get_current_model().numObjects() << " objects\n";
  printObjectNames(sr.get_current_model());

}
