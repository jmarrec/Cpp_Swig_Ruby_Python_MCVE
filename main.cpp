// CMake defines WITHPYTHON / WITHRUBY (or not)

#include <iostream>
#include <cassert>

#ifdef WITHRUBY
#include "RubyEngine.hpp"
#endif

#ifdef WITHPYTHON
#include "PythonEngine.hpp"
#endif

#include "Measure.hpp"
#include "SpecialRunner.hpp"

#include "Model.hpp"

int main([[maybe_unused]] const int argc, [[maybe_unused]] const char* argv[]) {

  std::string ruby_script_path;
  std::string python_script_path;

  constexpr bool enableGetLine = true;
  if constexpr (enableGetLine) {
    std::cout << "Enter path to ruby_script: ";
    std::getline(std::cin, ruby_script_path);

    std::cout << "Enter path to python_script: ";
    std::getline(std::cin, python_script_path);
  } else {
    ruby_script_path = "/home/julien/Software/Cpp_Swig_Ruby_Python_MCVE/ruby/test_measure.rb";
    python_script_path = "/home/julien/Software/Cpp_Swig_Ruby_Python_MCVE/python/test_measure.py";

    // Test with relative path, expects to find it in current dir (CURRENT DIR WHERE YOU RUN THE EXE, NOT THE EXE DIR ITSELF)
    ruby_script_path = "test_measure.rb";
    python_script_path = "test_measure.py";
  }

  #ifdef WITHRUBY
  Test::RubyEngine ruby;
  #endif

  #ifdef WITHPYTHON
  Test::PythonEngine python{argc, argv};
  #endif

  #ifdef WITHRUBY
  ruby.exec(R"(puts("Hello from Ruby"))");
  #endif

  #ifdef WITHPYTHON
  python.exec(R"(print("Hello From Python"))");
  #endif

  #ifdef WITHRUBY
  ruby.registerType<Test::Measure*>("Test::Measure *");
  #endif

  #ifdef WITHPYTHON
  python.registerType<Test::Measure*>("Test::Measure *");
  #endif

  #ifdef WITHRUBY
  // ruby.exec("require '/home/julien/Software/Cpp_Swig_Ruby_Python_MCVE/ruby/test_measure.rb'");
  std::cout << "Loading ruby_script '" << ruby_script_path << "'\n";
  if (auto pos = python_script_path.find_last_of("/\\"); pos != std::string::npos) {
    ruby.exec("require '" + ruby_script_path + "'");
  } else {
    ruby.exec("require_relative '" + ruby_script_path + "'");
  }

  auto ruby_measure = ruby.eval("RubyTestMeasure.new()");
  auto* ruby_measure_from_cpp = ruby.getAs<Test::Measure*>(ruby_measure);
  assert(ruby_measure_from_cpp);
  std::cout << "Ruby measure name: " << ruby_measure_from_cpp->name() << '\n';
  #endif

  #ifdef WITHPYTHON
  std::cout << "Loading python_script '" << python_script_path << "'\n";
  std::string moduleName;
  auto pos = python_script_path.find_last_of("/\\");
  if (pos != std::string::npos) {
    std::string dirName = python_script_path.substr(0, pos);
    std::string fileName = python_script_path.substr(pos+1);
    auto lastindex = fileName.find_last_of(".");
    moduleName = fileName.substr(0, lastindex);

    std::cout << "import sys\nsys.path.append('"+ dirName + "')\nprint(f'{sys.path}')\n\n";
    python.exec("import sys\nsys.path.append('"+ dirName + "')\nprint(f'{sys.path}')");
    std::cout << "import " + moduleName << '\n';
    python.exec("import " + moduleName);
  } else {
    auto lastindex = python_script_path.find_last_of(".");
    moduleName = python_script_path.substr(0, lastindex);

    std::cout << "import sys\nsys.path.append('.')\nprint(f'{sys.path}')\n\n";
     python.exec("import sys\nsys.path.append('.')\nprint(f'{sys.path}')");
    std::cout << "import " + moduleName << '\n';
    python.exec("import " + moduleName);
    std::cout << "ABBOOOOORT\n";
  }

  auto python_measure = python.eval(moduleName + ".PythonTestMeasure()");
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

  #ifdef WITHRUBY
    std::cout << "\n\n========== START RUBY ==========\n";
    ruby_measure_from_cpp->run(sr);
    std::cout << "========== FINISHED RUBY ==========\n\n";
  #else
    std::cout << "\n\n########## RUBY ISN'T ENABLED ##########\n\n";
  #endif


  std::cout << "C++: " <<  sr.get_current_model().numObjects() << " objects\n";
  printObjectNames(sr.get_current_model());

  #ifdef WITHPYTHON
    std::cout << "\n\n========== START PYTHON ==========\n";
    python_measure_from_cpp->run(sr);
    std::cout << "========== FINISHED PYTHON ==========\n\n";
  #endif

  #if defined(WITHPYTHON) && defined(WITHRUBY)
    std::cout << "After Running Ruby and Python: model is named " << sr.get_current_model().getName() << '\n';
  #elif defined (WITHRUBY)
    std::cout << "After Running Ruby only: model is named " << sr.get_current_model().getName() << '\n';
  #elif defined(WITHPYTHON)
    std::cout << "After Running Python only: model is named " << sr.get_current_model().getName() << '\n';
  #endif

  for (const auto& op : sr.get_current_model().opsPerformed()) {
    std::cout << "Op 'run' from script: " << op << '\n';
  }

  std::cout << "C++: " <<  sr.get_current_model().numObjects() << " objects\n";
  printObjectNames(sr.get_current_model());

}
