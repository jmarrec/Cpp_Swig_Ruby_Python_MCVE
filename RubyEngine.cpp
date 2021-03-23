#include "RubyEngine.hpp"
#include <string>

#ifdef __GNUC__
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wregister"
#endif

#include <ruby.h>

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

ScriptObject RubyEngine::exec(std::string_view sv) {
  std::string str{sv};
  rb_eval_string(str.c_str());
  return ScriptObject{};
}

// convert the underlying object to the correct type, then return it as a void *
// so the above template function can provide it back to the caller.
void* RubyEngine::get_as(ScriptObject&, const std::type_info&) {
  return nullptr;
}

}  // namespace Test
