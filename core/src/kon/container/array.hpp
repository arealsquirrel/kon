#ifndef KN_ARRAY_HPP
#define KN_ARRAY_HPP

#include "kon/core/allocator.hpp"
#include <kon/core/util.hpp>
#include <kon/core/core.hpp>

namespace kon {

/*
 * fixed size array that works with const
 */
template<typename T, u32 size>
class Array {
public:
	Array(Allocator *allocator) 
		: m_allocator(allocator) {
	
		m_array = m_allocator->allocate<T>(size);
	}

	/*
	 * copy constructor inherits allocator
	 * @warning, this shit does not work
	 */
	Array(const Array &array)
		: m_allocator(array.m_allocator) {
		
		m_array = m_allocator->allocate<T>(size);
	}
	
	~Array() {
		m_allocator->free(m_array, size);
	}

public:
	constexpr u32 get_size() { return size; }

	inline T *get_array() const { return m_array; }

public:
	T &operator [] (u32 index){ return m_array[index]; }
	const T &operator [] (u32 index) const { return m_array[index]; }

	T &get(u32 index) { return m_array[index]; }
	const T &get(u32 index) const { return m_array[index]; }

	void for_each(ForEachFunction<T> func) {
		for (int i = 0; i < size; i++) {
			func(m_array[i], i);
		}
	}

private:
	T *m_array;
	Allocator *m_allocator;
};

}

#endif
