#include "Measure.hpp"

namespace Test {
std::vector<std::string> Measure::arguments(const Model& model) const {
  return arguments_impl(model);
}
bool Measure::run(Runner& runner, const std::vector<std::string>& args) {
  return run_impl(runner, args);
}
}  // namespace Test