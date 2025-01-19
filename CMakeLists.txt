# (c) 2025 Daniele Rapetti
#
# This code is licensed under MIT license (see LICENSE for details)
cmake_minimum_required(VERSION 3.15...3.25)

project(
  Djson
  VERSION 0.1
  DESCRIPTION "My try at a json parser/writer"
  LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(BUILD_TESTING "build tests" OFF)
option(OPT_DEBUGWARNINGS "Some extra warning for debugging" OFF)
option(OPT_ERRORWARNINGS "Blocks compilation on warnings" OFF)

add_subdirectory(src)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
