#include "Model.hpp"
#include "ModelObject.hpp"

namespace Test {
  Model::Model(std::string name) : name_(std::move(name)) {}

  const std::string& Model::getName() const {
    return name_;
  }

  bool Model::setName(const std::string& name) {
    name_ = name;
    return true;
  }

  void Model::pushOp(const std::string& op_name) {
    opsPerformed_.push_back(op_name);
  }

  const std::vector<std::string>& Model::opsPerformed() const {
    return opsPerformed_;
  }

  int Model::numObjects() const {
    return static_cast<int>(m_objects.size());
  }

  std::vector<std::string> Model::objectNames() const {
    std::vector<std::string> result;
    for (auto objPtr : m_objects) {
      result.push_back(objPtr.getName());
    }
    return result;
  }

  ModelObject& Model::getObject(size_t index) {
    return m_objects[index];
  }

  void Model::pushObject(const std::string& objName) {
    m_objects.emplace_back(objName);
  }

}  // namespace Test
