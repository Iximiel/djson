# (c) 2025 Daniele Rapetti
#
# This code is licensed under MIT license (see LICENSE for details)

if(${OPT_DEBUGWARNINGS})
  if(MSVC)
    add_compile_options(/W4 /WX)
  else()
    add_compile_options(
      -Wall
      -Wextra
      -Wshadow
      -Wnon-virtual-dtor
      -Wold-style-cast
      -Wcast-align
      -Wunused
      -Woverloaded-virtual
      -Wpedantic
      -Wconversion
      -Wsign-conversion
      -Wnull-dereference
      -Wdouble-promotion
      -Wformat=2
      -Wduplicated-cond
      -Wduplicated-branches
      -Wlogical-op
      -Wuseless-cast
      # -Wlifetime
    )
  endif()
endif()

if(${OPT_ERRORWARNINGS})
  if(MSVC)
    add_compile_options(/WX)
  else()
    add_compile_options(-Werror)
  endif()
endif()

add_library(DJSON json.cpp json_read.cpp utils.cpp json_write.cpp)

target_include_directories(
  DJSON PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

add_library(DJSON::DJSON ALIAS DJSON)
# get_target_property(LIBA_INCLUDES DJSON::DJSON INCLUDE_DIRECTORIES)
# message("LIBA_INCLUDES: ${LIBA_INCLUDES}")
