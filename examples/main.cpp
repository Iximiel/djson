#include "djson/json_read.hpp"
#include "djson/json_write.hpp"
#include <fstream>
#include <iostream>

int main () {
  auto filename = "data.json";
  std::ifstream f (filename);

  std::optional<djson::Object> obj = djson::read (f);
  if (obj.has_value ()) {
    djson::write (std::cout, *obj);
    std::cout << "\n";
    auto &theObj = *obj;
    std::cout << "keys: " << std::endl;
    for (const auto &key : theObj.Keys ()) {
      std::cout << " - " << key << std::endl;
    }

    // get a specific value
    djson::Number value;
    theObj.get ("number", value);
    std::cout << "number: " << value << std::endl;
    // or
    value = theObj.get<djson::Number> ("number");
    std::cout << "number: " << value << std::endl;
    // or
    value = std::get<djson::Number> (theObj.at ("number"));
  } else {
    std::cout << "failed to open " << filename << std::endl;
  }
}