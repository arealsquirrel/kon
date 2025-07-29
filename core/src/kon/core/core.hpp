#ifndef KN_CORE_HPP
#define KN_CORE_HPP

#define KN_VERSION "1.0.0"

#define KN_MEM_POINTER(var) reinterpret_cast<char*>(var)

#define KN_BIT(x) 1 << x

namespace kon {

	
// type sizes as defined by this
// https://en.cppreference.com/w/cpp/language/types.html
// 32-bit = ILP32
// 64-bit = LLP64 for windows, LP64 for unix and linux

using u8 = unsigned char;
using i8 = char;

using u16 = unsigned short int;
using i16 = short int;

using u32 = unsigned int;
using i32 = int;

using u64 = unsigned long long int;
using i64 = long long int;

int test_function();

}

#endif
