//(c) 2024 Daniele Rapetti
// This code is licensed under MIT license (see LICENSE for details)

/* Plan
 - Number
 - String
 - Boolean
 - Array
 - Object
 - null
 */
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace json {
  using Number = double;
  using String = std::string;
  using Boolean = bool;
  struct Null {};
  struct Array;
  struct Object;
  using Node = std::variant<Number, String, Boolean, Null, Array, Object>;
  struct Array {
    std::vector<Node> list;
  };
  struct Object {
    std::vector<std::string> orederedkeys;
    std::map<std::string, Node> objects;
  };

} // namespace json