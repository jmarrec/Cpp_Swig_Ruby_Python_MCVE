#ifndef RUBYENGINE_included
#define RUBYENGINE_included

#include "ScriptEngine.hpp"

namespace Test {
class RubyEngine final : public ScriptEngine
{
 public:
  RubyEngine();
  ~RubyEngine() override;

  RubyEngine(const RubyEngine&) = delete;
  RubyEngine(RubyEngine&&) = delete;
  RubyEngine& operator=(const RubyEngine&) = delete;
  RubyEngine& operator=(RubyEngine&&) = delete;

  ScriptObject eval(std::string_view sv) override;
  void exec(std::string_view sv) override;

 protected:
  // convert the underlying object to the correct type, then return it as a void *
  // so the above template function can provide it back to the caller.
  void* getAs_impl(ScriptObject& obj, const std::type_info&) override;
};
}  // namespace Test

#endif
