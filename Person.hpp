#ifndef EXAMPLE_HPP
#define EXAMPLE_HPP

#include <string>
#include <string_view>

namespace Test {

class Person {
  public:

    Person(const std::string& name) noexcept;

    std::string getName() const;
    bool setName(const std::string& t_newName);
  private:
    std::string m_name;
};

std::ostream& operator<<(std::ostream&, const Test::Person&);

// A free-standing function taking an object as argument
std::string personName(const Person& person);

} // Namespace Test

#endif // ifndef PERSON_HPP
