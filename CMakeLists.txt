cmake_minimum_required(VERSION 3.17.5)

add_library(cpp_warning_options INTERFACE)
add_library(cpp_compile_options INTERFACE)

target_compile_features(cpp_compile_options INTERFACE cxx_std_17)

if(MSVC)
  target_compile_options(cpp_warning_options INTERFACE /W4 /Werror)
else()
  target_compile_options(
    cpp_warning_options INTERFACE -Wall -Wextra -Wconversion -Wpedantic -Werror)
#  target_compile_options(cpp_compile_options
#                         INTERFACE -fsanitize=undefined,address)
#  target_link_options(cpp_compile_options INTERFACE
#                      -fsanitize=undefined,address)
endif()

# Do not enable compiler specific extensions, for eg on GCC use -std=c++1z
# (=c++17) and not -std=gnu++17
set(CMAKE_CXX_EXTENSIONS OFF)

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
  set(CMAKE_BUILD_TYPE
      Release
      CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
                                               "MinSizeRel" "RelWithDebInfo")
endif()

# Search first in the binary dir, where conan will install finders, then search
# for modules in the root dir to override cmake ones Start with ROOT, then
# CMAKE_CURRENT_BINARY_DIR
list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}"
     "${PROJECT_SOURCE_DIR}/CMake")
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# Add to include path

# Project source directory
include_directories("${PROJECT_SOURCE_DIR}")

# ##############################################################################
# C M A K E    C O N T R O L                          #
# ##############################################################################

# High level project configuration Do we actually want everything to go to
# CMAKE_BINARY_DIR/Products, so that when you build OpenStudioApplication you
# get both OpenStudio (core) and Application in the same place?
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Products")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Products")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Products")

set(LIBRARY_SEARCH_DIRECTORY
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Release"
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Debug")

# Search first in the binary dir, where conan will install finders, then search
# for modules in the root dir to override cmake ones
list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}"
     "${PROJECT_SOURCE_DIR}/CMake")

# ##############################################################################
# R E G U L A R    O U P U T    A N D    L I B S            #
# ##############################################################################

set(target_name mylib)

set(${target_name}_src Measure.hpp Measure.cpp Model.hpp Model.cpp Runner.hpp
                       SpecialRunner.hpp)

add_library(${target_name} SHARED ${${target_name}_src})
target_link_libraries(${target_name} PRIVATE cpp_warning_options
                                             cpp_compile_options)

# ##############################################################################
# S W I G                               #
# ##############################################################################

option(BUILD_RUBY_BINDINGS "Build Ruby bindings" ON)

# Build CSharp bindings
option(BUILD_CSHARP_BINDINGS "Build CSharp bindings" OFF)

# Build Python bindings Requires: SWIG Python
option(BUILD_PYTHON_BINDINGS "Build Python bindings" ON)

# lib swig files
set(${target_name}_swig_src Measure.i)

foreach(SWIG_FILE ${${target_name}_swig_src})
  message("SWIG_FILE=${SWIG_FILE}")
  set_source_files_properties(${SWIG_FILE} PROPERTIES CPLUSPLUS ON)
endforeach()

find_package(SWIG REQUIRED COMPONENTS python ruby csharp)
message("${SWIG_USE_FILE}")
include(${SWIG_USE_FILE})

enable_testing()
include(CTest)

