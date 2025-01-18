//(c) 2025 Daniele Rapetti
// This code is licensed under MIT license (see LICENSE for details)
#include "json_read.hpp"
#include "utils.hpp"
#include <cctype>
#include <iostream>
#include <regex>

namespace djson {
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

  using utils::bracketsValidator;

  char spaceEater (std::istream &in) {
    char c;
    do {
      c = in.get ();
    } while (std::isspace (static_cast<unsigned char> (c)));
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

  std::optional<Object> read (std::istream &in) {
    if (auto myobj = internal::readObject (in); myobj.has_value ()) {
      return std::move (*myobj);
    } else {
      return std::nullopt;
    }
  }
  namespace internal {

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
    std::optional<Object> readObject (std::istream &in) {
      Object obj;
      bracketsValidator validator;

      if (char c = spaceEater (in); c != '{') {
        // cannot opend the parentesis
        std::cerr << "cannot open the parentesis" << std::endl;
        return std::nullopt;
      } else {
        validator.open (c);
      }
      while (true) {
        // now c should be the first " of a key
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
        std::string rawValue;
        auto redo = in.tellg ();
        // gets until the comma
        /// @todo: please no comma in the value
        std::getline (in, rawValue, ',');
        in.unget ();
        if (char c = in.peek (); c != ',') {
          // no comma found, go back and go for the parentesis
          in.seekg (redo);
          std::getline (in, rawValue, '}');
          in.unget ();
          if (char c = in.peek (); c != '}') {
            std::cerr << "cannot find the \"}\" after value " << rawValue
                      << std::endl;
            return std::nullopt;
          }
        }
        trim (rawValue);
        if (std::regex_match (rawValue, valueRegex)) {
          if (auto node = strtonode (rawValue); node.has_value ()) {
            obj[key] = std::move (*node);
          } else {
            std::cerr << "cannot read value \"" << rawValue << "\" for key "
                      << key << std::endl;
            return std::nullopt;
          }

        } else {
          // it could be an array or another object
          // go back to when weran gerline
          in.seekg (redo);
          char c = spaceEater (in);
          in.unget ();
          if (c == '[') {
            if (auto myarray = internal::readArray (in); myarray.has_value ()) {
              obj[key] = std::move (*myarray);
            } else {
              std::cerr << "cannot read array for key " << key << std::endl;
              return std::nullopt;
            }
          } else if (c == '{') {
            if (auto myobj = internal::readObject (in); myobj.has_value ()) {
              obj[key] = std::move (*myobj);
            } else {
              std::cerr << "cannot read Object for key " << key << std::endl;
              return std::nullopt;
            }
          }
        }
        // now we get again until the next comma or the end of the object
        if (char c = spaceEater (in); c == '}') {
          validator.close (c);
          return obj;
        } else if (c != ',') {
          std::cerr << "cannot find the end of the object" << std::endl;
          return std::nullopt;
        }
      }
    }

    std::optional<Array> readArray (std::istream &in) {
      Array arr;

      bracketsValidator validator;

      if (char c = spaceEater (in); c != '[') {
        // cannot open the parentesis
        std::cerr << "cannot open the parentesis" << std::endl;
        return std::nullopt;
      } else {
        validator.open (c);
      }
      while (true) {
        std::string rawValue;
        auto redo = in.tellg ();
        // gets until the comma
        /// @todo: please no comma in the value
        std::getline (in, rawValue, ',');
        in.unget ();
        if (char c = in.peek (); c != ',') {
          // no comma found, go back and go for the parentesis
          in.seekg (redo);
          std::getline (in, rawValue, ']');
          in.unget ();
          if (char c = in.peek (); c != ']') {
            std::cerr << "cannot find the \"}\" after value " << rawValue
                      << std::endl;
            return std::nullopt;
          }
        }
        trim (rawValue);
        if (std::regex_match (rawValue, valueRegex)) {
          if (auto node = strtonode (rawValue); node.has_value ()) {
            arr.emplace_back (std::move (*node));
          } else {
            std::cerr << "cannot read value \"" << rawValue << "\""
                      << std::endl;
            return std::nullopt;
          }
        } else {
          // it could be another array or an object
          // go back to when we ran gerline
          in.seekg (redo);
          char c = spaceEater (in);
          in.unget ();
          if (c == '[') {
            if (auto myarray = internal::readArray (in); myarray.has_value ()) {
              arr.emplace_back (std::move (*myarray));
            } else {
              std::cerr << "cannot read array " << std::endl;
              return std::nullopt;
            }
          } else if (c == '{') {
            if (auto myobj = internal::readObject (in); myobj.has_value ()) {
              arr.emplace_back (std::move (*myobj));
            } else {
              std::cerr << "cannot read Object" << std::endl;
              return std::nullopt;
            }
          }
        }
        // now we get again until the next comma or the end of the object
        if (char c = spaceEater (in); c == ']') {
          validator.close (c);
          return arr;
        } else if (c != ',') {
          std::cerr << "cannot find the end of the array" << std::endl;
          return std::nullopt;
        }
      }
    }
  } // namespace internal
} // namespace djson