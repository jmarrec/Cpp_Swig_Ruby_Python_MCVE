#include "Model.hpp"

namespace Test
{
  Model::Model(std::string name) : name_(std::move(name))
  {

  }

  const std::string &Model::get_name() const {
    return name_;
  }
}