# (c) 2025 Daniele Rapetti
#
# This code is licensed under MIT license (see LICENSE for details)

# enable FetchContent to get Catch2
include(FetchContent)

# todo: not download if found

# declare Catch2
FetchContent_Declare(
  Catch2 # name of the content
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git # the repository
  GIT_TAG v3.8.0)

# make available
FetchContent_MakeAvailable(Catch2)

if(NOT TARGET DJSON::DJSON)
  find_package(DJSON REQUIRED)
endif()

# These tests can use the Catch2-provided main
add_executable(tests test_object.cpp test_array.cpp test_utils.cpp
                     test_reader.cpp test_writer.cpp)
target_link_libraries(
  tests
  PRIVATE Catch2::Catch2WithMain
  PUBLIC DJSON::DJSON)

enable_testing()

include(CTest)
include(Catch)
catch_discover_tests(tests)
