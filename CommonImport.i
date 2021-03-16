#ifndef TEST_COMMONIMPORT_I
#define TEST_COMMONIMPORT_I

%{
  #include <sstream>
%}

#ifdef SWIGCSHARP

  %rename(LeftShift) operator <<;
  %rename(OStream) std::ostream;
  %rename(IStream) std::istream;

  // Stub out streams placed here instead of LanguageSpecific.i because it needs to be
  // in the set of imports.
  %nodefaultctor std::ostream;
  %nodefaultctor std::istream;
  namespace std {
    class ostream
    {
    };

    class istream
    {
    };
  }
#endif

#endif // UTILITIES_COMMONIMPORT_I
