#ifndef KN_GRAPHICS_MODULE_HPP
#define KN_GRAPHICS_MODULE_HPP

#include "kon/core/allocator.hpp"
#include "kon/engine/module.hpp"
#include "modules/graphics/vulkan/vulkan_context.hpp"

namespace kon {

class GraphicsModule : public Module {
public:
	GraphicsModule(Engine *engine, Allocator *allocator);
	~GraphicsModule();

	void init() override;
	void clean() override;
	void update() override;

	VulkanContext *get_graphics_context() { return &m_context; }

private:
	VulkanContext m_context;
};

}

#endif
