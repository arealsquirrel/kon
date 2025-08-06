#ifndef KN_ARRAYLIST_HPP
#define KN_ARRAYLIST_HPP

#include "kon/container/iterate.hpp"
#include "kon/core/assert.hpp"
#include <initializer_list>
#include <kon/core/core.hpp>
#include <kon/core/util.hpp>
#include <kon/core/allocator.hpp>

namespace kon {

template<typename T>
class ArrayList { 
public:
	ArrayList(Allocator *allocator)
		: m_allocator(allocator) {
		
		m_capacity = 0;
	}

	ArrayList(Allocator *allocator, std::initializer_list<T> list)
		: m_allocator(allocator) {
		
		resize(list.size());

		for(const T &t : list)
			add(t);
	}

	ArrayList(Allocator *allocator, u32 size)
		: m_allocator(allocator) {
		
		resize(size);
	}
	
	~ArrayList() {
		reset();
	}

	/*
	 * inherits the allocator
	 */
	ArrayList(const ArrayList &arraylist)
		: m_allocator(arraylist.m_allocator) {

		if(arraylist.empty()) return;

		resize(arraylist.m_capacity);

		for(u32 i = 0; i < arraylist.m_count; i++) {
			add(arraylist.get_buffer()[i]);
		}
	}

public:
	// ----------- MODIFICATION ----------- //
	
	// adds a new element to the top of the list
	T &add(const T &element) {
		if(m_count >= m_capacity) {
			resize(m_capacity+1);
		}

		T *e = new (m_buffer+m_count) T(element);
		m_count++;
		return *e;
	}

	// adds the element at the given index and moves the elements around it
	T &add(const T &element, u32 index) {
		if(m_count >= m_capacity) {
			resize(m_capacity+1);
		}

		move_elements(index+1, index, m_capacity-m_count+1);
		T *e = new (m_buffer+index) T(element);
		m_count++;
		return *e;
	}

	// adds a new element to the top of the list
	T &add(T &&element) {
		if(m_count >= m_capacity) {
			resize(m_capacity+1);
		}

		T *e = new (m_buffer+m_count) T{element};
		m_count++;
		return *e;
	}

	// please only call this function if you know what you are doing.
	// this is intened for vulkan functions that use this container to
	// get a VLA and then run the for each loops on it
	void set_count(u32 count) { m_count = count; }

	/*
	 * uses the copy constructor when copying the arrays
	 * size must be greater than the current size
	 */
	void resize(u32 size) {
		if(m_buffer == nullptr) {
			m_buffer = reinterpret_cast<T*>(
					m_allocator->allocate_mem(sizeof(T) * size));
			m_capacity = size;
			return;
		}

		T *temp = reinterpret_cast<T*>(m_allocator->allocate_mem(sizeof(T) * size));
		for(u32 i = 0; i < m_count; i++) {
			new (temp+i) T(m_buffer[i]);

			m_buffer[i].~T();
		}

		m_allocator->free_mem(KN_MEM_POINTER(m_buffer), get_byte_size());

		m_buffer = temp;
		m_capacity = size;
	}

	void reset() {
		if(empty()) return;

		for(u32 i = 0; i < m_count; i++) {
			m_buffer[i].~T();
		}

		m_allocator->free_mem(KN_MEM_POINTER(m_buffer), get_byte_size());
		m_buffer = nullptr;
		m_count = 0;
		m_capacity = 0;
	}

	void erase(u32 index) {
		KN_ASSERT(index < m_count, "erasing an element that does not exist");
		move_elements(index, index+1, m_capacity-index-1);
		m_count--;
    }

	// ----------- ACCESS ----------- //
	inline T *get_buffer() const { return m_buffer; }
	inline u32 get_byte_size() const { return sizeof(T) * m_capacity; }
	inline u32 get_capacity() const { return m_capacity; }
	inline u32 get_count() const { return m_count; }
	inline bool empty() const { return (m_count == 0); }

	inline T &get(u32 index) { return m_buffer[index]; }
	inline const T &get(u32 index) const { return m_buffer[index]; }

	// ----------- OPERATORS ----------- //
	inline T &operator[](u32 index) { return m_buffer[index]; }
	inline const T &operator[](u32 index) const { return m_buffer[index]; }

public:
	void for_each(foreach_function<T> f) {
		for(u32 i = 0; i < m_count; i++) {
			f(m_buffer[i]);
		}
	}

	void for_each(const foreach_function<T> f) const {
		for(u32 i = 0; i < m_count; i++) {
			f(m_buffer[i]);
		}
	}

	void view(const view_function<T> v, foreach_function<T> f) {
		for(u32 i = 0; i < m_count; i++) {
			auto &element = m_buffer[i];
			if(v(element))
				f(element);
		}
	}

private:
	void move_elements(size_t dest, size_t source, size_t num) {
		std::copy(m_buffer + source,
				 m_buffer + source + num,
				 m_buffer + dest);
	}

private:
	T *m_buffer {nullptr};

	Allocator *m_allocator;
	u32 m_capacity {0};
	u32 m_count {0};
};

}

#endif
