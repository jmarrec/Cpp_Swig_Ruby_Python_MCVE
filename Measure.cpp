#include "Measure.hpp"

namespace Test {
bool Measure::run(Runner& runner) {
  return run_impl(runner);
}
}  // namespace Test