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
  const Node &Object::at (const std::string &key) const {
    return objects.at (key);
  }

  bool Object::operator== (const Object &other) const {
    // I am also cheking that the keys are in the same order
    if (orderedKeys == other.orderedKeys) {
      for (const auto &[key, value] : objects) {
        if (value != other.objects.at (key)) {
          return false;
        }
      }
      return true;
    }

    return false;
  }
  bool Object::operator!= (const Object &other) const {
    return !(*this == other);
  }
  const std::vector<std::string> &Object::Keys () const { return orderedKeys; }

} // namespace djson