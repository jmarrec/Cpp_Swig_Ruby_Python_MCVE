#ifndef MEASURE_HPP
#define MEASURE_HPP

#include <string>
#include <string_view>
#include <iostream>
#include <vector>

namespace Test {
class Runner;

class Model;

class Measure
{
 public:
  Measure() = default;
  Measure(const Measure&) = delete;
  Measure(Measure&&) = delete;
  Measure& operator=(const Measure&) = delete;
  Measure& operator=(Measure&&) = delete;
  virtual ~Measure() = default;

  std::vector<std::string> arguments(const Model&) const;
  bool run(Runner&, const std::vector<std::string>& );

 protected:
  // protected virtual here allows us to easily change the internal interface
  // and behavior of measure
  virtual std::vector<std::string> arguments_impl(const Model&) const = 0;
  virtual bool run_impl(Runner&, const std::vector<std::string>& ) = 0;
};


}  // Namespace Test

#endif  // ifndef MEASURE_HPP
