
#include "kon/core/directory.hpp"
#include <kon/core/library.hpp>

#include <dlfcn.h>

namespace kon {
namespace platform {

LibraryHandle load_library(const char *path) {
	/// PathStat s = platform::get_path_stat(path);
	// if(s.directory || s.valid == false) return nullptr;

	LibraryHandle lib = dlopen(path, RTLD_LAZY);
	return lib;
}

FunctionHandle load_function(LibraryHandle handle, const char *name) {
	return dlsym(handle, name);
}

}
}

