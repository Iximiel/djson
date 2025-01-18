# (c) 2024 Daniele Rapetti
#
# This code is licensed under MIT license (see LICENSE for details)
cmake_minimum_required(VERSION 3.15...3.25)

project(
  json
  VERSION 0.1
  DESCRIPTION "My try at a json parser/writer"
  LANGUAGES CXX)

cxx_standard(20)

add_subdirectory(src)
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
