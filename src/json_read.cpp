//(c) 2025 Daniele Rapetti
// This code is licensed under MIT license (see LICENSE for details)
#include "json_read.hpp"
#include "utils.hpp"
#include <iostream>

namespace djson {
  using utils::bracketsValidator;

  std::optional<Object> read (std::istream &in) {
    // read whitespace until first '{'
    char c;
    while ((c = in.get ()) == ' ') {
    };
    if (c != '{') {
      return std::nullopt;
    }

    return internal::readObject (in);
  }
  namespace internal {

    std::optional<Object> readObject (std::istream &in) {
      return Object{};
    } // read
  } // namespace internal
} // namespace djson