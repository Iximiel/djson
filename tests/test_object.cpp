//(c) 2025 Daniele Rapetti
// This code is licensed under MIT license (see LICENSE for details)

#include "djson/json.hpp"
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE ("Set Number", "[crude]") {
  djson::Object x;
  x["Number"] = djson::Number (1);
  REQUIRE (std::holds_alternative<djson::Number> (x["Number"]));
  REQUIRE (std::get<djson::Number> (x["Number"]) == Catch::Approx (1));
}

TEST_CASE ("Set String", "[crude]") {
  djson::Object x;
  x["string"] = "hello";
  REQUIRE (std::holds_alternative<djson::String> (x["string"]));
  REQUIRE (std::get<djson::String> (x["string"]) == "hello");
}

TEST_CASE ("Set Boolean", "[crude]") {
  djson::Object x;
  x["Bool"] = true;
  REQUIRE (std::holds_alternative<djson::Boolean> (x["Bool"]));
  REQUIRE (std::get<djson::Boolean> (x["Bool"]) == true);
}

TEST_CASE ("Set Null", "[crude]") {
  djson::Object x;
  x["Null"] = djson::Null{};
  REQUIRE (std::holds_alternative<djson::Null> (x["Null"]));
  REQUIRE (std::get<djson::Null> (x["Null"]) == djson::Null{});
}

TEST_CASE ("Set Object", "[crude]") {
  djson::Object x;
  djson::Object inside;
  inside["Bool"] = true;
  inside["string"] = "hello";
  x["inside"] = inside;
  REQUIRE (std::holds_alternative<djson::Object> (x["inside"]));
  REQUIRE (std::get<djson::Object> (x["inside"]) == inside);
}

TEST_CASE ("Set Array", "[crude]") {
  djson::Object x;
  djson::Array arr;
  arr.emplace_back ("hello");
  arr.emplace_back (djson::Null{});
  arr.emplace_back (djson::Array{});
  x["Array"] = arr;
  REQUIRE (std::holds_alternative<djson::Array> (x["Array"]));
  REQUIRE (std::get<djson::Array> (x["Array"]) == arr);
}

TEST_CASE ("Test Equality", "[crude]") {
  djson::Object inside;
  inside["Bool"] = true;
  inside["string"] = "hello";
  djson::Object x;
  x["inside"] = inside;
  x["Number"] = djson::Number (1);
  djson::Object y;
  y["inside"] = inside;
  y["Number"] = djson::Number (1);
  REQUIRE (x == y);
}

TEST_CASE ("Test Inequality", "[crude]") {
  djson::Object inside;
  inside["Bool"] = true;
  inside["string"] = "hello";
  djson::Object x;
  x["inside"] = inside;
  x["Number"] = djson::Number (1);
  djson::Object y;
  y["Number"] = djson::Number (1);
  y["inside"] = inside;
  REQUIRE (x != y);
}

TEST_CASE ("Set with get", "[crude]") {
  djson::Object obj;

  obj["num"] = djson::Number (1);
  REQUIRE (std::get<djson::Number> (obj["num"]) == Catch::Approx (1));
  obj.get<djson::Number> ("num") = djson::Number (2);
  REQUIRE (std::get<djson::Number> (obj["num"]) == Catch::Approx (2));
}