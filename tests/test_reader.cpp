//(c) 2025 Daniele Rapetti
// This code is licensed under MIT license (see LICENSE for details)
#include "djson/json_read.hpp"

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <sstream>

TEST_CASE ("Reading an Object", "[read]") {
  std::stringstream ss ("\t\r\n "
                        R"(  {"num":1, "bool":false ,"string"
    :"hello",
    "thenull":
    null,
    "actuallyastring":"true" }
    )");
  auto x = djson::read (ss);
  REQUIRE (x.has_value ());
  REQUIRE (std::get<djson::Number> (x->at ("num")) == Catch::Approx (1));
  REQUIRE (std::get<djson::Boolean> (x->at ("bool")) == false);
  REQUIRE (std::get<djson::String> (x->at ("string")) == "hello");
  REQUIRE (std::get<djson::Null> (x->at ("thenull")) == djson::Null{});
  REQUIRE (std::get<djson::String> (x->at ("actuallyastring")) == "true");
}

TEST_CASE ("Reading an Object with spaces", "[read]") {
  std::stringstream ss (
    R"(  {"key with spaces" : "yes",
    "str with spaces" : "a b c "
    }
    )");
  auto x = djson::read (ss);
  REQUIRE (x.has_value ());

  REQUIRE (std::get<djson::String> (x->at ("key with spaces")) == "yes");
  REQUIRE (std::get<djson::String> (x->at ("str with spaces")) == "a b c ");
}

TEST_CASE ("Reading an Object within an object", "[read]") {
  std::stringstream ss (
    "\t\r\n "
    R"(  {"num":1,"bool":false,"string":"hello","thenull":null,"obj":{"num":1,"bool":false,"string":"hello","thenull":null}})");
  auto x = djson::read (ss);
  REQUIRE (x.has_value ());
  REQUIRE (std::get<djson::Number> (x->at ("num")) == Catch::Approx (1));
  REQUIRE (std::get<djson::Boolean> (x->at ("bool")) == false);
  REQUIRE (std::get<djson::String> (x->at ("string")) == "hello");
  REQUIRE (std::get<djson::Null> (x->at ("thenull")) == djson::Null{});

  auto &obj = std::get<djson::Object> (x->at ("obj"));
  REQUIRE (std::get<djson::Number> (obj.at ("num")) == Catch::Approx (1));
  REQUIRE (std::get<djson::Boolean> (obj.at ("bool")) == false);
  REQUIRE (std::get<djson::String> (obj.at ("string")) == "hello");
  REQUIRE (std::get<djson::Null> (obj.at ("thenull")) == djson::Null{});
}

TEST_CASE ("Reading an Array within an object", "[read]") {
  std::stringstream ss (

    R"(  {"arr":[false, "ciao" ,
    null ,3, {
        "in":"side"
        }]})");
  auto x = djson::read (ss);
  REQUIRE (x.has_value ());
  auto &arr = std::get<djson::Array> (x->at ("arr"));
  REQUIRE (std::get<djson::Boolean> (arr[0]) == false);
  REQUIRE (std::get<djson::String> (arr[1]) == "ciao");
  REQUIRE (std::get<djson::Null> (arr[2]) == djson::Null{});
  REQUIRE (std::get<djson::Number> (arr[3]) == Catch::Approx (3));
  REQUIRE (
    std::get<djson::String> (std::get<djson::Object> (arr[4]).at ("in")) ==
    "side");
}