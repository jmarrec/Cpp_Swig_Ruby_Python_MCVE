#ifndef TIME_I
#define TIME_I

%module testtime

%include <stl.i>
%include <typemaps.i>

%{
  #include <ctime>
  #include <Time.hpp>
%}

//%ignore test::Time::getTime;
// %ignore test::Time::setTime(const std::time_t&);

/* Apply all of the int typemaps to time_t: doesn't work */
%apply long long { time_t, std::time_t };

// The easiest to make swig understand std::time_t is this...
typedef long long time_t;

%include <Time.hpp>

#endif // TIME_I
