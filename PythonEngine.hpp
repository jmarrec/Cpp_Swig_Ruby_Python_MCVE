#ifndef PYTHONENGINE_included
#define PYTHONENGINE_included

#include "ScriptEngine.hpp"

namespace Test {
class PythonEngine final : public ScriptEngine
{
 public:
  PythonEngine(const int argc, const char* argv[]);
  ~PythonEngine() override;

  PythonEngine(const PythonEngine&) = delete;
  PythonEngine(PythonEngine&&) = delete;
  PythonEngine& operator=(const PythonEngine&) = delete;
  PythonEngine& operator=(PythonEngine&&) = delete;

  ScriptObject eval(std::string_view sv) override;
  void exec(std::string_view sv) override;

 protected:
  // convert the underlying object to the correct type, then return it as a void *
  // so the above template function can provide it back to the caller.
  void* getAs_impl(ScriptObject& obj, const std::type_info&) override;

 private:
  wchar_t* program;
};
}  // namespace Test

#endif
