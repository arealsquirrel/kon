
#include "engine.hpp"
#include <kon/engine/module.hpp>
#include "kon/core/allocator.hpp"
#include "kon/core/events.hpp"
#include "kon/core/object.hpp"
#include "kon/debug/instrumentation.hpp"
#include "kon/debug/log.hpp"
#include "kon/resource/resource_cache.hpp"

namespace kon {

Engine::Engine(EngineCreateInfo info) 
	: Object(this),
	  m_persistentMemory(info.persistentMemorySize),
	  m_persistentMemoryAllocator(&m_persistentMemory),
	  m_dynamicMemory(info.dynamicMemorySize),
	  m_dynamicMemoryAllocator(&m_dynamicMemory),
	  m_frameMemory(info.frameMemorySize),
	  m_frameAllocator(&m_frameMemory),

	  m_testAllocator(&m_persistentMemory),
	  m_moduleArray(this, reinterpret_cast<Allocator*>(&m_persistentMemoryAllocator)),
	  m_eventBus(&m_persistentMemoryAllocator),
	  // m_window(this, {"Kon Engine", 500, 500}),
	  m_resourceCache(&m_persistentMemoryAllocator, this) {


	m_eventBus.add_listener(
			{ [&](Event&){ m_running = false; }, get_instance_id() },
			EventEngineExit::get_static_uuid());
}

Engine::~Engine() = default;

void Engine::init() {
	KN_INSTRUMENT_FUNCTION();

	m_moduleArray.init_modules();
}

bool Engine::update() {
	KN_INSTRUMENT_FUNCTION();

	m_moduleArray.update_modules();

	return m_running;
}

void Engine::clean() {
	KN_INSTRUMENT_FUNCTION();

	m_resourceCache.unload_resource_group(ResourceCache::groupIDAll);
	m_moduleArray.clean_modules();
}

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

