#include "Person.hpp"

#include <iostream>

namespace Test {

Person::Person(const std::string& t_name) noexcept
  : m_name(t_name) { }

Person::Person(const Person& other)
  : m_name(other.m_name) { }


std::string Person::getName() const {
  return m_name;
}

bool Person::setName(const std::string& t_newName) {
  m_name = t_newName;
  return true;
}

void Person::illuminanceMapMaxValue(const std::string& name, double& minValue, double& maxValue) const {
  if (name.empty()) {
    minValue = 0.0;
    maxValue = 10.0;
  } else {
    minValue = 1.0;
    maxValue = 9.0;
  }
}

//void Person::trySwig(int& i) {
  //i = 10;
//}



std::ostream& operator<<(std::ostream& os, const Test::Person& p) {
  os << "Person named '" << p.getName() << "'";
  return os;
}

std::string personName(const Person& person) {
  return person.getName();
}

} // namespace Test
