// (c) 2025 Daniele Rapetti
// This code is licensed under MIT license (see LICENSE for details)

#include "utils.hpp"
namespace djson {

  namespace utils {
    bool bracketsValidator::open (char o) {
      opened.push_back (o);
      return (o == '{' || o == '[');
    }
    bool bracketsValidator::close (char c) {
      if (opened.empty ()) {
        return false;
      }
      char o = opened.back ();
      opened.pop_back ();
      switch (o) {
      case '{':
        return c == '}';
        break;
      case '[':
        return c == ']';
        break;
      default:
        return false;
      }
    }
    bool bracketsValidator::is_empty () { return opened.empty (); }
  } // namespace utils
} // namespace djson