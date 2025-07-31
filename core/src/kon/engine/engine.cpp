
#include "engine.hpp"
#include <kon/engine/module.hpp>
#include "kon/core/allocator.hpp"
#include "kon/core/object.hpp"

namespace kon {

Engine::Engine(EngineCreateInfo info) 
	: Object(this),
	  // m_block(info.memoryBudget),
	  m_persistentMemory(info.persistentMemorySize),
	  m_persistentMemoryAllocator(&m_persistentMemory),
	  m_dynamicMemory(info.dynamicMemorySize),
	  m_dynamicMemoryAllocator(&m_dynamicMemory),
	  m_frameMemory(info.frameMemorySize),
	  m_frameAllocator(&m_frameMemory),


	  m_testAllocator(&m_persistentMemory),
	  m_moduleArray(this, reinterpret_cast<Allocator*>(&m_persistentMemoryAllocator)),
	  m_eventBus(&m_persistentMemoryAllocator) {}

Engine::~Engine() = default;

void Engine::mem_dump() {
	KN_CORE_TRACE("------- ENGINE PERSISTENT MEMORY -------");
	KN_CORE_TRACE("mem block starts at {} size {}",
	 		(void*)m_persistentMemory.get_memory(), m_persistentMemory.get_size());
	FreeListAllocator::Header *node = m_persistentMemoryAllocator.get_start();
	while(node != nullptr) {
		KN_CORE_TRACE("\theader at {}:[{}] size {}", (void*)node, node->free, node->size);
		node = node->next;
	}
}

}

