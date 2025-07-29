
#include "engine.hpp"
#include <kon/engine/module.hpp>
#include "kon/core/allocator.hpp"
#include "kon/core/object.hpp"

namespace kon {

Engine::Engine(EngineCreateInfo info) 
	: Object(this),
	  m_block(info.memoryBudget),
	  m_enginePersistantBlock(m_block.get_memory(), info.memoryBudget * .75),
	  m_engineFrameBlock(m_block.get_memory() + m_enginePersistantBlock.get_size(), info.memoryBudget * .25),
	  m_engineMemory(&m_enginePersistantBlock),
	  m_frameAllocator(&m_engineFrameBlock),
	  m_moduleArray(this, reinterpret_cast<Allocator*>(&m_engineMemory)),
	  m_eventBus(&m_engineMemory) {}

Engine::~Engine() = default;

}

