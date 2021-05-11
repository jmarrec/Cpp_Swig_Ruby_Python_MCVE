#ifndef MODELOBJECT_HPP
#define MODELOBJECT_HPP

#include <string>

namespace Test {

  class ModelObject {
    public:
      explicit ModelObject(std::string name);
      const std::string& getName() const;
      bool setName(const std::string& name);

    private:
      std::string name_;
  };
}  // namespace Test

#endif
