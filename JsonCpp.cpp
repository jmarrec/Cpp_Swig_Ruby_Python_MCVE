#include "JsonCpp.hpp"
#include <json/json.h>

namespace test::testJSON {

Json::Value makeDumbJSON() {

  Json::Value result;

  result["Version"]["Version 1"]["version_identifier"] = "9.5";
  result["Building"]["Building 1"]["name"] = "My Building";
  result["Building"]["Building 1"]["north_axis"] = -30.5;

  return result;
}

std::string makeDumbJSONString() {
  return makeDumbJSON().toStyledString();
}


} // namespace test::testJSON
