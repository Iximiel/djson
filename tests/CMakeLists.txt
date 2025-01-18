# (c) 2024 Daniele Rapetti
#
# This code is licensed under MIT license (see LICENSE for details)
find_package(Catch2 3 REQUIRED)

# These tests can use the Catch2-provided main
add_executable(tests test_json.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
