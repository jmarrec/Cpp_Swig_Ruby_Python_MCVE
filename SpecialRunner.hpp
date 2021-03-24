#ifndef TEST_SWIG_SPECIALRUNNER_HPP
#define TEST_SWIG_SPECIALRUNNER_HPP

#include "Runner.hpp"
#include "Model.hpp"

namespace Test {
// a concrete implementation of the Runner class, for testing purposes
class SpecialRunner : public Runner
{
 public:
  SpecialRunner(Model model) : model_(std::move(model)) {}

 protected:
  Model& get_current_model_impl() override {
    return model_;
  }

 private:
  Model model_;
};
}  // namespace Test

#endif  //TEST_SWIG_SPECIALRUNNER_HPP
