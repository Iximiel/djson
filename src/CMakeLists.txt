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

add_library(Djson json.cpp json_read.cpp utils.cpp json_write.cpp)

target_include_directories(
  Djson
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
         $<INSTALL_INTERFACE:include>
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/djson)

add_library(Djson::Djson ALIAS Djson)
# get_target_property(LIBA_INCLUDES Djson::Djson INCLUDE_DIRECTORIES)
# message("LIBA_INCLUDES: ${LIBA_INCLUDES}")

# ##############################################################################
# Preparing the install package
include(GNUInstallDirs)
set(DIRS_CMAKE_INSTALL ${CMAKE_INSTALL_LIBDIR}/cmake/Djson)
include(GenerateExportHeader)
generate_export_header(Djson)
set_property(TARGET Djson PROPERTY VERSION ${PROJECT_VERSION})
set_property(TARGET Djson PROPERTY SOVERSION 1)

install(
  TARGETS Djson
  EXPORT DjsonTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/Djson/DjsonConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion)

# export for not installed packet
export(
  EXPORT DjsonTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/Djson/DjsonTargets.cmake"
  NAMESPACE Djson::)

# install for installed packet
install(
  EXPORT DjsonTargets
  FILE DjsonTargets.cmake
  NAMESPACE Djson::
  DESTINATION ${DIRS_CMAKE_INSTALL})

configure_file(cmake/DjsonConfig.cmake
               "${CMAKE_CURRENT_BINARY_DIR}/Djson/DjsonConfig.cmake" COPYONLY)

install(
  FILES cmake/DjsonConfig.cmake
        "${CMAKE_CURRENT_BINARY_DIR}/Djson/DjsonConfigVersion.cmake"
  DESTINATION ${DIRS_CMAKE_INSTALL}
  COMPONENT Devel)
