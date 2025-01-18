//(c) 2025 Daniele Rapetti
// This code is licensed under MIT license (see LICENSE for details)

#include "json.hpp"

namespace djson {
  Node &Object::operator[] (const std::string &key) {
    if (objects.find (key) == objects.end ()) {
      objects[key] = Node{};
      orderedKeys.emplace_back (key);
    }

    return objects[key];
  }
  Node &Object::at (const std::string &key) { return objects.at (key); }
} // namespace djson