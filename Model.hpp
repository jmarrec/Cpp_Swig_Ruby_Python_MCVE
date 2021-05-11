#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include <vector>
#include "ModelObject.hpp"

namespace Test {
  class Model
  {
    public:
      explicit Model(std::string name);
      const std::string& getName() const;
      bool setName(const std::string& name);

      void pushOp(const std::string& op_name);

      const std::vector<std::string>& opsPerformed() const;

      int numObjects() const;
      std::vector<std::string> objectNames() const;
      ModelObject& getObject(size_t index);
      void pushObject(const std::string& objName);
    private:
      std::string name_;
      std::vector<std::string> opsPerformed_;
      std::vector<ModelObject> m_objects;
  };
}  // namespace Test

#endif
