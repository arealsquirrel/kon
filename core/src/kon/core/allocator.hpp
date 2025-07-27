#ifndef KN_ALLOCATOR_HPP
#define KN_ALLOCATOR_HPP

#include <kon/core/core.hpp>
#include <utility>

namespace kon {

/*
 * standard way of describing a block of malloced memory
 */
struct MemoryBlock {
public:
	/*
	 * uses malloc to make some new memory.
	 * this will free the memory at the end of its lifetime
	 */
	MemoryBlock(u32 size);
	
	/*
	 * used to partition memory blocks
	 * will not free at the end of its lifetime
	 */
	MemoryBlock(char *block, u32 size);
	
	~MemoryBlock();

public:
	char *get_memory() const { return m_memory; }
	u32 get_size() const { return m_size; }
	MemoryBlock partition(u32 size, u32 offset=0);

private:
	char *m_memory;
	u32 m_size;
	bool m_partition;
};

/*
 * this is the base class for all of our allocators
 *
 * the base class is just a wrapper for malloc and free
 * all other allocators will write their own implementation
 */
class Allocator {
public:
	/*
	 * give it the block thats its gonna manage
	 */
	Allocator(const MemoryBlock *block);
	~Allocator();

public:
	virtual char *allocate_mem(u32 size);
	virtual void free_mem(char *mem, u32);

	/*
	 * returns the amount of used memory in the current
	 * allocator
	 */
	virtual u32 get_allocated_mem() const;

public:
	/*
	 * calls the constructor
	 */
	template<class T, typename ...Args>
	[[nodiscard]]
	T *allocate(u32 size=1, Args &&...args) {
		T *mem = reinterpret_cast<T*>(allocate_mem(sizeof(T) * size));
		for(u32 i = 0; i < size; i++)
			new (mem+i) T(std::forward<Args>(args)...);
		return mem;
	}

	/*
	 * calls the destructor
	 */
	template<class T>
	void free(T *mem, u32 size=1) {
		for(u32 i = 0; i < size; i++)
		 	(mem+i)->~T();

		this->free_mem(KN_MEM_POINTER(mem), size*sizeof(T));
	}

public:
	const MemoryBlock *get_block() const { return m_block; }

protected:
	const MemoryBlock *m_block;
};

/*
 * does not free memory, uses a stack pointer to 
 * traverse a stack of memory
 */
class StackAllocator : public Allocator {
public:
	StackAllocator(const MemoryBlock *block);
	~StackAllocator();

	char *allocate_mem(u32 size) override;

	/*
	 * we dont really care about the mem field
	 * with this one
	 */
	void free_mem(char*, u32 size) override;

	u32 get_allocated_mem() const override;

	void reset() { m_stackTop = m_block->get_memory(); }

private:
	char *m_stackTop;
};

/*
 * uses a doubly linked list to find usable blocks
 * of memory and easy traversal of the memory space
 */
class FreeListAllocator : public Allocator {
public:
	struct alignas(8) Header {
		u32 size;
		bool free;

		Header *next;
		Header *prev;
	};

public:
	FreeListAllocator(const MemoryBlock *block);
	~FreeListAllocator();

	char *allocate_mem(u32 size) override;

	/*
	 * we dont really care about the mem field
	 * with this one
	 */
	void free_mem(char *mem, u32) override;

	/*
	 * @warning not implemented yet
	 */
	u32 get_allocated_mem() const override;

	Header *get_start() const { return m_start; }
	Header *get_end() const { return m_end; }

private:
	void merge_freed(Header *header);

private:
	Header *m_start;
	Header *m_end;
};

}

#endif
