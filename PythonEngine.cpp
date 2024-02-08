#include "PythonEngine.hpp"

#include <string>
#include <stdexcept>

#ifdef __GNUC__
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#include <Python.h>
//#include "SWIGPythonRuntime.hxx"
#include "MeasurePYTHON_wrap.cxx"
#include "embedded_files.hxx"
#ifdef __GNUC__
#  pragma GCC diagnostic pop
#endif

namespace Test {

PythonEngine::PythonEngine([[maybe_unused]] const int argc, const char* argv[]) : program(Py_DecodeLocale(argv[0], nullptr)) {
  if (program == nullptr) {
    fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
    exit(1);
  }

  PyImport_AppendInittab("_mylib", PyInit__mylib);

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

  PyObject* globalDict = PyModule_GetDict(m);

  // Find mylib.py (the python thing that swig spits out)
  // PyRun_SimpleString("import sys\nsys.path.append('/home/julien/Software/Cpp_Swig_Ruby_Python_MCVE/build-modif/temp/')\nprint(f'{sys.path}')");

  //std::string myModuleStr("def add( n1 , n2 ):\n    return n1+n2");
  //PyObject* pCompiledFn = Py_CompileString( myModuleStr.c_str() , "" , Py_file_input );


  std::string fileContent = embedded_files::getFileAsString(":/python/mylib.py");
  // std::cout << fileContent << '\n';

  //PyObject* pCompiledFn = Py_CompileString( fileContent.c_str() , "" , Py_file_input ) ;
  // PyObject* pModule = PyImport_ExecCodeModule( "mylib" , pCompiledFn ) ;

  /*
  PyObject *pyModule = PyModule_New("mylib");
  // Set properties on the new module object
  PyModule_AddStringConstant(pyModule, "__file__", "");
  //PyObject *localDict = PyModule_GetDict(pyModule);   // Returns a borrowed reference: no need to Py_DECREF() it once we are done
  PyObject *builtins = PyEval_GetBuiltins();  // Returns a borrowed reference: no need to Py_DECREF() it once we are done
  PyDict_SetItemString(globalDict, "__builtins__", builtins);
  PyDict_SetItemString(globalDict, "mylib", pyModule);

  // Define code in the newly created module
  PyObject *pyValue = PyRun_String(fileContent.c_str(), Py_file_input, globalDict, globalDict);
  if (pyValue == NULL) {
    // Handle error
    std::cout << "Unable to initialize mylib\n";
  } else {
      Py_DECREF(pyValue);
  }
*/

  PyObject *builtins = PyEval_GetBuiltins();
  PyObject *compile = PyDict_GetItemString(builtins, "compile");
  PyObject *code = PyObject_CallFunction(compile, "sss", fileContent.c_str(), "mylib.py", "exec");
  PyObject *pyModule = PyImport_ExecCodeModule("mylib", code);


  // PyRun_SimpleString("import sys\nsys.path.append(':')\nprint(f'{sys.path}')");
  // PySys_SetPath(L"/home/julien/Software/Cpp_Swig_Ruby_Python_MCVE/build-modif/Products/python"); // Find mylib.py
  PyRun_SimpleString("print('Hello from PythonEngine::exec')");

  PyObject* v = PyRun_String(command.c_str(), Py_file_input, globalDict, globalDict);
  if (v == nullptr) {
    PyErr_Print();
    throw std::runtime_error("Error executing Python code");
  }

  //decref count returned from Python
  Py_DECREF(v);
  Py_DecRef( pyModule ) ;
  //Py_DecRef( pyCompiledFn ) ;

}

ScriptObject PythonEngine::eval(std::string_view sv) {
  std::string command{sv};

  PyObject* m = PyImport_AddModule("__main__");
  if (m == nullptr) {
    throw std::runtime_error("Unable to add module __main__ for python script execution");
  }

  PyObject* d = PyModule_GetDict(m);

  std::string fileContent = embedded_files::getFileAsString(":/python/mylib.py");
  //std::cout << fileContent << '\n';
  //PySys_SetPath(L"/home/julien/Software/Cpp_Swig_Ruby_Python_MCVE/build-modif/Products/python"); // Find mylib.py
  PyRun_SimpleString("print('Hello from PythonEngine::eval')");
  //PyObject *mylib_module = PyImport_Import(PyString_FromString("mylib"));
  //PyModule_AddObject(m, "mylib", mylib_module);

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

  auto* type = SWIG_Python_TypeQuery(type_name.c_str());

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
