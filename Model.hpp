#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include <vector>

namespace Test {
class Model
{
 public:
  explicit Model(std::string name);
  const std::string& getName() const;

  void pushOp(const std::string& op_name);

  const std::vector<std::string>& opsPerformed() const;

 private:
  std::string name_;
  std::vector<std::string> opsPerformed_;
};
}  // namespace Test

#endif
