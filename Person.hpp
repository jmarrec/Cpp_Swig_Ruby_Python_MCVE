#ifndef EXAMPLE_HPP
#define EXAMPLE_HPP

#include <string>
#include <string_view>
#include <iostream>
#include <ctime>

namespace test {

class Person {
  public:

    Person(const std::string& name) noexcept;

    Person(const Person& other);

    std::string getName() const;
    bool setName(const std::string& t_newName);

    void illuminanceMapMaxValue(const std::string& name, double& minValue, double& maxValue) const;

  private:
    std::string m_name;
};

std::ostream& operator<<(std::ostream&, const test::Person&);

// A free-standing function taking an object as argument
std::string personName(const Person& person);
inline void setName(Person &p, const std::string &newname) {
  p.setName(newname);
}

// get an integral representation of the pointer that is this Person
inline long long toInt(Person &p) {
  std::clog << "original pointer: " << &p << '\n';
  const auto result = reinterpret_cast<long long>(&p);
  std::clog << "toInt from C++ " << result << '\n';
  return result;
}

// take the integer from toInt and reinterpret_cast it back into a Person *, then return that as a reference
inline Person &fromInt(long long i) {
  auto *ptr = reinterpret_cast<Person *>(i);
  std::clog << "Reclaimed pointer: " << ptr << '\n';
  return *ptr;
}

} // Namespace test

#endif // ifndef PERSON_HPP
