#include <iostream>
#include "RubyEngine.hpp"
#include "PythonEngine.hpp"



int main(const int argc, const char* argv[]) {
  Test::RubyEngine ruby;
  Test::PythonEngine python{argc, argv};

  python.exec(R"(print("Hello From Python"))");
  ruby.exec(R"(puts("Hello from Ruby"))");

}
