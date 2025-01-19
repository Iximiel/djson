//(c) 2025 Daniele Rapetti
// This code is licensed under MIT license (see LICENSE for details)
#include "json_read.hpp"
#include <cctype>
#include <iostream>
#include <regex>

namespace djson {
  // I did a lot of metaprogramming here, I do not wanted to write the same
  // parser (for Array and Object) twice

  // this do not catch arrays
  //  (if fails is codeium fault)
#define nullstr "null"
#define truestr "true"
#define falsestr "false"
#define boolstr truestr "|" falsestr
#define numberstr "[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)"
#define stringstr "\"[^\"]*\""

  auto nullRegex = std::regex (nullstr);
  auto stringRegex = std::regex (stringstr);
  auto falseRegex = std::regex (falsestr);
  auto trueRegex = std::regex (truestr);
  auto boolRegex = std::regex (boolstr);
  auto numberRegex = std::regex (numberstr);
  auto valueRegex =
    std::regex (nullstr "|" boolstr "|" numberstr "|" stringstr);

  namespace internal {
    template <typename T>
    std::optional<T> reader (std::istream &in);

  }

  std::optional<Object> read (std::istream &in) {
    return internal::reader<Object> (in);
  }

  namespace internal {

    char spaceEater (std::istream &in) {
      char c;
      do {
        c = static_cast<char> (in.get ());
      } while (std::isspace (static_cast<unsigned char> (c)) || c == '\n');
      return c;
    }

    void ltrim (std::string &s) {
      s.erase (
        s.begin (), std::find_if (s.begin (), s.end (), [] (unsigned char ch) {
          return !std::isspace (ch);
        }));
    }

    void rtrim (std::string &s) {
      s.erase (
        std::find_if (
          s.rbegin (), s.rend (),
          [] (unsigned char ch) { return !std::isspace (ch); })
          .base (),
        s.end ());
    }

    void trim (std::string &s) {
      ltrim (s);
      rtrim (s);
    }

    std::optional<Node> strtonode (std::string rawValue) {
      if (std::regex_match (rawValue, numberRegex)) {
        return std::stod (rawValue);
      } else if (std::regex_match (rawValue, boolRegex)) {
        // return std::regex_match (rawValue, trueRegex)  is not ok:
        // beacause there can be a typo on the false
        if (std::regex_match (rawValue, trueRegex)) {
          return true;
        } else if (std::regex_match (rawValue, falseRegex)) {
          return false;
        } else {
          return std::nullopt;
        }
      } else if (std::smatch words;
                 std::regex_match (rawValue, words, stringRegex)) {
        auto t = words[0].str ();
        return t.substr (1, t.size () - 2);
      } else if (std::regex_match (rawValue, nullRegex)) {
        return Null{};
      } else {
        return std::nullopt;
      }
    }

    std::optional<std::string> getkey (std::istream &in) {
      if (char c = spaceEater (in); c != '"') {
        std::cerr << "key not found" << std::endl;
        return std::nullopt;
      }
      // look for the fist key:, a key can have spaces
      std::string key;
      std::getline (in, key, '"');
      // looking for the :
      if (char c = spaceEater (in); c != ':') {
        std::cerr << "cannot not found the \":\" after key " << key
                  << std::endl;
        return std::nullopt;
      }
      return key;
    }
    template <typename T>
    class assigner {
      assigner (T &recipient, std::istream &in) {}
      template <typename U>
      void operator() (U &&val) {}
      bool check () { return true; }
      std::string extraError () { return ""; }
    };

    template <>
    class assigner<Object> {
      Object &recipient;
      std::string key;
      bool ready = false;

    public:
      assigner (Object &container, std::istream &in) : recipient (container) {
        auto k = getkey (in);
        if (!k.has_value ()) {
          std::cerr << "key not found" << std::endl;
          return;
        }
        ready = true;
        key = *k;
      }
      template <typename T>
      void operator() (T &&val) {
        recipient[key] = std::move (val);
      }
      bool check () { return ready; }
      std::string extraError () { return " for key " + key; }
    };

    template <>
    class assigner<Array> {
      Array &recipient;

    public:
      assigner (Array &container, std::istream &) : recipient (container) {}
      template <typename T>
      void operator() (T &&val) {
        recipient.emplace_back (std::move (val));
      }
      constexpr bool check () { return true; }
      std::string extraError () { return ""; }
    };

    template <typename T>
    constexpr char open = '*';
    template <typename T>
    constexpr char close = '*';
    template <>
    constexpr char open<Object> = '{';
    template <>
    constexpr char close<Object> = '}';
    template <>
    constexpr char open<Array> = '[';
    template <>
    constexpr char close<Array> = ']';

    template <typename T>
    std::optional<T> reader (std::istream &in) {
      T obj;
      if (char c = spaceEater (in); c != open<T>) {
        std::cerr << "cannot open the parentesis" << std::endl;
        return std::nullopt;
      }
      while (true) {
        assigner<T> assigner (obj, in);
        if (!assigner.check ()) {
          std::cerr << "key not found" << std::endl;
          return std::nullopt;
        }

        std::string rawValue;
        auto redo = in.tellg ();
        {
          // gets until the comma or the end character
          // maybe I should look for something more faster
          char c = static_cast<char> (in.get ());
          while ((c != ',' && c != close<T>) && !in.eof ()) {
            rawValue += c;
            c = static_cast<char> (in.get ());
          }
          in.unget ();
        }
        // gets until the comma
        /// @todo: please no comma in the value
        trim (rawValue);
        if (std::regex_match (rawValue, valueRegex)) {
          if (auto node = strtonode (rawValue); node.has_value ()) {
            assigner (std::move (*node));
          } else {
            std::cerr << "cannot read value \"" << rawValue << "\""
                      << assigner.extraError () << std::endl;
            return std::nullopt;
          }
        } else {
          // it could be an array or an object
          // go back to when we ran getline
          in.seekg (redo);
          char c = spaceEater (in);
          in.unget ();
          if (c == '[') {
            if (auto myarray = internal::reader<Array> (in);
                myarray.has_value ()) {
              assigner (std::move (*myarray));
            } else {
              std::cerr << "cannot read Array" << assigner.extraError ()
                        << std::endl;
              return std::nullopt;
            }
          } else if (c == '{') {
            if (auto myobj = internal::reader<Object> (in);
                myobj.has_value ()) {
              assigner (std::move (*myobj));
            } else {
              std::cerr << "cannot read Object" << assigner.extraError ()
                        << std::endl;
              return std::nullopt;
            }
          }
        }
        // now we get again until the next comma or the end of the object
        if (char c = spaceEater (in); c == close<T>) {
          return obj;
        } else if (c != ',') {
          std::cerr << "cannot find the end of the object" << std::endl;
          return std::nullopt;
        }
      }
    }

  } // namespace internal
} // namespace djson