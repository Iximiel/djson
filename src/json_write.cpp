//(c) 2025 Daniele Rapetti
// This code is licensed under MIT license (see LICENSE for details)
#include "json_write.hpp"
#include <ostream>
namespace djson {

  struct json_write {
    // I know I could have declared a bung of << operator but this is funnier
    std::ostream &out;
    const std::string indent;
    void operator() (const Object &node) {
      if (node.Keys ().empty ()) {
        out << "{}";
        return;
      }
      out << "{" << "\n";
      bool first = true;
      for (const auto &key : node.Keys ()) {
        if (first) {
          first = false;
        } else {
          out << "," << "\n";
        }
        out << indent + "  " << "\"" << key << "\" : ";
        std::visit (json_write (out, indent + "  "), node.at (key));
      }
      out << "\n" << indent << "}";
    }

    void operator() (const Array &node) {
      if (node.empty ()) {
        out << "[]";
        return;
      }
      out << "[" << "\n";
      bool first = true;
      for (const auto &val : node) {
        if (first) {
          first = false;
        } else {
          out << "," << "\n";
        }
        out << indent + "  ";
        std::visit (json_write (out, indent + "  "), val);
      }
      out << "\n" << indent << "]";
    }

    void operator() (const Number &node) { out << node; }
    void operator() (const String &node) { out << "\"" << node << "\""; }
    void operator() (const Boolean &node) { out << (node ? "true" : "false"); }
    void operator() (const Null &node) { out << "null"; }
  };

  auto write (std::ostream &out, const Object &node, const std::string &indent)
    -> std::ostream & {
    json_write (out, indent) (node);
    return out;
  }
} // namespace djson
