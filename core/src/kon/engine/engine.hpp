#ifndef KN_ENGINE_HPP
#define KN_ENGINE_HPP

#include "kon/core/allocator.hpp"
#include "kon/core/object.hpp"
#include "kon/engine/module.hpp"
#include <kon/core/core.hpp>

namespace kon {

struct EngineCreateInfo {

u32 memoryBudget;

};

class Engine : public Object {
KN_OBJECT(Engine, Object)

public:
	Engine(EngineCreateInfo info);
	~Engine();

public:
	MemoryBlock *get_engine_block() { return &m_block; }
	FreeListAllocator *get_allocator_engine() { return &m_engineMemory; }
	StackAllocator *get_allocator_frame() { return &m_frameAllocator; }

	ModuleArray &get_modules() { return m_moduleArray; }

private:

	/* a lot of memory stuff */
	MemoryBlock m_block;
	MemoryBlock m_enginePersistantBlock;
	MemoryBlock m_engineFrameBlock;
	FreeListAllocator m_engineMemory;
	StackAllocator m_frameAllocator;
	
	ModuleArray m_moduleArray;

};

}

#endif
