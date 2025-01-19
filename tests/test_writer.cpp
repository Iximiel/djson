//(c) 2025 Daniele Rapetti
// This code is licensed under MIT license (see LICENSE for details)

#include "json_write.hpp"

#include "catch2/catch_test_macros.hpp"
#include <sstream>

TEST_CASE ("Writing an Object", "[write]") {
  djson::Object x;
  x["num"] = djson::Number (1);
  x["bool"] = djson::Boolean (false);
  x["string"] = djson::String ("hello");
  x["thenull"] = djson::Null{};
  x["actuallyastring"] = djson::String ("true");
  std::stringstream ss;
  djson::write (ss, x);
  REQUIRE (
    ss.str () ==
    R"({
  "num" : 1,
  "bool" : false,
  "string" : "hello",
  "thenull" : null,
  "actuallyastring" : "true"
})");
}

TEST_CASE ("Writing A nested object an Array", "[write]") {
  djson::Object x;
  x["string"] = djson::String ("hello");
  x["thenull"] = djson::Null{};
  x["actuallyastring"] = djson::String ("true");

  djson::Array arr;
  arr.emplace_back (djson::String ("hello"));
  arr.emplace_back (djson::Object ());
  std::get<djson::Object> (arr[1])["data"] = djson::String ("str");
  std::get<djson::Object> (arr[1])["sum"] = djson::Number (15);
  x["array"] = arr;

  std::stringstream ss;
  djson::write (ss, x);
  REQUIRE (
    ss.str () ==
    R"({
  "string" : "hello",
  "thenull" : null,
  "actuallyastring" : "true",
  "array" : [
    "hello",
    {
      "data" : "str",
      "sum" : 15
    }
  ]
})");
}

TEST_CASE ("Write an empty object", "[write]") {
  djson::Object x;
  std::stringstream ss;
  djson::write (ss, x);
  REQUIRE (ss.str () == "{}");
}

TEST_CASE ("Write empty sub objects", "[write]") {
  djson::Object x;
  x["array"] = djson::Array{};
  x["obj"] = djson::Object{};
  std::stringstream ss;
  djson::write (ss, x);
  REQUIRE (ss.str () == "{\n  \"array\" : [],\n  \"obj\" : {}\n}");
}