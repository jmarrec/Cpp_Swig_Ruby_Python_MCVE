#include "PythonEngine.hpp"

#include <string>
#include <stdexcept>

#ifdef __GNUC__
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#include <Python.h>
#include "SWIGPythonRuntime.hxx"

#ifdef __GNUC__
#  pragma GCC diagnostic pop
#endif

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

struct PythonObject {
  PythonObject() = default;

  PythonObject(PyObject *obj) noexcept : obj_(obj)
  {
    if (obj_) {
      Py_INCREF(obj_);
    }
  }

  PythonObject(const PythonObject &other) noexcept
   : obj_(other.obj_) {
    if (obj_) {
      Py_INCREF(obj_);
    }
  }

  PythonObject(PythonObject &&other) noexcept
    : obj_(other.obj_) {
    // no reason to inc/dec, we just stole the ref counted object
    // from other
    other.obj_ = nullptr;
  }

  PythonObject &operator=(const PythonObject &rhs) noexcept {
    if (&rhs != this) {
      obj_ = rhs.obj_;

      if (obj_) {
        Py_INCREF(obj_);
      }
    }

    return *this;
  }

  PythonObject &operator=(PythonObject &&rhs) noexcept {
    if (&rhs != this) {
      obj_ = rhs.obj_;
      rhs.obj_ = nullptr;
    }

    return *this;
  }

  ~PythonObject() {
    if (obj_) {
      Py_DECREF(obj_);
    }
  }

  PyObject *obj_ = nullptr;
};

void PythonEngine::exec(std::string_view sv) {
  std::string command{sv};

  PyObject* m = PyImport_AddModule("__main__");
  if (m == nullptr) {
    throw std::runtime_error("Unable to add module __main__ for python script execution");
  }

  PyObject* d = PyModule_GetDict(m);
  PyObject* v = PyRun_String(command.c_str(), Py_file_input, d, d);
  if (v == nullptr) {
    PyErr_Print();
    throw std::runtime_error("Error executing Python code");
  }

  //decref count returned from Python
  Py_DECREF(v);
}

ScriptObject PythonEngine::eval(std::string_view sv) {
  std::string command{sv};

  PyObject* m = PyImport_AddModule("__main__");
  if (m == nullptr) {
    throw std::runtime_error("Unable to add module __main__ for python script execution");
  }

  PyObject* d = PyModule_GetDict(m);
  PyObject* v = PyRun_String(command.c_str(), Py_eval_input, d, d);
  if (v == nullptr) {
    PyErr_Print();
    throw std::runtime_error("Error executing Python code");
  }

  //share in ownership
  PythonObject return_value(v);

  //decref count returned from Python
  Py_DECREF(v);

  return ScriptObject{return_value};
}

// convert the underlying object to the correct type, then return it as a void *
// so the above template function can provide it back to the caller.
void* PythonEngine::getAs_impl(ScriptObject& obj, const std::type_info& ti) {

  auto val = std::any_cast<PythonObject>(obj.object);

  const auto& type_name = getRegisteredTypeName(ti);

  void* return_value = nullptr;

  auto* type = SWIG_TypeQuery(type_name.c_str());

  if (!type) {
    throw std::runtime_error("Unable to find type in SWIG");
  }

  const auto result = SWIG_Python_ConvertPtr(val.obj_, &return_value, type, 0);

  if (!SWIG_IsOK(result)) {
    throw std::runtime_error("Error getting object from SWIG/Python");
  }

  return return_value;
}
}  // namespace Test
