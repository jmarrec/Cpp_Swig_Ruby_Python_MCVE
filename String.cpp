#include "String.hpp"

#include <codecvt>
#include <iomanip>
#include <cmath>

namespace Test {

/** char* to std::string. */
std::string toString(const char* s) {
  return std::string(s);
}

/** string to std::string. */
std::string toString(const std::string& s) {
  return s;
}

/** wchar_t* to std::string. */
std::string toString(const wchar_t* w) {
  return toString(std::wstring(w));
}

std::string toString(double v) {

  std::string result;

  // Return Infinity or NaN as strings, getDouble in IdfObject will fail to convert to double which will cause setDouble to fail
  if (std::isinf(v)) {
    if (v < 0) {
      result = "-Infinity";
    } else {
      result = "Infinity";
    }
  } else if (std::isnan(v)) {
    result = "NaN";
  } else {

    std::stringstream ss;
    ss << std::setprecision(std::numeric_limits<double>::digits10) << v;
    result = ss.str();
  }

  return result;
}

std::string toString(std::istream& s) {
  // istream -> string code from
  // http://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
  std::string contents;
  s.seekg(0, std::ios::end);
  contents.resize(s.tellg());
  s.seekg(0, std::ios::beg);
  s.read(&contents[0], contents.size());
  return contents;
}

///
/// Note that MSVC has linker issues with wchars for some weird reason
/// Hence these ifdef workarounds https://stackoverflow.com/questions/32055357/visual-studio-c-2015-stdcodecvt-with-char16-t-or-char32-t
///

std::wstring toWString(const std::string& s) {
#if _MSC_VER >= 1900
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
  const auto u16_conv = convert.from_bytes(s);
  return {u16_conv.begin(), u16_conv.end()};
#else
  std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
  const auto u16_conv = convert.from_bytes(s);
  return {u16_conv.begin(), u16_conv.end()};
#endif
}

std::string toString(const std::wstring& utf16_string) {
#if _MSC_VER >= 1900
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
  return convert.to_bytes(utf16_string.data(), utf16_string.data() + utf16_string.size());
#else
  std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
  const std::u16string u16string{utf16_string.begin(), utf16_string.end()};
  return convert.to_bytes(u16string);
#endif
}

}  // namespace Test