if(BUILD_PYTHON_BINDINGS)

  set(swig_target_name ${swig_target_name}_python)

  find_package(
    Python
    COMPONENTS Interpreter Development
    REQUIRED)

  swig_add_library(
    ${swig_target_name}
    TYPE MODULE
    LANGUAGE python OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/Products/python"
                    OUTFILE_DIR "${CMAKE_CURRENT_BINARY_DIR}/python_wrapper"
    SOURCES ${${target_name}_swig_src})

  set_target_properties(${swig_target_name} PROPERTIES OUTPUT_NAME
                                                       ${target_name})

  set_target_properties(
    ${swig_target_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY
                                   "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/python/")
  set_target_properties(
    ${swig_target_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY
                                   "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/python/")
  set_target_properties(
    ${swig_target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                   "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/python/")

#  target_include_directories(${swig_target_name} PRIVATE SYSTEM
#                                                         ${Python_INCLUDE_DIRS})

  swig_link_libraries(${swig_target_name} PUBLIC ${target_name} PRIVATE
                      cpp_compile_options)

#  if(MSVC)
    swig_link_libraries(${swig_target_name} PRIVATE Python::Module)
#  endif()

  add_test(
    NAME test_python_only
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/python
    COMMAND ${Python_EXECUTABLE} test_python_only.rb)

endif()

if(BUILD_RUBY_BINDINGS)
  find_package(Ruby REQUIRED)

  set(swig_target_name ${target_name}_ruby)

  swig_add_library(
    ${swig_target_name}
    TYPE MODULE
    LANGUAGE ruby OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/Products/ruby"
                  OUTFILE_DIR "${CMAKE_CURRENT_BINARY_DIR}/ruby_wrapper"
    SOURCES ${${target_name}_swig_src})

  set_target_properties(${swig_target_name} PROPERTIES OUTPUT_NAME
                                                       ${target_name})

  set_target_properties(
    ${swig_target_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY
                                   "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/ruby/")
  set_target_properties(
    ${swig_target_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY
                                   "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/ruby/")
  set_target_properties(
    ${swig_target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                   "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ruby/")

  target_include_directories(
    ${swig_target_name} PRIVATE SYSTEM ${RUBY_INCLUDE_DIRS} )

  swig_link_libraries(${swig_target_name} PUBLIC ${target_name} PRIVATE
                      ${RUBY_LIBRARIES} cpp_compile_options)

  add_test(
    NAME test_ruby_only
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/ruby
    COMMAND ${RUBY_EXECUTABLE} test_ruby_only.py)

endif()

if(BUILD_RUBY_BINDINGS AND BUILD_PYTHON_BINDINGS)

  add_test(
    NAME test_pycall
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/ruby
    COMMAND ${RUBY_EXECUTABLE} test_pycall.rb)

  add_test(
    NAME test_pycall_workaround
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/ruby
    COMMAND ${RUBY_EXECUTABLE} test_pycall_workaround.rb)

  add_test(
    NAME test_pycall_use_cpp_to_convert_pointers
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/ruby
    COMMAND ${RUBY_EXECUTABLE} test_pycall_use_cpp_to_convert_pointers.rb)

  add_test(
    NAME test_pycall_script
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/ruby
    COMMAND ${RUBY_EXECUTABLE} test_pycall_script.rb)

endif()

# Must call CMake itself in order to set the SWIG_LIB env var for
# add_custom_command
add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/python_wrapper/SWIGPythonRuntime.hxx"
  COMMAND
    ${CMAKE_COMMAND} -E env SWIG_LIB="${SWIG_DIR}" "${SWIG_EXECUTABLE}" "-v"
    "-python" -external-runtime
    "${CMAKE_CURRENT_BINARY_DIR}/python_wrapper/SWIGPythonRuntime.hxx")

# Must call CMake itself in order to set the SWIG_LIB env var for
# add_custom_command
add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/ruby_wrapper/SWIGRubyRuntime.hxx"
  COMMAND
    ${CMAKE_COMMAND} -E env SWIG_LIB="${SWIG_DIR}" "${SWIG_EXECUTABLE}" "-v"
    "-ruby" -external-runtime
    "${CMAKE_CURRENT_BINARY_DIR}/ruby_wrapper/SWIGRubyRuntime.hxx")

# Executable
add_executable(
  Test
  main.cpp
  ScriptEngine.hpp
  PythonEngine.hpp
  PythonEngine.cpp
  RubyEngine.hpp
  RubyEngine.cpp
  "${CMAKE_CURRENT_BINARY_DIR}/ruby_wrapper/SWIGRubyRuntime.hxx"
  "${CMAKE_CURRENT_BINARY_DIR}/python_wrapper/SWIGPythonRuntime.hxx")

target_link_libraries(Test PRIVATE ${target_name} cpp_compile_options
                                   cpp_warning_options ${RUBY_LIBRARY} Python::Python)

target_include_directories(Test PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/ruby_wrapper" "${CMAKE_CURRENT_BINARY_DIR}/python_wrapper"
        ${RUBY_INCLUDE_DIRS})

