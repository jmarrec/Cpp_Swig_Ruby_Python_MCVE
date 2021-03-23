#ifndef SCRIPTENGINE_included
#define SCRIPTENGINE_included

#include <string_view>
#include <any>
#include <typeinfo>

namespace Test
{
  class ScriptObject {
    std::any object;
    friend class ScriptEngine;
  };

  class ScriptEngine {
    public:
     ScriptEngine() = default;
     virtual ~ScriptEngine() = default;
     ScriptEngine(const ScriptEngine&) = delete;
     ScriptEngine(ScriptEngine&&) = delete;
     ScriptEngine& operator=(const ScriptEngine&) = delete;
     ScriptEngine& operator=(ScriptEngine&&) = delete;

     virtual ScriptObject exec(std::string_view sv) = 0;

    template<typename T>
      T &get_as(ScriptObject &obj) {
        void *result = get_as(obj, typeid(T));
        if (result) {
          return static_cast<T &>(result);
        } else {
          throw std::bad_cast();
        }
      }

    protected:
      // convert the underlying object to the correct type, then return it as a void *
      // so the above template function can provide it back to the caller.
      virtual void *get_as(ScriptObject &obj, const std::type_info &) = 0;

      static std::any &getInternals(ScriptObject &obj) noexcept { return obj.object; }
  };


}

#endif


