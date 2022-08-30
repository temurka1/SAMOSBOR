#pragma once

#ifdef ENABLE_TRACE
#ifdef _MSC_VER
#include <windows.h>
#include <sstream>
#define TRACE(x) do {  std::stringstream s;  s << x; OutputDebugStringA((LPCSTR)(s.str().c_str())); } while(0)
#else
#include <iostream>
#define TRACE(x)  std::clog << (x)
#endif
#else
# define TRACE(x)
#endif