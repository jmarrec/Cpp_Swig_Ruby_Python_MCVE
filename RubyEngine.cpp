#include "RubyEngine.hpp"

#include <string>

#ifdef __GNUC__
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wregister"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#include "SWIGRubyRuntime.hxx"

#ifdef __GNUC__
#  pragma GCC diagnostic pop
#endif

namespace Test {

RubyEngine::RubyEngine() {
  ruby_setup();
}

RubyEngine::~RubyEngine() {
  ruby_finalize();
}

static VALUE evaluateSimpleImpl(VALUE arg) {
  return rb_eval_string(StringValuePtr(arg));
}

VALUE evalString(const std::string& t_str) {
  VALUE val = rb_str_new2(t_str.c_str());
  int error;
  // save and restore the current working directory in case the call to ruby upsets it
  VALUE result = rb_protect(evaluateSimpleImpl, val, &error);
  if (error != 0) {
    VALUE errval = rb_eval_string("$!.to_s");
    char* str = StringValuePtr(errval);
    std::string err(str);
    VALUE locval = rb_eval_string("$@.to_s");
    str = StringValuePtr(locval);
    std::string loc(str);
    throw std::runtime_error("Error '" + err + "' at " + loc);
  }

  return result;
}

ScriptObject RubyEngine::eval(std::string_view sv) {
  std::string str{sv};
  return ScriptObject{evalString(str)};
}

void RubyEngine::exec(std::string_view sv) {
  std::string str{sv};
  [[maybe_unused]] const auto result = evalString(str);
}

// convert the underlying object to the correct type, then return it as a void *
// so the above template function can provide it back to the caller.
void* RubyEngine::getAs_impl(ScriptObject& obj, const std::type_info &ti) {
  auto val = std::any_cast<VALUE>(obj.object);

  const auto &type_name = getRegisteredTypeName(ti);

  void *return_value = nullptr;

  auto *type = SWIG_TypeQuery(type_name.c_str());

  if (!type) {
    throw std::runtime_error("Unable to find type in SWIG");
  }

  const auto result = SWIG_ConvertPtr(val, &return_value, type, 0);

  if (!SWIG_IsOK(result)) {
    throw std::runtime_error("Error getting object from SWIG/Ruby");
  }

  return return_value;
}

}  // namespace Test
