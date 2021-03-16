#ifndef TESTJSON_HPP
#define TESTJSON_HPP

#include <string>

namespace Json {
class Value;
}

namespace test::testJSON {

Json::Value makeDumbJSON();

std::string makeDumbJSONString();

} // namespace test::testJSON

#endif // TESTJSON_HPP
