#ifndef KN_ARRAY_HPP
#define KN_ARRAY_HPP

#include "kon/core/allocator.hpp"
#include <initializer_list>
#include <kon/core/util.hpp>
#include <kon/core/core.hpp>

namespace kon {


/*
 * fixed size array that works with const
 *
 * it is also completetly constexpr now.
 */
template<typename T, u32 size>
class Array {
public:
	Array() = default;

	constexpr Array(std::initializer_list<T> list) {
		int index = 0;
		for(const T &element : list) {
			m_array[index] = element;
			index++;
		}
	}

	/*
	 * copy constructor inherits allocator
	 * @warning, this shit does not work
	 */
	constexpr Array(const Array &array) = default;
	~Array() = default;

public:
	constexpr u32 get_size() { return size; }

	constexpr T *get_array() const { return m_array; }

public:
	constexpr T &operator [] (u32 index){ return m_array[index]; }
	constexpr const T &operator [] (u32 index) const { return m_array[index]; }

	constexpr T &get(u32 index) { return m_array[index]; }
	constexpr const T &get(u32 index) const { return m_array[index]; }

	constexpr void for_each(ForEachFunction<T> func) {
		for (int i = 0; i < size; i++) {
			func(m_array[i], i);
		}
	}

private:
	T m_array[size];
};

}

#endif
