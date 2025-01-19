//(c) 2025 Daniele Rapetti
// This code is licensed under MIT license (see LICENSE for details)
#ifndef DJSON_UTILS
#define DJSON_UTILS
#include <string>
namespace djson {
  namespace utils {
    class bracketsValidator {
      std::string opened;

    public:
      bool open (char o);
      bool close (char c);
      bool is_empty ();
    };
  } // namespace utils
} // namespace djson
#endif // DJSON_UTILS
