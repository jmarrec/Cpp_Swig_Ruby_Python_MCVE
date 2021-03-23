#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>

namespace Test {
class Model
{
 public:
  explicit Model(std::string name);
  const std::string& get_name() const;

 private:
  std::string name_;
};
}  // namespace Test

#endif
