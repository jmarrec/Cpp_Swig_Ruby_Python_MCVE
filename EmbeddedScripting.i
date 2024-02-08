#ifndef EMBEDDED_SCRIPTING_I
#define EMBEDDED_SCRIPTING_I

%module EmbeddedScripting

%include <std_string.i>


// DLM: including these files causes a crash, I don't know why
//%include <utilities/core/CommonInclude.i>
//%import <utilities/core/CommonImport.i>
//%import <utilities/Utilities.i>

// Note:  JM 2021-05-11
// Regarding Dan's note above: As soon as you include something like std_vector.i => multiple redefinition of GC_VALUE
// This will end up being defined inside the Wrap.cxx file as a static ...
// The main.cpp defines it for some reason
// %include <std_vector.i>

%{
  #include <embedded_files.hxx>
  #include "EmbeddedHelp.hpp"
%}

%ignore embedded_files::fileNames;

%include <embedded_files.hxx>
%include "EmbeddedHelp.hpp"

#endif // EMBEDDED_SCRIPTING_I
