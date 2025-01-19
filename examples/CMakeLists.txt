# (c) 2025 Daniele Rapetti
#
# This code is licensed under MIT license (see LICENSE for details)
cmake_minimum_required(VERSION 3.15...3.25)

project(
  Djson-examples
  VERSION 0.1
  DESCRIPTION "My try at a json parser/writer"
  LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Djson REQUIRED)
add_executable(main main.cpp)

target_link_libraries(main PRIVATE Djson::Djson)
