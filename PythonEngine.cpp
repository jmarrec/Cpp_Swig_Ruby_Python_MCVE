#include "PythonEngine.hpp"
#include <Python.h>
#include <string>

namespace Test {

PythonEngine::PythonEngine([[maybe_unused]] const int argc, const char* argv[]) : program(Py_DecodeLocale(argv[0], nullptr)) {
  if (program == nullptr) {
    fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
    exit(1);
  }

  Py_SetProgramName(program);  // optional but recommended
  Py_Initialize();
}

PythonEngine::~PythonEngine() {
  if (Py_FinalizeEx() < 0) {
    exit(120);
  }
  PyMem_RawFree(program);
}

ScriptObject PythonEngine::exec(std::string_view sv) {
  std::string str{sv};
  PyRun_SimpleString(str.c_str());
  return ScriptObject{};
}

// convert the underlying object to the correct type, then return it as a void *
// so the above template function can provide it back to the caller.
void* PythonEngine::get_as(ScriptObject&, const std::type_info&) {
  return nullptr;
}
}  // namespace Test
