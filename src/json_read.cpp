//(c) 2025 Daniele Rapetti
// This code is licensed under MIT license (see LICENSE for details)
#include "json_read.hpp"
#include "utils.hpp"
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
    while ((c = in.get ()) == ' ' || c == '\t' || c == '\n' || c == '\r') {
    }
    return c;
  }
  std::optional<Object> read (std::istream &in) {
    if (auto myobj = internal::readObject (in); myobj.has_value ()) {
      return std::move (*myobj);
    } else {
      return std::nullopt;
    }
  }
  namespace internal {

    std::optional<Object> readObject (std::istream &in) {
      Object obj;
      bracketsValidator validator;

      if (char c = spaceEater (in); c != '{') {
        // cannot opend the parentesis
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
        if (std::regex_match (rawValue, valueRegex)) {
          if (std::regex_match (rawValue, numberRegex)) {
            obj[key] = std::stod (rawValue);
          } else if (std::regex_match (rawValue, boolRegex)) {
            // obj[key] = std::regex_match (rawValue, trueRegex)  is not ok:
            // beacause there can be a typo on the false
            if (std::regex_match (rawValue, trueRegex)) {
              obj[key] = true;
            } else if (std::regex_match (rawValue, falseRegex)) {
              obj[key] = false;
            } else {
              return std::nullopt;
            }
          } else if (std::regex_match (rawValue, stringRegex)) {
            std::smatch words;
            std::regex_search (rawValue, words, stringRegex);
            auto t = words[0].str ();

            obj[key] = t.substr (1, t.size () - 2);
          } else if (std::regex_match (rawValue, nullRegex)) {
            obj[key] = Null{};
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
        }
      }
    }
    std::optional<Array> readArray (std::istream &in) { return std::nullopt; }
  } // namespace internal
} // namespace djson