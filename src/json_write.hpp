//(c) 2025 Daniele Rapetti
// This code is licensed under MIT license (see LICENSE for details)
#ifndef DJSON_JSON_WRITE
#define DJSON_JSON_WRITE
#include "json.hpp"
#include <iosfwd>
#include <string>
namespace djson {
  auto
  write (std::ostream &out, const Object &node, const std::string &indent = "")
    -> std::ostream &;

} // namespace djson

#endif // DJSON_JSON