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
  String.hpp
  String.cpp
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

enable_testing()
include(CTest)


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

  add_test(
    NAME test_ruby_only
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/ruby
    COMMAND ${Ruby_EXECUTABLE} test_ruby_only.rb
  )

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

  add_test(
    NAME test_python_only
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/python
    COMMAND ${Python_EXECUTABLE} test_python_only.py
  )

endif()


if(BUILD_RUBY_BINDINGS AND BUILD_PYTHON_BINDINGS)

  add_test(
    NAME test_pycall
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/ruby
    COMMAND ${Ruby_EXECUTABLE} test_pycall.rb
  )

  add_test(
    NAME test_pycall_workaround
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/ruby
    COMMAND ${Ruby_EXECUTABLE} test_pycall_workaround.rb
  )

  add_test(
    NAME test_pycall_use_cpp_to_convert_pointers
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/ruby
    COMMAND ${Ruby_EXECUTABLE} test_pycall_use_cpp_to_convert_pointers.rb
  )

  add_test(
    NAME test_pycall_script
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/ruby
    COMMAND ${Ruby_EXECUTABLE} test_pycall_script.rb
  )

endif()


if(BUILD_CSHARP_BINDINGS)


  set(swig_target_name ${target_name}_csharp)

  file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/csharp_wrapper/generated_sources")

  # TODO: I have no clue what the Products/csharp/mylib.so actually is...
  swig_add_library(${swig_target_name}
    TYPE MODULE   # OR SHARED?
    LANGUAGE CSharp
    OUTPUT_DIR "${PROJECT_BINARY_DIR}/csharp_wrapper/generated_sources"
    OUTFILE_DIR "${PROJECT_BINARY_DIR}/csharp_wrapper"
    SOURCES ${${target_name}_swig_src}
  )

  set_target_properties(${swig_target_name} PROPERTIES OUTPUT_NAME ${target_name})

  set_target_properties(${swig_target_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/csharp")
  set_target_properties(${swig_target_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/csharp/")
  set_target_properties(${swig_target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/csharp/")

  swig_link_libraries(${swig_target_name} ${target_name}) # ${Ruby_LIBRARIES})

  set_target_properties(${swig_target_name} PROPERTIES SWIG_USE_TARGET_INCLUDE_DIRECTORIES TRUE)

  set(CSHARP_VERSION_MAJOR 0)
  set(CSHARP_VERSION_MINOR 0)
  set(CSHARP_VERSION_PATCH 1)
  set(CSHARP_VERSION_BUILD 0)

  if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(CSHARP_PLATFORM "x64")
  else()
    set(CSHARP_PLATFORM "x86")
  endif()

  set(CSHARP_ASSEMBLY_VERSION "${CSHARP_VERSION_MAJOR}.${CSHARP_VERSION_MINOR}.${CSHARP_VERSION_PATCH}.${CSHARP_VERSION_BUILD}")

  set(CSHARP_TOOLS_VERSION "4.5")


  if (MSVC)
    # Untested, but maybe works?
    enable_language(CSharp)
    add_library(example_cs SHARED  $<TARGET_PROPERTY:${swig_target_name},SWIG_SUPPORT_FILES>)
    set_target_properties(example_cs PROPERTIES LINKER_LANGUAGE CSharp)
    set_target_properties(example_cs PROPERTIES DOTNET_TARGET_FRAMEWORK "netcoreapp3.1")
  else()
    find_program(DOTNET dotnet)
    if(NOT DOTNET)
      message(FATAL_ERROR "C# generation outside of MSVC requires .NET Core (dotnet)")
    endif()



    file(TO_NATIVE_PATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/csharp" CSHARP_LIBRARY_OUTPUT_DIRECTORY)
    file(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}/csharp" CSHARP_SOURCE_DIR)
    file(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}/csharp_wrapper" CSHARP_BINARY_DIR)


    file(MAKE_DIRECTORY "${CSHARP_LIBRARY_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/netcoreapp3.0")

    # TODO: there are probably adjustments to make to Mylib.csproj_dotnet.in
    # The Release/Debug can be stripped probably
    # Also probably don't need to use multiple `<TargetFrameworks>` which is the reason there's an extra netcoreapp3.0/ level
    set(OPENSTUDIO_CSHARP_DLL "${CSHARP_LIBRARY_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/netcoreapp3.0/mylib.dll")

    # ignore deprecated warnings since we will export deprecated methods
    add_definitions(-Wno-deprecated-declarations)

    configure_file("${PROJECT_SOURCE_DIR}/csharp/Mylib.csproj_dotnet.in" "${PROJECT_BINARY_DIR}/csharp_wrapper/Mylib.csproj")

    # custom command to make OPENSTUDIO_CSHARP_DLL
    add_custom_command(TARGET ${swig_target_name}
      POST_BUILD
      COMMAND "${DOTNET}" "build" "-c" "$<CONFIGURATION>" "-f" "netcoreapp3.0" "${PROJECT_BINARY_DIR}/csharp_wrapper/Mylib.csproj"
    )

  endif()

  #add_test(
    #NAME test_python_only
    #WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/python
    #COMMAND ${Python_EXECUTABLE} test_python_only.py
  #)

endif()
