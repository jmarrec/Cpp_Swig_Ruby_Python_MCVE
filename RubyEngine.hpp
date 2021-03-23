#ifndef RUBYENGINE_included
#define RUBYENGINE_included

#include "ScriptEngine.hpp"

namespace Test {
class RubyEngine final : public ScriptEngine
{
 public:
  RubyEngine();
  ~RubyEngine() override;

  RubyEngine(const RubyEngine &) = delete;
  RubyEngine(RubyEngine &&) = delete;
  RubyEngine &operator=(const RubyEngine &) = delete;
  RubyEngine &operator=(RubyEngine &&) = delete;

  ScriptObject exec(std::string_view sv) override;

 protected:
  // convert the underlying object to the correct type, then return it as a void *
  // so the above template function can provide it back to the caller.
  void* get_as(ScriptObject& obj, const std::type_info&) override;
};
}

#endif
