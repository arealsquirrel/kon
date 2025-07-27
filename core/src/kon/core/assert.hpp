#ifndef KN_ASSERT_HPP
#define KN_ASSERT_HPP

#include <kon/debug/log.hpp>

#define KN_ASSERT(cond, ...) if((cond) == false) { \
	KN_CORE_ERROR("ASSERT FAILED {}. Exiting program :3", #cond); \
	KN_CORE_ERROR(__VA_ARGS__); \
	abort(); \
}

#endif
