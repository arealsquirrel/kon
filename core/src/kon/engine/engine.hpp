#ifndef KN_ENGINE_HPP
#define KN_ENGINE_HPP

#include "kon/core/allocator.hpp"
#include "kon/core/events.hpp"
#include "kon/core/object.hpp"
#include "kon/core/window.hpp"
#include "kon/engine/module.hpp"
#include <kon/core/core.hpp>
#include <kon/resource/resource_cache.hpp>

namespace kon {

struct EngineCreateInfo {

u32 persistentMemorySize;
u32 dynamicMemorySize;
u32 frameMemorySize;

};

class Engine : public Object {
KN_OBJECT(Engine, Object)

public:
	Engine(EngineCreateInfo info);
	~Engine();

public:
	Allocator *get_allocator_persistent() { return &m_persistentMemoryAllocator; }
	Allocator *get_allocator_dynamic() { return &m_dynamicMemoryAllocator; }
	Allocator *get_allocator_frame() { return &m_frameAllocator; }

	ModuleArray &get_modules() { return m_moduleArray; }
	EventBus &get_event_bus() { return m_eventBus; }
	ResourceCache &get_resource_cache() { return m_resourceCache; }

	void mem_dump();

	void init();
	bool update();
	void clean();

private:

	/* a lot of memory stuff */
	MemoryBlock m_persistentMemory;
	FreeListAllocator m_persistentMemoryAllocator;
	MemoryBlock m_dynamicMemory;
	FreeListAllocator m_dynamicMemoryAllocator;
	MemoryBlock m_frameMemory;
	StackAllocator m_frameAllocator;
	Allocator m_testAllocator;
	
	ModuleArray m_moduleArray;
	EventBus m_eventBus;
	// Window m_window;
	ResourceCache m_resourceCache;

	bool m_running {true};
};

}

#endif
