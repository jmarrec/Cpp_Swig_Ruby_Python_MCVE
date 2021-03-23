#ifndef MEASURE_I
#define MEASURE_I

%module mylib
%module(directors="1") mylib 

%feature("director") Measure;
%feature("director") Runner;

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

