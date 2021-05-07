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

%feature("director") Measure;

%include <stl.i>
%include <std_string.i>
%include <std_vector.i>

%{
  #include <Model.hpp>
  #include <Runner.hpp>
  #include <Measure.hpp>
%}

%include <Model.hpp>
%include <Runner.hpp>
%include <Measure.hpp>

#endif //MEASURE_I

