#ifndef UTILITIES_CORE_STRING_HPP
#define UTILITIES_CORE_STRING_HPP

#include <string>
#include <vector>
#include <cwchar>

/** \file String.hpp
 *
 *  All strings are assumed to be UTF-8 encoded std::string.  Note that length of the std::string
 *  may therefore not match number of characters in the std::string. */

namespace test {

/** string to std::string. */
std::string toString(const std::string& s);

/** char* to std::string. */
std::string toString(const char* s);

/** wstring to std::string. */
std::string toString(const std::wstring& utf16_string);

/** string to std::wstring. */
std::wstring toWString(const std::string& s);

/** wchar_t* to std::string. */
std::string toString(const wchar_t* w);

/** Double to std::string at full precision. */
std::string toString(double v);

/** Load data in istream into string. */
std::string toString(std::istream& s);

}  // namespace test

#endif  // UTILITIES_CORE_STRING_HPP
