#ifndef KN_UTIL_HPP
#define KN_UTIL_HPP

#include <kon/core/core.hpp>
#include <functional>
#include <string>

namespace kon {

constexpr u32 constexpr_strlen(const char* s) {
    return std::char_traits<char>::length(s);
}

template<typename T>
using ForEachFunction = std::function<void(T &element, u32 index)>;

struct Point {
public:
	int x;
	int y;
};

struct Rect {
public:
	unsigned int width;
	unsigned int height;
	int x;
	int y;
};

struct Color {
public:
	union {
		struct {
			float r;
			float g;
			float b;
			float a;
		};

		float rgba[4] {0,0,0,0};
	};
};

template<typename First, typename Second>
struct Pair {
public:
	Pair() = default;
	constexpr Pair(First _f, Second _s)
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

template<typename T>
class UnCopyable {
public:
	UnCopyable(const T &t)
		: value(t) {}
	~UnCopyable() = default;
	UnCopyable(const UnCopyable&) = delete;
	void operator =(const UnCopyable&) = delete;

	const T *operator ->() const { return &value; }
	const T *get() const { return &value; }

private:
	T value;
};

template <typename E, E V> constexpr const char *enum_to_string() {
	return __PRETTY_FUNCTION__;
}

}

#endif
