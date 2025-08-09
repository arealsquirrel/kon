#ifndef KN_GRAPHICS_MODULE_HPP
#define KN_GRAPHICS_MODULE_HPP

#include "kon/core/allocator.hpp"
#include "kon/core/events.hpp"
#include "kon/core/object.hpp"
#include "kon/engine/module.hpp"
#include "kon/math/vector3.hpp"
#include "modules/graphics/vulkan/vulkan_context.hpp"

namespace kon {

struct Vertex {
	Vector3 position;
	float uv_x;
	Vector3 normal;
	float uv_y;
	Vector4 color;

	bool operator ==(const Vertex &vertex) const { 
		return (uv_x == vertex.uv_x &&
				uv_y == vertex.uv_y &&
				color == vertex.color &&
				position == vertex.position);
	}
};

class GraphicsModule : public Module, EventListener {
KN_OBJECT(GraphicsModule, Module)

public:
	GraphicsModule(Engine *engine, Allocator *allocator);
	~GraphicsModule();

	void init() override;
	void clean() override;
	void update() override;

	void render_debug() override;

	VulkanContext *get_graphics_context() { return &m_context; }

private:
	VulkanContext m_context;
};

}

#endif
