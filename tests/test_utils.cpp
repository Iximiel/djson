//(c) 2025 Daniele Rapetti
// This code is licensed under MIT license (see LICENSE for details)

#include "utils.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

TEST_CASE ("Brackets Validator with correct input", "[utils]") {
  std::string s = GENERATE (
    "{}", "[]", "{}{}", "[][]", "{}[]", "[]{}", "{{{{[{[{}]}{}]}}}}",
    "{[{}{}]}", R"({"asd":[1,2,3],{"temp":1.3,"hum":40.0}})");
  djson::utils::bracketsValidator v;
  for (char c : s) {
    switch (c) {
    case '{':
      [[fallthrough]];
    case '[':
      REQUIRE (v.open (c));
      /* code */
      break;
    case '}':
      [[fallthrough]];
    case ']':
      REQUIRE (v.close (c));
    default:
      break;
    }
  }
  REQUIRE (v.is_empty ());
}

TEST_CASE ("Brackets Validator with incorrect input", "[utils]") {
  std::string s = GENERATE (
    "{]", "[}", "{]}", "[]]", R"({]{"asd":[1,2,3],{"temp":1.3,"hum":40.0}})");
  djson::utils::bracketsValidator v;
  bool correct = true;
  for (char c : s) {
    switch (c) {
    case '{':
      [[fallthrough]];
    case '[':
      correct &= v.open (c);
      /* code */
      break;
    case '}':
      [[fallthrough]];
    case ']':
      correct &= v.close (c);
    default:
      break;
    }
  }
  REQUIRE (correct == false);
}

TEST_CASE ("Brackets Validator with odd input", "[utils]") {
  std::string s =
    GENERATE ("{[}", "[[]", R"({{"asd":[1,2,3],{"temp":1.3,"hum":40.0}})");
  djson::utils::bracketsValidator v;
  bool correct = true;
  for (char c : s) {
    switch (c) {
    case '{':
      [[fallthrough]];
    case '[':
      correct &= v.open (c);
      /* code */
      break;
    case '}':
      [[fallthrough]];
    case ']':
      correct &= v.close (c);
    default:
      break;
    }
  }
  REQUIRE (v.is_empty () == false);
}