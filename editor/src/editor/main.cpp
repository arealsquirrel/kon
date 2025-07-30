
#include "kon/core/assert.hpp"
#include "kon/core/library.hpp"
#include "kon/debug/timer.hpp"
#include <chrono>
#include <kon/debug/log.hpp>

using namespace kon;

int main() {
	LibraryHandle handle = platform::load_library("modules/libexample_module.so");
	if(handle == nullptr) KN_CORE_ERROR("uh oh");

	typedef int (*lmao_function_pointer)();

	lmao_function_pointer f;
	f = (lmao_function_pointer) platform::load_function(handle, "lmao");
	f();
}

