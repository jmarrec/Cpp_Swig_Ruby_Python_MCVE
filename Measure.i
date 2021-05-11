#ifndef MEASURE_I
#define MEASURE_I

%begin %{
  // ... code in begin section ...
%}

%runtime %{
  // ... code in runtime section ...
%}

%header %{
  // ... code in header section ...
%}

%wrapper %{
  // ... code in wrapper section ...
%}

%init %{
  // ... code in init section ...
%}

%module mylib
%module(directors="1") mylib

// Note:  JM 2021-05-11
// Regarding Dan's note above: As soon as you include something like std_vector.i (stl.i does it...) => multiple redefinition of GC_VALUE
// This will end up being defined inside the Wrap.cxx file as a static ...
// The main.cpp defines it for some reason
// %include <stl.i>

%include <std_string.i>

%feature("director") Measure;

#if defined(SWIGPYTHON)
%rename (PythonMeasure) Test::Measure;
//%rename ("Python%s") "";
#endif

%{
  #include <ModelObject.hpp>
  #include <Model.hpp>
  #include <Runner.hpp>
  #include <Measure.hpp>
%}

%include <ModelObject.hpp>
%include <Model.hpp>
%include <Runner.hpp>
%include <Measure.hpp>

#endif //MEASURE_I

