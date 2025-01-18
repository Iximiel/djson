//(c) 2025 Daniele Rapetti
// This code is licensed under MIT license (see LICENSE for details)

#ifndef DJSON_JSON
#define DJSON_JSON
#include <map>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace djson {
  using Number = double;
  using String = std::string;
  using Boolean = bool;
  struct Null {
    bool operator== (const Null &) const { return true; }
    bool operator!= (const Null &) const { return false; }
  };
  struct Array;
  class Object;
  using Node = std::variant<Number, String, Boolean, Null, Array, Object>;
  // I do not 100% like this
  struct Array : public std::vector<Node> {};

  class Object {
    std::vector<std::string> orderedKeys{};
    std::map<std::string, Node, std::less<>> objects{};

  public:
    Node &operator[] (const std::string &key);
    Node &at (const std::string &key);
    bool operator== (const Object &other) const;
    bool operator!= (const Object &other) const;
  };
  //   void operator= (std::ostream &os, const Node &node);

} // namespace djson
#endif // DJSON_JSON
