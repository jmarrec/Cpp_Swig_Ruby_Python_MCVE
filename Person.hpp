#ifndef EXAMPLE_HPP
#define EXAMPLE_HPP

#include <string>
#include <string_view>
#include <iostream>

namespace Test {

class Person {
  public:

    Person(const std::string& name) noexcept;

    Person(const Person& other);

    std::string getName() const;
    bool setName(const std::string& t_newName);

  private:
    std::string m_name;
};

std::ostream& operator<<(std::ostream&, const Test::Person&);

// A free-standing function taking an object as argument
std::string personName(const Person& person);
inline void setName(Person &p, const std::string &newname) {
  p.setName(newname);
}

} // Namespace Test

#endif // ifndef PERSON_HPP
