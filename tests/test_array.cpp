//(c) 2025 Daniele Rapetti
// This code is licensed under MIT license (see LICENSE for details)

#include "djson/json.hpp"
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE ("Array Set Numbers", "[Array][crude]") {
  djson::Array x;
  x.push_back (djson::Number (1));
  x.push_back (djson::Number (15));
  REQUIRE (std::holds_alternative<djson::Number> (x[0]));
  REQUIRE (std::get<djson::Number> (x[0]) == Catch::Approx (1));
  REQUIRE (std::holds_alternative<djson::Number> (x[1]));
  REQUIRE (std::get<djson::Number> (x[1]) == Catch::Approx (15));
}

TEST_CASE ("Set with get array", "[crude]") {
  djson::Array obj ({djson::Number (1), djson::Number (2), djson::Number (3)});

  REQUIRE (std::get<djson::Number> (obj[0]) == Catch::Approx (1));
  obj.get<djson::Number> (0) = djson::Number (2);
  REQUIRE (std::get<djson::Number> (obj[0]) == Catch::Approx (2));
}