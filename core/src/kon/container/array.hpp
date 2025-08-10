#ifndef KN_ARRAY_HPP
#define KN_ARRAY_HPP

#include "kon/container/iterate.hpp"
#include <initializer_list>
#include <kon/core/util.hpp>
#include <kon/core/core.hpp>
#include <utility>

namespace kon {


/*
 * fixed size array
 */
template<typename T, u32 size>
class Array {
public:
	Array() = default;

	Array(const Array &array)
		: m_count(array.m_count) {

		for(u32 i = 0; i < m_count; i++) {
			m_array[i] = array.get(i);
		}
	}

	Array(std::initializer_list<T> array) {
		for(const auto &elm : array) {
			add(elm);
		}
	}

	~Array() = default;

public:
	// ----------- MODIFICATION ----------- //
	T &add(const T &element) {
		m_array[m_count++] = element;
		return m_array[m_count-1];
	}

	inline void reset() { m_count = 0; }

	// ----------- ACCESS ----------- //
	
	// get an element out of the array
	inline T &get(u32 i) { return m_array[i]; }
	inline const T &get(u32 i) const { return m_array[i]; }

	// get the array pointer to the buffer
	inline T *get_buffer() { return &m_array[0]; }

	// get the array pointer to the buffer
	inline const T *get_buffer() const { return &m_array[0]; }

	// returns the amount of elements added to the array
	inline u32 get_count() const { return m_count; }

	// returns size of the array
	inline u32 get_capacity() const { return size; }

	// checks if the count is 0
	inline bool empty() const { return (m_count == 0); }

	// ----------- OPERATORS ----------- //
	inline T& operator[](u32 index) {
		return get(index);
	}

	const T& operator[](u32 index) const {
		return get(index);
	}

public:
	void for_each(foreach_function<T> f) {
		for(u32 i = 0; i < m_count; i++) {
			f(m_array[i]);
		}
	}

	void for_each(const foreach_function<T> f) const {
		for(u32 i = 0; i < m_count; i++) {
			f(m_array[i]);
		}
	}

	void view(const view_function<T> v, foreach_function<T> f) {
		for(u32 i = 0; i < m_count; i++) {
			auto &element = m_array[i];
			if(v(element))
				f(element);
		}
	}

	void view(const view_function<T> v, const foreach_function<T> f) const {
		for(u32 i = 0; i < m_count; i++) {
			auto &element = m_array[i];
			if(v(element))
				f(element);
		}
	}

private:
	u32 m_count {0};
	T m_array[size];
};

}

#endif
