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
  // As now the library does not support deleting things from the Objects, since
  // is more thinked as a layer between the program and the json files

  using Number = double;
  using String = std::string;
  using Boolean = bool;
  struct Null {
    bool operator== (const Null &) const { return true; }
    bool operator!= (const Null &) const { return false; }
  };
  struct Array;
  class Object;
  /// A Node may be a number, a string, a boolean, a null, an array or an object
  using Node = std::variant<Number, String, Boolean, Null, Array, Object>;
  // I do not 100% like this
  /// A ordered list enclosed by square brackets
  struct Array : public std::vector<Node> {};
  /// @brief A object enclosed by curly brackets (that contains named values)
  class Object {
    // I want to keep the order or the keys
    std::vector<std::string> orderedKeys{};
    // I do not want do write a find function
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
