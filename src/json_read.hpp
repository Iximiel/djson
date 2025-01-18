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
  namespace internal {

    std::optional<Object> readObject (std::istream &in);
    std::optional<Array> readArray (std::istream &in);
    std::optional<String> readString (std::istream &in);
    std::optional<Number> readNumber (std::istream &in);
    std::optional<Boolean> readBoolean (std::istream &in);
    std::optional<Null> readNull (std::istream &in);
  } // namespace internal
} // namespace djson
#endif // DJSON_JSON