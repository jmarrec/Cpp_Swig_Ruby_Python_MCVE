#ifndef CLI_EMBEDDEDHELP_HPP
#define CLI_EMBEDDEDHELP_HPP

#include <iostream>
#if defined __APPLE__
#  include <mach-o/dyld.h> /* _NSGetExecutablePath */
#  include <limits.h>      /* PATH_MAX */
#elif defined _WIN32
#  include <windows.h>
#endif

// Can't seem to find ext-init?!
//#ifdef SWIG
//%include <ext-init.hpp>
//#else
//#  include <ext-init.hpp>
//#endif

namespace embedded_help {

inline std::string applicationFilePath() {
#ifdef __APPLE__
  char path[PATH_MAX + 1];
  uint32_t size = sizeof(path);
  if (_NSGetExecutablePath(path, &size) == 0) {
    return std::string(path);
  }
#elif defined _WIN32
  TCHAR szPath[MAX_PATH];
  if (!GetModuleFileName(nullptr, szPath, MAX_PATH)) {
    return std::string(szPath);
  }
#endif
  return std::string();
}

}  // namespace embedded_help

#endif  // CLI_EMBEDDEDHELP_HPP
