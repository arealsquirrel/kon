
#include "allocator.hpp"
#include "kon/core/assert.hpp"
#include "kon/core/core.hpp"
#include "kon/debug/log.hpp"
#include <cstdlib>
#include <new>

namespace kon {

MemoryBlock::MemoryBlock(u32 size)
	: m_size(size), m_partition(false) {
	
	m_memory = reinterpret_cast<char*>(malloc(m_size));
}

MemoryBlock::MemoryBlock(char *block, u32 size)
	: m_memory(block), m_size(size), m_partition(true) {}

MemoryBlock::~MemoryBlock() {
	if(m_partition == false) { 
		free(m_memory);
	}
}

MemoryBlock MemoryBlock::partition(u32 size, u32 offset) {
	KN_ASSERT(offset+size > m_size, "partitioned block is larger than original");
	return MemoryBlock(m_memory+offset, size);	
}

Allocator::Allocator(const MemoryBlock *block)
	: m_block(block) {}

Allocator::~Allocator() {

}

char *Allocator::allocate_mem(u32 size) {
	return new char[size];
}

void Allocator::free_mem(char *mem, u32) {
	delete[] mem;
}

u32 Allocator::get_allocated_mem() const {
	return 0;
}

StackAllocator::StackAllocator(const MemoryBlock *block)
	: Allocator(block) {
	
	m_stackTop = m_block->get_memory();
}

StackAllocator::~StackAllocator() = default;

char *StackAllocator::allocate_mem(u32 size) {
	KN_ASSERT(get_allocated_mem() + size < m_block->get_size(), "requested memory excedes block size");

	char *mem = m_stackTop;
	m_stackTop += size;
	return mem;
}

void StackAllocator::free_mem(char *, u32 size) {
	// KN_ASSERT(get_allocated_mem() - size >= 0, "freeing more memory than the block");
	m_stackTop -= size;
}

u32 StackAllocator::get_allocated_mem() const {
	return m_stackTop - m_block->get_memory();
}

FreeListAllocator::FreeListAllocator(const MemoryBlock *block)
	: Allocator(block) {
	
	// make a linked list in the memory
	m_start = new (block->get_memory()) Header{
		block->get_size() - (u32)sizeof(Header),
		true,
		nullptr,
		nullptr
	};

	m_end = new (block->get_memory()+block->get_size()-sizeof(Header)) Header{
		0,
		false,
		nullptr,
		m_start
	};

	m_start->next = m_end;
}

FreeListAllocator::~FreeListAllocator() = default;

char *FreeListAllocator::allocate_mem(u32 size) {
	KN_ASSERT(get_allocated_mem() + size < m_block->get_size(), "requested memory execedes block size");

	Header *best = nullptr;

	Header *current = m_start;
	u32 totalSize = size + sizeof(Header);
	while(current != m_end) {
		if(current->free == false || current->size < totalSize) {
			current = current->next;
			continue;
		}

		if(current->size == totalSize) {
			return reinterpret_cast<char*>(current) + sizeof(Header);
		} 

		if(best == nullptr || current->size < best->size) {
			best = current;
		}

		current = current->next;
	}

	Header *split = new (
			KN_MEM_POINTER(best+1) + size
		) Header {best->size - size - (u32)(sizeof(Header)), true, best->next, best};

	best->free = false;
	best->size = size;
	best->next->prev = split;
	best->next = split;

	return KN_MEM_POINTER(best) + sizeof(Header);
}

void FreeListAllocator::free_mem(char *mem, u32) {
	Header *memHeader = reinterpret_cast<Header*>(
			KN_MEM_POINTER(mem) - sizeof(Header));

	memHeader->free = true;
	merge_freed(memHeader);
}

void FreeListAllocator::merge_freed(Header *header) {
	if(header != m_end && header->next->free) {
		header->size += header->next->size + sizeof(Header);
		header->next = header->next->next;
		header->next->prev = header;
	}

	if(header != m_start && header->prev->free) {
		Header *h = header->prev;
		h->size += header->size + sizeof(Header);
		h->next = header->next;
		h->next->prev = h;
	}
}

u32 FreeListAllocator::get_allocated_mem() const {
	Header *h = m_start;
	u32 size = 0;
	while(h != nullptr) {
		size  += (h->free == false) ? h->size : 0;
		h = h->next;
	}
	return size;
}

#define KN_PGA_GET_BLOCK(index) reinterpret_cast<Header*>( \
				m_block->get_memory() + ((m_pageSize + sizeof(Header)) * (index)))

#define KN_PGA_BLOCK_MEM(block) KN_MEM_POINTER(block+1)

PageAllocator::PageAllocator(const MemoryBlock *block, u32 pageSize) 
	: Allocator(block), m_pageSize(pageSize) {
	
	m_maxPages = block->get_size() / (pageSize + sizeof(Header));

	// make a linked list of free blocks
	for(u32 i = 0; i < m_maxPages-1; i++) {
		Header *h = KN_PGA_GET_BLOCK(i);
		h->free = true;
		h->nextfree = KN_PGA_GET_BLOCK(i+1);
	}

	m_freeTail = KN_PGA_GET_BLOCK(0);
	m_freeHead = KN_PGA_GET_BLOCK(m_maxPages-1);
	m_freeHead->free = true;
}

PageAllocator::~PageAllocator() = default;

char *PageAllocator::allocate_mem(u32) {
	// this is the main case for the allocator
	char *mem = KN_PGA_BLOCK_MEM(m_freeTail);
	m_freeTail->free = false;
	m_freeTail = m_freeTail->nextfree;
	m_usedPages++;

	return mem;
}

void PageAllocator::free_mem(char *mem, u32) {
	Header *h = reinterpret_cast<Header*>(mem-sizeof(Header));
	KN_ASSERT(h->free == false, "you want to free an already freed header???");
	h->free = true;
	m_freeHead->nextfree = h;
	m_freeHead = m_freeHead->nextfree;
}

/*
 * @warning not implemented yet
 */
u32 PageAllocator::get_allocated_mem() const {
	return 0;
}

}

