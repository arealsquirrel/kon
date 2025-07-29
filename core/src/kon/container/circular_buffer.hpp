#ifndef KN_CIRCULAR_BUFFER
#define KN_CIRCULAR_BUFFER

#include "kon/core/core.hpp"
#include <kon/core/allocator.hpp>

namespace kon {

/**
 * @brief A simple implementation of the circular queue - FIFO
 * 
 * @tparam T the type held by the queue
 */
template<typename T>
class CircleBuffer {
public:
    CircleBuffer(Allocator *allocator, u32 size) 
		: m_allocator(allocator), m_capacity(size) {

		m_buffer = reinterpret_cast<T*>(
				allocator->allocate_mem(size * sizeof(T)));
	}

    CircleBuffer(const CircleBuffer &buffer) 
		: m_allocator(buffer.m_allocator),
		m_front(buffer.m_front), m_capacity(buffer.m_size), m_size(buffer.m_size) {
		
		m_buffer = m_allocator->allocate_mem(m_size * sizeof(T));
		for(int i = 0; i < m_size; i++) {
			i = i % m_capacity;
			T *newe = new (this->m_buffer + (i+m_front)) T(buffer.m_buffer[i+m_front]);
		}
	}

    ~CircleBuffer() {
		for(int i = 0; i < m_size; i++)
			m_buffer[i + m_front].~T();

		m_allocator->free_mem(
				KN_MEM_POINTER(m_buffer), m_size * sizeof(T));
	}

public:
    T &get_front() {
        return this->m_buffer[m_front];
    }

    T &get_rear() {
        u32 rear = (m_front + m_size - 1) % m_capacity;
        return this->m_buffer[rear];
    }

    T &enqueue(const T &element) {
        if(m_size == this->m_capacity) return this->m_buffer[0];
        const u32 i = (m_front + m_size) % m_capacity;
        T *newe = new (this->m_buffer + i) T(element);
        m_size++;
        return *newe;
    }

    T &dequeue() {
        T &element = this->m_buffer[m_front];
        m_front = (m_front + 1) % m_capacity;
        m_size--;
        return element;
    }

    inline u32 get_front_index() const { return m_front; }
    inline u32 get_size() const { return m_size; } 
	inline u32 get_capacity() const { return m_capacity; }

private:
    u32 m_front {0};
    u32 m_size {0};
	u32 m_capacity {0};
	T *m_buffer;
	Allocator *m_allocator;
};

}

#endif
