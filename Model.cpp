#include "Model.hpp"

namespace Test {
Model::Model(std::string name) : name_(std::move(name)) {}

const std::string& Model::getName() const {
  return name_;
}

void Model::pushOp(const std::string& op_name) {
  opsPerformed_.push_back(op_name);
}

const std::vector<std::string>& Model::opsPerformed() const {
  return opsPerformed_;
}
}  // namespace Test