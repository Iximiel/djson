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