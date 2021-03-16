#ifndef TESTJSON_I
#define TESTJSON_I

%module testjson

%include <stl.i>
%include <typemaps.i>

%{
  #include <json/json.h>
  #include <JsonCpp.hpp>
%}

%include <JsonCpp.hpp>



#if defined SWIGPYTHON

// This definitely does not work
%typemap(out) Json::Value {

  if ($1->isIntegral()) {
    $result = PyLong_FromLongLong($1->asInt64());
  } else if ($1->isNumeric()) {
    $result = PyFloat_FromDouble($1->asDouble());
  } else if ($1->isString()) {
    $result = PyUnicode_FromString($->.asString());
  } else if ($1->isArray()) {
    $result = PyList_New();
    for( const auto& n : $1) {
        // TODO: this should do a recursive call to convert n (which is Json::Value) to a python type...
        PyList_Append(n);
    }
  } else if ($1->isObject()) {
    $result = PyDict_New();
    for( const auto& id : $1->getMemberNames()) {
        // TODO: this should do a recursive call to convert *$1[id] (which is a Json::Value) to a python type...
        PyDict_SetItemString($result, id.c_str(), *$1[id]);
    }
  }
}
#endif




// Manual workaround section
// I would ignore the makeDumbJSON above, then redefine it here as a function of the makeDumbJSONString instead
#if defined SWIGRUBY
  // This isn't super clean and there might be a better way with typemaps in SWIG rather than using a String in between,
  // but still helpful I think: we redefine toJSON that will return a native ruby hash. 'json' is part of ruby stdlib since at least ruby 2.5.5
  %init %{
    rb_eval_string("Test::testjson.module_eval { define_method(:makeDumbJSON2) { || self.makeDumbJSONString(); require 'json'; JSON.load(json_str);  }; module_function(:toJSON) }");
 %}
#endif


#if defined SWIGPYTHON
  // Let's use monkey-patching via unbound functions
  // Edit: not even needed here
  %pythoncode %{
# Manually added: Lazy-load the json module (python std lib) and return a dict via toJSONString
def makeDumbJSON2(*args) -> dict:
    import json
    return json.loads(makeDumbJSONString(*args))
  %}
#endif

#endif // TESTJSON_I
