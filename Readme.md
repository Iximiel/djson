# DJSON a json library made for learning the joy of parsing

> The 'D' is silent

My take on a json library, for fun and learing 

This is not complete, the parser I think is kinda meh, but it is an exercise for uisng the `<variant>` and try to use `<optional>` for avoiding throws.

I will add some options like the possibility of setting the lib as a shared object and the CI

Maybe I should try to look for more tests to make the parser better

## Usage

```C++
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
    // or this will likel became an optional return
    value = theObj.get<djson::Number> ("number");
    std::cout << "number: " << value << std::endl;
    // or
    value = std::get<djson::Number> (theObj.at ("number"));
  } else {
    std::cout << "failed to open " << filename << std::endl;
  }
}
```

```Cmake
find_package(Djson REQUIRED)
add_executable(main main.cpp)

target_link_libraries(
  main
  PRIVATE Djson::Djson)

```