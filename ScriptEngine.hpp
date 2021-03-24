#ifndef SCRIPTENGINE_included
#define SCRIPTENGINE_included

#include <any>
#include <map>
#include <string_view>
#include <typeinfo>
#include <functional>

namespace Test {
struct ScriptObject
{
  std::any object;
};

class ScriptEngine
{
 public:
  ScriptEngine() = default;
  virtual ~ScriptEngine() = default;
  ScriptEngine(const ScriptEngine&) = delete;
  ScriptEngine(ScriptEngine&&) = delete;
  ScriptEngine& operator=(const ScriptEngine&) = delete;
  ScriptEngine& operator=(ScriptEngine&&) = delete;

  virtual ScriptObject eval(std::string_view sv) = 0;

  // execute string without expecting a return value
  virtual void exec(std::string_view sv) = 0;

  template <typename T>
  T getAs(ScriptObject& obj) {
    void* result = getAs_impl(obj, typeid(T));
    if (result) {
      return static_cast<T>(result);
    } else {
      throw std::bad_cast();
    }
  }

  template <typename T>
  void registerType(std::string name) {
    types.emplace(std::cref(typeid(T)), std::move(name));
  }

 protected:
  // convert the underlying object to the correct type, then return it as a void *
  // so the above template function can provide it back to the caller.
  virtual void* getAs_impl(ScriptObject& obj, const std::type_info&) = 0;

  const std::string& getRegisteredTypeName(const std::type_info& type) {
    const auto& found_name = types.find(type);

    if (found_name != types.end()) {
      return found_name->second;
    }

    throw std::runtime_error("unknown type requested");
  }

 private:
  struct Compare
  {
    bool operator()(const std::reference_wrapper<const std::type_info> &lhs, const std::reference_wrapper<const std::type_info>& rhs) {
      return lhs.get().before(rhs.get());
    }
  };
  
  std::map<std::reference_wrapper<const std::type_info>, std::string, Compare> types;
};

}  // namespace Test

#endif
