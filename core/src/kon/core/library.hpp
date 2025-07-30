#ifndef KN_LIBRARY_HPP
#define KN_LIBRARY_HPP

#include <kon/core/core.hpp>

namespace kon {

using LibraryHandle = void*;
using FunctionHandle = void*;

namespace platform {

LibraryHandle load_library(const char *path);
FunctionHandle load_function(LibraryHandle handle, const char *name);

}

}

#endif
