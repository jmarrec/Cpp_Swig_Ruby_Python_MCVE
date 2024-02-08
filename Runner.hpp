#ifndef TEST_SWIG_RUNNER_HPP
#define TEST_SWIG_RUNNER_HPP

namespace Test {
class Model;

class Runner
{
 public:
  virtual ~Runner() = default;
  Runner &operator=(Runner &&) = delete;
  Runner &operator=(const Runner &) = delete;
  Runner(Runner &&) = delete;
  Runner(const Runner &) = delete;
  Runner() = default;

  Model &get_current_model() { return get_current_model_impl(); }

 protected:

  virtual Model& get_current_model_impl() = 0;

};
}  // namespace Test

#endif  //TEST_SWIG_RUNNER_HPP
