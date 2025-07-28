#ifndef KN_UTIL_HPP
#define KN_UTIL_HPP

#include <kon/core/core.hpp>
#include <functional>

namespace kon {

template<typename T>
using ForEachFunction = std::function<void(T &element, u32 index)>;

struct Point {
	int x;
	int y;
};

struct Rect {
	unsigned int width;
	unsigned int height;
	int x;
	int y;
};

template<typename First, typename Second>
struct Pair {
public:
	Pair() = default;
	Pair(First _f, Second _s)
		: first(_f), second(_s) {}
	~Pair() = default;
	Pair(const Pair &pair)
		: first(pair.first), second(pair.second) {}

public:
	First first;
	Second second;
};

template<class First, class Second>
void swap_pair(Pair<First, Second> &a, Pair<First, Second> &b) {
	std::swap(a.first, b.first);
	// std::swap(a.second, b.second);
}

}

#endif
