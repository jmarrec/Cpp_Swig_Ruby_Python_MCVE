#include "Person.hpp"
#include "Time.hpp"
#include <iostream>

int main() {
  test::Person p("John Doe");
  std::cout << p << '\n';

  test::Time t;
  std::cout << "Time: " << t.getTime() << '\n';
}
