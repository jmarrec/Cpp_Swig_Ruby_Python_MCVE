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



%include <stl.i>
%include <std_string.i>
%include <std_vector.i>

%feature("director") Measure;

#if defined(SWIGPYTHON)
%rename (PythonMeasure) Test::Measure;
#endif

%{
  #include <Model.hpp>
  #include <Runner.hpp>
  #include <Measure.hpp>
%}

%include <Model.hpp>
%include <Runner.hpp>
%include <Measure.hpp>

#endif //MEASURE_I
