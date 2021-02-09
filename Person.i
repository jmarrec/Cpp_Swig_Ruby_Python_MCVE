#ifndef PERSON_I
#define PERSON_I

%module person

%include <stl.i>
%include <std_string.i>

%{
  #include <sstream>
  #include <Person.hpp>
%}

%include <Person.hpp>

%extend Test::Person {
  // Use the overloaded operator<< for string representation
  std::string __str__() {
    std::ostringstream os;
    os << *$self;
    return os.str();
  }
};

#endif //PERSON_I

