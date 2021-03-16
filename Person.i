#ifndef PERSON_I
#define PERSON_I

%module mylib

%include <stl.i>
%include <std_string.i>
%include <CommonImport.i>

#ifdef SWIGCSHARP
  // Help in overload resolution
  %ignore test::toString(const char*);
  %ignore test::toString(const std::wstring&);
  %ignore test::toString(const wchar_t*);
  %ignore test::toString(std::istream&);
  %ignore test::toWString(const std::string&);
#endif

%{
  #include <cwchar>
  #include <sstream>
  #include <string>
  #include <Person.hpp>
  #include <String.hpp>
%}

%ignore test::Person::illuminanceMapMaxValue(std::string const &, double&, double&) const;

%include <Person.hpp>
%include <String.hpp>

%extend test::Person {
  // Use the overloaded operator<< for string representation
  std::string __str__() {
    std::ostringstream os;
    os << *$self;
    return os.str();
  }
  #ifdef SWIGRUBY
    // get an integral representation of the pointer that is this Person
    inline long long __toInt() {
      std::clog << "original pointer: " << $self << '\n';
      const auto result = reinterpret_cast<long long>($self);
      std::clog << "toInt from C++ " << result << '\n';
      return result;
    }


  #endif

  #ifdef SWIGPYTHON
    // take the integer from toInt and reinterpret_cast it back into a Person *, then return that as a reference
    static inline test::Person& _fromInt(long long i) {
      auto *ptr = reinterpret_cast<test::Person *>(i);
      std::clog << "Reclaimed pointer: " << ptr << '\n';
      return *ptr;
    }
  #endif
};

#endif //PERSON_I

