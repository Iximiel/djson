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
  struct Null {};
  struct Array;
  class Object;
  using Node = std::variant<Number, String, Boolean, Null, Array, Object>;
  struct Array {
    std::vector<Node> list;
  };
  class Object {
    std::vector<std::string> orderedKeys{};
    std::map<std::string, Node, std::less<>> objects{};

  public:
    Node &operator[] (const std::string &key);
    Node &at (const std::string &key);
  };
  //   void operator= (std::ostream &os, const Node &node);

} // namespace djson
#endif // DJSON_JSON
