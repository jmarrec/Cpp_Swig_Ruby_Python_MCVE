cmake_minimum_required(VERSION 3.18.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Do not enable compiler specific extensions, for eg on GCC use -std=c++1z (=c++17) and not -std=gnu++17
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_VERBOSE_MAKEFILE TRUE)


# Use ccache is available, has to be before "project()"
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  # Support Unix Makefiles and Ninja
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()


project(test_swig VERSION 0.0.1)

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

###############################################################################
#                                  N I N J A                                  #
###############################################################################

# Xcode/Ninja generators undefined MAKE
if(CMAKE_GENERATOR MATCHES "Make")
  set(MAKE "$(MAKE)")
else()
  set(MAKE make)
endif()

# Ninja support: has to be atop for it to take effect before anything else is done
# Add Color Output if Using Ninja
macro(AddCXXFlagIfSupported flag test)
  CHECK_CXX_COMPILER_FLAG(${flag} ${test})
  if(${${test}})
    message("adding ${flag}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}")
  endif()
endmacro()

if("Ninja" STREQUAL ${CMAKE_GENERATOR})
  # Clang
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    AddCXXFlagIfSupported(-fcolor-diagnostics COMPILER_SUPPORTS_fcolor-diagnostics)
  endif()

  # g++
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # For some reason it doesn't say its supported, but it works...
    # AddCXXFlagIfSupported(-fdiagnostics-color COMPILER_SUPPORTS_fdiagnostics-color)
    message(STATUS "Ninja: Forcing -fdiagnostics-color=always")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
  endif()
endif()


# Search first in the binary dir, where conan will install finders, then
# search for modules in the root dir to override cmake ones
# Start with ROOT, then PROJECT_BINARY_DIR
list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR} ${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/CMake")
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# Add to include path

# Project source directory
include_directories("${PROJECT_SOURCE_DIR}")

###############################################################################
#                         C M A K E    C O N T R O L                          #
###############################################################################

# High level project configuration
# Do we actually want everything to go to CMAKE_BINARY_DIR/Products,
# so that when you build OpenStudioApplication you get both OpenStudio (core) and Application in the same place?
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/Products")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/Products")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/Products")

set(LIBRARY_SEARCH_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Release" "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Debug")

# Search first in the binary dir, where conan will install finders, then
# search for modules in the root dir to override cmake ones
list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR} ${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/CMake")

#######################################################################
#           R E G U L A R    O U P U T    A N D    L I B S            #
#######################################################################

set(target_name mylib)

set(${target_name}_src
  Person.hpp
  Person.cpp
)

add_library(${target_name} ${${target_name}_src})
target_link_libraries(${target_name}
  ${${target_name}_depends}
)
# Executable
add_executable(Test
  main.cpp
)

target_link_libraries(Test
 ${target_name}
)


#######################################################################
#                               S W I G                               #
#######################################################################


option(BUILD_RUBY_BINDINGS "Build Ruby bindings" ON)

# Build CSharp bindings
option(BUILD_CSHARP_BINDINGS "Build CSharp bindings" OFF)

# Build Python bindings
# Requires: SWIG Python
option(BUILD_PYTHON_BINDINGS "Build Python bindings" ON)


# lib swig files
set(${target_name}_swig_src
  Person.i
)

foreach (SWIG_FILE ${${target_name}_swig_src})
  message("SWIG_FILE=${SWIG_FILE}")
  set_source_files_properties(${SWIG_FILE} PROPERTIES CPLUSPLUS ON)
endforeach()

find_package(SWIG REQUIRED COMPONENTS python ruby csharp)
message("${SWIG_USE_FILE}")
include(${SWIG_USE_FILE})

if (BUILD_PYTHON_BINDINGS)

  set(swig_target_name ${swig_target_name}_python)

  find_package(Python COMPONENTS Interpreter Development REQUIRED)

  include_directories(SYSTEM ${Python_INCLUDE_DIRS})

  swig_add_library(${swig_target_name}
    TYPE MODULE
    LANGUAGE python
    OUTPUT_DIR "${PROJECT_BINARY_DIR}/Products/python"
    OUTFILE_DIR "${PROJECT_BINARY_DIR}/python_wrapper"
    SOURCES ${${target_name}_swig_src}
  )

  set_target_properties(${swig_target_name} PROPERTIES OUTPUT_NAME ${target_name})

  set_target_properties(${swig_target_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/python/")
  set_target_properties(${swig_target_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/python/")
  set_target_properties(${swig_target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/python/")


  swig_link_libraries(${swig_target_name} ${target_name})


  if (MSVC)
    swig_link_libraries(${swig_target_name} PRIVATE Python::Module)
  endif()
endif()

if(BUILD_RUBY_BINDINGS)
  find_package(Ruby REQUIRED)

  include_directories(SYSTEM ${Ruby_INCLUDE_DIRS})

  set(swig_target_name ${target_name}_ruby)

  swig_add_library(${swig_target_name}
    TYPE MODULE
    LANGUAGE ruby
    OUTPUT_DIR "${PROJECT_BINARY_DIR}/Products/ruby"
    OUTFILE_DIR "${PROJECT_BINARY_DIR}/ruby_wrapper"
    SOURCES ${${target_name}_swig_src}
  )

  set_target_properties(${swig_target_name} PROPERTIES OUTPUT_NAME ${target_name})

  set_target_properties(${swig_target_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/ruby/")
  set_target_properties(${swig_target_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/ruby/")
  set_target_properties(${swig_target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ruby/")

  swig_link_libraries(${swig_target_name} ${target_name} ${Ruby_LIBRARIES})

endif()
