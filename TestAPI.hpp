#ifndef TESTAPI_HPP
#define TESTAPI_HPP

#if (_WIN32 || _MSC_VER) && !TEST_DIRECT_INCLUDE

#  ifdef SHARED_OS_LIBS

#    if defined(test_EXPORTS)
#      define TEST_API __declspec(dllexport)
#      define TEST_TEMPLATE_EXT
#    else
#      define TEST_API __declspec(dllimport)
#      define TEST_TEMPLATE_EXT extern
#    endif

#  else

#    define TEST_API
#    if defined(test_EXPORTS)
#      define TEST_TEMPLATE_EXT
#    else
#      define TEST_TEMPLATE_EXT extern
#    endif

#  endif

#else

#  define TEST_API
#  define TEST_TEMPLATE_EXT

#endif

#endif
