#include "ModelObject.hpp"

namespace Test {

  ModelObject::ModelObject(std::string name)
    : name_(std::move(name))
  {
  }

  const std::string& ModelObject::getName() const {
    return name_;
  }

  bool ModelObject::setName(const std::string& name) {
    name_ = name;
    return true;
  }

}  // namespace Test
