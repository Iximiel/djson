//(c) 2025 Daniele Rapetti
// This code is licensed under MIT license (see LICENSE for details)
#ifndef DJSON_JSON_READ
#define DJSON_JSON_READ
#include "json.hpp"

#include <iosfwd>
#include <optional>
namespace djson {
  // Here I am showing how I am bad at designing patterns

  std::optional<Object> read (std::istream &in);

} // namespace djson
#endif // DJSON_JSON