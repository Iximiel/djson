//(c) 2025 Daniele Rapetti
// This code is licensed under MIT license (see LICENSE for details)

#include "json.hpp"
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE ("Set Number", "[crude]") {
  djson::Object x;
  x["Number"] = djson::Number (1);
  REQUIRE (std::holds_alternative<djson::Number> (x["Number"]));
  REQUIRE (std::get<djson::Number> (x["Number"]) == Catch::Approx (1));
}
